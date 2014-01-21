#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

void draw_box(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void sys_init(void) {
}

/*
static __sfr __at 0x63 IoPPICtrl;
*/

int main() {
	printf("HELLO WORLD!\n");

	term_ANSIMode();
	term_ANSIClrScrn(ed_erase_all);

	draw_box(1, 1, 79, 22);
	draw_box(10, 4, 20, 15);
	draw_box(40, 10, 7, 10);
	draw_box(5, 0, 46, 16);
//	term_ANSIClrScrn(ed_erase_all);
	
	return (EXIT_SUCCESS);
}

void draw_box(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	uint8_t xIdx, yIdx;
	
	term_ANSIDirectCursorAddr(x + 1, y + 1);
	
	cpm_putchar('+');
	for (xIdx = x + 2; xIdx < (x + w); xIdx++) {
		cpm_putchar('-');
	}
	cpm_putchar('+');
	
	term_ANSICursorMove(1, md_move_left);
	term_ANSICursorMove(1, md_move_down);
	for (yIdx = y + 1; yIdx < (y + (h - 1)); yIdx++) {
		cpm_putchar('|');
		term_ANSICursorMove(1, md_move_down);
		term_ANSICursorMove(1, md_move_left);
	}
	cpm_putchar('+');
	
	term_ANSIDirectCursorAddr(x + 1, yIdx + 1);
	cpm_putchar('+');
	for (xIdx = x + 2; xIdx < (x + w); xIdx++) {
		cpm_putchar('-');
	}
	
	term_ANSIDirectCursorAddr(x + 1, y + 1);
	term_ANSICursorMove(1, md_move_down);
	for (yIdx = y + 1; yIdx < (y + (h - 1)); yIdx++) {
		cpm_putchar('|');
		term_ANSICursorMove(1, md_move_down);
		term_ANSICursorMove(1, md_move_left);
	}

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

