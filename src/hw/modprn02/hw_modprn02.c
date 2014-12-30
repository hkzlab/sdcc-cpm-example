#include "hw_modprn02.h"
#include "../common/hw_common.h"

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

// Initialize the CTC IC 
void ctc_init(MPRN_Channel chan, MPRN_BaudRate brate);
void sio_init(MPRN_Channel chan, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity);

/*********************************/

void setup_modprn(MPRN_Channel chan, MPRN_BaudRate brate, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity) {
	ctc_init(chan, brate);
	sio_init(chan, bpc, sbit, parity);
}

void ctc_init(MPRN_Channel chan, MPRN_BaudRate brate) {
	uint8_t ctc_command = 0x57; // Enable time constant sending, low-to-high pulse front trigger, counter mode, interrupts disabled. See CTC docs.
	
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, ctc_command); // Send the channel command
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, (uint8_t)brate); // Send the time constant. This will divide our input clock.
}

void sio_init(MPRN_Channel chan, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity) {
	// Register 0
	hw_outp(MODPRN02_SIO_A_CTRL + chan, SIO_BASIC_CMD_RST_CHN); // Reset the channel
	__asm
		nop
	__endasm;

	// Register 4
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x04); // Select register 4
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x40 |sbit | parity); // Set external sync, parity, stop bits and x16 clock mode
	
	// Register 5
	reg5_status[chan] = 0x08 | (bpc >> 1); // Enable Tx, set Tx bits
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]);
	
	// Register 1
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x01); // Select register 1
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Disable interrupts
	
	// Register 3
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x03); // Select register 3
	hw_outp(MODPRN02_SIO_A_CTRL + chan, (0x01|bpc)); // Set rx bits and enable RX
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
	uint8_t ch;

	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
	reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);

	if (!(reg_0 & SIO_REG0_RXAVAIL_FLAG)) { // If we already have a char waiting, raising the RTS line could cause overrun!
		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
		hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan] | SIO_REG5_RTS_FLAG);

		do {
			hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
			reg_0 = hw_inp(MODPRN02_SIO_A_CTRL + chan);
		} while (!(reg_0 & SIO_REG0_RXAVAIL_FLAG));

		hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
		hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]);
	}

	ch = hw_inp(MODPRN02_SIO_A_DATA + chan);

	return ch;
}

uint8_t modprn_getBreakStatus(MPRN_Channel chan) {
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x00); // Select register 0
	return (hw_inp(MODPRN02_SIO_A_CTRL + chan) & SIO_REG0_BREAK_FLAG);
}

void modprn_sendBreak(MPRN_Channel chan) {
	uint8_t idx;

	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan] | SIO_REG5_BREAK_FLAG); // Send the break signal

	idx = 0xFF;
	while(idx--) {
		__asm
			nop
		__endasm;
	}
	
	hw_outp(MODPRN02_SIO_A_CTRL + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_SIO_A_CTRL + chan, reg5_status[chan]); // Disable break signal
}
