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
#include "submenu.h"
#include "font.h"

#include "power.h"

// Zeit bis zum StandBy
static const uint16_t STANDBY_INTERVAL = 30 * 15; // time = INTERVAL / (15.26 Hz)

static uint16_t timer_value = 0;	// Wert des StandBy-Timers
uint8_t power_sleeping = 0;	// Aktueller Zustand (StandBy [1], Aktiviert [0])
static display_t current_display = DISPLAY_MAIN;// 0...Hauptanzeige, 1...Einstellungen

static void init_display();
static void draw_display();

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
	init_display();
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
		draw_display();
	}
}

void power_change_display(display_t new_display) {
	current_display = new_display;
	lcd_clear_all();
	init_display();
}

static void init_display() {
	switch (current_display) {
	case DISPLAY_MAIN:
		fixed_draw();
		submenu_init();
		break;
	case DISPLAY_SETTINGS:

		break;
	default:
		break;
	}
}

static void draw_display() {
	switch (current_display) {
	case DISPLAY_MAIN:
		rpm_draw();
		speed_draw();
		trip_draw();
		gear_draw();
		submenu_draw();
		break;
	case DISPLAY_SETTINGS:

		break;
	default:
		break;
	}
}
