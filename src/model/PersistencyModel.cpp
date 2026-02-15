#include "PersistencyModel.hpp"

PersistencyModel::PersistencyModel() : logger_m("PersistencyModel") {
    init();
}

void PersistencyModel::setDeviceName(const std::string& name) {
    saveValue(DEVICE_NAME, name);
}

std::string PersistencyModel::getDeviceName() {
    std::string result;
    if (getStringValue(DEVICE_NAME, result)) {
        return result;
    }
    return std::string(kDefaultDeviceName);
}

uint8_t PersistencyModel::getCommissioningStatus() {
    uint8_t result = 0;
    if (getU8Value(COMMISSIONNING_STATUS, result)) {
        return result;
    }
    else {
        logger_m.warn("Commissioning status not found, writing default value");
        setCommissioningStatus(0);
    }
    return 0;
}

void PersistencyModel::setCommissioningStatus(uint8_t status) {
    saveValue(COMMISSIONNING_STATUS, status);
}

void PersistencyModel::init() {
    persistency_init();
}

void PersistencyModel::uninit() {
    persistency_deinit();
}

// Private methods
bool PersistencyModel::saveValue(Key key, const std::string& value) {
    return persistency_write(key, value.c_str());
}

bool PersistencyModel::saveValue(Key key, uint32_t value) {
    return persistency_write(key, &value);
}

bool PersistencyModel::getStringValue(Key key, std::string& result) {
    bool valid  = false;
    char* value = nullptr;
    uint8_t len = 0;
    persistency_read(key, (void**) &value, &len);
    if (value != nullptr) {
        result = std::string(value, len);
        valid  = true;
    }
    return valid;
}

bool PersistencyModel::getU32Value(Key key, uint32_t& result) {
    bool valid      = false;
    uint32_t* value = NULL;
    uint8_t len     = 0;
    persistency_read(key, (void**) &value, &len);
    if (value != NULL) {
        result = *value;
        valid  = true;
    }
    return valid;
}

bool PersistencyModel::getU8Value(Key key, uint8_t& result) {
    bool valid     = false;
    uint8_t* value = NULL;
    uint8_t len    = 0;
    persistency_read(key, (void**) &value, &len);
    if (value != NULL) {
        result = *value;
        valid  = true;
    }
    return valid;
}
