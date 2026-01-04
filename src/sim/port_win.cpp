#if __WIN32__

#include <Windows.h>

#include "BeeLog.hpp"
#include "BeelightApp.hpp"
#include "port.hpp"

#define MY_DISP_HOR_RES (int32_t) 360
#define MY_DISP_VER_RES (int32_t) 360

// TODO ?
#define lvgl_port_lock(x)
#define lvgl_port_unlock()

static lv_indev_t *mouse_m;
static lv_timer_t *input_timer_m;
static lv_obj_t *hand_m;
static lv_display_t *disp = NULL;
static void input_handler(lv_timer_t *timer);
LV_IMAGE_DECLARE(img_hand_cursor);

void setup() {
    BeeLog::debug("init", "Initializing LVGL");
    lv_init();

    disp = lv_sdl_window_create(MY_DISP_HOR_RES, MY_DISP_VER_RES);
    lv_sdl_window_set_resizeable(disp, false);
    lv_sdl_window_set_title(disp, "beelight-simulator");

    mouse_m       = lv_sdl_mouse_create();
    input_timer_m = lv_timer_create(&input_handler, 50, NULL);

    hand_m = lv_image_create(lv_screen_active());
    lv_image_set_src(hand_m, &img_hand_cursor);

    BeeLog::debug("init", "Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    BeelightApp::instance();

    /* Release the mutex */
    lvgl_port_unlock();
}

void tearDown() {
    lv_sdl_quit();
    lv_deinit();
    BeeLog::debug("deinit", "Deinitialized LVGL");
}

void _usleep(uint32_t usecs) {
    Sleep(usecs / 1000);
}

uint16_t getScreenWidth() {
    return MY_DISP_HOR_RES;
}

uint16_t getScreenHeight() {
    return MY_DISP_VER_RES;
}

void plop(std::string msg);

static void input_handler(lv_timer_t *) {
    lv_point_t point;
    lv_indev_get_point(mouse_m, &point);
    lv_obj_set_parent(hand_m, lv_display_get_layer_sys(disp));
    lv_obj_set_pos(hand_m, point.x - 5, point.y);
}
#endif
