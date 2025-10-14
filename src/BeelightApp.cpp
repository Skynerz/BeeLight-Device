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

void BeelightApp_init() {
    BeeLog::debug("BeelightApp", "Initializing Beelight Application");

    (void) Event::instance();
    (void) NavigationModel::instance();
#ifdef SIMULATOR
    (void) BeelightCom_sim::instance()->init();
#else
    ble_init();
#endif
    ui_init();
}

void BeelightApp_deinit(){
#ifdef SIMULATOR
    BeelightCom_sim::instance()->uninit();
#endif
    run = 0;
}
