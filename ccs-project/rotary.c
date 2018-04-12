#include <msp430.h>
#include <stdint.h>
#include "power.h"

#include "rotary.h"


uint8_t rotary_pressed = 0; //!< Gibt, an ob der Rotary gedrückt wurde. Muss manuell resettet werden.
volatile int16_t rotary_counter = 0; //!< Gibt die aktuelle Position des Drehgebers an. Kann manipuliert werden.

/**
 * Initialisierung der Rotary-Ansteuerung
 * Initialisierung der drei Pins, des Timers A0
 */
void rotary_init() {
	P2DIR &= ~(BIT3 + BIT4 + BIT5);
	P2REN |= (BIT3 + BIT4 + BIT5);
	P2OUT |= (BIT3 + BIT4 + BIT5);

	P2IE |= BIT3 + BIT4;
	P2IES &= ~(BIT3 + BIT4 + BIT5);
	P2IFG &= ~(BIT3 + BIT4 + BIT5);

	TA0CCTL0 = CCIE;
	TA0CCR0 = 100;
}

/**
 * Ausschalten der Pullups zum Stromsparen,
 * Ausschalten des Timer A0
 */
void rotary_disable() {
	P2IE &= ~(BIT4 + BIT5);
	P2OUT &= ~(BIT4 + BIT5);

	TA0CTL = 0;
}

/**
 * ISR für Port 2
 *
 * Pin 2.0 (RPM): Aufwachen im StandBy
 * Pin 2.3 (BTN): Aufwachen im StandBy, rotary_pressed setzen (Normalbetrieb)
 * Pin 2.4 (ROTA): Starten des Timers A0
 */
#pragma vector=PORT2_VECTOR
interrupt void PORT2_ISR() {
	// RPM-Pin
	if (P2IFG & BIT0) {
		P2IFG &= ~BIT0;
		if (power_sleeping) {
			__low_power_mode_off_on_exit();
		}
	}

	// Button
	if (P2IFG & BIT3) {
		P2IFG &= ~BIT3;
		if (power_sleeping) {
			__low_power_mode_off_on_exit();
		} else {
			rotary_pressed = 1;
		}
	}

	// Rotary
	if (P2IFG & BIT4) {
		P2IFG &= ~BIT4;

		if (!(TA0CTL & MC_1)) {
			TA0CTL = TASSEL_2 + ID_3 + TACLR + MC_1;
		}
	}
}

/**
 * Timer A0 CCR0-Interrupt
 * Wird ausgelöst 800 µsec nach fallender Flanke an ROTA
 */
#pragma vector=TIMER0_A0_VECTOR
interrupt void TIMER0_A0_ISR() {
	TA0CTL = 0;

	if (P2IN & BIT4) {
		if (P2IN & BIT5) {
			rotary_counter++;
		} else {
			rotary_counter--;
		}
	}

	P2IE |= BIT4;
}

