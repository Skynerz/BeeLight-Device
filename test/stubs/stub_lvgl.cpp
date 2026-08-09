#include <lvgl.h>

uint8_t screenLoadAnimCalled = 0;
uint8_t screenLoadCalled     = 0;
uint8_t addEventCbCalled     = 0;

const lv_font_t lv_font_montserrat_28 = {};

const lv_font_t lv_font_montserrat_26 = {};

const lv_font_t lv_font_montserrat_20 = {};

void __attribute__((weak)) lv_screen_load_anim(lv_obj_t *, lv_screen_load_anim_t, uint32_t, uint32_t, bool) {
    screenLoadAnimCalled++;
}

void __attribute__((weak)) lv_screen_load(struct _lv_obj_t *) {
    screenLoadCalled++;
}

lv_obj_t *__attribute__((weak)) lv_obj_create(lv_obj_t *) {
    return nullptr;
}

void __attribute__((weak)) lv_obj_delete(lv_obj_t *) {
}

void __attribute__((weak)) lv_obj_set_size(lv_obj_t *, int32_t, int32_t) {
}

void __attribute__((weak)) lv_obj_set_style_bg_color(lv_obj_t *obj, lv_color_t value, lv_style_selector_t selector) {
}

void __attribute__((weak)) lv_obj_set_style_radius(lv_obj_t *obj, int32_t value, lv_style_selector_t selector) {
}

lv_color_t __attribute__((weak)) lv_color_hex(uint32_t) {
    lv_color_t ret;
    return ret;
}

void __attribute__((weak)) lv_log_register_print_cb(lv_log_print_g_cb_t) {
}

void __attribute__((weak)) lv_obj_dump_tree(lv_obj_t *) {
}

lv_event_dsc_t *__attribute__((weak)) lv_obj_add_event_cb(lv_obj_t *, lv_event_cb_t, lv_event_code_t, void *) {
    addEventCbCalled++;
    return NULL;
}

void *__attribute__((weak)) lv_event_get_user_data(lv_event_t *e) {
    return NULL;
}

void __attribute__((weak)) lv_label_set_text(lv_obj_t *obj, const char *text) {
}

int32_t __attribute__((weak)) lv_obj_get_width(const lv_obj_t *obj) {
    return 0;
}

lv_obj_t *__attribute__((weak)) lv_label_create(lv_obj_t *parent) {
    return (lv_obj_t*) 0x1234567;
}

void __attribute__((weak)) lv_obj_align(lv_obj_t *obj, lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
}

void __attribute__((weak)) lv_obj_set_style_text_font(lv_obj_t *obj, const lv_font_t *value,
                                                      lv_style_selector_t selector) {
                                                        
}

int32_t __attribute__((weak)) lv_obj_get_scroll_x(const lv_obj_t *obj) {
    return 0;
}

lv_obj_t *__attribute__((weak)) lv_event_get_current_target_obj(lv_event_t *e) {
    return nullptr;
}

lv_obj_t *__attribute__((weak)) lv_button_create(lv_obj_t *parent) {
    return nullptr;
}

void __attribute__((weak)) lv_obj_set_scrollbar_mode(lv_obj_t *obj, lv_scrollbar_mode_t mode) {
}

void __attribute__((weak)) lv_obj_set_scroll_dir(lv_obj_t *obj, lv_dir_t dir) {
}

void __attribute__((weak)) lv_obj_set_scroll_snap_x(lv_obj_t *obj, lv_scroll_snap_t align) {
}

void __attribute__((weak)) lv_obj_set_flag(lv_obj_t *obj, lv_obj_flag_t f, bool v) {
}

void __attribute__((weak)) lv_obj_align_to(lv_obj_t *obj, const lv_obj_t *base, lv_align_t align, int32_t x_ofs,
                                           int32_t y_ofs) {
}
