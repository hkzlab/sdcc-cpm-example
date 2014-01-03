#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __GNUC__
#include "cpmbdos.h"
#include "cprintf.h"
#endif

/* THESE ARE USED BY THE LIBRARY ROUTINES */
#ifndef __GNUC__
char getchar(void) {
	struct BDOSCALL cread = { C_READ, { (unsigned int)0 } };
	return cpmbdos(&cread);
}
void outchar(char c) {
	struct BDOSCALL cwrite = { C_WRITE, { (unsigned int)c } };
	cpmbdos(&cwrite);
}
#endif

void sys_init(void) {
}

int main() {
	// Prepare a command to send the BEL character
	struct BDOSCALL bellcall = { C_WRITE, {(unsigned int)7} };
	int idx;
	
	sys_init();
	
	printf("HELLO WORLD!\n");

	for (idx = 0; idx < 20; idx++) {
		printf("%d\n", idx);
		cpmbdos(&bellcall); // Make the console beep a bit!
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

