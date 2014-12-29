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

//	setup_modprn(Channel_A, BRate_9600, bpc_8, stop_1, parity_none, 1);
	setup_modprn(Channel_B, BRate_9600, bpc_8, stop_1, parity_none, 1);
}

int main() {
	uint8_t idx = 0xFF;
	uint8_t ch;
	sys_init();

	cprintf("Testing MODPRN Channel B\n");

	while(idx--) {
		ch = modprn_getch(Channel_B);
		cprintf("%c",ch);
		modprn_outch(Channel_B, ch);
	}

	return (EXIT_SUCCESS);
}

