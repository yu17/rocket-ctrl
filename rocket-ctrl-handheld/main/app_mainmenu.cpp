#include "app_mainmenu.hpp"

//struct menuitem_t *mainmenu;

const struct menu_page_t mainmenu_p={4,mainmenu,NULL,0};
const struct menu_page_t mainmenu_p_3={4,mainmenu_3,NULL,0};
const struct menu_page_t mainmenu_p_3_0={3,mainmenu_3_0,NULL,0};
const struct menu_page_t mainmenu_p_3_0_1={2,mainmenu_3_0_1,NULL,0};
const struct menu_page_t mainmenu_p_3_0_2={2,mainmenu_3_0_2,NULL,0};
const struct menu_page_t mainmenu_p_3_1={3,mainmenu_3_1,NULL,0};
const struct menu_page_t mainmenu_p_3_1_0={2,mainmenu_3_1_0,NULL,0};
const struct menu_page_t mainmenu_p_3_1_1={2,mainmenu_3_1_1,NULL,0};
const struct menu_page_t mainmenu_p_3_1_2={4,mainmenu_3_1_2,NULL,0};

const struct menu_item_t mainmenu[4]={
	{
		.desc="Trackers",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&app_tracker},
		.param=NULL
	},
	{
		.desc="LocalGPS",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&app_gps},
		.param=NULL
	},
	{
		.desc="Sensors",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&app_sensors},
		.param=NULL
	},
	{
		.desc="Settings",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3[4]={
	{
		.desc="System",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_0},
		.param=NULL
	},
	{
		.desc="GPS",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1},
		.param=NULL
	},
	{
		.desc="LoRa",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_enter},
		.param=NULL
	},
	{
		.desc="Sleep",
		.enter_behavior=1,
		.drop_menu=1,
		.routine={.func=&func_deepsleep},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3_0[3]={
	{
		.desc="Display",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_setbrightness},
		.param=NULL
	},
	{
		.desc="Bat Volt",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_0_1},
		.param=NULL
	},
	{
		.desc="Chip Temp",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_0_2},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3_0_1[2]={
	{
		.desc="Enable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_VOLT_1//&mainmenu_param_3_0_1_0
	},
	{
		.desc="Disable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_VOLT_2//&mainmenu_param_3_0_1_1
	}
};

const struct menu_item_t mainmenu_3_0_2[2]={
	{
		.desc="Enable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_TEMP_1//&mainmenu_param_3_0_2_0
	},
	{
		.desc="Disable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_TEMP_2//&mainmenu_param_3_0_2_1
	}
};

const struct menu_item_t mainmenu_3_1[3]={
	{
		.desc="Power",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1_0},
		.param=NULL
	},
	{
		.desc="Parser",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1_1},
		.param=NULL
	},
	{
		.desc="Frequency",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&mainmenu_p_3_1_2},
		.param=NULL
	}
};

const struct menu_item_t mainmenu_3_1_0[2]={
	{
		.desc="On",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_1_1//&mainmenu_param_3_1_0_0
	},
	{
		.desc="Off",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_1_2//&mainmenu_param_3_1_0_1
	}
};

const struct menu_item_t mainmenu_3_1_1[2]={
	{
		.desc="Enable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_2_1//&mainmenu_param_3_1_1_0
	},
	{
		.desc="Disable",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_2_2//&mainmenu_param_3_1_1_1
	}
};

const struct menu_item_t mainmenu_3_1_2[4]={
	{
		.desc="0.5 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_3_1//&mainmenu_param_3_1_2_0
	},
	{
		.desc="1 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_3_2//&mainmenu_param_3_1_2_1
	},
	{
		.desc="2 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_3_3//&mainmenu_param_3_1_2_2
	},
	{
		.desc="5 Hz",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&func_quick_settings},
		.param=(void*)SYS_GPS_3_4//&mainmenu_param_3_1_2_3
	}
};
