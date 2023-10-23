#ifndef app_mainmenuc
#define app_mainmenuc 2.2

#include <Arduino.h>

#include "common_menu.hpp"
#include "common_misc.hpp"
#include "app_shiptracker.hpp"
#include "app_loracomm.hpp"
#include "app_gps.hpp"
#include "app_sensors.hpp"

extern const struct menu_page_t mainmenu_p,
    mainmenu_p_settings,
        mainmenu_p_system,
            mainmenu_p_batvolt,
            mainmenu_p_chiptemp,
        mainmenu_p_gps,
            mainmenu_p_gps_power,
            mainmenu_p_gps_parser,
            mainmenu_p_gps_freq;

extern const struct menu_item_t mainmenu[5],
    mainmenu_settings[4],
        mainmenu_system[3],
            mainmenu_batvolt[2],
            mainmenu_chiptemp[2],
        mainmenu_gps[3],
            mainmenu_gps_power[2],
            mainmenu_gps_parser[2],
            mainmenu_gps_freq[4];

int8_t mainmenu_menumark(const uint8_t param);

struct menuitem_t *mainmenu_load(uint8_t levels[]);

//uint8_t app_mainmenu();

#endif