#ifndef _CPM_SYSFUNC_HEADER_
#define _CPM_SYSFUNC_HEADER_

#include "common_datatypes.h"

typedef struct {
	uint8_t drive;
	char filename[8];
	char filetype[3];
	uint8_t ex;
	uint8_t s1;
	uint8_t s2;
	uint8_t rc;
	uint8_t al[16];
	uint8_t cr;
	uint8_t r0;
	uint8_t r1;
	uint8_t r2;
} FCB;

void cpm_sysfunc_init(void);

char *cpm_gets(char *p);
char cpm_getchar(void);
void cpm_putchar(char c);

#endif /* _CPM_SYSFUNC_HEADER_ */
