#ifndef PORT_HPP
#define PORT_HPP

#ifndef __cplusplus
#include <stdint.h>
extern "C" {
#else
#include <cstdint>
#endif

void setup();
void _usleep(uint32_t usecs);
extern uint8_t run;

#ifndef __cplusplus
}
#endif

#endif
