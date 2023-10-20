/** \file common_menu.h
 * \brief Provides Menu API for system main menu as well as applications.
 */

#ifndef common_menu
#define common_menu 1.0

#include "resc_display.hpp"
#include "resc_joystick.hpp"

// ----- Menu -----
union menu_routine_t{
	const struct menu_page_t *page;
	void* (*func)(const void*);
};

struct menu_page_t{
	uint8_t length;
	const struct menu_item_t *items;
};

struct menu_item_t{
	uint8_t id;
	const char *desc;
	uint8_t enter_behavior;//0 - Menu; 1 - Function; 2 - Function with animation
	bool drop_menu;//False - Return to menu after routines exit; True - Leave the menu after routines exit
	union menu_routine_t routine;
	const void *param;
};

struct menu_stack_t{
	const struct menu_page_t *page;
	uint8_t item_index;
};

struct menuitem_t{
	uint8_t id;
	const char *desc;
	uint8_t enter_behavior;//0 - Menu; 1 - Function; 2 - Function with animation
	bool drop_menu;
	void* (*enter)(void*);
	//union menuitem_enter_t enterpp;
	void *param;
	struct menuitem_t *prev,*next;
};

void menu_render(struct menuitem_t *item, uint8_t pos, uint8_t scale);

void menu_destroy(struct menuitem_t *item);

uint8_t menu_exec(struct menuitem_t* (*menu_loader)(uint8_t[]), uint8_t* (*menu_marking)(uint8_t[]));

int menu_numinput(uint8_t digits_integer, uint8_t digits_decimal, int defval, const char* unit);

#endif
