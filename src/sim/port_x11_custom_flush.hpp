#ifndef PORT_X11_CUSTOM_FLUSH
#define PORT_X11_CUSTOM_FLUSH

#include <lvgl.h>

void x11_flush_init(lv_display_t *disp);
void x11_flush_deinit();

#endif
