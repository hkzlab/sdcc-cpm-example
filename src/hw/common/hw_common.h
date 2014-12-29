#ifndef _HWLIB_COMMON_HEADER_
#define _HWLIB_COMMON_HEADER_

#include "common_datatypes.h"

void hw_outp(uint8_t port, uint8_t data);
uint8_t hw_inp(uint8_t port) __naked;

void hw_smallDelay(uint8_t delay);

#endif /* _HWLIB_COMMON_HEADER_ */
