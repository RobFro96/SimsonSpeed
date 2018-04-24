#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "font.h"
#include "timer.h"
#include "rotary.h"
#include "power.h"
#include "trip.h"

#include "submenu.h"

static const uint8_t SUBMENU_X = 47; // X-Position des Menüs
static const uint8_t SUBMENU_Y = 7;	// Y-Page
static const char SUBMENU_ITEMS[][7] =
		{ "      ", "Reset1", "Reset2", "Einst." };
static const uint16_t disappear_interval = 5*16;

static uint8_t current_selection;
static uint16_t disappear_timer;

static void draw_menu();
static void select_menu();

void submenu_init() {
	current_selection = 0;
	rotary_counter = 0;
	rotary_pressed = 0;
	disappear_timer = timer_overflow + disappear_interval;

	draw_menu();
}

void submenu_draw() {
	if (timer_overflow > disappear_timer) {
		current_selection = 0;
		draw_menu();
	}

	if (rotary_counter != 0) {
		current_selection += rotary_counter;
		rotary_counter = 0;
		current_selection = current_selection & 0x3;
		disappear_timer = timer_overflow + disappear_interval;
		draw_menu();
		power_feed_timer();
	}

	if (rotary_pressed) {
		rotary_pressed = 0;
		select_menu();
	}
}

static void draw_menu() {
	font_draw_string(SUBMENU_X, SUBMENU_Y, SUBMENU_ITEMS[current_selection]);
}

static void select_menu() {
	switch (current_selection) {
	case 1:
		trip_reset(0);
		break;
	case 2:
		trip_reset(1);
		break;
	case 3:
		power_change_display(DISPLAY_SETTINGS);
		break;
	default:
		break;
	}
}
