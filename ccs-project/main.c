#include <msp430.h>
#include <stdint.h>
#include "power.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

	P2SEL &= ~(BIT6 + BIT7);
	P2SEL2 &= ~(BIT6 + BIT7);
	P1REN = BIT0 + BIT1 + BIT6 + BIT7;
	P2REN = BIT2 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

	power_init();

	__enable_interrupt();

	while (1) {
		power_main_loop();
	}
}
