#include <msp430.h>
#include <stdint.h>
#include "font.h"
#include "speed.h"
#include "timer.h"
#include "gear.h"

#include "settings_status.h"

/**
 * Anzeige der aktuellen Werte in der Einstellungsanzeige
 */
void settings_status_draw() {
	font_draw_number(84, 5, speed_calculate(), 3);

	uint8_t rpm = 0;	// 100 /min
	uint16_t rpm_periode = timer_get_rpm_periode();
	if (rpm_periode != 0xffff) {
		rpm = (uint8_t) (200000.0f / rpm_periode);
	}

	font_draw_number(78, 6, rpm * 100, 4);
	font_draw_number(84, 7, gear_calculate_value(), 3);
}
