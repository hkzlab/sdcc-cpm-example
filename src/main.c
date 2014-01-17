#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

void sys_init(void) {
}

/*
static __sfr __at 0x63 IoPPICtrl;
*/

int main() {
	static uint8_t dma_buf[128];

	// Prepare a command to send the BEL character
	BDOSCALL bellcall = { C_WRITE, {(unsigned int)7} };
	FCB cb;
	int idx;
	uint8_t x, y, ansi_param = 0;

	sys_init();

	cpm_setDMAAddr((uint16_t)dma_buf);

	printf("HELLO WORLD!\n");
/*
	for (idx = 0; idx < 20; idx++) {
		printf("%d\n", idx);
		cpmbdos(&bellcall); // Make the console beep a bit!
	}

	
	term_ANSIMode();
	term_ANSIClrScrn(ed_erase_all);

	for (x = 0; x < 80; x++) {
		for (y = 0; y < 24; y++) {
			term_ANSIDirectCursorAddr(x, y);
			cpm_putchar('X');
		}
	}
	
	ansi_param = ANSI_P_SET_REVR(ansi_param);	
	term_ANSISetParam(ansi_param);
	term_ANSIDirectCursorAddr(0, 0);
	for (x = 0; x < 80; x++) {
			term_ANSIDirectCursorAddr(x, 0);
			cpm_putchar('.');		
	}

	ansi_param = 0;
	ansi_param = ANSI_P_SET_AOFF(ansi_param);
	term_ANSISetParam(ansi_param);

	term_ANSIDirectCursorAddr(0, 0);
*/
	
	memset(&cb, 0, sizeof(FCB));
	cb.drive = 1;
	cpm_setFCBname("test", "txt", &cb);
	cpm_performFileOp(fop_makeFile, &cb);
	
	for (idx = 0; idx < 3; idx++) {
		memset(dma_buf, 0x58, 128);
		dma_buf[126] = '\r';
		dma_buf[127] = '\n';
		cpm_performFileOp(fop_writeSeqRecord, &cb);
	}

	cpm_performFileOp(fop_close, &cb);

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

