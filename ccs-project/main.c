#include <msp430.h>
#include <stdint.h>
#include "lcd.h"
#include "rpm.h"
#include "timer.h"
#include "digit.h"
#include "speed.h"
#include "trip.h"

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;		// Stop watchdog timer

	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

	lcd_init();
	timer_init();

	rpm_draw_label();
	speed_draw_label();
	trip_draw_label();

	__enable_interrupt();

	P1DIR |= BIT0;

	while (1) {
		rpm_draw_bar();
		speed_draw_speed();
		trip_draw_trip();
		P1DIR ^= BIT0;
	}
}
