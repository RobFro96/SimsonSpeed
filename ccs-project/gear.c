#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"

#include "gear.h"

// Pixeldaten dert Beschriftung (Gang), siehe gfx/gang.png
static const uint8_t GEAR_DATA_LABEL[] = { 0xe, 0x11, 0x15, 0x1d, 0x0, 0xc,
		0x12, 0x1e, 0x0, 0x1c, 0x2, 0x1e, 0x0, 0x4, 0x2a, 0x1e };

// Pixeldaten der Ziffern der Gänge, siehe gfx/gearfont.png
static const uint8_t GEAR_DATA_DIGITS[] = { 0x0, 0x0, 0x0, 0x0, 0xc0, 0x1, 0xc0,
		0x1, 0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1, 0xc0, 0x1, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x1c, 0x0, 0x1c, 0x0, 0xe, 0x0, 0xfe, 0x7f, 0xfe, 0x7f, 0xff,
		0x7f, 0xff, 0x7f, 0x0, 0x0, 0x0, 0x0, 0x1c, 0x70, 0x1e, 0x78, 0x1f,
		0x7c, 0xf, 0x7e, 0x7, 0x7f, 0x87, 0x77, 0xff, 0x73, 0xff, 0x71, 0xfe,
		0x70, 0x3c, 0x70, 0xc, 0x18, 0xe, 0x38, 0xf, 0x78, 0xc7, 0x71, 0xc7,
		0x71, 0xc7, 0x71, 0xef, 0x7b, 0xff, 0x7f, 0x7e, 0x3f, 0x3c, 0x1e, 0x0,
		0xf, 0xc0, 0xf, 0xe0, 0xf, 0xf8, 0xe, 0x3c, 0xe, 0xff, 0x7f, 0xff, 0x7f,
		0xff, 0x7f, 0xff, 0x7f, 0x0, 0xe };

static const uint8_t GEAR_LABEL_X = 47;		// Position der Beschriftung
static const uint8_t GEAR_LABEL_SIZE = 16;// Größe der Beschriftung (X-Richtung)
static const uint8_t GEAR_LABEL_Y = 4;		// Y-Page der Beschriftung

// Einstellung der Gänge
static const uint8_t GEAR_VALUE_HIGH[] = { 110, 55, 40 };// Maximum des Parameters
static const uint8_t GEAR_VALUE_LOW[] = { 90, 45, 30 };	// Minimum des Parameters
static const uint8_t GEAR_COUNT = 3;						// Anzahl der Gänge

static const uint8_t GEAR_GEAR_X = 51;	// Position der Anzeige
static const uint8_t GEAR_GEAR_Y = 5;	// Y-Page

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
	//if (gear_draw_value)
	//digit_draw_7x5_number(GEAR_LABEL_X, GEAR_LABEL_Y + 1, 3, gear_value);

	// Zeichnen der Ganganzeige
	const uint8_t *pt = &(GEAR_DATA_DIGITS[10 * 2 * gear]);

	for (uint8_t i = 0; i < 10; i++) {
		for (uint8_t y = 0; y < 2; y++) {
			lcd_set_pixels(GEAR_GEAR_X + i, GEAR_GEAR_Y + y, *pt);
			pt++;
		}
	}
}

