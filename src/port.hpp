#ifndef PORT_HPP
#define PORT_HPP

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

void setup();
void tearDown();
void _usleep(uint32_t usecs);
uint16_t getScreenWidth();
uint16_t getScreenHeight();
extern uint8_t run;

#endif
