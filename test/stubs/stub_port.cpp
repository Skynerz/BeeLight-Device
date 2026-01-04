#include "port.hpp"

void __attribute__((weak)) setup() {
}

void __attribute__((weak)) tearDown() {
}

void __attribute__((weak)) _usleep(uint32_t usecs) {
}

uint16_t __attribute__((weak)) getScreenWidth() {
    return 0;
}

uint16_t __attribute__((weak)) getScreenHeight() {
    return 0;
}
