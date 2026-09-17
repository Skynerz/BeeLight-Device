#include "ble_config.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "BeeLog.hpp"
#include "ble_characteristics.hpp"
#include "com/BeelightCom.hpp"
#include "esp_random.h"
#include "model/NavigationModel.hpp"
#include "model/PersistencyModel.hpp"
#include "ui/Dashboard.hpp"

// BLE global variables
static BLEServer *pServer = nullptr;
static BeeLog logger_m("BleConfig");
static uint32_t pin_m = 123456;
static bool pin_set_m = false;
static bool isBound_m = false;
static BLEService *genericService;
static BLEService *navService;

void ble_set_pin(uint32_t pin) {
    pin_set_m = true;
    pin_m     = pin;
}

bool ble_get_pin(uint32_t &pin) {
    pin = pin_m;
    return pin_set_m;
}

// Connection events callbacks
class BeelightServerConnectionCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) override {
        logger_m.debug("Connected");
    }

    void onDisconnect(BLEServer *pServer) override {
        logger_m.debug("Disconnected");
        Event::instance()->emit(BleEvents::EVENT_BLE_DISCONNECTED);
        isBound_m = false;
        NavigationModel::instance()->reset();
        pServer->startAdvertising();
    }
};

// Security connections Calbacks
class BeelightSecurityCallbacks : public BLESecurityCallbacks {
#if defined(CONFIG_BLUEDROID_ENABLED)
    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) override {
        if (cmpl.success)
            logger_m.debug("✅ ENCtication success");
        else
            Serial.printf("❌ ENCtication failed, stat=%d\n", cmpl.fail_reason);
    }
#elif defined(CONFIG_NIMBLE_ENABLED)
    void onAuthenticationComplete(ble_gap_conn_desc *cmpl) override {
        if (cmpl->sec_state.authenticated)
            logger_m.debug("✅ ENCtication success");
        if (cmpl->sec_state.bonded)
            logger_m.debug("✅ Bounded");
        if (cmpl->sec_state.authorize)
            logger_m.debug("✅ Authorized");
        if (cmpl->sec_state.encrypted) {
            logger_m.debug("✅ Encrypted");
            Event::instance()->emit(BleEvents::EVENT_BLE_CONNECTED);  // non a bouger si auth ok
            BLEDevice::stopAdvertising();                             // Stop advertising once connected
            isBound_m = true;
        }
    }
#endif
};

static BeelightServerConnectionCallbacks serverCallbacks_m;
static BeelightSecurityCallbacks securityCallbacks_m;
static BLEAdvertisementData advaData_m;
static BLESecurity security_m;
static CurrentTimeCallback currentTimeCallback_m;
static RemainingTimeBeforeArrivalCallback remainingTimeBeforeArrivalCallback_m;
static RemainingDistanceBeforeArrivalCallback remainingDistanceBeforeArrivalCallback_m;
static EstimatedArrivingTimeCallback estimatedArrivingTimeCallback_m;
static NextInstructionCallback nextInstructionCallback_m;
static NextInstructionDistanceCallback nextInstructionDistanceCallback_m;
static NextInstructionIconCallback nextInstructionIconCallback_m;

// Init security with callbacks
void ble_init_security() {
    security_m.setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
    security_m.setCapability(BLE_HS_IO_DISPLAY_ONLY);  // ou ESP_IO_CAP_NONE pour "Just Works"
    security_m.setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    pin_m = security_m.generateRandomPassKey();
    ble_set_pin(pin_m);
    logger_m.debug("PIN generated : " + std::to_string(pin_m));
    security_m.setPassKey(true, pin_m);
    BLEDevice::setSecurityCallbacks(&securityCallbacks_m);
}

// Start advertising
void ble_init_advertising() {
    // --- Advertising ---
    BLEAdvertising *advertising = BLEDevice::getAdvertising();

    advaData_m.setName(PersistencyModel::instance()->getDeviceName().c_str());
    advaData_m.setAppearance(DEVICE_APPEARANCE);
    advaData_m.setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);

    advertising->setAdvertisementData(advaData_m);

    advertising->addServiceUUID(SERVICE_UUID_GENERIC);
    advertising->addServiceUUID(SERVICE_UUID_NAVIGATION);
    advertising->setScanResponse(true);
    advertising->setScanResponseData(advaData_m);
}

void ble_start_advertising() {
    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advaData_m.setName(PersistencyModel::instance()->getDeviceName().c_str());
    advertising->setAdvertisementData(advaData_m);

    advertising->start();
    logger_m.info("BLE Advertising started");
}

void ble_stop_advertising() {
    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->stop();
    logger_m.info("BLE Advertising stopped");
}

// Init BLE service
void ble_init() {
    logger_m.info("Initializing BLE...");
    auto deviceName      = PersistencyModel::instance()->getDeviceName();
    String deviceNameStr = String(deviceName.c_str());
    BLEDevice::init(deviceNameStr);

    ble_init_security();

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks_m);

    // --- Generic Service ---
    BLEService *genericService = pServer->createService(SERVICE_UUID_GENERIC);

    /// CURRENT TIME -----------------------------------------------------------------------
    BLECharacteristic *currentTime =
        genericService->createCharacteristic(CHARAC_UUID_TIME, BLECharacteristic::PROPERTY_WRITE);
    currentTime->setCallbacks(&currentTimeCallback_m);

    genericService->start();

    // --- Navigation Service ---
    BLEService *navService = pServer->createService(BLEUUID(SERVICE_UUID_NAVIGATION));

    constexpr uint32_t writeProperties = BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_ENC |
                               BLECharacteristic::PROPERTY_WRITE_AUTHEN;

    /// ESTIMATED TIME REMAINING BEFORE ARRIVAL -------------------------------------------
    BLECharacteristic *charEta = navService->createCharacteristic(CHARAC_UUID_ETA, writeProperties);
    charEta->setCallbacks(&remainingTimeBeforeArrivalCallback_m);

    /// ESTIMATED DISTANCE BEFORE ARRIVAL -------------------------------------------------
    BLECharacteristic *charEda = navService->createCharacteristic(CHARAC_UUID_EDA, writeProperties);
    charEda->setCallbacks(&remainingDistanceBeforeArrivalCallback_m);

    /// ESTIMATED TIME OF ARRIVAL ---------------------------------------------------------
    BLECharacteristic *charArrivingTime = navService->createCharacteristic(CHARAC_UUID_ARRIVING_TIME, writeProperties);
    charArrivingTime->setCallbacks(&estimatedArrivingTimeCallback_m);

    /// NEXT INSTRUCTION ------------------------------------------------------------------
    BLECharacteristic *charInstruction = navService->createCharacteristic(CHARAC_UUID_INSTRUCTION, writeProperties);
    charInstruction->setCallbacks(&nextInstructionCallback_m);

    /// NEXT INSTRUCTION DISTANCE ------------------------------------------------------------------
    BLECharacteristic *charInstructionDistance =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_DISTANCE, writeProperties);
    charInstructionDistance->setCallbacks(&nextInstructionDistanceCallback_m);

    /// NEXT INSTRUCTION ICON -------------------------------------------------------------
    BLECharacteristic *charIcon = navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_ICON, writeProperties);
    charIcon->setCallbacks(&nextInstructionIconCallback_m);

    navService->start();
}

void ble_uninit() {
    nimble_port_deinit();
}

bool ble_is_connected() {
    return isBound_m;
}
