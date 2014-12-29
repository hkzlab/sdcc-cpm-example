#ifndef _HWLIB_MODPRN02_HEADER_
#define _HWLIB_MODPRN02_HEADER_

#include "common_datatypes.h"

#define MODPRN02_BASE_PORT 0x48

#define MODPRN02_CLOCK (2457600L / 2) // Hz

// SIO
/*
#define MODPRN02_SIO_A_DATA (MODPRN02_BASE_PORT + 0x00)
#define MODPRN02_SIO_A_CTRL (MODPRN02_BASE_PORT + 0x01)
#define MODPRN02_SIO_B_DATA (MODPRN02_BASE_PORT + 0x02)
#define MODPRN02_SIO_B_CTRL (MODPRN02_BASE_PORT + 0x03)
*/
#define MODPRN02_SIO_A_DATA (MODPRN02_BASE_PORT + 0x00)
#define MODPRN02_SIO_B_DATA (MODPRN02_BASE_PORT + 0x01)
#define MODPRN02_SIO_A_CTRL (MODPRN02_BASE_PORT + 0x02)
#define MODPRN02_SIO_B_CTRL (MODPRN02_BASE_PORT + 0x03)

// CTC
#define MODPRN02_CTC_CHAN_0 (MODPRN02_BASE_PORT + 0x04 + 0x00)
#define MODPRN02_CTC_CHAN_1 (MODPRN02_BASE_PORT + 0x04 + 0x01)
#define MODPRN02_CTC_CHAN_2 (MODPRN02_BASE_PORT + 0x04 + 0x02)
#define MODPRN02_CTC_CHAN_3 (MODPRN02_BASE_PORT + 0x04 + 0x03)

typedef enum {
	Channel_A = 0,
	Channel_B = 1
} MPRN_Channel;

typedef enum {
	BRate_4800 = (MODPRN02_CLOCK / 4800L),
	BRate_9600 = (MODPRN02_CLOCK / 9600L),
	BRate_19200 = (MODPRN02_CLOCK / 19200L),
	BRate_38400 = (MODPRN02_CLOCK / 38400L),
	BRate_76800 = (MODPRN02_CLOCK / 76800L),
	BRate_153600 = (MODPRN02_CLOCK / 153600L),
	BRate_307200 = (MODPRN02_CLOCK / 307200L)
} MPRN_BaudRate;

typedef enum {
	bpc_5 = 0x00,
	bpc_6 = 0x40,
	bpc_7 = 0x80,
	bpc_8 = 0xC0
} MPRN_BPC;

typedef enum {
	stop_1 = 0x04,
	stop_1_5 = 0x08,
	stop_2 = 0x0C
} MPRN_Stop;

typedef enum {
	parity_even = 0x03,
	parity_odd = 0x01,
	parity_none = 0x00
} MPRN_Parity;


void setup_modprn(MPRN_Channel chan, MPRN_BaudRate brate, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity, uint8_t flowControl);

#endif /* _HWLIB_MODPRN02_HEADER_ */

