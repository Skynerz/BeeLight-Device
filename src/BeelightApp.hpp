#ifndef BEELIGHT_APP_HPP
#define BEELIGHT_APP_HPP

#include <lvgl.h>

#include "ScreenNavigation.hpp"

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
    
    template <typename NextScreen, typename std::enable_if<std::is_base_of<AbstractScreen, NextScreen>::value>::type * = nullptr>
    void navigateTo(NavigationTransition transition = NavigationTransition::FADE_IN, bool clearHistory = false)
    {
        navigation_m.navigateTo<NextScreen>(transition, clearHistory);
    }

    void back(NavigationTransition transition = NavigationTransition::FADE_IN);
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

    ScreenNavigation navigation_m;
    lv_timer_t* timer_m;
};
#endif
