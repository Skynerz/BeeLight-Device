#include "BeelightApp.hpp"

#include "BeeLog.hpp"
#include "Event.hpp"
#include "model/NavigationModel.hpp"
#include "model/PersistencyModel.hpp"
#include "ui/AbstractScreen.hpp"
#include "ui/ScreenNavigation.hpp"
#include "ui/SplashScreen.hpp"

void BeelightApp::init() {
    BeeLog::debug("BeelightApp", "Initializing Beelight Application");

    (void) PersistencyModel::instance();
    (void) Event::instance();
    (void) NavigationModel::instance();
    (void) ScreenNavigation::instance();
    auto comInstance = getComInstance();
    comInstance->registerEvents();
    comInstance->init();

    ScreenNavigation::instance()->navigateTo<SplashScreen>();
    timer_m = lv_timer_create(
        [](lv_timer_t *timer) {
            BeelightApp *app = static_cast<BeelightApp *>(lv_timer_get_user_data(timer));
            app->timerEvent();
        },
        1000, this);
}

void BeelightApp::deinit() {
    getComInstance()->uninit();
    PersistencyModel::instance()->uninit();
    run = 0;
}

void BeelightApp::timerEvent() {
    ScreenNavigation::instance()->onTimerEvent();
}
