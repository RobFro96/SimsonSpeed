#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "rpm.h"
#include "timer.h"
#include "digit.h"
#include "speed.h"
#include "trip.h"
#include "gear.h"
#include "touch.h"

#include "power.h"

// Zeit bis zum StandBy
static const uint16_t STANDBY_INTERVAL = 30 * 15; // time = INTERVAL / (15.26 Hz)

static uint16_t timer_value = 0;	// Wert des StandBy-Timers
static uint8_t sleeping = 0;// Aktueller Zustand (StandBy [1], Aktiviert [0])

/**
 * Wechsel des Power-Zustandes zu aktiv.
 * Alle Module werden aktiviert, LCD wird neu gezeichnet
 */
void power_init() {
	// Interrupt-Enable für RPM und Speed deaktivieren
	// Reaktion auf Pegelwechsel findet nun über Timer statt
	P2IE &= ~(BIT0 + BIT1);

	// Initialisieren der Module
	lcd_init();
	timer_init();
	touch_init();

	// Zeichnen der statische Elemente des LCD
	rpm_draw_label();
	speed_draw_label();
	trip_draw_label();
	gear_draw_label();
}

/**
 * Wechsel des Power-Zustandes zu standby.
 * Module werden deaktiviert
 */
void power_fall_asleep() {
	// Einstellen der Wakeup-Pins
	P2DIR &= ~BIT0;
	P2SEL &= ~BIT0;
	P2SEL2 &= ~BIT0;
	P2IE |= BIT0;
	P2IFG &= ~BIT0;

	// Deaktivieren der Module
	lcd_disable();
	timer_disable();
	touch_disable();
}

/**
 * Timer wird hochgezählt. StandBy kommt näher.
 */
void power_tick_timer() {
	timer_value++;
}

/**
 * Timer wird resettet. StandBy wird aufgehalten.
 */
void power_feed_timer() {
	timer_value = 0;
}

/**
 * Rumpf der Main-Loop
 */
void power_main_loop() {
	if (timer_value > STANDBY_INTERVAL) {
		// Standy aktivieren
		power_fall_asleep();
		sleeping = 1;
		__low_power_mode_4();	// Warten im LPM

		// Aufwachen
		sleeping = 0;
		power_init();
		power_feed_timer();

	} else {
		// Aktualisieren der Anzeigen
		rpm_draw_bar();
		speed_draw_speed();
		trip_draw_trip();
		gear_draw_gear();
	}
}

/**
 * ISR für Port 2
 *
 * Pin 2.0 (RPM): Aufwachen im StandBy
 * Pin 2.3 (BTN): Aufwachen im StandBy
 */
#pragma vector=PORT2_VECTOR
interrupt void PORT2_ISR() {
	if (P2IFG & BIT0) {
		P2IFG &= ~BIT0;
		if (sleeping) {
			__low_power_mode_off_on_exit();
		}
	}

	if (P2IFG & BIT3) {
		P2IFG &= ~BIT3;
		if (sleeping) {
			__low_power_mode_off_on_exit();
		}
	}
}
