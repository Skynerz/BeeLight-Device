#include "ble_characteristics.hpp"
#include <Arduino.h>
#include "BeeLog.hpp"
#include "model/NavigationModel.hpp"

static BeeLog logger_m{"BleConfig"};

void CurrentTimeCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed Current Time: %s\n", pCharacteristic->getValue().c_str());
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
        Serial.printf("Current Time Set: %s\n", value.c_str());
        NavigationModel::instance()->setCurrentTime(std::string(value.c_str()));
    } else {
        logger_m.warn("Rxed empty current time");
    }
}

void RemainingTimeBeforeArrivalCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed ETA: %s\n", pCharacteristic->getValue().c_str());
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
        Serial.printf("ETA set: %s\n", value.c_str());
        NavigationModel::instance()->setEstTimeBeforeArrival(std::string(value.c_str()));
    } else {
        logger_m.debug("Rxed empty ETA");
    }
}

void RemainingDistanceBeforeArrivalCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed EDA: %s\n", pCharacteristic->getValue().c_str());
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
        Serial.printf("EDA Set: %s\n", value.c_str());
        NavigationModel::instance()->setEstDistanceBeforeArrival(std::string(value.c_str()));
    } else {
        logger_m.warn("Rxed empty EDA value");
    }
}

void EstimatedArrivingTimeCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed arriving time: %s\n", pCharacteristic->getValue().c_str());
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
        Serial.printf("Arriving Time Set: %s\n", value.c_str());
        NavigationModel::instance()->setArrivingTime(std::string(value.c_str()));
    } else {
        logger_m.warn("Rxed empty  value");
    }
}

void NextInstructionCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed instruction: %s\n", pCharacteristic->getValue().c_str());
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
        Serial.printf("Instruction Set: %s\n", value.c_str());
        NavigationModel::instance()->setNextInstruction(std::string(value.c_str()));
    } else {
        logger_m.warn("Rxed empty instruction value");
    }
}

void NextInstructionDistanceCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed instruction dist: %s\n", pCharacteristic->getValue().c_str());
    String value = pCharacteristic->getValue();
    if (value.length() > 0) {
        Serial.printf("Instruction dist Set: %s\n", value.c_str());
        NavigationModel::instance()->setRemainingDistanceBeforeNextInstruction(std::string(value.c_str()));
    } else {
        logger_m.warn("Rxed empty instruction dist value");
    }
}

void NextInstructionIconCallback::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.printf("Rxed instruction sz %d\n", pCharacteristic->getLength());
    if (pCharacteristic->getLength() == 1) {
        Serial.printf("Instruction icon Set: %d\n", pCharacteristic->getData()[0]);
        NavigationModel::instance()->setNextInstructionIcon(
            static_cast<InstructionIcon::Values>(pCharacteristic->getData()[0]));
    } else {
        logger_m.warn("Rxed empty instruction icon");
    }
}
