#ifndef BEELIGHT_APP_HPP
#define BEELIGHT_APP_HPP

#include <lvgl.h>

// void BeelightApp_init();
// void BeelightApp_deinit();

#include "ScreenNavigation.hpp"

// #define HISTORY_DEPTH 5

class BeelightApp
{
public:
    static BeelightApp *instance()
    {
        static BeelightApp *instance = nullptr;
        if (instance == nullptr)
        {
            instance = new BeelightApp();
        }
        return instance;
    }
    // enum NavigationTransition
    // {
    //     NORMAL,
    //     TO_LEFT,
    //     TO_RIGHT
    // };

    template <typename NewScreen, typename std::enable_if<std::is_base_of<AbstractScreen, NewScreen>::value>::type * = nullptr>
    void navigateTo(NavigationTransition transition = NavigationTransition::NORMAL, bool clearHistory = false)
    {
        navigation_m.navigateTo<NewScreen>(transition, clearHistory);
    }
    // {
    //     if(stackIndex < (HISTORY_DEPTH - 1)) {
    //         AbstractScreen *newScreen = new NewScreen();
    //         navigate(newScreen, transition, clearHistory);
    //         screenStack_m[stackIndex++] = newScreen;
    //     }
    // }
    void back(NavigationTransition transition = NavigationTransition::NORMAL, bool clearHistory = false);
    void timerEvent();

private:
    BeelightApp()
    {
        init();
    }
    ~BeelightApp()
    {
        deinit();
    }

    void init();
    void deinit();

    //void navigateTo(AbstractScreen *newScreen, NavigationTransition transition = NORMAL, bool clearHistory = false);

    // void pop() {
    //     if(stackIndex) {
    //         screenStack_m[stackIndex--] = NULL;
    //     }
    // }

    // AbstractScreen* top() {
    //     return screenStack_m[stackIndex - 1];
    // }
    
    // AbstractScreen* screenStack_m[HISTORY_DEPTH];
    // uint8_t stackIndex = 0;
    ScreenNavigation navigation_m;
    lv_timer_t* timer_m;
};
#endif
