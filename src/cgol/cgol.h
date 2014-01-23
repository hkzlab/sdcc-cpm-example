#ifndef _CGOL_HEADER_
#define _CGOL_HEADER_

#include "common_datatypes.h"

#define GRID_WIDTH 70
#define GRID_HEIGHT 20

uint8_t *cgol_getGrid(void);
void cgol_step(void);
void cgol_init(void);

#endif /* _CGOL_HEADER_ */
