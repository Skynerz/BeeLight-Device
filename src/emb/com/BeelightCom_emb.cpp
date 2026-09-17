#include "BeelightCom_emb.hpp"
#include "emb/ble/ble_config.h"

void BeelightCom_Impl::init() {
    ble_init();
}

void BeelightCom_Impl::uninit() {
    // TODO ?
}

void BeelightCom_Impl::start_advertising(void) {
    ble_start_advertising();
}

void BeelightCom_Impl::stop_advertising(void) {
    ble_stop_advertising();
}

bool BeelightCom_Impl::is_connected() {
    return ble_is_connected();
}

bool BeelightCom_Impl::get_pin(uint32_t& pin) {
    return ble_get_pin(pin);
}
