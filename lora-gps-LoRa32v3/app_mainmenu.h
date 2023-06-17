#ifndef app_mainmenuc
#define app_mainmenuc 1.0

#include <Arduino.h>

#include "common_misc.h"
#include "common_menu.h"
#include "app_shiptracker.h"
#include "app_gps.h"
#include "app_bme680.h"

extern struct menuitem_t *mainmenu;

struct menuitem_t *mainmenu_load(uint8_t levels[]);

uint8_t app_mainmenu();

#endif