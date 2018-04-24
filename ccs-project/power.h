#ifndef POWER_H_
#define POWER_H_

extern uint8_t power_sleeping;

typedef enum {
	DISPLAY_MAIN, DISPLAY_SETTINGS
} display_t;

void power_init();
void power_fall_asleep();
void power_feed_timer();
void power_tick_timer();
void power_main_loop();
void power_change_display(display_t new_display);

#endif /* POWER_H_ */
