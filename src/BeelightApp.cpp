#include "BeelightApp.hpp"
#include "BeeLog.hpp"
#include "Event.hpp"
#include "model/NavigationModel.hpp"
#include "ui/ui.h"
#ifdef SIMULATOR
#include "sim/com/BeelightCom_sim.hpp"
#endif

void BeelightApp_init(lv_display_t *disp) {
    BeeLog::debug("BeelightApp", "Initializing Beelight Application");

    (void) Event::instance(disp);
    (void) NavigationModel::instance();
#ifdef SIMULATOR
    (void) BeelightCom_sim::instance()->init();
#else
    //TODO
#endif
    ui_init();
}