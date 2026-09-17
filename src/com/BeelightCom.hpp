#ifndef BEELIGHTCOM_HPP
#define BEELIGHTCOM_HPP

#include <cassert>

#include "BeeLog.hpp"
#include "Event.hpp"

enum BleEvents : uint8_t {
    EVENT_BLE_CONNECTED = Event::EVENT_BLE,
    EVENT_BLE_DISCONNECTED
};

class BeelightCom {
   public:
    BeelightCom()          = default;
    virtual ~BeelightCom() = default;
    virtual void init()    = 0;
    virtual void uninit()  = 0;

    virtual void start_advertising() = 0;
    virtual void stop_advertising()  = 0;
    virtual bool is_connected()      = 0;
    virtual bool get_pin(uint32_t&) {
        return false;
    }

    void registerEvents() {
        Event::instance()->registerEvent(BleEvents::EVENT_BLE_CONNECTED);
        Event::instance()->registerEvent(BleEvents::EVENT_BLE_DISCONNECTED);
    }
    
    BeeLog* getLogger() {
        return &log_m;
    }

   protected:
    BeeLog log_m{"BeelightCom"};
};

#endif
