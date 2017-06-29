#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"
#include "speed.h"

const uint16_t CIRCUM = 180;	// Radumfang in cm

static const uint8_t SPEED_VERT_X = 47;
static const uint8_t SPEED_KMH_X = 47;
static const uint8_t SPEED_KMH_SIZE = 19;
static const uint8_t SPEED_KMH_Y = 7;
static const uint8_t SPEED_VALUE_Y = 4;
static const uint8_t SPEED_VALUE_X_1 = 2;
static const uint8_t SPEED_VALUE_X_2 = 24;

static const uint8_t SPEED_DATA_KMH[] = { 0x7d, 0x21, 0x51, 0x1, 0x61, 0x11, 0x71, 0x11,
		0x71, 0x1, 0x61, 0x11, 0xd, 0x1, 0x7d, 0x11, 0x61, 0x1, 0xff };

void speed_draw_label() {
	// Horizontal Line
	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		lcd_set_pixels(x, 3, BIT3);
	}

	// Vertical
	lcd_set_pixels(SPEED_VERT_X, 3, 0b11111000);

	for (uint8_t y = 4; y <= 6; y++) {
		lcd_set_pixels(SPEED_VERT_X, y, 0xff);
	}

	// KM/H Label
	for (uint8_t x = 0; x < SPEED_KMH_SIZE; x++) {
		lcd_set_pixels(x + SPEED_KMH_X, SPEED_KMH_Y, SPEED_DATA_KMH[x]);
	}
}

void speed_draw_speed() {
	uint8_t speed = 0;

	if (speed_periode != 0xffff) {
		speed = (uint8_t)(CIRCUM * 3.6f / (speed_periode * 64e-4));
	}

	uint8_t first_digit = speed / 10;

	// First Digit
	if (first_digit > 0 && first_digit <= 9) {
		digit_draw_20x32(SPEED_VALUE_X_1, SPEED_VALUE_Y, first_digit);
	} else {
		digit_clear_20x32(SPEED_VALUE_X_1, SPEED_VALUE_Y);
	}

	// Second Digit
	digit_draw_20x32(SPEED_VALUE_X_2, SPEED_VALUE_Y, speed % 10);
}
