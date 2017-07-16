#include <msp430.h>
#include <stdint.h>
#include "power.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;
	BCSCTL1 |= DIVA_3;

	P1REN = BIT1 + BIT6 + BIT7;
	P2REN = BIT0 + BIT4 + BIT5;

	// ACLK-Test
	//P1DIR |= BIT0;
	//P1SEL |= BIT0;

	__enable_interrupt();

	power_init();

	while (1) {
		power_main_loop();
	}
}
