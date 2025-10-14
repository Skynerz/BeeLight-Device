#ifndef LVGL_V9_PORT_H
#define LVGL_V9_PORT_H

#include <sdkconfig.h>
#ifdef CONFIG_ARDUINO_RUNNING_CORE
#include <Arduino.h>
#endif
#include <esp_display_panel.hpp>
#include <lvgl.h>

/**
 * LVGL related parameters, can be adjusted by users
 */
#define LVGL_PORT_TICK_PERIOD_MS                (2) // The period of the LVGL tick task, in milliseconds

/**
 *
 * LVGL buffer related parameters, can be adjusted by users:
 *
 *  (These parameters will be useless if the avoid tearing function is enabled)
 *
 *  - Memory type for buffer allocation:
 *      - MALLOC_CAP_SPIRAM: Allocate LVGL buffer in PSRAM
 *      - MALLOC_CAP_INTERNAL: Allocate LVGL buffer in SRAM
 *
 *      (The SRAM is faster than PSRAM, but the PSRAM has a larger capacity)
 *      (For SPI/QSPI LCD, it is recommended to allocate the buffer in SRAM, because the SPI DMA does not directly support PSRAM now)
 *
 *  - The size (in bytes) and number of buffers:
 *      - Lager buffer size can improve FPS, but it will occupy more memory. Maximum buffer size is `width * height`.
 *      - The number of buffers should be 1 or 2.
 */
#define LVGL_PORT_BUFFER_MALLOC_CAPS            (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)       // Allocate LVGL buffer in SRAM
// #define LVGL_PORT_BUFFER_MALLOC_CAPS            (MALLOC_CAP_SPIRAM)      // Allocate LVGL buffer in PSRAM

#define LVGL_PORT_BUFFER_SIZE_HEIGHT            (20)
#define LVGL_PORT_BUFFER_NUM                    (2)

/**
 * LVGL timer handle task related parameters, can be adjusted by users
 */
#define LVGL_PORT_TASK_MAX_DELAY_MS             (500)       // The maximum delay of the LVGL timer task, in milliseconds
#define LVGL_PORT_TASK_MIN_DELAY_MS             (2)         // The minimum delay of the LVGL timer task, in milliseconds
#define LVGL_PORT_TASK_STACK_SIZE               (6 * 1024)  // The stack size of the LVGL timer task, in bytes
#define LVGL_PORT_TASK_PRIORITY                 (2)         // The priority of the LVGL timer task
#ifdef ARDUINO_RUNNING_CORE
#define LVGL_PORT_TASK_CORE                     (ARDUINO_RUNNING_CORE)  // Valid if using Arduino
#else
#define LVGL_PORT_TASK_CORE                     (0)                     // Valid if using ESP-IDF
#endif

bool lvgl_port_lock(int timeout_ms);
bool lvgl_port_unlock(void);
bool lvgl_port_init(esp_panel::drivers::LCD *lcd, esp_panel::drivers::Touch *tp, lv_display_t **disp);
bool lvgl_port_deinit(void);

#endif /* LVGL_V9_PORT_H */