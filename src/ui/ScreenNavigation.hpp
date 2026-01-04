#ifndef SCREEN_NAVIGATION_HPP
#define SCREEN_NAVIGATION_HPP

#include "AbstractScreen.hpp"
#include "BeeLog.hpp"

#define HISTORY_DEPTH 3

enum NavigationTransition
{
    NORMAL,
    TO_LEFT,
    TO_RIGHT,
    FADE_IN,
};

class ScreenNavigation
{
public:
    ScreenNavigation() = default;
    ~ScreenNavigation();

    template <typename NextScreen, typename std::enable_if<std::is_base_of<AbstractScreen, NextScreen>::value>::type * = nullptr>
    void navigateTo(NavigationTransition transition = FADE_IN, bool clearHistory = false)
    {
        if (stackIndex_m < HISTORY_DEPTH)
        {
            logger_m.debug("navigateTo stackIndex_m=" + std::to_string(stackIndex_m));
            ScreenInstanciator instanciator = createInstance<NextScreen>;
            AbstractScreen *newScreen = instanciator();
            if(clearHistory) {
                pop(clearHistory);
            }
            destroyInstance(&topScreen_m);
            navigate(newScreen, transition);
            topScreen_m = newScreen;
            screenStack_m[stackIndex_m++] = instanciator;
        }
        else {
            logger_m.warn("History is full");
        }
    }

    void back(NavigationTransition transition);
    void onTimerEvent();

private:
    void navigate(AbstractScreen *newScreen, NavigationTransition transition = NORMAL);
    void pop(bool clearHistory = false);
    AbstractScreen *top()
    {
        return topScreen_m;
    }

    template <class NextScreen, typename std::enable_if<std::is_base_of<AbstractScreen, NextScreen>::value>::type * = nullptr>
    static AbstractScreen *createInstance()
    {
        return new NextScreen();
    }

    static void destroyInstance(AbstractScreen **instance)
    {
        delete *instance;
        *instance = nullptr;
    }

    using ScreenInstanciator = AbstractScreen * (*)();

    ScreenInstanciator getTopInstanciator()
    {
        return screenStack_m[stackIndex_m - 1];
    }

    BeeLog logger_m{"ScreenNavigation"};
    ScreenInstanciator screenStack_m[HISTORY_DEPTH]{0};
    AbstractScreen *topScreen_m{nullptr};
    uint8_t stackIndex_m{0};
};
#endif