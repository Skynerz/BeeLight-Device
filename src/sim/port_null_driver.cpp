#if CICD_BUILD

#include "port.hpp"

void setup() {
}
void tearDown() {
}
void _usleep(uint32_t usecs) {
}

uint16_t getScreenWidth() {
    return 360;
}

uint16_t getScreenHeight() {
    return 360;
}

#endif
