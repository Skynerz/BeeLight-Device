#include "ScreenNavigation.hpp"

void ScreenNavigation::back(NavigationTransition transition, bool clearHistory)
{
    if (stackIndex_m > 1)
    {
        AbstractScreen *currentScreen = top();
        pop();
        AbstractScreen *newScreen = top();
        navigate(newScreen, transition, clearHistory);
        delete currentScreen;
    }
}

void ScreenNavigation::onTimerEvent()
{
    top()->onTimerEvent();
}

void ScreenNavigation::navigate(AbstractScreen *newScreen, NavigationTransition transition, bool clearHistory)
{
    if (stackIndex_m == 0)
    {
        newScreen->populate();
        lv_screen_load(newScreen->getCurrentScreen());
    }
    else
    {
        newScreen->populate();

        switch (transition)
        {
        case NavigationTransition::TO_LEFT:
            lv_screen_load_anim(newScreen->getCurrentScreen(), LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 500, 0, true);
            break;
        case NavigationTransition::TO_RIGHT:
            lv_screen_load_anim(newScreen->getCurrentScreen(), LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 500, 0, true);
            break;
        case NavigationTransition::NORMAL:
        default:
            lv_screen_load_anim(newScreen->getCurrentScreen(), LV_SCREEN_LOAD_ANIM_NONE, 0, 0, true);
            break;
        }

        if (clearHistory)
        {
            while (stackIndex_m)
            {
                AbstractScreen *toDelete = top();
                pop();
                delete toDelete;
            }
        }
    }
}
