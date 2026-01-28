
#include <lvgl.h>

#include "port.hpp"
#include "ui/Dashboard.hpp"

using namespace std;

uint8_t run = 1;

void loop() {
    while (run) {
        uint32_t delay;
#ifdef SIMULATOR
        delay = lv_timer_handler();
        if (delay < 1)
            delay = 1; /*delay for at least 1 ms*/
        else if (delay == LV_NO_TIMER_READY)
            delay = LV_DEF_REFR_PERIOD; /*handle LV_NO_TIMER_READY. Another
                                           option is to `sleep` for longer*/
#else
        delay = 1000;
#endif
        _usleep(delay * 1000);
    }
}

#if __WIN32__
int WinMain(void)
#else
int main(void)
#endif
{
    setup();
    loop();
    tearDown();
    return EXIT_SUCCESS;
}
