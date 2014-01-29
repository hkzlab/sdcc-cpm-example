#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

#include "cgol/cgol.h"


void draw_box(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void print_step(uint16_t step, uint8_t x, uint8_t y);

void sys_init(void) {
}

/*
static __sfr __at 0x63 IoPPICtrl;
*/

int main() {
	const char *gol_title = "                            Conway's  Game of Life                             ";
	const char *gol_footer = " Step:                                                                         ";
	
	int counter = 200;
	uint8_t *grid, x, y;
	uint16_t cur_step = 0;

	term_ANSIMode();
	term_ANSIClrScrn(ed_erase_all);

	draw_box(0, 1, 63, 22);
	draw_box(62, 1, 17, 22);

	term_ANSISetParam(ANSI_P_SET_REVR(0));
	term_ANSIDirectCursorAddr(1, 1);
	for (x = 0; gol_title[x] != '\0'; x++) {
		cpm_putchar(gol_title[x]);
	}
	
	term_ANSIDirectCursorAddr(1, 24);
	for (x = 0; gol_footer[x] != '\0'; x++) {
		cpm_putchar(gol_footer[x]);
	}
	term_ANSISetParam(ANSI_P_SET_AOFF(0));

	print_step(cur_step, 8, 24);

	cgol_init();

	while (counter--) {
		print_step(cur_step, 8, 24);
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
		cur_step++;
	}

	term_ANSIClrScrn(ed_erase_all);
	
	return (EXIT_SUCCESS);
}

void print_step(uint16_t step, uint8_t x, uint8_t y) {
	uint8_t buf[4];

	buf[0] = ((step >> 12) & 0x0F);
	buf[1] = ((step >>  8) & 0x0F);
	buf[2] = ((step >>  4) & 0x0F);
	buf[3] = ((step >>  0) & 0x0F);

	term_ANSIDirectCursorAddr(x, y);

	term_ANSISetParam(ANSI_P_SET_REVR(0));

	cpm_putchar(buf[0] > 9 ? buf[0] + 0x37 : buf[0] + 0x30);
	cpm_putchar(buf[1] > 9 ? buf[1] + 0x37 : buf[1] + 0x30);
	cpm_putchar(buf[2] > 9 ? buf[2] + 0x37 : buf[2] + 0x30);
	cpm_putchar(buf[3] > 9 ? buf[3] + 0x37 : buf[3] + 0x30);

	term_ANSISetParam(ANSI_P_SET_AOFF(0));
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

