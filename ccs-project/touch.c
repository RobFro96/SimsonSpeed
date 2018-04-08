#include <msp430.h>
#include <stdint.h>
#include "digit.h"
#include "trip.h"

#include "touch.h"

// TODO Überarbeitung -> Implementierung des Rotary Controllers

static uint16_t pressed_ticks = 0;

void touch_init() {
	P2DIR &= ~(BIT3);
	P2REN |= BIT3;
	P2OUT |= BIT3;
	P2IE &= ~(BIT3);
}

void touch_disable() {
	P2IE |= BIT3;
	P2IFG &= ~BIT3;
}

// Executed by TimerA1, 15,.. Hz
void touch_on_timer() {
	if (P2IN & BIT3) {
		if (pressed_ticks > 0) {
			//trip_on_touch(pressed_ticks);
		}
		pressed_ticks = 0;
	} else {
		pressed_ticks++;
	}
}



