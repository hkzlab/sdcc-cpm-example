#include "ansi_term.h"

#include "cbm_sysfunc.h"

#define ESC_CHR 0x1B
#define VT100_ANSI_CMD "<" // Sets VT100 and compatibles to ANSI mode 

#define ANSI_CLRSCRN	"[2J"	// Erase entire screen
#define ANSI_CLRREMSCRN	"[J"	// Erase from cursor to end of screen
#define ANSI_CLRREMLNE	"[K"	// Erase from cursor to end of line
#define ANSI_CLRBEGLNE	"[1K"	// Erase from beginning of line to cursor
#define ANSI_CRLNE		"[2K"	// Erase line containing cursor


void term_sendCommand(char *cmd);

void term_ANSIMode(void) {
	term_sendCommand(VT100_ANSI_CMD);
}

void term_sendCommand(char *cmd) {
	int idx = 0;

	cbm_putchar(ESC_CHR);
	while (cmd[idx] != '\0')
		cbm_putchar(cmd[idx]);
}
