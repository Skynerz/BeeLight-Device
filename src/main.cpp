#include <Arduino.h>
#include <esp_display_panel.hpp>
#include "lvgl_v8_port.h"

#include "ui/ui.h"
#include "ble/ble_config.h"

using namespace esp_panel::drivers;
using namespace esp_panel::board;
using namespace std;


void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing board");
    Board *board = new Board();
    board->init();
    assert(board->begin());

    Serial.println("Initializing LVGL");
    lvgl_port_init(board->getLCD(), board->getTouch());

    Serial.println("Creating UI");
    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    ui_init();
    ble_init();

    /* Release the mutex */
    lvgl_port_unlock();
}

void loop()
{
    delay(1000);
}
