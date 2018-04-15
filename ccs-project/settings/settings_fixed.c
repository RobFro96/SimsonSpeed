#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "sprite.h"
#include "font.h"

#include "settings_fixed.h"

void settings_fixed_draw() {
	font_draw_string_bold(0, 0, "Einstellungen");
}
