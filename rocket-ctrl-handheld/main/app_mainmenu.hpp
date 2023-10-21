#ifndef app_mainmenuc
#define app_mainmenuc 1.0

#include <Arduino.h>

#include "common_menu.hpp"
#include "common_misc.hpp"
#include "app_shiptracker.hpp"
#include "app_gps.hpp"
#include "app_sensors.hpp"

//extern struct menuitem_t *mainmenu;

extern const struct menu_page_t mainmenu_p,
    mainmenu_p_3,
        mainmenu_p_3_0,
            mainmenu_p_3_0_1,
            mainmenu_p_3_0_2,
        mainmenu_p_3_1,
            mainmenu_p_3_1_0,
            mainmenu_p_3_1_1,
            mainmenu_p_3_1_2;

extern const struct menu_item_t mainmenu[4],
    mainmenu_3[4],
        mainmenu_3_0[3],
            mainmenu_3_0_1[2],
            mainmenu_3_0_2[2],
        mainmenu_3_1[3],
            mainmenu_3_1_0[2],
            mainmenu_3_1_1[2],
            mainmenu_3_1_2[4];

struct menuitem_t *mainmenu_load(uint8_t levels[]);

//uint8_t app_mainmenu();

#endif