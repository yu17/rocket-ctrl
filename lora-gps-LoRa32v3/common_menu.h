#ifndef common_menu
#define common_menu 1.0

#include "resc_display.h"
#include "resc_keyboard.h"

// ----- Menu -----
struct menuitem_t{
	uint8_t id;
	const char *desc;
	bool enter_behavior;//0 - Menu; 1 - Function
	bool drop_menu;
	void* (*enter)(void*);
	void *param;
	struct menuitem_t *prev,*next;
};

void menu_render(struct menuitem_t *item, uint8_t pos, uint8_t scale);

void menu_destroy(struct menuitem_t *item);

uint8_t menu_exec(struct menuitem_t* (*menu_loader)(uint8_t[]));

#endif