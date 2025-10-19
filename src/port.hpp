#ifndef PORT_HPP
#define PORT_HPP

#ifndef __cplusplus
#include <stdint.h>
extern "C" {
#else
#include <cstdint>
#endif

void setup();
void tearDown();
void _usleep(uint32_t usecs);
uint16_t getScreenWidth();
uint16_t getScreenHeight();
extern uint8_t run;

#ifndef __cplusplus
}
#endif

#endif
