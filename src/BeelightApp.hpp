#ifndef BEELIGHT_APP_HPP
#define BEELIGHT_APP_HPP

#include <lvgl.h>

#include "ui/ScreenNavigation.hpp"

#ifdef SIMULATOR
#include "sim/com/BeelightCom_sim.hpp"
#else
#include "emb/com/BeelightCom_emb.hpp"
#endif

class BeelightApp {
   public:
    static BeelightApp *instance() {
        static BeelightApp *instance = nullptr;
        if (instance == nullptr) {
            instance = new BeelightApp();
        }
        return instance;
    }

    static BeelightCom *getComInstance() {
        static BeelightCom *instance;
        if (instance == nullptr) {
            instance = new BeelightCom_Impl();
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
