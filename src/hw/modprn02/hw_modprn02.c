#include "hw_modprn02.h"
#include "../common/hw_common.h"

#include "cprintf.h"

#define SIO_VECT_LOC_MASK 0xF1

#define SIO_BASIC_CMD_NULL	0x00
#define SIO_BASIC_CMD_ABRT	0x08
#define SIO_BASIC_CMD_RST_EXTINT	0x10
#define SIO_BASIC_CMD_RST_CHN	0x18
#define SIO_BASIC_CMD_RST_RXINT	0x20
#define SIO_BASIC_CMD_RST_TXINT	0x28
#define SIO_BASIC_CMD_RST_ERR	0x30
#define SIO_BASIC_CMD_RET_INT	0x38

#define SIO_CRC_CMD_NULL	0x00
#define SIO_CRC_CMD_RST_RXCRC_CHK	0x40
#define SIO_CRC_CMD_RST_TXCRC_GEN	0x80
#define SIO_CRC_CMD_RST_CRC	0xC0

#define SIO_REG0_CTS_FLAG	0x20	
#define SIO_REG0_RXAVAIL_FLAG	0x01
#define SIO_REG0_TXEMPTY_FLAG	0x04
#define SIO_REG0_BREAK_FLAG	0x80

#define SIO_REG5_RTS_FLAG	0x02
#define SIO_REG5_BREAK_FLAG	0x10

static uint8_t reg5_status[] = {0, 0};
static uint8_t reg3_status[] = {0, 0};

static volatile uint8_t ch_buf[2][2] = {{0, 0}, {0, 0}};

// Initialize the CTC IC 
void ctc_init(MPRN_Channel chan, MPRN_BaudRate brate);
void sio_init(MPRN_Channel chan, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity, uint8_t ivect_start);

// Interrupt handlers
// Channel A
void chA_intHandler_rx_specialCond(void) __naked;
void chA_intHandler_rx_charAvail(void) __interrupt;
void chA_intHandler_tx_bufEmpty(void) __interrupt;
void chA_intHandler_statChng(void) __naked;

// Channel A
void chB_intHandler_rx_specialCond(void) __naked;
void chB_intHandler_rx_charAvail(void) __naked;
void chB_intHandler_tx_bufEmpty(void) __naked;
void chB_intHandler_statChng(void) __naked;

/*********************************/

void setup_modprn(MPRN_Channel chan, MPRN_BaudRate brate, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity, uint8_t ivect_start) {
	ctc_init(chan, brate);
	sio_init(chan, bpc, sbit, parity, ivect_start);
}

void ctc_init(MPRN_Channel chan, MPRN_BaudRate brate) {
	uint8_t ctc_command = 0x57; // Enable time constant sending, low-to-high pulse front trigger, counter mode, interrupts disabled. See CTC docs.
	
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, ctc_command); // Send the channel command
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, (uint8_t)brate); // Send the time constant. This will divide our input clock.
}

void sio_init(MPRN_Channel chan, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity, uint8_t ivect_start) {
	// Register 0
	hw_outp(MODPRN02_SIO_A_CTRL + chan, SIO_BASIC_CMD_RST_CHN); // Reset the channel
	
	// Delay a bit, to make sure the channel got reset
	__asm
		nop
		nop
		nop
	__endasm;

	// Register 4
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x04); // Select register 4
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x40 |sbit | parity); // Set parity, stop bits and x16 clock mode
	
	// Register 5
	reg5_status[chan] = 0x08 | (bpc >> 1); // Enable Tx, set Tx bits, RTS off
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]);

#if 1
//	ivect_start &= SIO_VECT_LOC_MASK;

	// Channel B handlers
	//hw_addInterruptHandler(ivect_start | 0x00, (uint16_t)chB_intHandler_tx_bufEmpty);
	//hw_addInterruptHandler(ivect_start | 0x02, (uint16_t)chB_intHandler_statChng);
	hw_addInterruptHandler(ivect_start | 0x04, (uint16_t)chB_intHandler_rx_charAvail);
	//hw_addInterruptHandler(ivect_start | 0x06, (uint16_t)chB_intHandler_rx_specialCond);

	// Channel A handlers
	//hw_addInterruptHandler(ivect_start | 0x08, (uint16_t)chA_intHandler_tx_bufEmpty);
	//hw_addInterruptHandler(ivect_start | 0x0A, (uint16_t)chA_intHandler_statChng);
	hw_addInterruptHandler(ivect_start | 0x0C, (uint16_t)chA_intHandler_rx_charAvail);
	//hw_addInterruptHandler(ivect_start | 0x0E, (uint16_t)chA_intHandler_rx_specialCond);

	hw_outp(MODPRN02_SIO_B_CTRL, 0x02); // Select register 2
	hw_outp(MODPRN02_SIO_B_CTRL, ivect_start); // Interrupt vector 0
