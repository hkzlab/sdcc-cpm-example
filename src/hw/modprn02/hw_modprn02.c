#include "hw_modprn02.h"
#include "../common/hw_common.h"

// Initialize the CTC IC 
void ctc_init(MPRN_Channel chan, MPRN_BaudRate brate);

/*********************************/

void setup_modprn(MPRN_Channel chan, MPRN_BaudRate brate, MPRN_BPC bpc, MPRN_Stop sbit, MPRN_Parity parity) {
	ctc_init(chan, brate);
}

void ctc_init(MPRN_Channel chan, MPRN_BaudRate brate) {
	uint8_t idx;
	uint8_t ctc_command = 0x57; // Enable time constant sending, low-to-high pulse front trigger, counter mode, interrupts disabled
	
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, ctc_command);
	hw_outp(MODPRN02_CTC_CHAN_0 + chan, (uint8_t)brate);
}
