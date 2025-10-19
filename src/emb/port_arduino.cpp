
#include <Arduino.h>
#include <esp_display_panel.hpp>
#include "lvgl_v9_port.h"
#include "BeelightApp.hpp"
#include "BeeLog.hpp"

using namespace esp_panel::drivers;
using namespace esp_panel::board;

static Board* board;
void setup()
{
    BeeLog logger("Port Arduino");
    logger.init();
    logger.info("Initializing board");

    board = new Board();
    ESP_UTILS_CHECK_FALSE_EXIT(board->init(), "Board init failed");
    ESP_UTILS_CHECK_FALSE_EXIT(board->begin(), "Board begin failed");
    
    logger.info("Initializing LVGL");
    //let the hw invert colors
    //board->getLCD()->invertColor(true);
    lv_display_t *disp = nullptr;
    lvgl_port_init(board->getLCD(), board->getTouch(), &disp);

    logger.info("Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    BeelightApp_init();

    /* Release the mutex */
    lvgl_port_unlock();
}

void tearDown() {
    lvgl_port_deinit();
}

void _usleep(uint32_t usecs) {
    delay(usecs / 1000);
}

uint16_t getScreenWidth() {
    return board->getLCD()->getFrameWidth();
}

uint16_t getScreenHeight() {
    return board->getLCD()->getFrameHeight();
}
