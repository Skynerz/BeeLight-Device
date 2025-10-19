
#include "ui/ui.h"
#include <lvgl.h>
#include "port.hpp"

using namespace std;

static void loop()
{
    while (1)
    {
        uint32_t delay = lv_timer_handler();
        if (delay < 1)
            delay = 1; /*delay for at least 1 ms*/
        else if (delay == LV_NO_TIMER_READY)
            delay = LV_DEF_REFR_PERIOD; /*handle LV_NO_TIMER_READY. Another option is to `sleep` for longer*/
        _usleep(delay * 1000);
    }
}

int main(void)
{
    lv_init();
    setup();
    loop();
    lv_deinit();

    return EXIT_SUCCESS;
}
