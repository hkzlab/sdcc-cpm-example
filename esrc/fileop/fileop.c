#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpmbdos.h"
#include "cprintf.h"
#include "syslib/cpm_sysfunc.h"
#include "syslib/ansi_term.h"

void sys_init(void) {
	cpm_sysfunc_init();
}

void print_fcb(FCB *fcb_ptr);

int main() {
	uint8_t rval;
	FCB *fcb_ptr = NULL;

	sys_init();

	cprintf("CP/M LIB TESTER: File operations.\n\n");

	fcb_ptr = malloc(sizeof(FCB));

	cprintf("Trying to open \"fileop.com\"...");
	memset(fcb_ptr, 0, sizeof(FCB));
	cpm_setFCBname("fileop", "com", fcb_ptr);
	rval = cpm_performFileOp(fop_open, fcb_ptr);
	cprintf(" ret.val %02X\n", rval);

	if (rval != 0xFF) {
		cprintf("File found, reading the FCB!\n");

		print_fcb(fcb_ptr);

		cprintf("Done, closing the file... ");
		rval = cpm_performFileOp(fop_close, fcb_ptr);
		cprintf(" ret.val %02X\n", rval);
	}
	
	cprintf("Trying a file search... ");

	memset(fcb_ptr, 0, sizeof(FCB));
	cpm_setFCBname("f?l??p", "??m", fcb_ptr);
	
	rval = cpm_performFileOp(fop_firstNameNatch, fcb_ptr);
	
	cprintf("Result %02X\n", rval);

	free(fcb_ptr);

	return (EXIT_SUCCESS);
}

void print_fcb(FCB *fcb_ptr) {
		cprintf("\tname ->\t%c%c%c%c%c%c%c%c\n", fcb_ptr->filename[0], fcb_ptr->filename[1], fcb_ptr->filename[2], fcb_ptr->filename[3], fcb_ptr->filename[4], fcb_ptr->filename[5], fcb_ptr->filename[6], fcb_ptr->filename[7]);
		cprintf("\ttype ->\t%c%c%c\n", fcb_ptr->filetype[0], fcb_ptr->filetype[1], fcb_ptr->filetype[2]);
		cprintf("\t  ex ->\t%02X\n",fcb_ptr->ex);
		cprintf("\tresv ->\t%04X\n",fcb_ptr->resv);
		cprintf("\t  rc ->\t%02X\n",fcb_ptr->rc);
		cprintf("\talb0 ->\t%04X %04X %04X %04X\n", fcb_ptr->alb[0], fcb_ptr->alb[1], fcb_ptr->alb[2], fcb_ptr->alb[3]);
		cprintf("\talb1 ->\t%04X %04X %04X %04X\n", fcb_ptr->alb[4], fcb_ptr->alb[5], fcb_ptr->alb[6], fcb_ptr->alb[7]);
		cprintf("\tsreq ->\t%02X\n",fcb_ptr->seqreq);
		cprintf("\trrec ->\t%04X\n",fcb_ptr->rrec);
		cprintf("\trreo ->\t%02X\n\n",fcb_ptr->rrecob);
}
