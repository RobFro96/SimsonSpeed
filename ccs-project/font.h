/*
 * font.h
 *
 *  Created on: 04.04.2018
 *      Author: rob
 */

#ifndef FONT_H_
#define FONT_H_

void font_draw_char(uint8_t x, uint8_t y_page, char c);
void font_draw_string(uint8_t x, uint8_t y_page, char *str);

#endif /* FONT_H_ */
