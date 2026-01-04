#include "BeelightApp.hpp"
#include "port.hpp"
#include "BeeLog.hpp"
#include "Event.hpp"
#include "model/NavigationModel.hpp"
#include "ui/ui.h"
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
    // ui_init();
    navigateTo<Dashboard>();
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

// void BeelightApp::navigateTo(AbstractScreen *newScreen, ScreenNavigation::NavigationTransition transition, bool clearHistory)
// {
//     navigation_m.navigateTo(newScreen,transition, clearHistory);
// }

void BeelightApp::back(NavigationTransition transition, bool clearHistory)
{
    navigation_m.back(transition, clearHistory);
    // if (stackIndex > 1)
    // {
    //     AbstractScreen *currentScreen = top();
    //     pop();
    //     AbstractScreen *newScreen = top();
    //     navigate(newScreen, transition, clearHistory);
    //     delete currentScreen;
    // }
}

void BeelightApp::timerEvent()
{
    // top()->onTimerEvent();
    navigation_m.onTimerEvent();
}

// void BeelightApp::navigate(AbstractScreen *newScreen, NavigationTransition transition, bool clearHistory)
// {
//     if (stackIndex == 0)
//     {
//         newScreen->populate();
//         lv_screen_load(newScreen->getCurrentScreen());
//     }
//     else
//     {
//         newScreen->populate();

//         switch (transition)
//         {
//         case NavigationTransition::TO_LEFT:
//             lv_screen_load_anim(newScreen->getCurrentScreen(), LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0, true);
//             break;
//         case NavigationTransition::TO_RIGHT:
//             lv_screen_load_anim(newScreen->getCurrentScreen(), LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 500, 0, true);
//             break;
//         case NavigationTransition::NORMAL:
//         default:
//             lv_screen_load_anim(newScreen->getCurrentScreen(), LV_SCREEN_LOAD_ANIM_NONE, 0, 0, true);
//             break;
//         }

//         if (clearHistory)
//         {
//             while (stackIndex)
//             {
//                 AbstractScreen *toDelete = top();
//                 pop();
//                 delete toDelete;
//             }
//         }
//     }
// }
