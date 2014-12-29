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

	setup_modprn(Channel_A, BRate_9600, bpc_8, stop_1, parity_none, 1);
}

int main() {
	const char *test_string = "TEST A B C D!\r\n\n";
	int idx;

	sys_init();

	cprintf("Testing MODPRN Channel A\n");

	idx = 0;
	while(test_string[idx] != 0) {
		modprn_outch(Channel_A, test_string[idx]);
		idx++;
	}
	idx = 0;
	while(idx < 10) {
		cprintf("%c\n", modprn_getch(Channel_A));
		idx++;	
	}

	return (EXIT_SUCCESS);
}

