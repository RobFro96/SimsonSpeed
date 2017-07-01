#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "timer.h"
#include "gear.h"

static const uint8_t GEAR_DATA_LABEL[] = { 0x3e, 0x41, 0x49, 0x49, 0x3a, 0x0, 0x20, 0x54,
		0x54, 0x54, 0x78, 0x0, 0x7c, 0x8, 0x4, 0x4, 0x78, 0x0, 0x18, 0xa4, 0xa4,
		0xa4, 0x7c, 0x0, 0x36, 0x36 };

static const uint8_t GEAR_LABEL_X = 49;
static const uint8_t GEAR_LABEL_SIZE = 26;
static const uint8_t GEAR_LABEL_Y = 4;

static const uint8_t GEAR_VALUE_HIGH[] = {105, 53, 37};
static const uint8_t GEAR_VALUE_LOW[] = {95, 47, 32};
static const uint8_t GEAR_COUNT = 3;

void gear_draw_label() {
	// Gear Label
	for (uint8_t x = 0; x < GEAR_LABEL_SIZE; x++) {
		lcd_set_pixels(x + GEAR_LABEL_X, GEAR_LABEL_Y, GEAR_DATA_LABEL[x]);
	}
}

void gear_draw_gear() {
	uint8_t gear = 0;
	uint16_t gear_value = 0;
	uint16_t rpm_periode = timer_get_rpm_periode();

	if (rpm_periode != 0xffff && speed_periode != 0xffff) {
		gear_value = speed_periode / (rpm_periode >> 6);

		for (uint8_t i = 0; i<GEAR_COUNT;i++) {
			if (gear_value >= GEAR_VALUE_LOW[i] && gear_value < GEAR_VALUE_HIGH[i]) {
				gear = i +1;
				break;
			}
		}
	}
}

