#include <msp430.h>
#include <stdint.h>

#include "lcd.h"

// Konstanten
const uint8_t LCD_X_SIZE = 102;			// Anzahl Pixel in X-Richtung
const uint8_t LCD_Y_SIZE = 64;			// Anzahl Pixel in Y-Richtung
const uint8_t LCD_Y_SIZE_PAGES = 64 / 8;// Anzahl der Pages (8 Y-Pixel pro Page)

// Prototypen
static void lcd_set_cd(uint8_t value);
static void lcd_reset_h();
static void lcd_reset_l();

/**
 * Initialisierung des LCD
 */
void lcd_init() {
	// Pins initialisieren
	P1DIR |= BIT3 + BIT5;
	lcd_reset_l();

	// SPI initialisieren

	// Pin-Select
	P1SEL |= BIT2 + BIT4;
	P1SEL2 |= BIT2 + BIT4;

	// UCA0-Modul initialisieren
	UCA0CTL1 |= UCSWRST;	// Reset Halten
	UCA0CTL0 = UCCKPL + UCMSB + UCMST + UCSYNC;	// Clock Polarity, MSB first, Master, Synchronous Mode
	UCA0CTL1 |= UCSSEL_2;	// SMCLK
	UCA0BR0 = 0x01;			// Prescaler: 1 -> 1 MHz
	UCA0BR1 = 0;
	UCA0MCTL = 0;			// keine Modulation
	UCA0CTL1 &= ~UCSWRST;	// Reset Loslassen

	// Reset Pin deaktivieren
	lcd_reset_h();

	//Initialize bottom view 3.3V (booster on) 8Bit SPI
	lcd_send(0x40, 0); //Startline 0
	lcd_send(0xA1, 0); //SEG reverse
	lcd_send(0xC0, 0); //Set COM direction (COM0-COM63)
	lcd_send(0xA4, 0); //Set all Pixel to on
	lcd_send(0xA6, 0); //Display inverse off
	lcd_send(0xA2, 0); //Set bias 1/9
	lcd_send(0x2F, 0); //Booster, regulator, follower on
	lcd_send(0xAF, 0); //Display on

	lcd_clear_all();

}

/**
 * Deaktivieren des LCD -> kein Stromverbrauch durch LCD
 */
void lcd_disable() {
	lcd_reset_l();
}

/**
 * Senden eines Bytes über SPI
 *
 * @param value	Byte
 */
void lcd_spi_send_byte(uint8_t value) {
	while (!(IFG2 & UCA0TXIFG))
		;
	UCA0TXBUF = value;
	while (!(IFG2 & UCA0TXIFG))
		;
}

/**
 * Senden eines Bytes an LCD
 * CD = 1 -> Command
 * CD = 0 -> Data
 *
 * @param data	Byte
 * @param cd	Daten oder Befehl
 */
void lcd_send(uint8_t data, uint8_t cd) {
	lcd_set_cd(cd);			// CD-Pin Setzen
	lcd_spi_send_byte(data);	// Daten über SPI übertragen
}

/**
 * Setzen einer Page des LCD
 *
 * @param x			X-Position
 * @param y_page	Y-Page
 * @param byte		Pixelwerte
 */
void lcd_set_pixels(uint8_t x, uint8_t y_page, uint8_t byte) {
	// Position setzen
	lcd_send((x & 0x0f), 0);		// Low Nibble der X-Position
	lcd_send(0x10 + (x >> 4), 0);	// High Nibble
	lcd_send(0xB0 + y_page, 0);		// Page

	lcd_send(byte, 1);				// Pixelwerte senden
}

/**
 * Setzt alle Pixel des LCD auf 0
 */
void lcd_clear_all() {

	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		for (uint8_t y = 0; y < LCD_Y_SIZE_PAGES; y++) {
			lcd_set_pixels(x, y, 0x00);
		}
	}
}

/**
 * Reset-Pin wird auf H-Pegel gesetzt -> LCD einschalten
 */
static void lcd_reset_h() {
	P1OUT |= BIT3;
}

/**
 * Reset-Pin wird auf L-Pegel gesetzt -> LCD anschalten
 */
static void lcd_reset_l() {
	P1OUT &= ~ BIT3;
}

/**
 * Setze CD-Pin auf den angegebenen Pegel
 *
 * @param value	Pegel des CD-Pins
 */
static void lcd_set_cd(uint8_t value) {
	if (value) {
		P1OUT |= BIT5;
	} else {
		P1OUT &= ~BIT5;
	}
}
