#include <msp430.h>
#include <stdint.h>
#include "timer.h"

const uint16_t DEBOUNCE_LENGTH = 4*64;	// LENGTH * 64us = Zeit

uint16_t rpm_periode = 0xffff;
uint16_t speed_periode = 0xffff;
static uint16_t last_ccr0 = 0;
static uint16_t last_ccr1 = 0;
static uint8_t overflowed_rpm = 1;
static uint8_t overflowed_speed = 1;
static uint16_t overflow = 0;

void timer_init() {
	// Timer Setup
	TA1CTL = TASSEL_2 + TACLR + TAIE;
	TA1CCTL0 = CM_2 + CCIS_0 + SCS + CAP + CCIE;
	TA1CCTL1 = CM_2 + CCIS_0 + SCS + CAP + CCIE;
	TA1CCTL2 = 0;
	TA1CCR0 = 0;
	TA1CCR1 = 0;
	TA1CCR2 = 0;

	// RPM & Speed Pin Setup
	P2REN |= BIT0 + BIT1;
	P2DIR &= ~(BIT0 + BIT1);
	P2SEL |= BIT0 + BIT1;
	P2SEL2 &= ~(BIT0 + BIT1);

	// Start Timer
	TA1CTL |= MC_2;
}

#pragma vector=TIMER1_A0_VECTOR
interrupt void TIMER1_A0_ISR() {
	// Store RPM Capture
	uint16_t ccr0 = TA1CCR0;
	rpm_periode = ccr0 - last_ccr0;
	last_ccr0 = ccr0;
	overflowed_rpm = 0;
}

#pragma vector=TIMER1_A1_VECTOR
interrupt void TIMER1_A1_ISR() {
	switch (TA1IV) {
	case 2: {
		// Speed Pin Interruptes, Enabled Delay CCR2 Compare for Debouncing
		TA1CCR2 = TA1CCR1 + DEBOUNCE_LENGTH;
		TA1CCTL2 = CCIE;
		break;
	}
	case 4: {
		// Delayed Speed Pin Interrupt
		TA1CCTL2 = 0;	// Disable CCR2 Interrupt

		// Check: Interrupt was not Bouncing
		if (TA1CCTL1 & CCI)
			return;

		// Calculate extended CCR1 Speed Capture
		uint16_t ccr1 = (overflow << 10) + (TA1CCR1 >> 6);

		// Check: last_ccr1 was not overflowed
		if (last_ccr1 != 0xffff) {
			speed_periode = ccr1 - last_ccr1;
		}

		// Set last_crr1 with current
		last_ccr1 = ccr1;
		overflowed_speed = 0;
	}
		break;
	case 10:
		// If rpm-overflowed, clear rpm_periode
		if (overflowed_rpm) {
			rpm_periode = 0xffff;
		}

		// set rpm overflow flag
		overflowed_rpm = 1;

		// increase overflow-counter
		overflow++;

		// If overflow-counter overflowed
		if ((overflow & 0xf) == 0xf) {

			// Clear speed periode, if overflowed
			if (overflowed_speed) {
				speed_periode = 0xffff;
				last_ccr1 = 0xffff;
			}

			// set speed overflow flag
			overflowed_speed = 1;
		}
		break;
	}
}
