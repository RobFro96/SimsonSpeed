#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "sprite.h"
#include "font.h"

#include "settings_fixed.h"

static sprite_t sprite_current = { 53, 4, 27, { 0x1, 0x79, 0x15, 0x79, 0x1,
		0x7d, 0x21, 0x51, 0x1, 0x3d, 0x49, 0x1, 0x79, 0x41, 0x79, 0x1, 0x31,
		0x69, 0x51, 0x1, 0x5, 0x7d, 0x1, 0x5, 0x7d, 0x1, 0x29 } }; //!< Überschrift der Anzeige der aktuellen Daten, siehe gfx/settings_current.png

/**
 * Zeichnen der festen Elemente der Einstellungsanzeige
 */
void settings_fixed_draw() {
	font_draw_string_bold(0, 0, "Einstellungen");

	font_draw_string(0, 1, "Gesamt:");
	font_draw_string(74, 1, "km");

	font_draw_string(0, 2, "Radumf:");
	font_draw_string(62, 2, "cm");

	font_draw_string(0, 3, "G\x80nge :");

	for (uint8_t i = 1; i <= 4; i++) {
		font_draw_char(0, 3 + i, 48 + i);
		lcd_set_pixels(6, 3 + i, 0b01101100);
		lcd_set_pixels(7, 3 + i, 0b01101100);
		font_draw_char(27, 3 + i, '-');
		lcd_set_pixels(52, 3 + i, 0xff);
	}

	sprite_draw(&sprite_current);
	for (uint8_t x = 80; x < LCD_X_SIZE; x++) {
		lcd_set_pixels(x, 4, 1);
	}

	font_draw_string(54, 5, "kmh:");
	font_draw_string(54, 6, "RPM:");
	font_draw_string(54, 7, "Gang:");

}
