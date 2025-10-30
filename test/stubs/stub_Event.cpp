#include "Event.hpp"

void __attribute__((weak)) Event::registerEvent(uint8_t)
{
}

void __attribute__((weak)) Event::emit(uint8_t, void *)
{
}

void __attribute__((weak)) Event::connect(lv_obj_t *, uint8_t, lv_event_cb_t, void *)
{
}

Event::EventHandler* __attribute__((weak)) Event::getLvEvent(uint8_t)
{
    return nullptr;
}
