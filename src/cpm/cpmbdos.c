#include "cpmbdos.h"

uint8_t cpmbdos(BDOSCALL *p) __naked {
	p;
	__asm
		push	ix
		ld		ix,#0
		add		ix,sp
		
		ld		l,4(ix)
		ld		h,5(ix)
		ld		c,(hl)	; Load function
		inc		hl
		ld		e,(hl)	; Prepare parameter in E ...
		inc		hl
		ld		d,(hl)  ; And prepare parameter in D
		call	5		; Make BDOS call!
		pop		ix
		ret
	__endasm;
}

void cpmbdos_extn(BDOSCALL *p, uint16_t* ret_ba, uint16_t *ret_hl) __naked {
	p; ret_ba; ret_hl;

	__asm
		push	ix
		ld		ix,#0x00
		add		ix,sp

		ld		l,4(ix)	; Prepare HL to contain the parameter address
		ld		h,5(ix)

		ld		c,(hl)	; Load p->func8 in register C

		inc		hl		; Inrease the address so we point to first byte of p->parm16
		ld		e,(hl)	; Read first byte...
		inc		hl
		ld		d,(hl)	; Read second byte. We have p->parm16 in DE

		call	5		; Execute BDOS call!

		; We now have the return values in BA and HL
		push bc
		push hl

		ld		ix,#0x00
		add		ix,sp

		ld		l,8(ix)
		ld		h,9(ix)	; p->func8
		inc		hl		; Skip p->func8 ...
		inc		hl		; Skip p->parm16
		inc		hl		; First byte of ret_ba
		ld		(hl),b
		inc		hl
		ld		(hl),a

		pop		bc		; Recover the HL we have pushed
		inc		hl
		ld		(hl),b
		inc		hl
		ld		(hl),c

		pop		bc ; Restore BC
		pop		ix ; Restore IX
		ret
	__endasm;
}

