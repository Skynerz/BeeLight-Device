#ifndef BLE_CARACTERISTICS_HPP
#define BLE_CARACTERISTICS_HPP

#include <BLECharacteristic.h>

class CurrentTimeCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

class RemainingTimeBeforeArrivalCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

class RemainingDistanceBeforeArrivalCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

class EstimatedArrivingTimeCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

class NextInstructionCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

class NextInstructionDistanceCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

class NextInstructionIconCallback : public BLECharacteristicCallbacks {
   public:
    void onWrite(BLECharacteristic *pCharacteristic) override;
};

#endif  // BLE_CARACTERISTICS_HPP