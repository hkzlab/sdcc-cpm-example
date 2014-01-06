#include "sysfunc.h"
#include "include/cpmbdos.h"

#include <stdlib.h>

typedef struct {
	unsigned char size;
	unsigned char len;
	char bytes[80];
} RS_BUFFER;

static BDOSCALL bdos_readstr;
static RS_BUFFER rs_buf;

void cpm_sysfunc_init(void) {
	// Initialize READSTR BDOS call
	bdos_readstr.func8 = C_READSTR;
	bdos_readstr.parm16 = (unsigned int)&rs_buf;

}

char *cpm_gets(char *p) {
	return NULL;
}
