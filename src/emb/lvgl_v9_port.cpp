/*
 * SPDX-FileCopyrightText: 2024-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_timer.h"
#undef ESP_UTILS_LOG_TAG
#define ESP_UTILS_LOG_TAG "LvPort"
#include "esp_lib_utils.h"
#include "lvgl_v9_port.h"

using namespace esp_panel::drivers;

#define LVGL_PORT_ENABLE_ROTATION_OPTIMIZED     (1)
#define LVGL_PORT_BUFFER_NUM_MAX                (2)

static SemaphoreHandle_t lvgl_mux = nullptr;                  // LVGL mutex
static TaskHandle_t lvgl_task_handle = nullptr;
static esp_timer_handle_t lvgl_tick_timer = NULL;
static void *lvgl_buf[LVGL_PORT_BUFFER_NUM_MAX] = {};

#if LVGL_PORT_AVOID_TEAR
#if LVGL_PORT_DIRECT_MODE

static void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    LCD *lcd = (LCD *)drv->user_data;

    /* Action after last area refresh */
    if (lv_disp_flush_is_last(drv)) {
        /* Switch the current LCD frame buffer to `color_map` */
        lcd->switchFrameBufferTo(color_map);

        /* Waiting for the last frame buffer to complete transmission */
        ulTaskNotifyValueClear(NULL, ULONG_MAX);
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }

    lv_disp_flush_ready(drv);
}

#elif LVGL_PORT_FULL_REFRESH && LVGL_PORT_DISP_BUFFER_NUM == 2

static void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    LCD *lcd = (LCD *)drv->user_data;

    /* Switch the current LCD frame buffer to `color_map` */
    lcd->switchFrameBufferTo(color_map);

    /* Waiting for the last frame buffer to complete transmission */
    ulTaskNotifyValueClear(NULL, ULONG_MAX);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    lv_disp_flush_ready(drv);
}

#elif LVGL_PORT_FULL_REFRESH && LVGL_PORT_DISP_BUFFER_NUM == 3

void flush_callback(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    LCD *lcd = (LCD *)drv->user_data;

    drv->draw_buf->buf1 = color_map;
    drv->draw_buf->buf2 = lvgl_port_flush_next_buf;
    lvgl_port_flush_next_buf = color_map;

    /* Switch the current LCD frame buffer to `color_map` */
    lcd->switchFrameBufferTo(color_map);

    lvgl_port_lcd_next_buf = color_map;

    lv_disp_flush_ready(drv);
}
#endif

IRAM_ATTR bool onLcdVsyncCallback(void *user_data)
{
    BaseType_t need_yield = pdFALSE;
#if LVGL_PORT_FULL_REFRESH && (LVGL_PORT_DISP_BUFFER_NUM == 3) && (LVGL_PORT_ROTATION_DEGREE == 0)
    if (lvgl_port_lcd_next_buf != lvgl_port_lcd_last_buf) {
        lvgl_port_flush_next_buf = lvgl_port_lcd_last_buf;
        lvgl_port_lcd_last_buf = lvgl_port_lcd_next_buf;
    }
#else
    TaskHandle_t task_handle = (TaskHandle_t)user_data;
    // Notify that the current LCD frame buffer has been transmitted
    xTaskNotifyFromISR(task_handle, ULONG_MAX, eNoAction, &need_yield);
#endif
    return (need_yield == pdTRUE);
}

#else
static void rounder_callback(LCD *lcd, lv_area_t *area)
{
    uint8_t x_align = lcd->getBasicAttributes().basic_bus_spec.x_coord_align;
    uint8_t y_align = lcd->getBasicAttributes().basic_bus_spec.y_coord_align;

    if (x_align > 1) {
        // round the start of coordinate down to the nearest aligned value
        area->x1 &= ~(x_align - 1);
        // round the end of coordinate up to the nearest aligned value
        area->x2 = (area->x2 & ~(x_align - 1)) + x_align - 1;
    }

    if (y_align > 1) {
        // round the start of coordinate down to the nearest aligned value
        area->y1 &= ~(y_align - 1);
        // round the end of coordinate up to the nearest aligned value
        area->y2 = (area->y2 & ~(y_align - 1)) + y_align - 1;
    }
}

