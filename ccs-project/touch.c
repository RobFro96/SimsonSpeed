#include <msp430.h>
#include <stdint.h>

#include "digit.h"
#include "touch.h"
#include "trip.h"

static const uint8_t THRESHOLD = 28;

static uint16_t last_ccr0;
static uint16_t min = 0xffff;
static uint16_t pressed_time = 0;
static uint8_t calibrate;

void touch_init() {
	P2DIR &= ~(BIT3);
	P2REN &= ~(BIT3);
	P2SEL &= ~(BIT3);
	P2SEL2 |= (BIT3);

	TA0CTL = TASSEL_3;
	TA0CCTL0 = CM_2 + CCIS_1 + CAP + CCIE;
	TA0CCR0 = 0;

	TA0CTL |= MC_2;

	// Calibration
	min = 0xffff;
	__delay_cycles(50000);
	calibrate = 1;
	__delay_cycles(50000);
	calibrate = 0;

	min -= THRESHOLD;
}

void touch_disable() {
	TA0CTL &= ~MC_2;
	P2REN |= BIT3;
	P2SEL2 &= ~(BIT3);
}

#pragma vector=TIMER0_A0_VECTOR
interrupt void TIMER0_A0_ISR() {
	uint16_t ccr0 = TA0CCR0;
	uint16_t value = ccr0 - last_ccr0;
	last_ccr0 = ccr0;

	if (value < min) {
		if (calibrate) {
			min = value;
		} else {
			if (pressed_time != 0xffff)
				pressed_time++;

		}
	} else {
		trip_on_touch(pressed_time);
		pressed_time = 0;
	}
}

