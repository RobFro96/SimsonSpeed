#include <msp430.h>
#include <stdint.h>
#include "power.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

	P2SEL &= ~(BIT6+BIT7);
	P1REN = BIT0 + BIT1 + BIT6 + BIT7;
	P2REN = BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

	__enable_interrupt();

	power_init();

	while (1) {
		power_main_loop();
	}
}
