#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

#include "hw/modprn02/hw_modprn02.h"

void sys_init(void) {
	cpm_sysfunc_init();

	setup_modprn(Channel_A, BRate_19200, bpc_8, stop_1, parity_none);
	setup_modprn(Channel_B, BRate_19200, bpc_8, stop_1, parity_none);
}

#define CHAR_BUF_SIZE 64

int main() {
	uint16_t idx = 0x1FFF;
	uint8_t buf[CHAR_BUF_SIZE], read_size, b_idx;
//	uint8_t ch;
	
	sys_init();

	cprintf("Testing MODPRN Channel A\n");
/*
	while(idx--) {
		ch = modprn_getch(Channel_A);
		modprn_outch(Channel_A, ch);
	}
*/

	while(idx--) {
		read_size = modprn_getchBuf(Channel_A, buf, CHAR_BUF_SIZE);
		for(b_idx = 0; b_idx < read_size; b_idx++) {
			modprn_outch(Channel_A, buf[b_idx]);
		}
	}

	return (EXIT_SUCCESS);
}

