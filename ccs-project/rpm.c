#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"

#include "rpm.h"

// Konstanten
static const uint8_t RPM_BAR_Y = 0;			// Y-Page der RPM-Anzeige
static const uint8_t RPM_LABEL_Y = 2;		// Y-Page der Beschriftung
static const uint8_t RPM_UNIT_X = 84;		// Position von x1000/min
static const uint8_t RPM_UNIT_X_SIZE = 18;	// Größe

// Pixeldaten der Anzeige der Einheit (x1000/min), siehe gfx/unit.png
static const uint8_t RPM_DATA_UNIT[] = { 0xa, 0x4, 0xa, 0x80, 0x42, 0x2f, 0x0,
		0xc6, 0x29, 0xe6, 0x20, 0xe6, 0x9, 0xa6, 0x0, 0xc6, 0x29, 0xe6 };

/**
 * Aktualisieren der Anzeige
 */
void rpm_draw_bar() {

	// Berechnung der Anzahl der Pixel des Balken
	uint8_t rpm_pxls = 0;
	uint16_t rpm_periode = timer_get_rpm_periode();
	if (rpm_periode != 0xffff) {
		rpm_pxls = (uint8_t) (200000.0f / rpm_periode);
	}

	// Füllen der RPM-Anzeige
	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		uint16_t value = 0b0100000000000001;

		// Füllen der Spalte
		if (x <= rpm_pxls || x == LCD_X_SIZE - 1) {
			value = 0b0111111111111111;
		}

		// Setzen des Markers durch Invertierung
		if (x % 10 == 0 && x > 0 && x < 100) {
			value ^= 0b0011111100000000;
		}

		// Senden der Daten
		lcd_set_pixels(x, RPM_BAR_Y, value);
		lcd_set_pixels(x, RPM_BAR_Y + 1, value >> 8);
	}
}

/**
 * Zeichnen des statischen Teil der Anzeige
 */
void rpm_draw_label() {
	// Beschriftung der Anzeige
	digit_draw_6x8(0, RPM_LABEL_Y, 0);
	for (uint8_t x = 2; x <= 8; x += 2) {
		digit_draw_6x8(x * 10 - 3, RPM_LABEL_Y, x);
	}

	// Zeichnen der Einheit (x1000/min)
	for (uint8_t x = 0; x < RPM_UNIT_X_SIZE; x++) {
		lcd_set_pixels(x + RPM_UNIT_X, RPM_LABEL_Y, RPM_DATA_UNIT[x]);
	}
}
