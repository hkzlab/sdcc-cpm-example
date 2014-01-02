#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "portab.h"
#ifndef __GNUC__
#include "cpmbdos.h"
#include "cprintf.h"
#endif

/* THESE ARE USED BY THE LIBRARY ROUTINES */
#ifndef __GNUC__
char getchar(void)
{
        struct BDOSCALL cread = { C_READ, { (unsigned int)0 } };
        return cpmbdos(&cread);
}
void outchar(char c)
{
        struct BDOSCALL cwrite = { C_WRITE, { (unsigned int)c } };
        cpmbdos(&cwrite);
}
#endif

void sys_init(void) {
	cpmbdos_init_structs(); // Initialize CP/M BDOS interface structures
}

int main()
{
	int idx;

	sys_init();

	printf("HELLO WORLD!\n");

	for (idx = 0; idx < 100; idx++) {
		printf("%d\n", idx);
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

