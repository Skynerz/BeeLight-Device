#ifndef FLUSH_FB_H
#define FLUSH_FB_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

// #if LV_COLOR_DEPTH == 16
typedef lv_color16_t color_t;

#define COLOR_SIZE sizeof(color_t)
#define PX_SIZE(w) ((w) * COLOR_SIZE)

typedef struct {
    color_t *internalBuffer;
    size_t bufSize;
    lv_area_t flush_area;
    lv_display_flush_cb_t ll_flush_cb;
    uint8_t x_coord_align;
    uint8_t y_coord_align;
    uint8_t can_flush_ready;
} FlushFb_t;

void fb_flush_init(void *internalBuffer, size_t bufSize, lv_display_flush_cb_t ll_flush_cb, uint8_t can_flush_ready,
                   uint8_t x_coord_align, uint8_t y_coord_align);
void fb_flush_callback(lv_display_t *drv, const lv_area_t *area, uint8_t *px_map);
void fb_flush_compute_area(const lv_area_t *area, int32_t *width, int32_t *height);

#ifdef __cplusplus
}
#endif

#endif
