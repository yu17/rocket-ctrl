#include "app_mainmenu.h"

struct menuitem_t *mainmenu;

// Setup static menu entries
struct menuitem_t *mainmenu_load(uint8_t levels[]) {
	struct menuitem_t *menu=NULL;
	if (levels[0]==0) {
		// Main menu
		menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		//1-Trackers
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="Trackers";
		item->enter_behavior=1;
		item->drop_menu=1;
		item->enter=&app_tracker;
		item->param=NULL;
		item->prev=NULL;
		//2-LocalGPS
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="LocalGPS";
		item->enter_behavior=1;
		item->drop_menu=1;
		item->enter=&app_gps;
		item->param=NULL;
		//3-Sensors
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=3;
		item->desc="Sensors";
		item->enter_behavior=1;
		item->drop_menu=1;
		item->enter=&app_sensors;
		item->param=NULL;
		//4-Settings
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=4;
		item->desc="Settings";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		item->next=NULL;
	}
	else if (levels[0]==4) {
		if (levels[1]==0) {
			// 4-Settings
			menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			//4-1-System
			struct menuitem_t *item=menu;
			item->id=1;
			item->desc="System";
			item->enter_behavior=0;
			item->drop_menu=0;
			item->enter=NULL;
			item->param=NULL;
			item->prev=NULL;
			//4-2-GPS
			item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			item->next->prev=item;
			item=item->next;
			item->id=2;
			item->desc="GPS";
			item->enter_behavior=0;
			item->drop_menu=0;
			item->enter=NULL;
			item->param=NULL;
			//4-3-LoRa
			item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			item->next->prev=item;
			item=item->next;
			item->id=3;
			item->desc="LoRa";
			item->enter_behavior=1;
			item->drop_menu=0;
			item->enter=&loraconf_enter;
			item->param=NULL;
			//4-4-Sleep
			item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			item->next->prev=item;
			item=item->next;
			item->id=4;
			item->desc="Sleep";
			item->enter_behavior=1;
			item->drop_menu=1;
			item->enter=&func_deepsleep;
			item->param=NULL;
			item->next=NULL;
		}
		else if (levels[1]==1) {
			if (levels[2]==0) {
				//4-1-System
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-1-1-Display
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Display";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_setbrightness;
				item->param=NULL;
				item->prev=NULL;
				//4-1-2-Background Voltage Update
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Bat Volt";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				//4-1-3-Background Chip Temperature Update
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=3;
				item->desc="Chip Temp";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				item->next=NULL;
			}
			else if (levels[2]==2) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-1-2-1-Background Voltage Update Enabled
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Enable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_VOLT_1;
				item->prev=NULL;
				//4-1-2-2-Background Voltage Update Disabled
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Disable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_VOLT_2;
				item->next=NULL;
			}
			else if (levels[2]==3) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-1-3-1-Background Chip Temperature Update Enabled
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Enable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_TEMP_1;
				item->prev=NULL;
				//4-1-3-2-Background Chip Temperature Update Disabled
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Disable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_TEMP_2;
				item->next=NULL;
			}
		}
		else if (levels[1]==2) {
			if (levels[2]==0) {
				//4-2-GPS
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-1-GPS Power
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Power";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				item->prev=NULL;
				//4-2-2-GPS Background Update
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Parser";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				//4-2-3-GPS Update Frequency
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=3;
				item->desc="Frequency";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				item->next=NULL;
			}
			else if (levels[2]==1) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-1-1-GPS Power On
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="On";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_1_1;
				item->prev=NULL;
				//4-2-1-2-GPS Power Off
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Off";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_1_2;
				item->next=NULL;
			}
			else if (levels[2]==2) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-2-1-GPS Background Update Enabled
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Enable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_2_1;
				item->prev=NULL;
				//4-2-2-2-GPS Background Update Disabled
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Disable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_2_2;
				item->next=NULL;
			}
			else if (levels[2]==3) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-3-1-GPS Freq 0.5Hz
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="0.5 Hz";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_3_1;
				item->prev=NULL;
				//4-2-3-2-GPS Freq 1Hz
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="1 Hz";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_3_2;
				//4-2-3-3-GPS Freq 2Hz
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=3;
				item->desc="2 Hz";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_3_3;
				//4-2-3-4-GPS Freq 5Hz
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=4;
				item->desc="5 Hz";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(uint8_t));
				*((uint8_t*)(item->param))=SYS_GPS_3_4;
				item->next=NULL;
			}
		}
	}
	return menu;
}

// main menu program
//void app_mainmenu() {
//	menu_exec(mainmenu_load);
//}