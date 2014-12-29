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
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, SIO_BASIC_CMD_RST_CHN); // Reset the channel

	// Register 1
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x01); // Select register 1
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x00); // Disable interrupts
	
	// Register 3
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x03); // Select register 3
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, (0x01 | bpc)); // Set rx bits and enable RX
	
	// Register 3
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x03); // Select register 3
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, (0x01 | bpc)); // Set rx bits and enable RX
	
	// Register 4
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x04); // Select register 4
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x30 | sbit | parity); // Set parity, stop bits, external sync and X1 clock mode
	
	// Register 5
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x05); // Select register 5
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, 0x08 | (bpc >> 1)); // Enable Tx, set Tx bits
}
