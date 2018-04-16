#ifndef LCD_SPRITE_H_
#define LCD_SPRITE_H_

typedef const struct {
	uint8_t x;		//!< X-Position
	uint8_t y;		//!< Y-Page
	uint8_t size;	//!< Größe in X-Richtung
	uint8_t data[];	//!< Pixelwerte
} sprite_t;

void sprite_draw(sprite_t *sprite);
void sprite_draw_inverted(sprite_t *sprite, uint8_t invert);

#endif /* LCD_SPRITE_H_ */
