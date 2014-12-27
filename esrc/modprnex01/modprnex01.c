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

	setup_modprn(Channel_A, BRate_9600, bpc_8, stop_1, parity_none);
}

int main() {
	sys_init();

	cprintf("This does NOTHING!!! %u %u %u\n", 1, 2, 3);

	return (EXIT_SUCCESS);
}

