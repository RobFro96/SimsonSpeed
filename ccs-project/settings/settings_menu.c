#include <msp430.h>
#include <stdint.h>
#include "font.h"
#include "rotary.h"
#include "sprite.h"
#include "power.h"
#include "trip.h"

#include "settings_menu.h"

static sprite_t sprite_exit = { 94, 0, 7, { 0x42, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x42 } }; //!< Anzeige zum Beenden der Ansicht, siehe gfx/settings_exit.png
static sprite_t sprite_standard = { 55, 3, 36, { 0x7f, 0x41, 0x0, 0x24, 0x2a, 0x12, 0x0, 0x1e, 0x24, 0x0, 0x18, 0x24, 0x3c, 0x0, 0x3c, 0x4, 0x38, 0x0, 0x18,
		0x24, 0x3e, 0x0, 0x18, 0x24, 0x3c, 0x0, 0x3c, 0x8, 0x4, 0x0, 0x18, 0x24, 0x3e, 0x0, 0x41, 0x7f } }; //!<Button zum Setzen der Standard-Werte, siehe gfx/settings_standard.png

static const uint8_t ITEM_COUNT = 15;
static int8_t current_item;
static uint8_t selected;
settings_t settings;

static void change_menu_point();
static void select_menu_point();
static void change_item_value();
static void deselect_menu_point();
static void modulo(int8_t *number, uint8_t divider);
static void set_default_gear_values();

void setting_default() {
	settings.total10000 = 0;
	settings.total100 = 0;
	settings.total1 = 0;
	settings.circum = 167;
	settings.gear_count = 3;
	set_default_gear_values();
}

void settings_menu_init() {
	current_item = ITEM_CLOSE;
	selected = 0;

	trip_get_total(&settings);

	settings_menu_draw();
}

void settings_menu_update() {
	if (rotary_counter != 0) {

		if (!selected) {
			change_menu_point();
		} else {
			change_item_value();
		}

		rotary_counter = 0;
		settings_menu_draw();
		power_feed_timer();
	}

	if (rotary_pressed) {
		if (!selected) {
			select_menu_point();
		} else {
			deselect_menu_point();
		}
		rotary_pressed = 0;

		if (current_item != ITEM_CLOSE)
			settings_menu_draw();

		power_feed_timer();
	}
}

void settings_menu_draw() {
	sprite_draw_inverted(&sprite_exit, current_item == ITEM_CLOSE);

	font_draw_number_invert(42, 1, settings.total10000, 1, current_item == ITEM_TOTAL_10000, 1);
	font_draw_number_invert(48, 1, settings.total100, 2, current_item == ITEM_TOTAL_100, 1);
	font_draw_number_invert(60, 1, settings.total1, 2, current_item == ITEM_TOTAL_1, 1);

	font_draw_number_invert(42, 2, settings.circum, 3, current_item == ITEM_CIRCUM, 0);

	font_draw_number_invert(42, 3, settings.gear_count, 1, current_item == ITEM_GEAR_COUNT, 0);
	sprite_draw_inverted(&sprite_standard, current_item == ITEM_GEAR_DEFAULT);

	for (uint8_t gear = 0; gear < 4; gear++) {
		font_draw_number_invert(9, 4 + gear, settings.gears_low[gear], 3, current_item == ITEM_GEAR_1L + 2 * gear, 0);
		font_draw_number_invert(33, 4 + gear, settings.gears_high[gear], 3, current_item == ITEM_GEAR_1H + 2 * gear, 0);
	}
}

static void change_menu_point() {
	current_item += rotary_counter;
	modulo(&current_item, ITEM_COUNT);
}

static void select_menu_point() {
	switch (current_item) {
	case ITEM_CLOSE:
		power_change_display(DISPLAY_MAIN);
		break;
	case ITEM_GEAR_COUNT:
		settings.gear_count = settings.gear_count == 3 ? 4 : 3;
		break;
	case ITEM_GEAR_DEFAULT:
		set_default_gear_values();
		break;
	default:
		selected = 1;
		break;
	}
}

static void change_item_value() {
	switch (current_item) {
	case ITEM_TOTAL_10000:
		settings.total10000 += rotary_counter;
		modulo(&(settings.total10000), 10);
		break;
	case ITEM_TOTAL_100:
		settings.total100 += rotary_counter;
		modulo(&(settings.total100), 100);
		break;
	case ITEM_TOTAL_1:
		settings.total1 += rotary_counter;
		modulo(&(settings.total1), 100);
		break;
	case ITEM_CIRCUM:
		settings.circum += rotary_counter;
		break;
	default:
		if (current_item >= ITEM_GEAR_1L) {
			if (current_item & 1) {
				settings.gears_low[(current_item - ITEM_GEAR_1L) >> 1] += rotary_counter;
			} else {
				settings.gears_high[(current_item - ITEM_GEAR_1H) >> 1] += rotary_counter;
			}
		}
		break;
	}
}

static void deselect_menu_point() {
	selected = 0;

	if (current_item == ITEM_TOTAL_1 || current_item == ITEM_TOTAL_100 || current_item == ITEM_TOTAL_10000) {
		trip_set_total(&settings);
	}
}

static void modulo(int8_t *number, uint8_t divider) {
	while (*number < 0) {
		*number += divider;
	}

	while (*number >= divider) {
		*number -= divider;
	}
}

static void set_default_gear_values() {
	switch (settings.gear_count) {
	case 3:
		settings.gears_high[3] = 0;
		settings.gears_low[3] = 0;
		settings.gears_high[2] = 40;
		settings.gears_low[2] = 30;
		settings.gears_high[1] = 55;
		settings.gears_low[1] = 45;
		settings.gears_high[0] = 110;
		settings.gears_low[0] = 90;
		break;
	case 4:
		settings.gears_high[3] = 110;
		settings.gears_low[3] = 90;
		settings.gears_high[2] = 38;
		settings.gears_low[2] = 32;
		settings.gears_high[1] = 46;
		settings.gears_low[1] = 40;
		settings.gears_high[0] = 60;
		settings.gears_low[0] = 50;
		break;
	default:
		break;
	}
}
