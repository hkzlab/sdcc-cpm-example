#include "ansi_term.h"

#include "cbm_sysfunc.h"

#define ESC_CHR 0x1B
#define VT100_ANSI_CMD "\x1B<" // Sets VT100 and compatibles to ANSI mode 

#define ANSI_CLRSCRN "\x1B[2J" // Erase entire screen
#define ANSI_CLRTOEND "\x1B[J" // Erase from cursor to end of screen


void term_sendCommand(char *cmd);

void term_ANSIMode(void) {
	term_sendCommand(VT100_ANSI_CMD);
}

void term_sendCommand(char *cmd) {
	int idx = 0;
	while (cmd[idx] != '\0')
		cbm_putchar(cmd[idx]);
}
