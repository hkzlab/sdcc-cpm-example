#ifndef _ANSI_TERM_HEADER_
#define _ANSI_TERM_HEADER_

#include "common_datatypes.h"

typedef enum {
	erase_before = 0,
	erase_after = 1,
	erase_all = 2
} EraseDir;

typedef union {
	uint8_t all_off : 1;
	uint8_t bold_on : 1;
	uint8_t undr_on : 1;
	uint8_t blnk_on : 1;
	uint8_t revr_on : 1;
	uint8_t filler	: 3;
} ANSI_Param;

void term_ANSIMode(void); 

void term_ANSIClrScrn(EraseDir dir);
void term_ANSIClrLine(EraseDir dir);
void term_ANSIDirectCursorAddr(uint8_t column, uint8_t line);
void term_ANSISetParam(uint8_t parm); // Cast from type ANSI_Param

#endif /* _ANSI_TERM_HEADER_ */
