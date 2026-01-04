#include "Event.hpp"

void Event::registerEvent(uint8_t eventId) {
    if (registeredEvents_m.find(eventId) == registeredEvents_m.end()) {
        registeredEvents_m[eventId] = EventHandler(lv_event_register_id(), nullptr);
    }
}

void Event::emit(uint8_t eventId, void* data) {
    auto event = getLvEvent(eventId);
    if (event) {
        lv_obj_send_event(event->second, (lv_event_code_t) event->first, data);
    }
}

void Event::connect(lv_obj_t* obj, uint8_t eventId, lv_event_cb_t cb, void* data) {
    auto event = instance()->getLvEvent(eventId);
    if (event && obj) {
        event->second = obj;
        lv_obj_add_event_cb(obj, cb, (lv_event_code_t) event->first, data);
    }
}

void Event::disconnect(lv_obj_t* obj, uint8_t eventId, lv_event_cb_t cb) {
    auto event = instance()->getLvEvent(eventId);
    if (event && obj) {
        event->second = NULL;
        lv_obj_remove_event_cb(obj, cb);
    }
}

Event::EventHandler* Event::getLvEvent(uint8_t eventId) {
    auto it = registeredEvents_m.find(eventId);
    if (it != registeredEvents_m.end()) {
        return &it->second;
    } else {
        return nullptr;
    }
}
