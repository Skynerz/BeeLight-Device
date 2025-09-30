#ifndef EVENT_HPP
#define EVENT_HPP

#include <map>
#include <lvgl.h>

class Event {
public:
    static Event *instance(lv_display_t *disp = nullptr)
    {
        static Event *instance = nullptr;
        if (instance == nullptr)
        {
            instance = new Event(disp);
        }
        return instance;
    }

    Event(lv_display_t *disp) : disp_m(disp)
    {
    }
    void registerEvent(uint8_t eventId);
    void emit(uint8_t eventId, void* data = nullptr);
    void connect(lv_obj_t* obj, uint8_t eventId, lv_event_cb_t cb, void *data = nullptr);

private:
// internal id to LVGL event
    using EventHandler = std::pair<uint32_t, lv_obj_t *>;
    EventHandler* getLvEvent(uint8_t eventId); 
    std::map<uint8_t, EventHandler> registeredEvents_m; //TODO mutex
    lv_display_t *disp_m;
};


#endif