void flush_callback(lv_display_t *drv, const lv_area_t *area, uint8_t *px_map)
{
    LCD *lcd = (LCD *)lv_display_get_driver_data(drv);
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

    if ((lcd->getBasicAttributes().basic_bus_spec.x_coord_align > 1) ||
        (lcd->getBasicAttributes().basic_bus_spec.y_coord_align > 1))
    {
        rounder_callback(lcd, (lv_area_t *)area);
    }

    lcd->drawBitmap(offsetx1, offsety1, offsetx2 - offsetx1 + 1, offsety2 - offsety1 + 1, (const uint8_t *)px_map);
    // For RGB LCD, directly notify LVGL that the buffer is ready
    if (lcd->getBus()->getBasicAttributes().type == ESP_PANEL_BUS_TYPE_RGB) {
        lv_disp_flush_ready(drv);
    }
}
#endif /* LVGL_PORT_AVOID_TEAR */

static lv_display_t *display_init(LCD *lcd)
{
    ESP_UTILS_CHECK_FALSE_RETURN(lcd != nullptr, nullptr, "Invalid LCD device");
    ESP_UTILS_CHECK_FALSE_RETURN(lcd->getRefreshPanelHandle() != nullptr, nullptr, "LCD device is not initialized");

    static lv_display_t * disp;

    // Alloc draw buffers used by LVGL
    auto lcd_width = lcd->getFrameWidth();
    auto lcd_height = lcd->getFrameHeight();
    int buffer_size = 0;

    ESP_UTILS_LOGD("Malloc memory for LVGL buffer");
#if !LVGL_PORT_AVOID_TEAR
    // Avoid tearing function is disabled
#if (LVGL_PORT_BUFFER_MALLOC_CAPS == MALLOC_CAP_SPIRAM)
    buffer_size = lcd_width * lcd_height;
#else
    buffer_size = lcd_width * LVGL_PORT_BUFFER_SIZE_HEIGHT;
#endif
    for (int i = 0; (i < LVGL_PORT_BUFFER_NUM) && (i < LVGL_PORT_BUFFER_NUM_MAX); i++) {
        lvgl_buf[i] = heap_caps_malloc(buffer_size * sizeof(lv_color_t), LVGL_PORT_BUFFER_MALLOC_CAPS);
        assert(lvgl_buf[i]);
        ESP_UTILS_LOGD("Buffer[%d] address: %p, size: %d", i, lvgl_buf[i], buffer_size * sizeof(lv_color_t));
    }
#else
    // To avoid the tearing effect, we should use at least two frame buffers: one for LVGL rendering and another for LCD refresh
    buffer_size = lcd_width * lcd_height;
#if (LVGL_PORT_DISP_BUFFER_NUM >= 3) && (LVGL_PORT_ROTATION_DEGREE == 0) && LVGL_PORT_FULL_REFRESH

    // With the usage of three buffers and full-refresh, we always have one buffer available for rendering,
    // eliminating the need to wait for the LCD's sync signal
    lvgl_port_lcd_last_buf = lcd->getFrameBufferByIndex(0);
    lvgl_buf[0] = lcd->getFrameBufferByIndex(1);
    lvgl_buf[1] = lcd->getFrameBufferByIndex(2);
    lvgl_port_lcd_next_buf = lvgl_port_lcd_last_buf;
    lvgl_port_flush_next_buf = lvgl_buf[1];

#elif (LVGL_PORT_DISP_BUFFER_NUM >= 3) && (LVGL_PORT_ROTATION_DEGREE != 0)

    lvgl_buf[0] = lcd->getFrameBufferByIndex(2);

#elif LVGL_PORT_DISP_BUFFER_NUM >= 2

    for (int i = 0; (i < LVGL_PORT_DISP_BUFFER_NUM) && (i < LVGL_PORT_BUFFER_NUM_MAX); i++) {
        lvgl_buf[i] = lcd->getFrameBufferByIndex(i);
    }

#endif
#endif /* LVGL_PORT_AVOID_TEAR */

    disp = lv_display_create(lcd_width, lcd_height);
    lv_display_set_flush_cb(disp, flush_callback);
    lv_display_set_driver_data(disp, (void *)lcd);
#if (LVGL_PORT_BUFFER_MALLOC_CAPS == MALLOC_CAP_SPIRAM)
    lv_display_set_buffers(disp, lvgl_buf[0], lvgl_buf[1], buffer_size * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_DIRECT);
#else
    lv_display_set_buffers(disp, lvgl_buf[0], lvgl_buf[1], buffer_size * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif
    return disp;
}

static void touchpad_read(lv_indev_t *indev_drv, lv_indev_data_t *data)
{
    Touch *tp = (Touch *) lv_indev_get_driver_data(indev_drv);
    TouchPoint point;

    /* Read data from touch controller */
    int read_touch_result = tp->readPoints(&point, 1, 0);
    if (read_touch_result > 0) {
        data->point.x = point.x;
        data->point.y = point.y;
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static lv_indev_t *indev_init(Touch *tp)
{
    ESP_UTILS_CHECK_FALSE_RETURN(tp != nullptr, nullptr, "Invalid touch device");
    ESP_UTILS_CHECK_FALSE_RETURN(tp->getPanelHandle() != nullptr, nullptr, "Touch device is not initialized");

    static lv_indev_t* indev_drv_tp;

    ESP_UTILS_LOGD("Register input driver to LVGL");
    indev_drv_tp = lv_indev_create();
    lv_indev_set_type(indev_drv_tp, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_drv_tp, touchpad_read);
    lv_indev_set_driver_data(indev_drv_tp, (void *)tp);

    return indev_drv_tp;
}

#if !LV_TICK_CUSTOM
static void tick_increment(void *arg)
{
    /* Tell LVGL how many milliseconds have elapsed */
    lv_tick_inc(LVGL_PORT_TICK_PERIOD_MS);
}

static bool tick_init(void)
{
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &tick_increment,
        .name = "LVGL tick"
    };
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer), false, "Create LVGL tick timer failed"
    );
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_timer_start_periodic(lvgl_tick_timer, LVGL_PORT_TICK_PERIOD_MS * 1000), false,
        "Start LVGL tick timer failed"
    );

    return true;
}

