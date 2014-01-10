#include "cpm_sysfunc.h"

#include "common_datatypes.h"
#include "cpmbdos.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
	uint8_t size;
	uint8_t len;
	char bytes[80];
} RS_BUFFER;

static BDOSCALL bdos_readstr;
static RS_BUFFER rs_buf;

void cpm_sysfunc_init(void) {
	// Initialize READSTR BDOS call
	bdos_readstr.func8 = C_READSTR;
	bdos_readstr.parm16 = (uint16_t)&rs_buf;
}

char *cpm_gets(char *p) {
	memset(rs_buf.bytes, 0, sizeof(rs_buf.bytes));
	rs_buf.size = sizeof(rs_buf.bytes);
	rs_buf.len = 0;

	cpmbdos(&bdos_readstr);

	rs_buf.bytes[rs_buf.len] = '\n';
	strcpy(p, rs_buf.bytes);

	return p;
}

char cpm_getchar(void) {
	BDOSCALL cread = { C_READ, { (uint16_t)0 } };
	return cpmbdos(&cread);
}

void cpm_putchar(char c) {
	BDOSCALL cwrite = { C_WRITE, { (uint16_t)c } };
	cpmbdos(&cwrite);
}

uint8_t cpm_openFile(FCB *cb) {
	int idx;
	BDOSCALL fopen = { F_OPEN, {(uint16_t)cb} };

	cb->ex = cb->s1 = 0;
	cb->s2 = cb->rc = 0;

	for (idx = 0; idx < 8; idx++)
		cb->filename[idx] &= 0x7F;

	for (idx = 0; idx < 3; idx++)
		cb->filetype[idx] &= 0x7F;

	return cpmbdos(&fopen);
}

uint8_t cpm_closeFile(FCB *cb) {
	BDOSCALL fclose = { F_CLOSE, {(uint16_t)cb} };

	return cpmbdos(&fclose);
}

uint8_t cpm_makeFile(FCB *cb) {
	BDOSCALL fmake = { F_MAKE, {(uint16_t)cb} };
	
	return cpmbdos(&fmake);
}

uint8_t cpm_deleteFile(FCB *cb) {
	BDOSCALL fdel = { F_DELETE, {(uint16_t)cb} };

	return cpmbdos(&fdel);
}

uint8_t cpm_setFileAttribs(FCB *cb) {
	BDOSCALL fattr = { F_ATTRIB, {(uint16_t)cb} };

	return cpmbdos(&fattr);	
}

void cpm_setDMAAddr(uint16_t addr) {
	BDOSCALL fdma = { F_DMAOFF, {addr} };

	cpmbdos(&fdma);
}

uint8_t cpm_getCurDrive(void) {
	BDOSCALL drv = { DRV_GET, { 0 } };

	return cpmbdos(&drv);	
}

uint8_t cpm_resetDrives(void) {
	BDOSCALL drv = { DRV_ALLRESET, { 0 } };

	return cpmbdos(&drv);	
}

uint8_t cpm_setCurDrive(uint8_t drive) {
	BDOSCALL drv = { DRV_SET, { drive } };

	return cpmbdos(&drv);
}

uint8_t cpm_readRecord(FCB *cb) {
	BDOSCALL fread = { F_READ, {(uint16_t)cb} };

	return cpmbdos(&fread);
}

uint8_t cpm_writeRecord(FCB *cb) {
	BDOSCALL fwrite = { F_WRITE, {(uint16_t)cb} };

	return cpmbdos(&fwrite);
}
