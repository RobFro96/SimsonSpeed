#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "rpm.h"
#include "timer.h"
#include "digit.h"
#include "speed.h"
#include "trip.h"
#include "gear.h"
#include "fixed.h"
#include "rotary.h"

#include "font.h"

#include "power.h"

// Zeit bis zum StandBy
static const uint16_t STANDBY_INTERVAL = 30 * 15; // time = INTERVAL / (15.26 Hz)

static uint16_t timer_value = 0;	// Wert des StandBy-Timers
uint8_t power_sleeping = 0;// Aktueller Zustand (StandBy [1], Aktiviert [0])

/**
 * Wechsel des Power-Zustandes zu aktiv.
 * Alle Module werden aktiviert, LCD wird neu gezeichnet
 */
void power_init() {
	// Interrupt-Enable für RPM und Speed deaktivieren
	// Reaktion auf Pegelwechsel findet nun über Timer statt
	P2IE &= ~BIT0;

	// Initialisieren der Module
	lcd_init();
	timer_init();
	rotary_init();

	// Zeichnen der statische Elemente des LCD
	fixed_draw();
}

/**
 * Wechsel des Power-Zustandes zu standby.
 * Module werden deaktiviert
 */
void power_fall_asleep() {
	// Einstellen des RPM-Pin mit Interrupt -> Wakeup
	P2DIR &= ~BIT0;
	P2SEL &= ~BIT0;
	P2SEL2 &= ~BIT0;
	P2IE |= BIT0;
	P2IFG &= ~BIT0;

	// Deaktivieren der Module
	lcd_disable();
	timer_disable();
	rotary_disable();
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
		power_sleeping = 1;
		__low_power_mode_4();	// Warten im LPM

		// Aufwachen
		power_sleeping = 0;
		power_init();
		power_feed_timer();

	} else {
		// Aktualisieren der Anzeigen
		rpm_draw();
		speed_draw();
		trip_draw();
		gear_draw();

		font_draw_number(53, 7, rotary_counter, 5);
	}
}
