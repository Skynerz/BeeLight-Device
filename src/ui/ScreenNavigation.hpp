#ifndef SCREEN_NAVIGATION_HPP
#define SCREEN_NAVIGATION_HPP

#include "AbstractScreen.hpp"
#include "BeeLog.hpp"

#define HISTORY_DEPTH 3

enum NavigationTransition {
    NORMAL,
    TO_LEFT,
    TO_RIGHT,
    FADE_IN,
};

class ScreenNavigation {
   public:
    static ScreenNavigation *instance() {
        static ScreenNavigation *instance = nullptr;
        if (instance == nullptr) {
            instance = new ScreenNavigation();
        }
        return instance;
    }

    template <typename NextScreen,
              typename std::enable_if<std::is_base_of<AbstractScreen, NextScreen>::value>::type * = nullptr>
    void navigateTo(NavigationTransition transition = FADE_IN, bool clearHistory = false) {
        if (stackIndex_m < HISTORY_DEPTH) {
            ScreenInstanciator instanciator = createInstance<NextScreen>;
            AbstractScreen *newScreen       = instanciator();
            logger_m.debug("navigateTo " + newScreen->getName() + " stackIndex_m=" + std::to_string(stackIndex_m));
            navigate(newScreen, transition);
            destroyInstance(&topScreen_m);
            topScreen_m = newScreen;
            if (clearHistory) {
                pop(clearHistory);
            }
            screenStack_m[stackIndex_m++] = instanciator;
        } else {
            logger_m.warn("History is full");
        }
    }

    void back(NavigationTransition transition = NORMAL);
    void onTimerEvent();

   private:
    ScreenNavigation() = default;
    ~ScreenNavigation();

    static void onScreenUnloaded(lv_event_t *event);

    void navigate(AbstractScreen *newScreen, NavigationTransition transition = NORMAL);
    void pop(bool clearHistory = false);
    AbstractScreen *top() {
        return topScreen_m;
    }

    template <class NextScreen,
              typename std::enable_if<std::is_base_of<AbstractScreen, NextScreen>::value>::type * = nullptr>
    static AbstractScreen *createInstance() {
        return new NextScreen();
    }

    static void destroyInstance(AbstractScreen **instance) {
        delete *instance;
        *instance = nullptr;
    }

    using ScreenInstanciator = AbstractScreen *(*) ();

    ScreenInstanciator getTopInstanciator() {
        if (stackIndex_m) {
            return screenStack_m[stackIndex_m - 1];
        } else {
            return nullptr;
        }
    }

    BeeLog logger_m{"ScreenNavigation"};
    ScreenInstanciator screenStack_m[HISTORY_DEPTH]{0};
    AbstractScreen *topScreen_m{nullptr};
    AbstractScreen *previousScreen_m{nullptr};
    uint8_t stackIndex_m{0};
};

#endif
