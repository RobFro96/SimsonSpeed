#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"

#include "speed.h"

const uint16_t CIRCUM = 167;	// Radumfang in cm

static const uint8_t SPEED_VERT1_X = 45;// X-Position vertikaler Strich zum Abtrennen links
static const uint8_t SPEED_VERT2_X = 64;// X-Position vertikaler Strich zum Abtrennen rechts
static const uint8_t SPEED_KMH_X = 45;		// X-Position des km/h-Labels
static const uint8_t SPEED_KMH_SIZE = 19;	// Größe des km/h-Labels
static const uint8_t SPEED_KMH_Y = 7;		// Y-Page des km/h-Labels
static const uint8_t SPEED_VALUE_Y = 4;		// Y-Page der Anzeige
static const uint8_t SPEED_VALUE_X_1 = 2;	// X-Position der 1. Ziffer
static const uint8_t SPEED_VALUE_X_2 = 24;	// X-Position der 2. Ziffer

// Pixeldaten des km/h-Labels mit Abtrennung, siehe gfx/kmh.png
static const uint8_t SPEED_DATA_KMH[] =
		{ 0x1, 0x7d, 0x21, 0x51, 0x1, 0x61, 0x11, 0x71, 0x11, 0x71, 0x1, 0x61,
				0x11, 0xd, 0x1, 0x7d, 0x11, 0x61, 0x1 };

/**
 * Zeichnen des statischen Teil der Anzeige
 */
void speed_draw_label() {
	// Horizontale Linie über gesamten Bildschirm
	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		lcd_set_pixels(x, 3, BIT3);
	}

	// Vertikale Abtrennung
	lcd_set_pixels(SPEED_VERT1_X, 3, 0b11111000);
	for (uint8_t y = 4; y <= 6; y++) {
		lcd_set_pixels(SPEED_VERT1_X, y, 0xff);
	}

	lcd_set_pixels(SPEED_VERT2_X, 3, 0b11111000);
	for (uint8_t y = 4; y <= 7; y++) {
		lcd_set_pixels(SPEED_VERT2_X, y, 0xff);
	}

	// km/h-Label
	for (uint8_t x = 0; x < SPEED_KMH_SIZE; x++) {
		lcd_set_pixels(x + SPEED_KMH_X, SPEED_KMH_Y, SPEED_DATA_KMH[x]);
	}
}

/**
 * Aktualisieren der Anzeige
 */
void speed_draw_speed() {
	uint8_t speed = 0;

	// Berechnung der Geschwindigkeit
	if (speed_periode != 0xffff) {
		speed = (uint8_t) (CIRCUM * 3.6f / (speed_periode * 64e-4));
	}

	// Anzeige der ersten Ziffer
	uint8_t first_digit = speed / 10;
	if (first_digit > 0 && first_digit <= 9) {
		digit_draw_20x32(SPEED_VALUE_X_1, SPEED_VALUE_Y, first_digit);
	} else {
		digit_clear_20x32(SPEED_VALUE_X_1, SPEED_VALUE_Y);
	}

	// Zweite Ziffer
	digit_draw_20x32(SPEED_VALUE_X_2, SPEED_VALUE_Y, speed % 10);
}
