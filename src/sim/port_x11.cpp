#include <lvgl.h>
#include "port.hpp" 
#include "BeeLog.hpp"
#include "ui/ui.h"
#include <unistd.h> //todo windows alt

#define MY_DISP_HOR_RES (int32_t) 360
#define MY_DISP_VER_RES (int32_t) 360

//TODO ?
#define lvgl_port_lock(x)
#define lvgl_port_unlock()

void setup() {
    BeeLog::debug("init", "Initializing LVGL");
    lv_display_t *disp = NULL;
    LV_IMAGE_DECLARE(img_hand_cursor);

    disp = lv_x11_window_create("beelight-simulator", MY_DISP_HOR_RES, MY_DISP_VER_RES);
    
    /* initialize X11 input drivers (for keyboard, mouse & mousewheel) */
    lv_x11_inputs_create(disp, &img_hand_cursor);

    BeeLog::debug("init", "Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    ui_init();

    /* Release the mutex */
    lvgl_port_unlock();
}

void _usleep(uint32_t usecs) {
    usleep(usecs);
}