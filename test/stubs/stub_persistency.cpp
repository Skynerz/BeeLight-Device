#include "common/port_persistency.h"
#include <map>
#include <string>

uint8_t persistency_init_called = 0;
uint8_t persistency_deinit_called = 0;
uint8_t persistency_write_called = 0;
uint8_t persistency_read_called = 0;

uint8_t persistency_write_return_value = 0;
uint8_t persistency_read_return_value = 0;
Key_t persistency_read_key = NONE;
Key_t persistency_write_key = NONE;

std::map<Key_t, std::string> keyToString;

void __attribute__((weak)) persistency_init(void) {
    keyToString.clear();
    persistency_init_called++;
}

void __attribute__((weak)) persistency_deinit(void) {
    persistency_deinit_called++;
}

uint8_t __attribute__((weak)) persistency_write(const Key_t key, const void* value) {
    persistency_write_called++;
    keyToString[key] = std::string((const char*) value);
    return persistency_write_return_value;
}

uint8_t __attribute__((weak)) persistency_read(const Key_t key, void** value, uint8_t* len) {
    persistency_read_called++;
    *value = NULL;
    *len   = 0;
    if(persistency_read_return_value) {
        std::map<Key_t, std::string>::iterator it = keyToString.find(key);
        if (it != keyToString.end()) {
            *value = (void*) it->second.c_str();
            *len   = it->second.length();
        }
    }
    return persistency_read_return_value;
}
