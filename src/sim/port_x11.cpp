#if CONFIG_LV_USE_X11

#include "BeelightApp.hpp"
#include "port.hpp" 
#include "BeeLog.hpp"
#include <unistd.h> //todo windows alt
#include <csignal>

#define MY_DISP_HOR_RES (int32_t) 360
#define MY_DISP_VER_RES (int32_t) 360

//TODO ?
#define lvgl_port_lock(x)
#define lvgl_port_unlock()

void setup() {
    BeeLog::debug("init", "Initializing LVGL");
    lv_init();
    lv_display_t *disp = NULL;
    LV_IMAGE_DECLARE(img_hand_cursor);

    disp = lv_x11_window_create("beelight-simulator", MY_DISP_HOR_RES, MY_DISP_VER_RES);
    
    /* initialize X11 input drivers (for keyboard, mouse & mousewheel) */
    lv_x11_inputs_create(disp, &img_hand_cursor);

    BeeLog::debug("init", "Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    BeelightApp_init();

    //TODO on a jamais l'evenement puisque queue free apres envoi
    lv_display_add_event_cb(disp, [](lv_event_t * e){
        if(lv_event_get_code(e) == LV_EVENT_DELETE) {
            BeeLog::info("BeelightApp", "Delete event received, exiting...");
            BeelightApp_deinit();
        }
    }, LV_EVENT_DELETE, nullptr);

    /* Release the mutex */
    lvgl_port_unlock();
}

void tearDown() {
    lv_deinit();
    BeeLog::debug("deinit", "Deinitialized LVGL");
}

void _usleep(uint32_t usecs) {
    usleep(usecs);
}

uint16_t getScreenWidth() {
    return MY_DISP_HOR_RES;
}

uint16_t getScreenHeight() {
    return MY_DISP_VER_RES;
}

#endif
