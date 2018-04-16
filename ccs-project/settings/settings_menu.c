#include <msp430.h>
#include <stdint.h>
#include "font.h"
#include "rotary.h"
#include "sprite.h"
#include "power.h"

#include "settings_menu.h"

static sprite_t sprite_exit = { 94, 0, 7, { 0x42, 0x66, 0x3c, 0x18, 0x3c, 0x66,
		0x42 } }; //!< Anzeige zum Beenden der Ansicht, siehe gfx/settings_exit.png
static sprite_t sprite_standard = { 55, 3, 36, { 0x7f, 0x41, 0x0, 0x24, 0x2a,
		0x12, 0x0, 0x1e, 0x24, 0x0, 0x18, 0x24, 0x3c, 0x0, 0x3c, 0x4, 0x38, 0x0,
		0x18, 0x24, 0x3e, 0x0, 0x18, 0x24, 0x3c, 0x0, 0x3c, 0x8, 0x4, 0x0, 0x18,
		0x24, 0x3e, 0x0, 0x41, 0x7f } }; //!<Button zum Setzen der Standard-Werte, siehe gfx/settings_standard.png

static const uint8_t ITEM_COUNT = 14;
static int8_t current_item;
static settings_t settings;

void settings_menu_init() {
	current_item = ITEM_CLOSE;

	settings.total1 = 0;
	settings.total100 = 99;
	settings.total10000 = 1;
	settings.circum = 167;
	settings.gear_count = 3;

	settings_menu_draw();
}

void settings_menu_update() {
	if (rotary_counter != 0) {
		current_item += rotary_counter;
		rotary_counter = 0;

		while (current_item < 0) {
			current_item += ITEM_COUNT;
		}

		while (current_item >= ITEM_COUNT) {
			current_item -= ITEM_COUNT;
		}

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
}
