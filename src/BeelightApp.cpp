#include "BeelightApp.hpp"
#include "port.hpp"
#include "BeeLog.hpp"
#include "Event.hpp"
#include "model/NavigationModel.hpp"
#include "ui/SplashScreen.hpp"
#ifdef SIMULATOR
#include "sim/com/BeelightCom_sim.hpp"
#else
#include "emb/ble/ble_config.h"
#endif
#include "AbstractScreen.hpp"

void BeelightApp::init()
{
    BeeLog::debug("BeelightApp", "Initializing Beelight Application");

    (void)Event::instance();
    (void)NavigationModel::instance();
#ifdef SIMULATOR
    (void)BeelightCom_sim::instance()->init();
#else
    ble_init();
#endif
    navigateTo<SplashScreen>();
    timer_m = lv_timer_create([](lv_timer_t *timer)
                              {
        BeelightApp *app = static_cast<BeelightApp *>(lv_timer_get_user_data(timer));
        app->timerEvent(); }, 1000, this);
}

void BeelightApp::deinit()
{
#ifdef SIMULATOR
    BeelightCom_sim::instance()->uninit();
#endif
    run = 0;
}
void BeelightApp::back(NavigationTransition transition)
{
    navigation_m.back(transition);
}

void BeelightApp::timerEvent()
{
    navigation_m.onTimerEvent();
}
