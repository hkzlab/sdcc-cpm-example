#ifndef _ANSI_TERM_HEADER_
#define _ANSI_TERM_HEADER_

#include "common_datatypes.h"

typedef enum {
	erase_before = 0,
	erase_after = 1,
	erase_all = 2
} EraseDir;

typedef enum {
	move_up = 0,
	move_down = 1,
	move_left = 2,
	move_right = 3
} ModeDir;

void term_ANSIMode(void); 

void term_ANSIClrScrn(EraseDir dir);
void term_ANSIClrLine(EraseDir dir);
void term_ANSIDirectCursorAddr(uint8_t column, uint8_t line);
void term_ANSICursorMove(uint8_t spaces, ModeDir dir);

// prm format:
// BIT 0 -> all off
// BIT 1 -> bold on
// BIT 2 -> underscore on
// BIT 3 -> blink on
// BIT 4 -> reverse on
void term_ANSISetParam(uint8_t prm);

#endif /* _ANSI_TERM_HEADER_ */
