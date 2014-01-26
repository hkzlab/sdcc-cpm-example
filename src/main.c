#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

#include "cgol/cgol.h"


void draw_box(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

void sys_init(void) {
}

/*
static __sfr __at 0x63 IoPPICtrl;
*/

int main() {
	const char *gol_title = "                           Conway's  Game of Life                           ";
	
	int counter = 10;
	uint8_t *grid, x, y;

	term_ANSIMode();
	term_ANSIClrScrn(ed_erase_all);

	draw_box(0, 1, 54, 22);
	draw_box(53, 1, 23, 22);

	term_ANSIDirectCursorAddr(1, 1);
	term_ANSISetParam(ANSI_P_SET_REVR(0));

	for (x = 0; gol_title[x] != '\0'; x++) {
		cpm_putchar(gol_title[x]);
	}

	term_ANSISetParam(ANSI_P_SET_AOFF(0));

	cgol_init();

	while (counter--) {
		grid = cgol_getGrid();

	
		for (y = 0; y < GRID_HEIGHT; y++) {
			term_ANSIDirectCursorAddr(2, y + 3);

			for (x = 0; x < GRID_WIDTH; x++) {
				if (grid[x + (y * GRID_WIDTH)]) cpm_putchar('X');
				else cpm_putchar(' ');
			}
		}

		//printf("address %.4X\n", grid);
		cgol_step();
	}

	term_ANSIClrScrn(ed_erase_all);
	
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

