#ifndef SPLASHSCREEN_HPP
#define SPLASHSCREEN_HPP

#include "AbstractScreen.hpp"
#include "DirectionIconWidget.hpp"

class SplashScreen : public AbstractScreen {
   public:
    SplashScreen() : AbstractScreen("SplashScreen"), icon_m(obj()) {
    }

    void populate() override;
    void onTimerEvent() override;

   private:
    DirectionIconWidget icon_m;
    lv_obj_t* top_label_m;
    lv_obj_t* status_label_m;
    uint8_t tick_m{0};
};

#endif
