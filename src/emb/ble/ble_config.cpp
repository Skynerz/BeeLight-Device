#include "ble_config.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "BeeLog.hpp"
#include "ble_characteristics.hpp"
#include "model/NavigationModel.hpp"
#include "model/PersistencyModel.hpp"
#include "ui/Dashboard.hpp"

// BLE global variables
static BLEServer *pServer   = nullptr;
bool deviceConnected = false;

static BeeLog logger_m("BleConfig");
// END TODO

// Connection events callbacks
class BeelightServerConnectionCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) override {
        deviceConnected = true;
        logger_m.debug("📱 Connected");
        setConnected(true);
        BLEDevice::stopAdvertising();  // Stop advertising once connected
    }

    void onDisconnect(BLEServer *pServer) override {
        deviceConnected = false;
        logger_m.debug("❌ Disconnected");
        setConnected(false);
        NavigationModel::instance()->reset();
        pServer->startAdvertising();
    }
};

// Security connections Calbacks
class BeelightSecurityCallbacks : public BLESecurityCallbacks {
    bool onConfirmPIN(uint32_t pin) override {
        Serial.printf("PIN: %06u\n", pin);
        return true;
    }

    bool onSecurityRequest() override {
        return true;
    }

    void onPassKeyNotify(uint32_t pass_key) override {
        Serial.printf("PassKey: %06u\n", pass_key);
    }

    uint32_t onPassKeyRequest() override {
        return 123456;  // ou return 0 pour "Just Works"
    }

#if defined(CONFIG_BLUEDROID_ENABLED)
    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) override {
        if (cmpl.success)
            logger_m.debug("✅ Authentication success");
        else
            Serial.printf("❌ Authentication failed, stat=%d\n", cmpl.fail_reason);
    }
#elif defined(CONFIG_NIMBLE_ENABLED)
    void onAuthenticationComplete(ble_gap_conn_desc *cmpl) override {
        if (cmpl->sec_state.authenticated)
            logger_m.debug("✅ Authentication success");
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
    BLESecurity *pSecurity = &security_m;
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
    pSecurity->setCapability(ESP_IO_CAP_NONE);  // ou ESP_IO_CAP_NONE pour "Just Works"
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    BLEDevice::setSecurityCallbacks(&securityCallbacks_m);
}

// Start advertising
void ble_start_advertising() {
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

    advertising->start();
    logger_m.info("BLE Advertising started");
}

// Init BLE service
void ble_init() {
    logger_m.info("Initializing BLE...");
    auto deviceName      = PersistencyModel::instance()->getDeviceName();
    String deviceNameStr = String(deviceName.c_str());
    BLEDevice::init(deviceNameStr);

    ble_init_security();

    pServer           = BLEDevice::createServer();
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

    /// ESTIMATED TIME REMAINING BEFORE ARRIVAL -------------------------------------------
    BLECharacteristic *charEta = navService->createCharacteristic(CHARAC_UUID_ETA, BLECharacteristic::PROPERTY_WRITE);
    charEta->setCallbacks(&remainingTimeBeforeArrivalCallback_m);

    /// ESTIMATED DISTANCE BEFORE ARRIVAL -------------------------------------------------
    BLECharacteristic *charEda = navService->createCharacteristic(CHARAC_UUID_EDA, BLECharacteristic::PROPERTY_WRITE);
    charEda->setCallbacks(&remainingDistanceBeforeArrivalCallback_m);
    
    /// ESTIMATED TIME OF ARRIVAL ---------------------------------------------------------
    BLECharacteristic *charArrivingTime =
        navService->createCharacteristic(CHARAC_UUID_ARRIVING_TIME, BLECharacteristic::PROPERTY_WRITE);
    charArrivingTime->setCallbacks(&estimatedArrivingTimeCallback_m);

    /// NEXT INSTRUCTION ------------------------------------------------------------------
    BLECharacteristic *charInstruction =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION, BLECharacteristic::PROPERTY_WRITE);
    charInstruction->setCallbacks(&nextInstructionCallback_m);

    /// NEXT INSTRUCTION DISTANCE ------------------------------------------------------------------
    BLECharacteristic *charInstructionDistance =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_DISTANCE, BLECharacteristic::PROPERTY_WRITE);
    charInstructionDistance->setCallbacks(&nextInstructionDistanceCallback_m);

    /// NEXT INSTRUCTION ICON -------------------------------------------------------------
    BLECharacteristic *charIcon =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_ICON, BLECharacteristic::PROPERTY_WRITE);
    charIcon->setCallbacks(&nextInstructionIconCallback_m);

    navService->start();
    ble_start_advertising();
}
