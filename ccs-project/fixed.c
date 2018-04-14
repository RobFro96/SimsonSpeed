#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"

typedef const struct {
	uint8_t x;		//!< X-Position
	uint8_t y;		//!< Y-Page
	uint8_t size;	//!< Größe in X-Richtung
	uint8_t data[];	//!< Pixelwerte
} small_sprite_t;

static const uint8_t SEP_Y_START = 3; //!< Y-Page des Beginn der Striche und der Beschriftungen
static const uint8_t SEP_Y_END = 7;	//!< Y-Page des Ende der vertikalen Striche
static const uint8_t SEP_HORZ_BIT = BIT1;//!< Bestimmt die Höhe des horizontalen Strichs
static const uint8_t SEP_T_PIECE = 0b11111110; //!< Pixelwerte des T-Stücks (Verbindung zwischen horz. und vert. Strich)
static const uint8_t SEP_VERT1_X = 45; //!< X-Position vertikaler Strich zum Abtrennen links
static const uint8_t SEP_VERT2_X = 83; //!< X-Position vertikaler Strich zum Abtrennen rechts

static const uint8_t RPM_LABEL_Y = 2;	//!< Y-Page der RPM Ticks

static small_sprite_t label_kmh = { 14, 3, 17, { 0x7a, 0x22, 0x52, 0x2, 0x62,
		0x12, 0x72, 0x12, 0x72, 0x2, 0x42, 0x32, 0xa, 0x2, 0x7a, 0x12, 0x62 } };//!< KM/H-Label, siehe gfx/label_kmh.png

static small_sprite_t label_km = { 60, 3, 9, { 0x7a, 0x22, 0x52, 0x2, 0x62,
		0x12, 0x72, 0x12, 0x72 } };	//!< KM-Label, siehe gfx/label_km.png

static small_sprite_t label_gang = { 85, 3, 16, { 0x32, 0x4a, 0x6a, 0x2, 0x62,
		0x52, 0x72, 0x42, 0x2, 0x62, 0x12, 0x72, 0x2, 0x12, 0xaa, 0x7a } };	//!< Gang-Label, siehe gfx/label_gang.png

static small_sprite_t label_rpm = { 84, 2, 18, { 0xa, 0x4, 0xa, 0x80, 0x42,
		0x2f, 0x0, 0xc6, 0x29, 0xe6, 0x20, 0xe6, 0x9, 0xa6, 0x0, 0xc6, 0x29,
		0xe6 } };	//!< RPM-Label, siehe gfx/label_rpm.png

static small_sprite_t label_trip1 = { 47, 4, 6, {0x66, 0x7f, 0x7f, 0x60, 0x0, 0x28} };	//!< Trip1-Label, siehe gfx/label_trip1.png
static small_sprite_t label_trip2 = { 47, 5, 6, {0x66, 0x73, 0x7f, 0x6e, 0x0, 0x28} };	//!< Trip2-Label, siehe gfx/label_trip2.png
static small_sprite_t label_trip_total = { 47, 6, 5, {0x3, 0x7f, 0x7f, 0x3, 0x28} };	//!< Trip1-Gesamt-Label, siehe gfx/label_trip_total.png


// Prototypen
static void draw_rpm_ticks();
static void draw_sprite(small_sprite_t *sprite);
static void draw_seperator();

/**
 * Zeichen der festen Elemente der Anzeige.
 */
void fixed_draw() {
	draw_rpm_ticks();

	draw_seperator();

	draw_sprite(&label_rpm);
	draw_sprite(&label_kmh);
	draw_sprite(&label_km);
	draw_sprite(&label_gang);

	draw_sprite(&label_trip1);
	draw_sprite(&label_trip2);
	draw_sprite(&label_trip_total);
}

/**
 * Zeichnen der Achsenbeschriftung der RPM-Bar
 */
static void draw_rpm_ticks() {
	digit_draw_6x8(0, RPM_LABEL_Y, 0);
	for (uint8_t x = 1; x <= 4; x += 1) {
		digit_draw_6x8(x * 20 - 3, RPM_LABEL_Y, x);
	}
}

/**
 * Zeichnen der Trennlinien
 */
static void draw_seperator() {
	// Horizontale Linie über gesamten Bildschirm
	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		lcd_set_pixels(x, SEP_Y_START, SEP_HORZ_BIT);
	}

	// Vertikale Abtrennung 1
	lcd_set_pixels(SEP_VERT1_X, 3, SEP_T_PIECE);
	for (uint8_t y = SEP_Y_START + 1; y <= SEP_Y_END; y++) {
		lcd_set_pixels(SEP_VERT1_X, y, 0xff);
	}

	// Vertikale Abtrennung 2
	lcd_set_pixels(SEP_VERT2_X, 3, SEP_T_PIECE);
	for (uint8_t y = SEP_Y_START + 1; y <= SEP_Y_END; y++) {
		lcd_set_pixels(SEP_VERT2_X, y, 0xff);
	}
}

/**
 * Zeichnen des angegebenen Sprites an der definierten Position
 *
 * @param sprite	Sprite, das gezeichnet werden soll
 */
static void draw_sprite(small_sprite_t *sprite) {
	const uint8_t *pt = sprite->data;

	for (uint8_t x = sprite->x; x < sprite->x + sprite->size; x++) {
		lcd_set_pixels(x, sprite->y, *pt);
		pt++;
	}
}
