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

	setup_modprn(Channel_A, BRate_38400, bpc_8, stop_1, parity_none);
	setup_modprn(Channel_B, BRate_38400, bpc_8, stop_1, parity_none);
}

int main() {
	uint16_t idx = 0x1FFF;
	uint8_t ch;
	sys_init();

	cprintf("Testing MODPRN Channel A\n");

	while(idx--) {
		ch = modprn_getch(Channel_A);
		cprintf("%.2X\r\n",ch);
		modprn_outch(Channel_A, ch);
	}

	return (EXIT_SUCCESS);
}

