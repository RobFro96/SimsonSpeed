#include <msp430.h>
#include <stdint.h>
#include "lcd.h"

const uint8_t LCD_X_SIZE = 102;
const uint8_t LCD_Y_SIZE = 64;
const uint8_t LCD_Y_SIZE_PAGES = 64 / 8;

static void lcd_set_cd(uint8_t value);
static void lcd_reset_h();
static void lcd_reset_l();

void lcd_init() {
	// Init Pins
	P1DIR |= BIT3 + BIT5;
	lcd_reset_l();

	// Init SPI
	P1SEL |= BIT2 + BIT4;
	P1SEL2 |= BIT2 + BIT4;

	UCA0CTL1 |= UCSWRST;
	UCA0CTL0 = UCCKPL + UCMSB + UCMST + UCSYNC;
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = 0x02;
	UCA0BR1 = 0;
	UCA0MCTL = 0;

	UCA0CTL1 &= ~UCSWRST;

	// Diable Display Reset
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

void lcd_disable() {
	lcd_reset_l();
}

void lcd_spi_send_byte(uint8_t value) {
	while (!(IFG2 & UCA0TXIFG))
		;
	UCA0TXBUF = value;
	while (!(IFG2 & UCA0TXIFG))
		;
}

void lcd_send(uint8_t data, uint8_t cd) {
	lcd_set_cd(cd);
	lcd_spi_send_byte(data);
}

void lcd_set_pixels(uint8_t x, uint8_t y_page, uint8_t byte) {
	lcd_send((x & 0x0f), 0);
	lcd_send(0x10 + (x >> 4), 0);
	lcd_send(0xB0 + y_page, 0);

	lcd_send(byte, 1);
}

void lcd_clear_all() {

	for (uint8_t x = 0; x < LCD_X_SIZE; x++) {
		for (uint8_t y = 0; y < LCD_Y_SIZE_PAGES; y++) {
			lcd_set_pixels(x, y, 0x00);
		}
	}
}

static void lcd_reset_h() {
	P1OUT |= BIT3;
}

static void lcd_reset_l() {
	P1OUT &= ~ BIT3;
}

static void lcd_set_cd(uint8_t value) {
	if (value) {
		P1OUT |= BIT5;
	} else {
		P1OUT &= ~BIT5;
	}
}
