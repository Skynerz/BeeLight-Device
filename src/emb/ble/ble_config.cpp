#include "ble_config.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "BeeLog.hpp"
#include "ui/Dashboard.hpp"
#define MAX_IMG_SIZE 16384  // prévoir assez large pour ton PNG 126x126

// BLE global variables
BLEServer *pServer   = nullptr;
bool deviceConnected = false;

static uint8_t img_buf[MAX_IMG_SIZE];
static size_t img_len       = 0;
static size_t expected_size = 0;
static bool receiving_img   = false;
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

// Init security with callbacks
void ble_init_security() {
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
    pSecurity->setCapability(ESP_IO_CAP_NONE);  // ou ESP_IO_CAP_NONE pour "Just Works"
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    BLEDevice::setSecurityCallbacks(new BeelightSecurityCallbacks());
}

// Start advertising
void ble_start_advertising() {
    // --- Advertising ---
    BLEAdvertising *advertising = BLEDevice::getAdvertising();

    BLEAdvertisementData *advData = new BLEAdvertisementData();
    advData->setName(DEVICE_NAME);
    advData->setAppearance(DEVICE_APPEARANCE);
    advData->setFlags(ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT);

    advertising->setAdvertisementData(*advData);

    advertising->addServiceUUID(SERVICE_UUID_GENERIC);
    advertising->addServiceUUID(SERVICE_UUID_NAVIGATION);
    advertising->setScanResponse(true);
    advertising->setScanResponseData(*advData);

    advertising->start();
    logger_m.info("BLE Advertising started");
}

// Init BLE service
void ble_init() {
    BLEDevice::init(DEVICE_NAME);

    ble_init_security();

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BeelightServerConnectionCallbacks());

    // --- Generic Service ---
    BLEService *genericService = pServer->createService(SERVICE_UUID_GENERIC);

    /// CURRENT TIME -----------------------------------------------------------------------
    BLECharacteristic *currentTime =
        genericService->createCharacteristic(CHARAC_UUID_TIME, BLECharacteristic::PROPERTY_WRITE);
    class CurrentTimeCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed Current Time: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Current Time Set: %s\n", value.c_str());
                NavigationModel::instance()->setCurrentTime(std::string(value.c_str()));
            } else {
                logger_m.warn("Rxed empty current time");
            }
        }
    };
    currentTime->setCallbacks(new CurrentTimeCallback);

    genericService->start();

    // --- Navigation Service ---
    BLEService *navService = pServer->createService(SERVICE_UUID_NAVIGATION);

    /// ESTIMATED TIME REMAINING BEFORE ARRIVAL -------------------------------------------
    BLECharacteristic *charEta = navService->createCharacteristic(CHARAC_UUID_ETA, BLECharacteristic::PROPERTY_WRITE);
    class RemainingTimeBeforeArrivalCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed ETA: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("ETA set: %s\n", value.c_str());
                NavigationModel::instance()->setEstTimeBeforeArrival(std::string(value.c_str()));
            } else {
                logger_m.debug("Rxed empty ETA");
            }
        }
    };
    charEta->setCallbacks(new RemainingTimeBeforeArrivalCallback());

    /// ESTIMATED DISTANCE BEFORE ARRIVAL -------------------------------------------------
    BLECharacteristic *charEda = navService->createCharacteristic(CHARAC_UUID_EDA, BLECharacteristic::PROPERTY_WRITE);
    class RemainingDistanceBeforeArrivalCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed EDA: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("EDA Set: %s\n", value.c_str());
                NavigationModel::instance()->setEstDistanceBeforeArrival(std::string(value.c_str()));
            } else {
                logger_m.warn("Rxed empty EDA value");
            }
        }
    };
    charEda->setCallbacks(new RemainingDistanceBeforeArrivalCallback());

    /// ESTIMATED TIME OF ARRIVAL ---------------------------------------------------------
    BLECharacteristic *charArrivingTime =
        navService->createCharacteristic(CHARAC_UUID_ARRIVING_TIME, BLECharacteristic::PROPERTY_WRITE);
    class EstimatedArrivingTimeCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed arriving time: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Arriving Time Set: %s\n", value.c_str());
                NavigationModel::instance()->setArrivingTime(std::string(value.c_str()));
            } else {
                logger_m.warn("Rxed empty  value");
            }
        }
    };
    charArrivingTime->setCallbacks(new EstimatedArrivingTimeCallback());

    /// NEXT INSTRUCTION ------------------------------------------------------------------
    BLECharacteristic *charInstruction =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION, BLECharacteristic::PROPERTY_WRITE);
    class NextInstructionCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed instruction: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Instruction Set: %s\n", value.c_str());
                NavigationModel::instance()->setNextInstruction(std::string(value.c_str()));
            } else {
                logger_m.warn("Rxed empty instruction value");
            }
        }
    };
    charInstruction->setCallbacks(new NextInstructionCallback());

    /// NEXT INSTRUCTION DISTANCE ------------------------------------------------------------------
    BLECharacteristic *charInstructionDistance =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_DISTANCE, BLECharacteristic::PROPERTY_WRITE);
    class NextInstructionDistanceCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed instruction dist: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Instruction dist Set: %s\n", value.c_str());
                NavigationModel::instance()->setRemainingDistanceBeforeNextInstruction(std::string(value.c_str()));
            } else {
                logger_m.warn("Rxed empty instruction dist value");
            }
        }
    };
    charInstructionDistance->setCallbacks(new NextInstructionDistanceCallback());

    /// NEXT INSTRUCTION ICON -------------------------------------------------------------
    BLECharacteristic *charIcon =
        navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_ICON, BLECharacteristic::PROPERTY_WRITE);
    // Callback definition
    class NextInstructionIconCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic) {
            Serial.printf("Rxed instruction sz %d\n", pCharacteristic->getLength());
            if (pCharacteristic->getLength() == 1) {
                Serial.printf("Instruction icon Set: %d\n", pCharacteristic->getData()[0]);
                NavigationModel::instance()->setNextInstructionIcon(
                    static_cast<InstructionIcon::Values>(pCharacteristic->getData()[0]));
            } else {
                logger_m.warn("Rxed empty instruction icon");
            }
        }
    };
    charIcon->setCallbacks(new NextInstructionIconCallback());

    navService->start();
    ble_start_advertising();
}
