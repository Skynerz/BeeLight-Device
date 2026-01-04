#include <lvgl.h>

uint8_t screenLoadAnimCalled = 0;
uint8_t screenLoadCalled     = 0;
uint8_t addEventCbCalled     = 0;

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
