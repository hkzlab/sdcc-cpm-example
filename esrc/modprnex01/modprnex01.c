#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

#include "hw/common/hw_common.h"
#include "hw/modprn02/hw_modprn02.h"

void sys_init(void) {
	cpm_sysfunc_init();

	cprintf("Init...\r\n");
	
	hw_setupInterrupts();
	
	setup_modprn(Channel_A, BRate_38400, bpc_8, stop_1, parity_none);
	setup_modprn(Channel_B, BRate_38400, bpc_8, stop_1, parity_none);
	modprn_setupInterrupts(0x00);

	hw_enableInterrupts();
}

int main() {
	uint16_t idx = 0x0FFF;
	uint8_t ch;

	sys_init();

/*
	while(idx--) {
		ch = modprn_getch(Channel_A);
		modprn_outch(Channel_A, ch);
	}
*/


	while(idx--) {
		modprn_outch(Channel_A, modprn_int_getch(Channel_A));
	}

	return (EXIT_SUCCESS);
}
