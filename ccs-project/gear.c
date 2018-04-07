#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"

#include "gear.h"

// Pixeldaten dert Beschriftung (Gang), siehe gfx/gang.png
static const uint8_t GEAR_DATA_LABEL[] = { 0x3e, 0x41, 0x49, 0x49, 0x3a, 0x0,
		0x20, 0x54, 0x54, 0x54, 0x78, 0x0, 0x7c, 0x8, 0x4, 0x4, 0x78, 0x0, 0x18,
		0xa4, 0xa4, 0xa4, 0x7c, 0x0, 0x36, 0x36 };

// Pixeldaten der Ziffern der Gänge, siehe gfx/gearfont.png
static const uint8_t GEAR_DATA_DIGITS[] = { 0x0, 0x0, 0x0, 0x0, 0x80, 0x3, 0x80,
		0x3, 0x80, 0x3, 0x80, 0x3, 0x80, 0x3, 0x80, 0x3, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x38, 0x0, 0x38, 0x0, 0x1c, 0x0, 0xfc, 0xff, 0xfc, 0xff, 0xfe,
		0xff, 0xfe, 0xff, 0x0, 0x0, 0x0, 0x0, 0x38, 0xe0, 0x3c, 0xf0, 0x3e,
		0xf8, 0x1e, 0xfc, 0xe, 0xfe, 0xe, 0xef, 0xfe, 0xe7, 0xfe, 0xe3, 0xfc,
		0xe1, 0x78, 0xe0, 0x18, 0x30, 0x1c, 0x70, 0x1e, 0xf0, 0x8e, 0xe3, 0x8e,
		0xe3, 0x8e, 0xe3, 0xde, 0xf7, 0xfe, 0xff, 0xfc, 0x7e, 0x78, 0x3c, 0x0,
		0x1e, 0x80, 0x1f, 0xc0, 0x1f, 0xf0, 0x1d, 0x78, 0x1c, 0xfe, 0xff, 0xfe,
		0xff, 0xfe, 0xff, 0xfe, 0xff, 0x0, 0x1c };

static const uint8_t GEAR_LABEL_X = 49;		// Position der Beschriftung
static const uint8_t GEAR_LABEL_SIZE = 26;	// Größe der Beschriftung (X-Richtung)
static const uint8_t GEAR_LABEL_Y = 4;		// Y-Page der Beschriftung

// Einstellung der Gänge
static const uint8_t GEAR_VALUE_HIGH[] = { 110, 55, 40 };	// Maximum des Parameters
static const uint8_t GEAR_VALUE_LOW[] = { 90, 45, 30 };		// Minimum des Parameters
static const uint8_t GEAR_COUNT = 3;						// Anzahl der Gänge

static const uint8_t GEAR_GEAR_X = 79;	// Position der Anzeige
static const uint8_t GEAR_GEAR_Y = 4;	// Y-Page

// Debugging-Anzeige des Parameters
// TODO: Remove
uint8_t gear_draw_value = 0;

/**
 * Zeichnen der Beschriftung
 */
void gear_draw_label() {
	// Gear Label
	for (uint8_t x = 0; x < GEAR_LABEL_SIZE; x++) {
		lcd_set_pixels(x + GEAR_LABEL_X, GEAR_LABEL_Y, GEAR_DATA_LABEL[x]);
	}
}

/**
 * Aktualisierung der Anzeige
 */
void gear_draw_gear() {
	uint8_t gear = 0;
	uint16_t gear_value = 0;
	uint16_t rpm_periode = timer_get_rpm_periode();

	if (rpm_periode != 0xffff && speed_periode != 0xffff) {
		// Berechnung des Parameters
		gear_value = speed_periode / (rpm_periode >> 6);

		// Auswählen des Ganges
		for (uint8_t i = 0; i < GEAR_COUNT; i++) {
			if (gear_value >= GEAR_VALUE_LOW[i]
					&& gear_value < GEAR_VALUE_HIGH[i]) {
				gear = i + 1;
				break;
			}
		}
	}

	// Debugging-Anzeige des Parameters
	// TODO: Remove
	if (gear_draw_value)
		digit_draw_7x5_number(GEAR_LABEL_X, GEAR_LABEL_Y + 1, 3, gear_value);

	// Zeichnen der Ganganzeige
	const uint8_t *pt = &(GEAR_DATA_DIGITS[10 * 2 * gear]);

	for (uint8_t i = 0; i < 10; i++) {
		for (uint8_t y = 0; y < 2; y++) {
			lcd_set_pixels(GEAR_GEAR_X + i, GEAR_GEAR_Y + y, *pt);
			pt++;
		}
	}
}

