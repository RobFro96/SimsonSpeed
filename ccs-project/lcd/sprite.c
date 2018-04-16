#include <msp430.h>
#include <stdint.h>
#include "lcd.h"

#include "sprite.h"

/**
 * Zeichnen des angegebenen Sprites an der definierten Position
 *
 * @param sprite	Sprite, das gezeichnet werden soll
 */
void sprite_draw(sprite_t *sprite) {
	const uint8_t *pt = sprite->data;

	for (uint8_t x = sprite->x; x < sprite->x + sprite->size; x++) {
		lcd_set_pixels(x, sprite->y, *pt);
		pt++;
	}
}

void sprite_draw_inverted(sprite_t *sprite, uint8_t invert) {
	uint8_t mask = invert ? 0xff : 0;
	const uint8_t *pt = sprite->data;

	for (uint8_t x = sprite->x; x < sprite->x + sprite->size; x++) {
		lcd_set_pixels(x, sprite->y, *pt ^ mask);
		pt++;
	}
}
