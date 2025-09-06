#include "ble_config.h"

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// TODO Refactor to avoid direct dependency on UI
#include "ui/ui.h"

#define MAX_IMG_SIZE 16384  // prévoir assez large pour ton PNG 126x126

// BLE global variables
BLEServer *pServer = nullptr;
bool deviceConnected = false;

static uint8_t img_buf[MAX_IMG_SIZE];
static size_t img_len = 0;
static size_t expected_size = 0;
static bool receiving_img = false;
// END TODO

// Connection events callbacks
class BeelightServerConnectionCallbacks : public BLEServerCallbacks {
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
        return 123456; // ou return 0 pour "Just Works"
    }

    void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl) override {
        if (cmpl.success)
            Serial.println("✅ Authentication success");
        else
            Serial.printf("❌ Authentication failed, stat=%d\n", cmpl.fail_reason);
    }
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
    Serial.println("BLE Advertising started");
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
    BLECharacteristic *currentTime = genericService->createCharacteristic(CHARAC_UUID_TIME, BLECharacteristic::PROPERTY_WRITE);
    currentTime->setValue("00:00");

    // Callback definition 
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

    // Callback definition 
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

    // Callback definition 
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

    // Callback definition 
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

    // Callback definition 
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
    BLECharacteristic *charIcon = navService->createCharacteristic(CHARAC_UUID_INSTRUCTION_ICON, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_READ);
    charIcon->setValue("Icon data");

    // Callback definition 
    class NextInstructionIconCallback : public BLECharacteristicCallbacks {
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t *param) {
            uint8_t* value = param->write.value;
            uint16_t dataLength = param->write.len;
            
            // On first packet on this characteristic, the content corresponds to the size (4 bytes little endian)
            if(!receiving_img) {
                receiving_img = true;
                expected_size = pCharacteristic->getValue().toInt();
                img_len = 0;

            // On next packets, we receive the actual data
            } else {
                // Add data to buffer
                memcpy(&img_buf[img_len], value, dataLength);
                img_len += dataLength;

                // DEBUG -----------------------------------------------------------------------------
                // Debug: print received data as hex
                // for (uint8_t i = 0; i < dataLength; i++)
                // {
                //     Serial.printf("%02X", &img_buf[img_len-dataLength+i]);
                // }
                // Serial.println();
                // Serial.printf("Received %d bytes, total %d/%d\n", dataLength, img_len, expected_size);
                // DEBUG -----------------------------------------------------------------------------

                // If we have received the full image, display it
                if(img_len >= expected_size) {
                    // Serial.println("PNG complete, displaying...");
                    setDirectionIcon(img_buf, img_len);

                    receiving_img = false;
                    img_buf[0] = 0; // Clear buffer
                    img_len = 0;
                    expected_size = 0;
                }
            }
        }
    };
    charIcon->setCallbacks(new NextInstructionIconCallback());

    navService->start();
    ble_start_advertising();
}