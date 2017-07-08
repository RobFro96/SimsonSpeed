#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "speed.h"
#include "trip.h"
#include "power.h"

static uint16_t distance[] = { 0, 0 }; 	// Distanz in 100 m
static uint16_t distance_fraction = 0; 	// Teil-Distanz in cm
static uint8_t current_distance = 0;

static const uint8_t TRIP_DATA_LABEL[] = { 0x1, 0x1, 0x7f, 0x1, 0x1, 0x0, 0x7c,
		0x8, 0x4, 0x4, 0x8, 0x0, 0x0, 0x44, 0x7d, 0x40, 0x0, 0x0, 0xfc, 0x24,
		0x24, 0x24, 0x18, 0x0, 0x36, 0x36 };

static const uint8_t TRTP_LABEL_SIZE = 26;
static const uint8_t TRIP_LABEL_X = 49;
static const uint8_t TRIP_Y = 6;
static const uint8_t TRIP_DEC_PNT_X = 94;
static const uint8_t TRIP_VALUE_X_1 = 76;
static const uint8_t TRIP_VALUE_X_2 = 82;
static const uint8_t TRIP_VALUE_X_3 = 88;
static const uint8_t TRIP_VALUE_X_4 = 97;
static const uint8_t TRIP_BRACE_X = 91;
static const uint8_t TRIP_BRACE_Y = 7;

void trip_on_rotation() {
	distance_fraction += CIRCUM;

	// After 100 m in distance_fraction carry in distance
	while (distance_fraction > 10000) {
		distance_fraction -= 10000;
		distance[0]++;
		distance[1]++;
	}
}

void trip_draw_label() {
	// Trip Label
	for (uint8_t x = 0; x < TRTP_LABEL_SIZE; x++) {
		lcd_set_pixels(x + TRIP_LABEL_X, TRIP_Y, TRIP_DATA_LABEL[x]);
	}

	// Print Decimal Point
	lcd_set_pixels(TRIP_DEC_PNT_X, TRIP_Y, 0b01100000);
	lcd_set_pixels(TRIP_DEC_PNT_X + 1, TRIP_Y, 0b01100000);

	// Print Trip Select Braces
	lcd_set_pixels(TRIP_BRACE_X, TRIP_BRACE_Y, 0b01111111);
	lcd_set_pixels(TRIP_BRACE_X + 1, TRIP_BRACE_Y, 0b01000001);
	lcd_set_pixels(TRIP_BRACE_X + 9, TRIP_BRACE_Y, 0b01000001);
	lcd_set_pixels(TRIP_BRACE_X + 10, TRIP_BRACE_Y, 0b01111111);

}

void trip_draw_trip() {
	uint16_t distance_temp = distance[current_distance];

	// 100 m Place
	digit_draw_7x5(TRIP_VALUE_X_4, TRIP_Y, distance_temp % 10);

	// 1 km Place
	distance_temp = distance_temp / 10;
	digit_draw_7x5(TRIP_VALUE_X_3, TRIP_Y, distance_temp % 10);

	// 10 km Place
	distance_temp = distance_temp / 10;
	uint8_t digit = distance_temp % 10;

	if (distance_temp == 0) {
		digit_clear_7x5(TRIP_VALUE_X_2, TRIP_Y);
	} else {
		digit_draw_7x5(TRIP_VALUE_X_2, TRIP_Y, digit);
	}

	// 100 km Place
	distance_temp = distance_temp / 10;
	digit = distance_temp % 10;

	if (distance_temp == 0) {
		digit_clear_7x5(TRIP_VALUE_X_1, TRIP_Y);
	} else {
		digit_draw_7x5(TRIP_VALUE_X_1, TRIP_Y, digit);
	}

	// Trip Select
	digit_draw_7x5(TRIP_BRACE_X + 3, TRIP_BRACE_Y, current_distance + 1);
}

void trip_on_touch(uint8_t button, uint16_t time) {
	if (button == 1 && time > 1000) {
		// left button pressed for 2 secs
		distance[current_distance] = 0;
		power_feed_timer();
	} else if (button == 0 && time > 200) {
		// right button pressed for short time
		current_distance ^= BIT0;
		power_feed_timer();
	}
}
