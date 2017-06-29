#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"
#include "rpm.h"

const uint8_t RPM_BAR_Y = 0;
const uint8_t RPM_LABEL_Y = 2;
const uint8_t RPM_UNIT_X = 84;
const uint8_t RPM_UNIT_X_SIZE = 18;

static const uint8_t RPM_DATA_UNIT[] = { 0xa, 0x4, 0xa, 0x80, 0x42, 0x2f, 0x0,
		0xc6, 0x29, 0xe6, 0x20, 0xe6, 0x9, 0xa6, 0x0, 0xc6, 0x29, 0xe6 };

void rpm_draw_bar() {
	uint8_t rpm_pxls = 0;
	if (rpm_periode != 0xffff) {
		rpm_pxls = (uint8_t)(100000.0f / rpm_periode);
	}

	// Draw Bar Graph
	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		uint16_t value = 0b0100000000000001;

		// Filled Bar
		if (x <= rpm_pxls || x == LCD_X_SIZE - 1) {
			value = 0b0111111111111111;
		}

		// Marker
		if (x % 10 == 0 && x > 0 && x < 100) {
			value ^= 0b0011111100000000;
		}

		lcd_set_pixels(x, RPM_BAR_Y, value);
		lcd_set_pixels(x, RPM_BAR_Y + 1, value >> 8);
	}
}

void rpm_draw_label() {
	// Bar Labeling
	digit_draw_6x8(0, RPM_LABEL_Y, 0);
	for (uint8_t x = 2; x <= 8; x += 2) {
		digit_draw_6x8(x * 10 - 3, RPM_LABEL_Y, x);
	}

	// Draw Unit
	for (uint8_t x = 0; x < RPM_UNIT_X_SIZE; x++) {
		lcd_set_pixels(x + RPM_UNIT_X, RPM_LABEL_Y, RPM_DATA_UNIT[x]);
	}
}
