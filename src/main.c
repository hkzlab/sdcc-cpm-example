#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

void sys_init(void) {
}

/*
static __sfr __at 0x63 IoPPICtrl;
*/

int main() {
	// Prepare a command to send the BEL character
	BDOSCALL bellcall = { C_WRITE, {(unsigned int)7} };
	LineMode lm;
	int idx;
	uint8_t x, y, ansi_param = 0;
	EraseDir eDir = erase_all;

	sys_init();
	
	printf("HELLO WORLD!\n");

	for (idx = 0; idx < 20; idx++) {
		printf("%d\n", idx);
		cpmbdos(&bellcall); // Make the console beep a bit!
	}

	term_ANSIMode();
	term_ANSIClrScrn(eDir);

	for (x = 0; x < 80; x++) {
		for (y = 0; y < 24; y++) {
			term_ANSIDirectCursorAddr(x, y);
			cpm_putchar('X');
		}
	}
	
	ansi_param = ANSI_P_SET_REVR(ansi_param);	
	term_ANSISetParam(ansi_param);
	term_ANSIDirectCursorAddr(0, 0);
	for (x = 0; x < 80; x++) {
			term_ANSIDirectCursorAddr(x, 0);
			cpm_putchar('.');		
	}

	ansi_param = 0;
	ansi_param = ANSI_P_SET_AOFF(ansi_param);
	term_ANSISetParam(ansi_param);

	term_ANSIDirectCursorAddr(0, 0);
	for (y = 0; y < 24; y++) {
			term_ANSIDirectCursorAddr(0, y);
			if (y % 2)
				 lm = doubleh_top;
			else
				 lm = doubleh_bottom;

			term_ANSILineMode(lm);			
	}

	return (EXIT_SUCCESS);
}

void delay(unsigned char d) {
	d;

	__asm
		; Save used registers
		nop
		; Read parameters from stack
		nop
		; Do the work
		nop
		; Restore saved registers
		nop
	__endasm;
}

