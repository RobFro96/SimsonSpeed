#ifndef SETTINGS_SETTINGS_MENU_H_
#define SETTINGS_SETTINGS_MENU_H_

typedef enum {
	ITEM_CLOSE,
	ITEM_TOTAL_10000,
	ITEM_TOTAL_100,
	ITEM_TOTAL_1,
	ITEM_CIRCUM,
	ITEM_GEAR_COUNT,
	ITEM_GEAR_DEFAULT,
	ITEM_GEAR_1L,
	ITEM_GEAR_1H,
	ITEM_GEAR_2L,
	ITEM_GEAR_2H,
	ITEM_GEAR_3L,
	ITEM_GEAR_3H,
	ITEM_GEAR_4L,
	ITEM_GEAR_4H
} settings_menu_item_t;

typedef struct {
	int8_t total10000;
	int8_t total100;
	int8_t total1;
	uint8_t circum;
	uint8_t gear_count;
	uint8_t gears_high[4];
	uint8_t gears_low[4];
} settings_t;

extern settings_t settings;

void setting_default();
void settings_menu_init();
void settings_menu_update();
void settings_menu_draw();

#endif /* SETTINGS_SETTINGS_MENU_H_ */
