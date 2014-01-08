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
	int idx;
	uint8_t x, y;
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

