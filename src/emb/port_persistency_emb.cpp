#include <Preferences.h>

#include <map>

#include "BeeLog.hpp"
#include "common/port_persistency.h"

static constexpr const char* PERSISTENCY_NAMESPACE = "beelight";
static constexpr size_t MAX_STRING_LENGTH          = 64;

static Preferences prefs;
static BeeLog logger_m("Persistency");
static std::map<Key_t, std::string> keyNames = {
    {NONE, "NONE"}, {COMMISSIONNING_STATUS, "COMMISS_STATUS"}, {DEVICE_NAME, "DEVICE_NAME"}, {PASSWORD, "PASSWORD"}};

void persistency_init(void) {
    if (!prefs.begin(PERSISTENCY_NAMESPACE, false)) {
        logger_m.error("Failed to initialize preferences");
    }
}

void persistency_deinit(void) {
    prefs.end();
}

uint8_t persistency_write(const Key_t key, const void* value) {
    size_t ret = 0;
    switch (key) {
        case COMMISSIONNING_STATUS:
            ret = prefs.putUChar(keyNames[key].c_str(), *(const char*) value);
            break;
        case DEVICE_NAME:
        case PASSWORD:
            ret = prefs.putString(keyNames[key].c_str(), (const char*) value);
            break;
        default:
            logger_m.warn("Attempt to write unknown key: " + std::to_string(key));
            break;
    }
    return static_cast<uint8_t>(ret);
}

uint8_t persistency_read(const Key_t key, void** value, uint8_t* len) {
    uint8_t ret = 0;
    if (!prefs.isKey(keyNames[key].c_str())) {
        *len   = 0;
        *value = NULL;
        logger_m.warn("Key not found: " + std::to_string(key));
    } else {
        switch (key) {
            case COMMISSIONNING_STATUS: {
                static uint8_t commissionning_status = 0;
                commissionning_status                = prefs.getUChar(keyNames[key].c_str(), 0);
                *value                               = &commissionning_status;
                *len                                 = sizeof(uint8_t);
                ret                                  = 1;
                break;
            }
            case DEVICE_NAME: {
                static char deviceName[MAX_STRING_LENGTH];
                size_t read_len = prefs.getString(keyNames[key].c_str(), deviceName, MAX_STRING_LENGTH);
                if (read_len > 0) {
                    *value = deviceName;
                    *len   = read_len;
                    ret    = 1;
                }
                break;
            }
            case PASSWORD: {
                static char password[MAX_STRING_LENGTH];
                size_t read_len = prefs.getString(keyNames[key].c_str(), password, MAX_STRING_LENGTH);
                if (read_len > 0) {
                    *value = password;
                    *len   = read_len;
                    ret    = 1;
                }
                break;
            }
            default:
                logger_m.warn("Attempt to read unknown key: " + std::to_string(key));
                break;
        }
    }
    return ret;
}
