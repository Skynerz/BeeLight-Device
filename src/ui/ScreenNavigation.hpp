#ifndef SCREEN_NAVIGATION_HPP
#define SCREEN_NAVIGATION_HPP

#include "AbstractScreen.hpp"
#include "BeeLog.hpp"

#define HISTORY_DEPTH 3

enum NavigationTransition
{
    NORMAL,
    TO_LEFT,
    TO_RIGHT
};

class ScreenNavigation
{
public:
    ScreenNavigation() = default;

    template <typename NewScreen, typename std::enable_if<std::is_base_of<AbstractScreen, NewScreen>::value>::type * = nullptr>
    void navigateTo(NavigationTransition transition = NORMAL, bool clearHistory = false)
    {
        if (stackIndex_m < (HISTORY_DEPTH - 1))
        {
            logger_m.debug("navigateTo stackIndex_m=" + std::to_string(stackIndex_m));
            AbstractScreen *newScreen = new NewScreen();
            navigate(newScreen, transition, clearHistory);
            screenStack_m[stackIndex_m++] = newScreen;
        }
        else {
            logger_m.warn("History is full");
        }
    }

    void back(NavigationTransition transition = NORMAL, bool clearHistory = false);
    void onTimerEvent();

private:
    void navigate(AbstractScreen *newScreen, NavigationTransition transition = NORMAL, bool clearHistory = false);

    void pop()
    {
        if (stackIndex_m)
        {
            screenStack_m[stackIndex_m--] = NULL;
        }
    }

    AbstractScreen *top()
    {
        return screenStack_m[stackIndex_m - 1];
    }

    BeeLog logger_m{"ScreenNavigation"};
    AbstractScreen *screenStack_m[HISTORY_DEPTH];
    uint8_t stackIndex_m = 0;
};
#endif