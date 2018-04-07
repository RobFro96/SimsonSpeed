#include <msp430.h>
#include <stdint.h>
#include "power.h"

/**
 * Hauptprogramm
 *
 * @return	Ende des Hauptprogramms wird nie erreicht -> kein Rückgabewert
 */
int main(void) {
	// Stopp Watchdog-Timer
	WDTCTL = WDTPW | WDTHOLD;

	// 1 MHz Main Clock Kalibrierung setzen
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

	// 32kHz-Quartz an Pin 2.6, 2.7 deaktivieren, alle ungenutzten Pins -> Pullup-Widerstände
	P2SEL &= ~(BIT6 + BIT7);
	P1REN = BIT0 + BIT1 + BIT6 + BIT7;
	P2REN = BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;

	__enable_interrupt();

	// Initialisierung und Main-Loop findet im Modul power statt.
	power_init();

	while (1) {
		power_main_loop();
	}
}
