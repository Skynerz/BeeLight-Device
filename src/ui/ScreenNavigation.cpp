#include "ScreenNavigation.hpp"

ScreenNavigation::~ScreenNavigation() {
    pop(true);
    destroyInstance(&topScreen_m);
}

void ScreenNavigation::onScreenUnloaded(lv_event_t *event) {
    AbstractScreen *newScreen = (AbstractScreen *) lv_event_get_user_data(event);
    if (newScreen) {
        newScreen->onScreenLoaded();
    } else {
        BeeLog::warn("ScreenNavigation", "onScreenUnloaded screen pointer null");
    }
}

void ScreenNavigation::back(NavigationTransition transition) {
    if (stackIndex_m > 1) {
        pop(false);
        destroyInstance(&topScreen_m);
        topScreen_m = getTopInstanciator()();
        navigate(topScreen_m, transition);
    }
}

void ScreenNavigation::onTimerEvent() {
    top()->onTimerEvent();
}

void ScreenNavigation::navigate(AbstractScreen *newScreen, NavigationTransition transition) {
    newScreen->populate();
    if (stackIndex_m == 0) {
        lv_screen_load(newScreen->obj());
    } else {
        // autoriser allocation une fois
        lv_obj_add_event_cb(top()->obj(), &onScreenUnloaded, LV_EVENT_SCREEN_UNLOADED, newScreen);
        switch (transition) {
            case NavigationTransition::TO_LEFT:
                lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_MOVE_LEFT, 1000, 0, true);
                break;
            case NavigationTransition::TO_RIGHT:
                lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_MOVE_RIGHT, 1000, 0, true);
                break;
            case NavigationTransition::FADE_IN:
                lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_FADE_IN, 1000, 0, true);
                break;
            case NavigationTransition::NORMAL:
            default:
                lv_screen_load_anim(newScreen->obj(), LV_SCREEN_LOAD_ANIM_NONE, 0, 0, true);
                break;
        }
    }
    newScreen->onPostPopulate();
}

void ScreenNavigation::pop(bool clearHistory) {
    if (clearHistory) {
        while (stackIndex_m) {
            screenStack_m[stackIndex_m--] = nullptr;
        }
    } else {
        if (stackIndex_m) {
            screenStack_m[stackIndex_m--] = nullptr;
        }
    }
}
