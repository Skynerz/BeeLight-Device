#include "fb_flush.h"

#include <lvgl.h>
#include <string.h>

#include "beelog_wrapper.h"

#if LV_COLOR_DEPTH == 16
typedef lv_color16_t color_t;
#else
typedef lv_color_t color_t;
#endif

FlushFb_t s_flush;

void fb_flush_compute_area(const lv_area_t *area, int32_t *width, int32_t *height) {
    *width  = area->x2 - area->x1 + 1;
    *height = area->y2 - area->y1 + 1;
}

void fb_flush_init(void *internalBuffer, size_t bufSize, lv_display_flush_cb_t ll_flush_cb, uint8_t can_flush_ready,
                   uint8_t x_coord_align, uint8_t y_coord_align) {
    s_flush.internalBuffer  = internalBuffer;
    s_flush.bufSize         = bufSize;
    s_flush.ll_flush_cb     = ll_flush_cb;
    s_flush.x_coord_align   = x_coord_align;
    s_flush.y_coord_align   = y_coord_align;
    s_flush.can_flush_ready = can_flush_ready;
}

// from lvgl_port
static void rounder_callback(lv_area_t *area) {
    uint8_t x_align = s_flush.x_coord_align;
    uint8_t y_align = s_flush.y_coord_align;

    if (x_align > 1) {
        // round the start of coordinate down to the nearest aligned value
        area->x1 &= ~(x_align - 1);
        // round the end of coordinate down to the nearest aligned value
        area->x2 = (area->x2 & ~(x_align - 1)) + (x_align - 1);
    }

    if (y_align > 1) {
        // round the start of coordinate down to the nearest aligned value
        area->y1 &= ~(y_align - 1);
        // round the end of coordinate down to the nearest aligned value
        area->y2 = (area->y2 & ~(y_align - 1)) + (y_align - 1);
    }
}

void fb_flush_callback(lv_display_t *drv, const lv_area_t *area, uint8_t *px_map) {
    const int32_t hor_res = lv_display_get_horizontal_resolution(drv);
    int32_t width, height;
    fb_flush_compute_area(area, &width, &height);
    lv_area_t flush_area;
    int32_t flush_width, flush_height;
    memcpy(&flush_area, area, sizeof(lv_area_t));

    if ((s_flush.x_coord_align > 1) || (s_flush.y_coord_align > 1)) {
        rounder_callback(&flush_area);
    }

    fb_flush_compute_area(&flush_area, &flush_width, &flush_height);
    uint8_t flushAreaTooBig = ((flush_width * flush_height) > s_flush.bufSize ? 1 : 0);
    if (((width >= flush_width) && (height >= flush_height)) || flushAreaTooBig)  // same size or too big
    {
        if (flushAreaTooBig) {
            beelog_print(BEELOG_LEVEL_WARN, __FILE__, "area to flush is bigger than internal buffer %dx%d", flush_width,
                         flush_height);
        }
        s_flush.ll_flush_cb(drv, area, px_map);
    } else {
        // write into internal framebuffer
        memcpy(s_flush.internalBuffer, px_map, PX_SIZE(width * height));

        // beelog_print(BEELOG_LEVEL_DEBUG, __FILE__, "r(%d:%d) -> (%d:%d)
        // %dx%d", flush_area.x1, flush_area.y1, flush_area.x2, flush_area.y2,
        // flush_width, flush_height);

        // reconstruct area to flush from internal framebuffer
        color_t *src_data = s_flush.internalBuffer;
        color_t *dst_data = (color_t *) px_map;
        for (int32_t y = 0; y < flush_height; y++) {
            if (y < height) {
                memcpy(dst_data, src_data, PX_SIZE(width));
                // beelog_print(BEELOG_LEVEL_DEBUG, __FILE__, "dst_data %p",
                // dst_data); copy pixel from left
                for (int32_t x = width; x < flush_width; x++) {
                    dst_data[x] = dst_data[x - 1];
                }
                src_data += width;
            } else {
                // copy last line
                color_t *pPrevLine = &((color_t *) px_map)[(y - 1) * flush_width];
                // beelog_print(BEELOG_LEVEL_DEBUG, __FILE__, "pLastLine %p",
                // pPrevLine);
                memcpy(dst_data, pPrevLine, PX_SIZE(flush_width));
            }
            dst_data += flush_width;
        }
        s_flush.ll_flush_cb(drv, &flush_area, px_map);
    }

    if (s_flush.can_flush_ready) {
        /* Inform the graphics library that you are ready with the flushing */
        lv_display_flush_ready(drv);
    }
}
