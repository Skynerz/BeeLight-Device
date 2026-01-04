#include "ScreenNavigation.hpp"

ScreenNavigation::~ScreenNavigation()
{
    pop(true);
    destroyInstance(&topScreen_m);
}

void ScreenNavigation::back(NavigationTransition transition)
{
    if (stackIndex_m > 0)
    {
        pop(false);
        destroyInstance(&topScreen_m);
        topScreen_m = getTopInstanciator()();
        navigate(topScreen_m, transition);
    }
}

void ScreenNavigation::onTimerEvent()
{
    top()->onTimerEvent();
}

void ScreenNavigation::navigate(AbstractScreen *newScreen, NavigationTransition transition)
{
    if (stackIndex_m == 0)
    {
        newScreen->populate();
        lv_screen_load(newScreen->obj());
    }
    else
    {
        // newScreen->populate();

        switch (transition)
        {
        case NavigationTransition::TO_LEFT:
            lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0, true);
            break;
        case NavigationTransition::TO_RIGHT:
            lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 500, 0, true);
            break;
        case NavigationTransition::FADE_IN:
            lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_FADE_IN, 500, 0, true);
            break;
        case NavigationTransition::NORMAL:
        default:
            lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_NONE, 0, 0, true);
            break;
        }

        newScreen->populate();
    }
}

void ScreenNavigation::pop(bool clearHistory)
{
    if (clearHistory)
    {
        while (stackIndex_m)
        {
            screenStack_m[stackIndex_m--] = nullptr;
        }
    }
    else
    {
        if (stackIndex_m)
        {
            screenStack_m[stackIndex_m--] = nullptr;
        }
    }
}

