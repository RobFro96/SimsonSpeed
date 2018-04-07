#include <msp430.h>
#include <stdint.h>
#include "trip.h"
#include "power.h"
#include "touch.h"

#include "timer.h"

// Entprellen des Magnetschalters
const uint16_t DEBOUNCE_LENGTH = 256;	// Time = LENGTH * 1us

// Variablen
static uint16_t rpm_periodes[] = { 0xffff, 0xffff };// Zwei RPM-Perioden im RAM
uint16_t speed_periode = 0xffff;	// Periodendauer der Geschwindigkeitsmessung
static uint16_t last_ccr0 = 0;		// Alter Wert des CCR0-Registers
static uint16_t last_ccr1 = 0;		// Alter Wert des CCR1-Registers
static uint8_t overflowed_rpm = 1;	// Gibt an, ob es zum Overflow des CCR0 kam
static uint8_t overflowed_speed = 1;// Gibt an, ob es zum Overflow des CCR1 kam
static uint16_t overflow = 0;// Zähler der Timer-Overflows, da Geschwindigkeitsperiode zwangsweise über mehrer Overflows geht

// Prototypen
static void isr_TA1_ccr1();
static void isr_TA1_ccr2();
static void isr_TA1_overflow();

/**
 * Initialisierung des Timers, Drehzahl- und Geschwindigkeitsmessung
 */
void timer_init() {
	// Timer Setup
	TA1CTL = TASSEL_2 + TACLR + TAIE;
	TA1CCTL0 = CM_2 + CCIS_0 + SCS + CAP + CCIE;
	TA1CCTL1 = CM_2 + CCIS_0 + SCS + CAP + CCIE;
	TA1CCTL2 = 0;
	TA1CCR0 = 0;
	TA1CCR1 = 0;
	TA1CCR2 = 0;

	// RPM & Speed Pin Setup
	P2DIR |= BIT2;
	P2OUT |= BIT2;
	P2DIR &= ~(BIT0 + BIT1);
	P2SEL |= BIT0 + BIT1;
	P2SEL2 &= ~(BIT0 + BIT1);

	// Start Timer
	TA1CTL |= MC_2;
}

/**
 * Deaktivieren des Timers, nötig für Low-Power
 */
void timer_disable() {
	TA1CTL &= ~ MC_2;

	P2DIR &= ~BIT2;
	P2OUT &= ~BIT2;
}

/**
 * Gibt die RPM-Periode zurück
 *
 * @return	RPM-Periode
 */
uint16_t timer_get_rpm_periode() {
	if (rpm_periodes[0] < rpm_periodes[1] * 0.75f) {
		return rpm_periodes[1];
	} else {
		return rpm_periodes[0];
	}
}

/**
 * ISR des CCR0
 */
#pragma vector=TIMER1_A0_VECTOR
interrupt void TIMER1_A0_ISR() {
	// Abspeichern der RPM-Periode
	uint16_t ccr0 = TA1CCR0;
	rpm_periodes[1] = rpm_periodes[0];	// Alter Wert in Element 1
	rpm_periodes[0] = ccr0 - last_ccr0;	// Neuer Wert in Element 0
	last_ccr0 = ccr0;
	overflowed_rpm = 0;

	// Verhindern des Stand-By
	power_feed_timer();
}

/**
 * ISR andere Timer
 * Weiterleiten an folgenden Funktionen
 */
#pragma vector=TIMER1_A1_VECTOR
interrupt void TIMER1_A1_ISR() {
	switch (TA1IV) {
	case 2:
		isr_TA1_ccr1();
		break;
	case 4:
		isr_TA1_ccr2();
		break;
	case 10:
		isr_TA1_overflow();
		break;
	}
}

/**
 * ISR des CCR1
 */
static void isr_TA1_ccr1() {
	// Speed Pin Interupt -> Aktivierung des CCR2 zum Entprellen
	TA1CCR2 = TA1CCR1 + DEBOUNCE_LENGTH;
	TA1CCTL2 = CCIE;
}

/**
 * ISR CCR2
 */
static void isr_TA1_ccr2() {
	// Verzögerter Interrupt des Speed-Pins

	TA1CCTL2 = 0;	// Deaktivieren CCR2 Interrupt

	// Prüfe, ob vorangegangen CCR1 nicht durch Prellen enstanden ist
	if (TA1CCTL1 & CCI)
		return;

	// Randumdrehung an Trip weiterleiten
	trip_on_rotation();

	// Zusammensetzen des Timers-Wertes aus CCR1-Wert und Timer-Overflow-Wert
	uint16_t ccr1 = (overflow << 10) + (TA1CCR1 >> 6);

	// Prüfe: kein Overflow des Speed-Timers
	if (last_ccr1 != 0xffff) {
		speed_periode = ccr1 - last_ccr1;
	}

	// Setzen des letzen Wertes -> ermöglicht nächste Timermessung
	last_ccr1 = ccr1;
	overflowed_speed = 0;

	// StandBy verhindern
	power_feed_timer();
}

/**
 * ISR des Timer-Overflow
 * Ausgeführt mit 15,26 Hz
 */
static void isr_TA1_overflow() {
	// RPM-Overflow beim zweiten Mal -> Löschen der Werte
	if (overflowed_rpm) {
		rpm_periodes[0] = 0xffff;
		rpm_periodes[1] = 0xffff;
	}

	// RPM-Overflow-Flag setzen, erst beim 2. Mal ist Overflow sicher
	overflowed_rpm = 1;

	// Overflow-Counter erhöhen -> wird benötigt zur Speed-Messung
	overflow++;

	// Overflow-Counter overflowed (Nibble-Overflow)
	if ((overflow & 0xf) == 0xf) {

		// Löschen der Speed-Periode
		if (overflowed_speed) {
			speed_periode = 0xffff;
			last_ccr1 = 0xffff;
		}

		// Setzen der Overflow-Flag
		overflowed_speed = 1;
	}

	// Funktionen ausführen, die 15,26 Hz-Timer benötigen
	power_tick_timer();	// StandBy hochzählen
	touch_on_timer();	// Touch-Updaten
}
