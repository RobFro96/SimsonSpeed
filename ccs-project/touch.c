#include <msp430.h>
#include <stdint.h>

#include "digit.h"
#include "touch.h"

static const uint8_t THRESHOLD = 28;

static uint8_t current_sensor;
static uint16_t last_ccr0;
static uint16_t mins[] = { 0xffff, 0xffff };
static uint16_t pressed[] = { 0, 0 };
static uint8_t calibrate;

static void set_pin_selects();

void touch_init() {
	P2DIR &= ~(BIT3 + BIT4);
	P2REN &= ~(BIT3 + BIT4);
	P2SEL &= ~(BIT3 + BIT4);
	P2SEL2 &= ~(BIT3 + BIT4);

	TA0CTL = TASSEL_3;
	TA0CCTL0 = CM_2 + CCIS_1 + CAP + CCIE;
	TA0CCR0 = 0;

	TA0CTL |= MC_2;

	current_sensor = 0;

	// Calibration
	mins[0] = 0xffff;
	mins[1] = 0xffff;
	set_pin_selects();
	__delay_cycles(50000);
	calibrate = 1;
	__delay_cycles(50000);
	__delay_cycles(50000);
	calibrate = 0;

	mins[0] -= THRESHOLD;
	mins[1] -= THRESHOLD;
}

void touch_disable() {
	TA0CTL &= ~MC_2;
	P2REN |= BIT3 + BIT4;
	P2SEL2 &= ~(BIT3 + BIT4);
}

static void set_pin_selects() {
	if (current_sensor == 0) {
		P2SEL2 &= ~BIT4;
		P2SEL2 |= BIT3;
	} else {
		P2SEL2 &= ~BIT3;
		P2SEL2 |= BIT4;
	}

}

#pragma vector=TIMER0_A0_VECTOR
interrupt void TIMER0_A0_ISR() {
	set_pin_selects();
	current_sensor ^= BIT0;

	uint16_t ccr0 = TA0CCR0;
	uint16_t value = ccr0 - last_ccr0;
	last_ccr0 = ccr0;

	if (value < mins[current_sensor]) {
		if (calibrate) {
			mins[current_sensor] = value;
		} else {
			if (pressed[current_sensor] != 0xffff)
				pressed[current_sensor]++;

		}
	} else {
		trip_on_touch(current_sensor, pressed[current_sensor]);
		pressed[current_sensor] = 0;
	}
}

