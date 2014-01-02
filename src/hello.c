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
	sys_init();

	printf("HELLO WORLD!\n");

	return (EXIT_SUCCESS);
}

