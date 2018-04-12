#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "font.h"

static const uint8_t SUBMENU_X = 47; // X-Position des Menüs
static const uint8_t SUBMENU_Y = 7;	// Y-Page
static const char SUBMENU_ITEMS[][7] = {"Reset1", "Reset2", "Einst."};

void submenu_draw() {
	font_draw_string(SUBMENU_X, SUBMENU_Y, SUBMENU_ITEMS[0]);
}
