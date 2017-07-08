#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "rpm.h"
#include "timer.h"
#include "digit.h"
#include "speed.h"
#include "trip.h"
#include "gear.h"
#include "power.h"
#include "touch.h"

static const uint16_t STANDBY_INTERVAL = 30 * 15; // time = INTERVAL / (15.26 Hz)

static uint16_t timer_value = 0;
static uint8_t sleeping = 0;

void power_init() {
	P2IE &= ~(BIT0 + BIT1);

	lcd_init();
	timer_init();
	touch_init();

	rpm_draw_label();
	speed_draw_label();
	trip_draw_label();
	gear_draw_label();
}

static void power_fall_asleep() {
	// Set-Up Wakeup-Pins
	P2DIR &= ~BIT0;
	P2SEL &= ~BIT0;
	P2SEL2 &= ~BIT0;
	P2IE |= BIT0;
	P2IFG &= ~BIT0;

	lcd_disable();
	timer_disable();
	touch_disable();
}

void power_tick_timer() {
	timer_value++;
}

void power_feed_timer() {
	timer_value = 0;
}

void power_main_loop() {
	if (timer_value > STANDBY_INTERVAL) {
		// Fall Asleep
		power_fall_asleep();
		sleeping = 1;
		__low_power_mode_4();

		// Wake-Up
		sleeping = 0;
		power_init();
		power_feed_timer();

	} else {
		rpm_draw_bar();
		speed_draw_speed();
		trip_draw_trip();
		gear_draw_gear();
	}
}

#pragma vector=PORT2_VECTOR
interrupt void PORT2_ISR() {
	if (P2IFG & BIT0) {
		P2IFG &= ~BIT0;
		if (sleeping) {
			__low_power_mode_off_on_exit();
		}
	}
}
