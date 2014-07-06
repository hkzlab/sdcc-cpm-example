#include "cpmbdos.h"

uint8_t cpmbdos(BDOSCALL *p) __naked {
	p;
	__asm
		push	ix
		ld		ix,#0
		add		ix,sp
		
		ld		l,4(ix)
		ld		h,5(ix)
		ld		c,(hl)
		inc		hl
		ld		e,(hl)
		inc		hl
		ld		d,(hl)
		call	5
		pop		ix
		ret
	__endasm;
}
