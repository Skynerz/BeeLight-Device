#ifndef ABSTRACT_SCREEN_HPP
#define ABSTRACT_SCREEN_HPP

#include <lvgl.h>

#include <vector>

#include "BeeLog.hpp"
#include "Event.hpp"
#include "port.hpp"

struct Context {};

class AbstractScreen {
   public:
    AbstractScreen(const std::string name = "AbstractScreen") : name_m(name), logger_m(name) {
        logger_m.debug("Constructor");
        screen_m = lv_obj_create(NULL);
#ifdef SIMULATOR
        screenBg_m = lv_obj_create(screen_m);
        lv_obj_set_size(screenBg_m, getScreenWidth(), getScreenHeight());
        lv_obj_set_style_bg_color(screenBg_m, lv_color_hex(0xFFFFFF), 0);
        lv_obj_set_style_radius(screenBg_m, getScreenWidth() / 2, 0);
#endif
    }

    virtual ~AbstractScreen() {
        logger_m.debug("Destructor");
        for (const auto &connection : connections_m) {
            Event::instance()->disconnect(connection.obj, connection.eventId, connection.cb);
        }
        connections_m.clear();
    }

    void connect(lv_obj_t *obj, uint8_t eventId, lv_event_cb_t cb, void *data = nullptr) {
        Event::instance()->connect(obj, eventId, cb, data);
        connections_m.push_back(ScreenEvent{obj, eventId, cb, data});
    }

    virtual void populate() = 0;

    virtual void onTimerEvent() {
        // Nothing to do
    }

    virtual void onPostPopulate() {
        logger_m.debug("onPostPopulate");
        lv_obj_dump_tree(screen_m);
    }

    virtual void onScreenLoaded() {
        // Nothing to do
    }

    lv_obj_t *obj() const {
        return screen_m;
    }

    const std::string &getName() const {
        return name_m;
    }

   private:
    std::string name_m;
    BeeLog logger_m;
    lv_obj_t *screen_m;
    lv_obj_t *screenBg_m;
    struct ScreenEvent {
        lv_obj_t *obj;
        uint8_t eventId;
        lv_event_cb_t cb;
        void *data;
    };
    std::vector<ScreenEvent> connections_m;
};

#endif