#endif

	// Register 1
#if 0
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x01); // Select register 1
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Disable interrupts
#else
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x01); // Select register 1
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x1C); // Enable interrupts for received chars, TX and status affect vector
#endif

	// Register 3
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x03); // Select register 3
	hw_outp(MODPRN02_SIO_A_CTRL + chan, (0x01|bpc)); // Set rx bits and enable RX

	// Register 5 again...
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan] | SIO_REG5_RTS_FLAG);
}

void modprn_outch(MPRN_Channel chan, uint8_t ch) {
	uint8_t reg_0 = 0;

	do {
		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
		reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);
	} while(!(reg_0 & SIO_REG0_TXEMPTY_FLAG) || !(reg_0 & SIO_REG0_CTS_FLAG));

	hw_outp(MODPRN02_SIO_A_DATA + chan, ch);
}


uint8_t modprn_getch(MPRN_Channel chan) {
	uint8_t reg_0 = 0;

	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
	reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);

	if (!(reg_0 & SIO_REG0_RXAVAIL_FLAG)) { // If we already have a char waiting, raising the RTS line could cause overrun!
		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
		hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan] | SIO_REG5_RTS_FLAG);

		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
		do {
			reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);
		} while (!(reg_0 & SIO_REG0_RXAVAIL_FLAG));

		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
		hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]);
	}

	return hw_inp(MODPRN02_SIO_A_DATA + chan);
}

uint8_t modprn_getchBuf(MPRN_Channel chan, uint8_t *buf, uint8_t bufSize) {
	uint8_t reg_0 = 0;
	uint8_t buf_used = 0;

	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
	reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);

	if (!(reg_0 & SIO_REG0_RXAVAIL_FLAG)) { // If we already have a char waiting, raising the RTS line could cause overrun!
		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
		hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan] | SIO_REG5_RTS_FLAG);

		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
		do {
			reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);
		} while (!(reg_0 & SIO_REG0_RXAVAIL_FLAG));

		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
		hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]);
	}

	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0	
	do {
		buf[buf_used] = hw_inp(MODPRN02_SIO_A_DATA + chan);
		buf_used++;

		reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);
	} while ((reg_0 & SIO_REG0_RXAVAIL_FLAG) && (buf_used < bufSize));

	return buf_used;
}

uint8_t modprn_getBreakStatus(MPRN_Channel chan) {
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
	return (hw_inp(MODPRN02_SIO_A_CTRL + chan) & SIO_REG0_BREAK_FLAG);
}

void modprn_sendBreak(MPRN_Channel chan) {
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan] | SIO_REG5_BREAK_FLAG); // Send the break signal

	__asm
		nop
	__endasm;
	
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]); // Disable break signal
}

// Interrupt Handlers...
// Channel A
void chA_intHandler_rx_specialCond(void) __naked {
	__asm
		push af

		ld a,#SIO_BASIC_CMD_RST_ERR
		out (#MODPRN02_SIO_A_CTRL),a

		ei

		pop af

		reti
	__endasm;
}

void chA_intHandler_rx_charAvail(void) __interrupt {
	hw_outp(MODPRN02_SIO_A_CTRL, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL, reg5_status[0]); // Lower RTS

	ch_buf[0][0] = hw_inp(MODPRN02_SIO_A_DATA);
	ch_buf[1][0] = 1;

	__asm
		ei
	__endasm;
}

uint8_t modprn_int_getch(MPRN_Channel chan) {
	uint8_t chbuf;

	while(!ch_buf[1][chan]);

	chbuf = ch_buf[0][chan];
	ch_buf[1][0] = 0;

	hw_outp(MODPRN02_SIO_A_CTRL, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL, reg5_status[0] | SIO_REG5_RTS_FLAG); // Raise RTS

	return chbuf;
}

void chA_intHandler_tx_bufEmpty(void) __interrupt {
	__asm
		ei
	__endasm;
}

void chA_intHandler_statChng(void) __naked {
	__asm
		ei

		reti
	__endasm;
}

// Channel B
void chB_intHandler_rx_specialCond(void) __naked {
	__asm
		push af

	//	ld a,#SIO_BASIC_CMD_RST_ERR
	//	out (#MODPRN02_SIO_B_CTRL),a

		ei

		pop af
		reti
	__endasm;
}

void chB_intHandler_rx_charAvail(void) __naked {
	__asm
		push af

		ei

		pop af
		reti
	__endasm;
}

void chB_intHandler_tx_bufEmpty(void) __naked {
	__asm
		push af

		ei

		pop af
		reti
	__endasm;
}

void chB_intHandler_statChng(void) __naked {
	__asm
		push af

		ei

		pop af
		reti
	__endasm;
}
