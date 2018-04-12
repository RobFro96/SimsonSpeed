#ifndef TIMER_H_
#define TIMER_H_

extern uint16_t speed_periode;
extern uint16_t timer_overflow;

uint16_t timer_get_rpm_periode();

void timer_init();
void timer_disable();

#endif /* TIMER_H_ */
