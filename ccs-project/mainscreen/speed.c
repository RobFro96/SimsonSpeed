#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"

#include "speed.h"

const uint16_t CIRCUM = 167;	// Radumfang in cm

static const uint8_t SPEED_VALUE_Y = 4;		// Y-Page der Anzeige
static const uint8_t SPEED_VALUE_X_1 = 2;	// X-Position der 1. Ziffer
static const uint8_t SPEED_VALUE_X_2 = 24;	// X-Position der 2. Ziffer

/**
 * Aktualisieren der Anzeige
 */
void speed_draw() {
	uint8_t speed = speed_calculate();

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

/**
 * Gibt die Geschwindigkeit in km/h zurück
 *
 * @return Geschwindigkeit in km/h
 */
uint8_t speed_calculate() {
	if (speed_periode != 0xffff) {
		return (uint8_t) (CIRCUM * 3.6f / (speed_periode * 64e-4));
	} else {
		return 0;
	}
}
