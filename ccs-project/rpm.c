#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"

#include "rpm.h"

// Konstanten
static const uint8_t RPM_BAR_Y = 0;			// Y-Page der RPM-Anzeige

/**
 * Aktualisieren der Anzeige
 */
void rpm_draw() {

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
