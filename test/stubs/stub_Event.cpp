#include "Event.hpp"

uint8_t registerEventCalledCount = 0;
uint8_t emitCalledCount          = 0;
uint8_t connectCalledCount       = 0;

void __attribute__((weak)) Event::registerEvent(uint8_t) {
    registerEventCalledCount++;
}

void __attribute__((weak)) Event::emit(uint8_t, void *) {
    emitCalledCount++;
}

void __attribute__((weak)) Event::connect(lv_obj_t *, uint8_t, lv_event_cb_t, void *) {
    connectCalledCount++;
}

Event::EventHandler *__attribute__((weak)) Event::getLvEvent(uint8_t) {
    return nullptr;
}
