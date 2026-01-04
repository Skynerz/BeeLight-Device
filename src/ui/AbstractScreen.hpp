#ifndef ABSTRACT_SCREEN_HPP
#define ABSTRACT_SCREEN_HPP

#include <lvgl.h>
#include <stack>
#include <vector>
#include "port.hpp"
#include "Event.hpp"
#include "BeeLog.hpp"

class AbstractScreen
{
public:
    AbstractScreen(const std::string tag = "AbstractScreen") : logger_m(tag)
    {
        screen_m = lv_obj_create(NULL);
#ifdef SIMULATOR
        screenBg_m = lv_obj_create(screen_m);
        lv_obj_set_size(screenBg_m, getScreenWidth(), getScreenHeight());
        lv_obj_set_style_bg_color(screenBg_m, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_radius(screenBg_m, getScreenWidth() / 2, 0);
#endif
    }

    virtual ~AbstractScreen()
    {
        for (const auto &connection : connections_m)
        {
            Event::instance()->disconnect(connection.obj, connection.eventId, connection.cb);
            logger_m.debug("disconnect " + std::to_string(connection.eventId));
        }
        connections_m.clear();
    }

    void connect(lv_obj_t *obj, uint8_t eventId, lv_event_cb_t cb, void *data = nullptr)
    {
        Event::instance()->connect(obj, eventId, cb);
        connections_m.push_back(ScreenEvent{obj, eventId, cb, data});
        logger_m.debug("connect " + std::to_string(eventId));
    }

    virtual void populate() = 0;
    virtual void onTimerEvent()
    {
        // Nothing to do
    }

    lv_obj_t *getCurrentScreen() const
    {
        return screen_m;
    }

private:
    BeeLog logger_m;
    lv_obj_t *screen_m;
    lv_obj_t *screenBg_m;
    struct ScreenEvent
    {
        lv_obj_t *obj;
        uint8_t eventId;
        lv_event_cb_t cb;
        void *data;
    };
    std::vector<ScreenEvent> connections_m;
};

#endif