static bool tick_deinit(void)
{
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_timer_stop(lvgl_tick_timer), false, "Stop LVGL tick timer failed"
    );
    ESP_UTILS_CHECK_ERROR_RETURN(
        esp_timer_delete(lvgl_tick_timer), false, "Delete LVGL tick timer failed"
    );
    return true;
}
#endif

static void lvgl_port_task(void *arg)
{
    ESP_UTILS_LOGD("Starting LVGL task");

    uint32_t task_delay_ms = LVGL_PORT_TASK_MAX_DELAY_MS;
    while (1) {
        if (lvgl_port_lock(-1)) {
            task_delay_ms = lv_timer_handler();
            lvgl_port_unlock();
        }
        if (task_delay_ms > LVGL_PORT_TASK_MAX_DELAY_MS) {
            task_delay_ms = LVGL_PORT_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < LVGL_PORT_TASK_MIN_DELAY_MS) {
            task_delay_ms = LVGL_PORT_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

IRAM_ATTR bool onDrawBitmapFinishCallback(void *user_data)
{
    lv_display_t *disp = (lv_display_t *)user_data;

    lv_disp_flush_ready(disp);

    return false;
}

bool lvgl_port_init(LCD *lcd, Touch *tp, lv_display_t **out_disp)
{
    ESP_UTILS_CHECK_FALSE_RETURN(lcd != nullptr, false, "Invalid LCD device");

    auto bus_type = lcd->getBus()->getBasicAttributes().type;
#if LVGL_PORT_AVOID_TEAR
    ESP_UTILS_CHECK_FALSE_RETURN(
        (bus_type == ESP_PANEL_BUS_TYPE_RGB) || (bus_type == ESP_PANEL_BUS_TYPE_MIPI_DSI), false,
        "Avoid tearing function only works with RGB/MIPI-DSI LCD now"
    );
    ESP_UTILS_LOGI(
        "Avoid tearing is enabled, mode: %d, rotation: %d", LVGL_PORT_AVOID_TEARING_MODE, LVGL_PORT_ROTATION_DEGREE
    );
#endif

    lv_display_t *disp = nullptr;
    lv_indev_t *indev = nullptr;

    lv_init();
#if !LV_TICK_CUSTOM
    ESP_UTILS_CHECK_FALSE_RETURN(tick_init(), false, "Initialize LVGL tick failed");
#endif

    ESP_UTILS_LOGI("Initializing LVGL display driver");
    disp = display_init(lcd);
    ESP_UTILS_CHECK_NULL_RETURN(disp, false, "Initialize LVGL display driver failed");
    // Record the initial rotation of the display
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_0);

    // For non-RGB LCD, need to notify LVGL that the buffer is ready when the refresh is finished
    if (bus_type != ESP_PANEL_BUS_TYPE_RGB) {
        ESP_UTILS_LOGD("Attach refresh finish callback to LCD");
        lcd->attachDrawBitmapFinishCallback(onDrawBitmapFinishCallback, (void *) disp);
    }

    if (tp != nullptr) {
        ESP_UTILS_LOGD("Initialize LVGL input driver");
        indev = indev_init(tp);
        ESP_UTILS_CHECK_NULL_RETURN(indev, false, "Initialize LVGL input driver failed");
    }

    ESP_UTILS_LOGD("Create mutex for LVGL");
    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    ESP_UTILS_CHECK_NULL_RETURN(lvgl_mux, false, "Create LVGL mutex failed");

    ESP_UTILS_LOGD("Create LVGL task");
    BaseType_t core_id = (LVGL_PORT_TASK_CORE < 0) ? tskNO_AFFINITY : LVGL_PORT_TASK_CORE;
    BaseType_t ret = xTaskCreatePinnedToCore(lvgl_port_task, "lvgl", LVGL_PORT_TASK_STACK_SIZE, NULL,
                     LVGL_PORT_TASK_PRIORITY, &lvgl_task_handle, core_id);
    ESP_UTILS_CHECK_FALSE_RETURN(ret == pdPASS, false, "Create LVGL task failed");

#if LVGL_PORT_AVOID_TEAR
    lcd->attachRefreshFinishCallback(onLcdVsyncCallback, (void *)lvgl_task_handle);
#endif

    *out_disp = disp;
    return true;
}

bool lvgl_port_lock(int timeout_ms)
{
    ESP_UTILS_CHECK_NULL_RETURN(lvgl_mux, false, "LVGL mutex is not initialized");

    const TickType_t timeout_ticks = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return (xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE);
}

bool lvgl_port_unlock(void)
{
    ESP_UTILS_CHECK_NULL_RETURN(lvgl_mux, false, "LVGL mutex is not initialized");

    xSemaphoreGiveRecursive(lvgl_mux);

    return true;
}

bool lvgl_port_deinit(void)
{
#if !LV_TICK_CUSTOM
    ESP_UTILS_CHECK_FALSE_RETURN(tick_deinit(), false, "Deinitialize LVGL tick failed");
#endif

    ESP_UTILS_CHECK_FALSE_RETURN(lvgl_port_lock(-1), false, "Lock LVGL failed");
    if (lvgl_task_handle != nullptr) {
        vTaskDelete(lvgl_task_handle);
        lvgl_task_handle = nullptr;
    }
    ESP_UTILS_CHECK_FALSE_RETURN(lvgl_port_unlock(), false, "Unlock LVGL failed");

#if LV_ENABLE_GC || !LV_MEM_CUSTOM
    lv_deinit();
#else
    ESP_UTILS_LOGW("LVGL memory is custom, `lv_deinit()` will not work");
#endif
#if !LVGL_PORT_AVOID_TEAR
    for (int i = 0; i < LVGL_PORT_BUFFER_NUM; i++) {
        if (lvgl_buf[i] != nullptr) {
            free(lvgl_buf[i]);
            lvgl_buf[i] = nullptr;
        }
    }
#endif
    if (lvgl_mux != nullptr) {
        vSemaphoreDelete(lvgl_mux);
        lvgl_mux = nullptr;
    }

    return true;
}
