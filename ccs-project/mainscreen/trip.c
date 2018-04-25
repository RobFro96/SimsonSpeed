#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "digit.h"
#include "font.h"
#include "speed.h"
#include "settings_menu.h"

#include "trip.h"

static const uint8_t DISTANCES_X = 56;
static const uint8_t DISTANCE_COMMA_X = 74;
static const uint8_t DISTANCE_100M_X = 77;
static const uint8_t DISTANCE_TOTAL_X = 53;
static const uint8_t DISTANCE_Y_START = 4;
static const uint8_t DISTANCE_COMMA_DATA = 0b01100000;

static uint16_t distances[] = { 0, 0 }; 	//!> Trip 1 und 2 in 100 m
static uint32_t distance_total = 0;	//!> Gesamtdistanz in 100 m
static uint16_t distance_fraction = 0; 	//!> Teildistanz für alle in cm

static void draw_distance(uint8_t id);

/**
 * Hochzählen der Distanzmessung, wenn es zu einer Radumdrehung kommt
 */
void trip_on_rotation() {
	distance_fraction += settings.circum;// Hinzuaddieren des Radumfangs zur Teil-Distanz

	// Wenn Teildistanz größer als 100m (10.000 cm) werden die beiden Trips hochgezählt
	while (distance_fraction > 10000) {
		distance_fraction -= 10000;	// Abziehen der 100 m
		distances[0]++;
		distances[1]++;
		distance_total++;
	}
}

/**
 * Aktualisieren der Anzeige
 */
void trip_draw() {
	draw_distance(0);
	draw_distance(1);

	font_draw_number(DISTANCE_TOTAL_X, DISTANCE_Y_START + 2, distance_total / 10, 5);
}

/**
 * Setzt den angegebenen Trip auf den Wert 0 zurück.
 * @param id	Id des Trips (0 oder 1^)
 */
void trip_reset(uint8_t id) {
	distances[id] = 0;
}

/**
 * Setzt die Gesamtkilometer im Einstellungs-struct settings
 *
 * @param settings
 */
void trip_get_total(settings_t *settings) {
	uint32_t total = distance_total / 10;
	settings->total1 = total % 100;
	total /= 100;
	settings->total100 = total % 100;
	total /= 100;
	settings->total10000 = total % 10;
}

/**
 * Setzt die Gesamtkilometer aus dem Einstellungs-struct settings
 * @param settings
 */
void trip_set_total(settings_t *settings) {
	distance_total = 10L * settings->total1;
	distance_total += 1000L * settings->total100;
	distance_total += 100000L* settings->total10000;
}

/**
 * Zeichnen eines Trips
 * @param id	Id des Trips (0 oder 1) -> Bestimmt Wert und Y-Page
 */
static void draw_distance(uint8_t id) {
	uint8_t y_page = DISTANCE_Y_START + id;
	font_draw_number(DISTANCES_X, y_page, distances[id] / 10, 3);
	lcd_set_pixels(DISTANCE_COMMA_X, y_page, DISTANCE_COMMA_DATA);
	lcd_set_pixels(DISTANCE_COMMA_X + 1, y_page, DISTANCE_COMMA_DATA);
	font_draw_char(DISTANCE_100M_X, y_page, '0' + (distances[id] % 10));
}
