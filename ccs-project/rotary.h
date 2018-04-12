#ifndef ROTARY_H_
#define ROTARY_H_

extern uint8_t rotary_pressed;
extern volatile int16_t rotary_counter;

void rotary_init();
void rotary_disable();

#endif /* ROTARY_H_ */
