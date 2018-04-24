/*
 * trip.h
 *
 *  Created on: 29.06.2017
 *      Author: rob
 */

#ifndef TRIP_H_
#define TRIP_H_

#include "settings_menu.h"

void trip_on_rotation();
void trip_draw();
void trip_reset(uint8_t id);
void trip_get_total(settings_t *settings);
void trip_set_total(settings_t *settings);

#endif /* TRIP_H_ */
