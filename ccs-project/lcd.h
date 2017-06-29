
#ifndef LCD_H_
#define LCD_H_

extern const uint8_t LCD_X_SIZE;
extern const uint8_t LCD_Y_SIZE;
extern const uint8_t LCD_Y_SIZE_PAGES;

void lcd_init();
void lcd_spi_send_byte(uint8_t value);
void lcd_send(uint8_t data, uint8_t cd);
void lcd_set_pixels(uint8_t x, uint8_t y_page, uint8_t byte);
void lcd_clear_all();

#endif /* LCD_H_ */
