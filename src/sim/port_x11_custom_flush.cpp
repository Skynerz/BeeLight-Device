#include <lvgl.h>

#if LV_USE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "common/fb_flush.h"

#if COMMON_FLUSH
color_t *internalFb;
#else
static lv_area_t flush_area;
#endif

typedef struct {
    /* header (containing X Display + input user data pointer - keep aligned
     * with x11_input module!) */
    _x11_user_hdr_t hdr;
    /* X11 related information */
    Window window;        /**< X11 window object */
    GC gc;                /**< X11 graphics context object */
    Visual *visual;       /**< X11 visual */
    int dplanes;          /**< X11 display depth */
    XImage *ximage;       /**< X11 XImage cache object for updating window content */
    Atom wmDeleteMessage; /**< X11 atom to window object */
    void *xdata;          /**< allocated data for XImage */
} reduced_x11_disp_data_t;

static inline lv_color32_t get_px(color_t p) {
    lv_color32_t out = {.blue  = static_cast<uint8_t>(p.blue << 3),
                        .green = static_cast<uint8_t>(p.green << 2),
                        .red   = static_cast<uint8_t>(p.red << 3)};
#if INVERT_COLORS
    out.blue  = ~(out.blue);
    out.green = ~(out.green);
    out.red   = ~(out.red);
#endif
    return out;
}

void x11_flush_cb_custom(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
#if COMMON_FLUSH
    fb_flush_callback(disp, area, px_map);
#else  // extracted from lv_x11_display.c
    reduced_x11_disp_data_t *xd = (reduced_x11_disp_data_t *) lv_display_get_driver_data(disp);
    LV_ASSERT_NULL(xd);
    LV_ASSERT_NULL(xd->xdata);

    static const lv_area_t inv_area = {.x1 = 0xFFFF, .y1 = 0xFFFF, .x2 = 0, .y2 = 0};

    flush_area.x1 = MIN(flush_area.x1, area->x1);
    flush_area.x2 = MAX(flush_area.x2, area->x2);
    flush_area.y1 = MIN(flush_area.y1, area->y1);
    flush_area.y2 = MAX(flush_area.y2, area->y2);

    int32_t hor_res = lv_display_get_horizontal_resolution(disp);

    uint32_t dst_offs;
    lv_color32_t *dst_data;
    color_t *src_data = (color_t *) px_map;

    // write into internal framebuffer
    for (int16_t y = area->y1; y <= area->y2; y++) {
        dst_offs = area->x1 + y * hor_res;
        dst_data = &((lv_color32_t *) (xd->xdata))[dst_offs];
        for (int16_t x = area->x1; x <= area->x2; x++, src_data++, dst_data++) {
            *dst_data = get_px(*src_data);
        }
    }

    if (lv_display_flush_is_last(disp)) {
        /* refresh collected display update area only */
        int16_t upd_w = flush_area.x2 - flush_area.x1 + 1;
        int16_t upd_h = flush_area.y2 - flush_area.y1 + 1;
        XPutImage(xd->hdr.display, xd->window, xd->gc, xd->ximage, flush_area.x1, flush_area.y1, flush_area.x1,
                  flush_area.y1, upd_w, upd_h);

        /* invalidate collected area */
        flush_area = inv_area;
    }
    /* Inform the graphics library that you are ready with the flushing */
    lv_display_flush_ready(disp);
#endif
}

static void flush_cb(lv_display_t *drv, const lv_area_t *flush_area, uint8_t *px_map) {
    reduced_x11_disp_data_t *xd = (reduced_x11_disp_data_t *) lv_display_get_driver_data(drv);
    int32_t upd_w, upd_h;
    fb_flush_compute_area(flush_area, &upd_w, &upd_h);

    int32_t hor_res = lv_display_get_horizontal_resolution(drv);

    uint32_t dst_offs;
    lv_color32_t *dst_data;
    color_t *src_data = (color_t *) px_map;
    // write into internal framebuffer
    for (int16_t y = flush_area->y1; y <= flush_area->y2; y++) {
        dst_offs = flush_area->x1 + y * hor_res;
        dst_data = &((lv_color32_t *) (xd->xdata))[dst_offs];
        for (int16_t x = flush_area->x1; x <= flush_area->x2; x++, src_data++, dst_data++) {
            *dst_data = get_px(*src_data);
        }
    }

    XPutImage(xd->hdr.display, xd->window, xd->gc, xd->ximage, flush_area->x1, flush_area->y1, flush_area->x1,
              flush_area->y1, upd_w, upd_h);
}

void x11_flush_init(lv_display_t *disp) {
    lv_display_set_flush_cb(disp, x11_flush_cb_custom);
#if COMMON_FLUSH
    reduced_x11_disp_data_t *xd = (reduced_x11_disp_data_t *) lv_display_get_driver_data(disp);
    int32_t hor_res             = lv_display_get_horizontal_resolution(disp);
    int32_t ver_res             = lv_display_get_vertical_resolution(disp);
    size_t bufSize              = lv_display_get_draw_buf_size(disp) + PX_SIZE(hor_res);
    internalFb                  = (color_t *) malloc(bufSize);
    fb_flush_init(internalFb, bufSize, flush_cb, 1, 2, 2);
#endif
}

void x11_flush_deinit() {
#if COMMON_FLUSH
    free(internalFb);
#endif
}

#endif
