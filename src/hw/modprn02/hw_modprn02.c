#include "hw_modprn02.h"
#include "../common/hw_common.h"

// Initialize the CTC IC 
void ctc_init(MPRN_BaudRate brate);

/*********************************/

void ctc_init(MPRN_BaudRate brate) {
	uint8_t idx;
	uint8_t ctc_command = 0x57; // Enable time constant sending, low-to-high pulse front trigger, counter mode, interrupts disabled
	uint8_t t_const = (uint8_t)brate;
	
	for (idx = 0; idx < 4; idx++) {
		hw_outp(MODPRN02_CTC_CHAN_0 + idx, ctc_command);
		hw_outp(MODPRN02_CTC_CHAN_0 + idx, t_const);
	}
}
