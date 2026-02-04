#ifndef PERSISTENCY_MODEL_HPP
#define PERSISTENCY_MODEL_HPP

#include <string>

#include "BeeLog.hpp"
#include "common/port_persistency.h"

class PersistencyModel {
   public:
    static PersistencyModel* instance() {
        static PersistencyModel* instance = nullptr;
        if (instance == nullptr) {
            instance = new PersistencyModel();
        }
        return instance;
    }
    void uninit();

    void setDeviceName(const std::string& name);
    std::string getDeviceName();
    static constexpr std::string_view kDefaultDeviceName{"BeeLight"};

    uint8_t getCommissioningStatus();
    void setCommissioningStatus(uint8_t status);

   private:
    PersistencyModel();
    void init();
    using Key = Key_t;
    bool saveValue(Key key, const std::string& value);
    bool saveValue(Key key, uint32_t value);

    bool getStringValue(Key key, std::string& result);
    bool getU32Value(Key key, uint32_t& result);
    bool getU8Value(Key key, uint8_t& result);

    BeeLog logger_m;
};

#endif
