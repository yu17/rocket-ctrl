/** \file common_menu.h
 * \brief Provides Menu API for system main menu as well as applications.
 */

#ifndef common_menu
#define common_menu 1.0

#include "resc_display.hpp"
#include "resc_joystick.hpp"

#define MENU_STACK_MAXDEPTH 10

// ----- Menu -----
union menu_routine_t{
	const struct menu_page_t *page;
	void* (*func)(const void*);
};

struct menu_page_t{
	const uint8_t length;
	const struct menu_item_t *items;
	int8_t (* const marking)(uint8_t);//Marking loader function
	const uint8_t marking_param;
};

struct menu_item_t{
	const char *desc;
	uint8_t enter_behavior;//0 - Menu; 1 - Function; 2 - Function with animation
	bool drop_menu;//False - Return to menu after routines exit; True - Leave the menu after routines exit
	union menu_routine_t routine;
	const void *param;
};

struct menu_stack_t{
	const struct menu_page_t *page;
	uint8_t item_id;
};

void menu_render(struct menu_page_t *page, uint8_t id, uint8_t pos, uint8_t scale);

uint8_t menu_exec(const struct menu_page_t *page);

int menu_numinput(uint8_t digits_integer, uint8_t digits_decimal, int defval, const char* unit);

int8_t menu_rangeinput(int8_t min, int8_t max, int8_t interval, int8_t defval, const char* unit);

#endif
