#ifndef POWER_H_
#define POWER_H_

extern uint8_t power_sleeping;

void power_init();
void power_fall_asleep();
void power_feed_timer();
void power_tick_timer();
void power_main_loop();


#endif /* POWER_H_ */
