/*
 * trip.h
 *
 *  Created on: 29.06.2017
 *      Author: rob
 */

#ifndef TRIP_H_
#define TRIP_H_

typedef enum {
	DISPLAY_MAIN, DISPLAY_SETTINGS
} display_t;

void trip_on_rotation();
void trip_draw();
void trip_reset(uint8_t id);
void power_change_display(display_t new_display);

#endif /* TRIP_H_ */
