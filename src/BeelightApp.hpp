#ifndef BEELIGHT_APP_HPP
#define BEELIGHT_APP_HPP

#include <lvgl.h>

#include "ui/ScreenNavigation.hpp"

class BeelightApp {
   public:
    static BeelightApp *instance() {
        static BeelightApp *instance = nullptr;
        if (instance == nullptr) {
            instance = new BeelightApp();
        }
        return instance;
    }

    void timerEvent();

   private:
    BeelightApp() {
        init();
    }

    ~BeelightApp() {
        deinit();
    }

    void init();
    void deinit();

    lv_timer_t *timer_m;
};
#endif
