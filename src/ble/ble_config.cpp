#include "ble_config.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// TODO Refactor
#include "ui/ui.h"

BLEServer *pServer = nullptr;

bool deviceConnected = false;

class BeelightServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) override {
        deviceConnected = true;
        Serial.println("📱 Connected");
        setConnected(true);
        BLEDevice::stopAdvertising(); // Stop advertising once connected
    }

    void onDisconnect(BLEServer *pServer) override {
        deviceConnected = false;
        Serial.println("❌ Disconnected");
        setConnected(false);
        clearData();
        pServer->startAdvertising();
    }
};

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
        return 123456; // ou return 0 pour "Just Works"
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) override {
        if (cmpl.success)
            Serial.println("✅ Authentication success");
        else
            Serial.printf("❌ Authentication failed, stat=%d\n", cmpl.fail_reason);
    }
};


void ble_init_security() {
    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
    pSecurity->setCapability(ESP_IO_CAP_NONE);  // ou ESP_IO_CAP_NONE pour "Just Works"
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    BLEDevice::setSecurityCallbacks(new BeelightSecurityCallbacks());
}

void ble_init() {
    BLEDevice::init(DEVICE_NAME);

    ble_init_security();

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BeelightServerCallbacks());

    // --- Generic Service ---
    BLEService *genericService = pServer->createService(SERVICE_UUID_GENERIC);


    /// CURRENT TIME -----------------------------------------------------------------------
    BLECharacteristic *currentTime = genericService->createCharacteristic(CHARAC_UUID_TIME, BLECharacteristic::PROPERTY_WRITE);
    currentTime->setValue("00:00");
    class CurrentTimeCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            Serial.printf("Rxed Current Time: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Current Time Set: %s\n", value.c_str());
                setTime(value);
            } else {
                Serial.println("Rxed empty current time");
            }
        }
    };
    currentTime->setCallbacks(new CurrentTimeCallback);

    genericService->start();

     // --- Navigation Service ---
    BLEService *navService = pServer->createService(SERVICE_UUID_NAVIGATION);

    /// ESTIMATED TIME REMAINING BEFORE ARRIVAL -------------------------------------------
    BLECharacteristic *charEta = navService->createCharacteristic(CHARAC_UUID_ETA, BLECharacteristic::PROPERTY_WRITE);
    charEta->setValue("Remain. time");
    class RemainingTimeBeforeArrivalCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            Serial.printf("Rxed ETA: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("ETA set: %s\n", value.c_str());
                setEta(value);
            } else {
                Serial.println("Rxed empty ETA");
            }
        }
    };
    charEta->setCallbacks(new RemainingTimeBeforeArrivalCallback());

    /// ESTIMATED DISTANCE BEFORE ARRIVAL -------------------------------------------------
    BLECharacteristic *charEda = navService->createCharacteristic(CHARAC_UUID_EDA, BLECharacteristic::PROPERTY_WRITE);
    charEda->setValue("Remain. dist");
    class RemainingDistanceBeforeArrivalCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            Serial.printf("Rxed EDA: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("EDA Set: %s\n", value.c_str());
                setEda(value);
            } else {
                Serial.println("Rxed empty EDA value");
            }
        }
    };
    charEda->setCallbacks(new RemainingDistanceBeforeArrivalCallback());

    /// ESTIMATED TIME OF ARRIVAL ---------------------------------------------------------
    BLECharacteristic *charArrivingTime = navService->createCharacteristic(
        CHARAC_UUID_ARRIVING_TIME,
        BLECharacteristic::PROPERTY_WRITE
    );
    charArrivingTime->setValue("HH:MM");
    class EstimatedArrivingTimeCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            Serial.printf("Rxed arriving time: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Arriving Time Set: %s\n", value.c_str());
                setArrivingTime(value);
            } else {
                Serial.println("Rxed empty  value");
            }
        }
    };
    charArrivingTime->setCallbacks(new EstimatedArrivingTimeCallback());

    /// NEXT INSTRUCTION ------------------------------------------------------------------
    BLECharacteristic *charInstruction = navService->createCharacteristic(CHARAC_UUID_INSTRUCTION, BLECharacteristic::PROPERTY_WRITE);
    charInstruction->setValue("Instruction");
    class NextInstructionCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            Serial.printf("Rxed instruction: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Instruction Set: %s\n", value.c_str());
                setDirection(value);
            } else {
                Serial.println("Rxed empty instruction value");
            }
        }
    };
    charInstruction->setCallbacks(new NextInstructionCallback());
    
    /// NEXT INSTRUCTION DISTANCE ------------------------------------------------------------------
    BLECharacteristic *charInstructionDistance = navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_DISTANCE, BLECharacteristic::PROPERTY_WRITE);
    charInstructionDistance->setValue("Instruction Distance");
    class NextInstructionDistanceCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            Serial.printf("Rxed instruction dist: %s\n", pCharacteristic->getValue().c_str());
            String value = pCharacteristic->getValue();
            if (value.length() > 0) {
                Serial.printf("Instruction dist Set: %s\n", value.c_str());
                setDirectionDistance(value);
            } else {
                Serial.println("Rxed empty instruction dist value");
            }
        }
    };
    charInstructionDistance->setCallbacks(new NextInstructionDistanceCallback());

    /// NEXT INSTRUCTION ICON -------------------------------------------------------------
    // BLECharacteristic *charIcon = navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_ICON, BLECharacteristic::PROPERTY_WRITE);
    // charIcon->setValue((uint8_t *)"ICONDATA", 8);  // Remplacer par tes données binaires
    // class NextInstructionIconCallback : public BLECharacteristicCallbacks {
    //     void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
    //         Serial.println("Rxed instruction icon");
    //         String value = pCharacteristic->getValue();
    //         if (value.length() > 0) {
    //             Serial.println("Instruction Icon Set");
    //             setDirectionIcon((const uint8_t *)value.c_str(), 126);
    //         } else {
    //             Serial.println("Rxed empty instruction icon value");
    //         }
    //     }
    // };
    // charIcon->setCallbacks(new NextInstructionIconCallback());

    navService->start();

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
    Serial.println("BLE Advertising started");
}