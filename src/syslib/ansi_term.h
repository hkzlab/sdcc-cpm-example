#ifndef _ANSI_TERM_HEADER_
#define _ANSI_TERM_HEADER_

#include "common_datatypes.h"

typedef enum {
	erase_before = 0,
	erase_after = 1,
	erase_all = 2
} EraseDir;

void term_ANSIMode(void); 

void term_ANSIClrScrn(EraseDir dir);
void term_ANSIClrLine(EraseDir dir);
void term_ANSIDirectCursorAddr(uint8_t column, uint8_t line);

#endif /* _ANSI_TERM_HEADER_ */
