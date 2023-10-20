#include "app_mainmenu.hpp"

//struct menuitem_t *mainmenu;

const struct menu_page_t mainmenu_p_0={4,mainmenu_0};
const struct menu_page_t mainmenu_p_3={4,mainmenu_3};
const struct menu_page_t mainmenu_p_3_0={3,mainmenu_3_0};
const struct menu_page_t mainmenu_p_3_0_1={2,mainmenu_3_0_1};
const struct menu_page_t mainmenu_p_3_0_2={2,mainmenu_3_0_2};
const struct menu_page_t mainmenu_p_3_1={3,mainmenu_3_1};
const struct menu_page_t mainmenu_p_3_1_0={2,mainmenu_3_1_0};
const struct menu_page_t mainmenu_p_3_1_1={2,mainmenu_3_1_1};
const struct menu_page_t mainmenu_p_3_1_2={4,mainmenu_3_1_2};

const struct menu_item_t mainmenu_0[4]={
	{
		.id=0,
		.desc="Trackers",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&app_tracker},
		.param=NULL
	},
	{
		.id=1,
		.desc="LocalGPS",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&app_gps},
		.param=NULL
	},
	{
		.id=2,
		.desc="Sensors",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&app_sensors},
		.param=NULL
	},
	{
		.id=3,
		.desc="Settings",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3[4]={
	{
		.id=0,
		.desc="System",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_0},
		.param=NULL
	},
	{
		.id=1,
		.desc="GPS",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1},
		.param=NULL
	},
	{
		.id=2,
		.desc="LoRa",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_enter},
		.param=NULL
	},
	{
		.id=3,
		.desc="Sleep",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&func_deepsleep},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3_0[3]={
	{
		.id=0,
		.desc="Display",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_setbrightness},
		.param=NULL
	},
	{
		.id=1,
		.desc="Bat Volt",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_0_1},
		.param=NULL
	},
	{
		.id=2,
		.desc="Chip Temp",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_0_2},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3_0_1[2]={
	{
		.id=0,
		.desc="Enable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_0_1_0
	},
	{
		.id=1,
		.desc="Disable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_0_1_1
	}
};
const uint8_t mainmenu_param_3_0_1_0=SYS_VOLT_1;
const uint8_t mainmenu_param_3_0_1_1=SYS_VOLT_2;

const struct menu_item_t mainmenu_3_0_2[2]={
	{
		.id=0,
		.desc="Enable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_0_2_0
	},
	{
		.id=1,
		.desc="Disable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_0_2_1
	}
};
const uint8_t mainmenu_param_3_0_2_0=SYS_TEMP_1;
const uint8_t mainmenu_param_3_0_2_1=SYS_TEMP_2;

const struct menu_item_t mainmenu_3_1[3]={
	{
		.id=0,
		.desc="Power",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1_0},
		.param=NULL
	},
	{
		.id=1,
		.desc="Parser",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1_1},
		.param=NULL
	},
	{
		.id=2,
		.desc="Frequency",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1_2},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3_1_0[2]={
	{
		.id=0,
		.desc="On",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_0_0
	},
	{
		.id=1,
		.desc="Off",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_0_1
	}
};
const uint8_t mainmenu_param_3_1_0_0=SYS_GPS_1_1;
const uint8_t mainmenu_param_3_1_0_1=SYS_GPS_1_2;

const struct menu_item_t mainmenu_3_1_1[2]={
	{
		.id=0,
		.desc="Enable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_1_0
	},
	{
		.id=1,
		.desc="Disable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_1_1
	}
};
const uint8_t mainmenu_param_3_1_1_0=SYS_GPS_2_1;
const uint8_t mainmenu_param_3_1_1_1=SYS_GPS_2_2;

const struct menu_item_t mainmenu_3_1_2[4]={
	{
		.id=0,
		.desc="0.5 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_2_0
	},
	{
		.id=1,
		.desc="1 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_2_1
	},
	{
		.id=2,
		.desc="2 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_2_2
	},
	{
		.id=3,
		.desc="5 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=&mainmenu_param_3_1_2_3
	}
};
const uint8_t mainmenu_param_3_1_2_0=SYS_GPS_3_1;
const uint8_t mainmenu_param_3_1_2_1=SYS_GPS_3_2;
const uint8_t mainmenu_param_3_1_2_2=SYS_GPS_3_3;
const uint8_t mainmenu_param_3_1_2_3=SYS_GPS_3_4;

/*
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
*/

// main menu program
//void app_mainmenu() {
//	menu_exec(mainmenu_load);
//}