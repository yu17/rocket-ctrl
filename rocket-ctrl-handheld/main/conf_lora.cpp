#include "conf_lora.hpp"

uint32_t LoRa_Freq = 915000000;				//frequency of transmissions in hertz
uint32_t LoRa_Offset = 0;					//offset frequency for calibration purposes
uint8_t LoRa_Bandwidth = LORA_BW_125;		//LoRa bandwidth
uint8_t LoRa_SpreadingFactor = LORA_SF7;	//LoRa spreading factor
uint8_t LoRa_CodeRate = LORA_CR_4_5;		//LoRa coding rate
uint8_t LoRa_Optimisation = LDRO_AUTO;		//low data rate optimisation setting, normally set to auto
int8_t LoRa_TXpower = 22;					//LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

SX126XLT LoRa;
int8_t PacketRSSI;
int8_t PacketSNR;
uint8_t PacketLen;
uint8_t PacketID;
uint8_t *PacketBuffer[255];

void* loraconf_enter(const void *param) {menu_exec(&loraconfmenu_p);return NULL;}

const struct menu_page_t loraconfmenu_p={9,loraconfmenu,0};
const struct menu_page_t loraconfmenu_p_2={10,loraconfmenu_2,0};
const struct menu_page_t loraconfmenu_p_3={8,loraconfmenu_3,0};
const struct menu_page_t loraconfmenu_p_4={4,loraconfmenu_4,0};
const struct menu_page_t loraconfmenu_p_5={4,loraconfmenu_4,0};
const struct menu_page_t loraconfmenu_p_6={4,loraconfmenu_4,0};
const struct menu_page_t loraconfmenu_p_7={4,loraconfmenu_4,0};
const struct menu_page_t loraconfmenu_p_8={4,loraconfmenu_4,0};

const struct menu_item_t loraconfmenu[9]={
	{
		.desc="Frequency",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)LORA_CONF_FREQ
	},
	{
		.desc="Offset",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)LORA_CONF_OFFSET
	},
	{
		.desc="Bandwidth",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_2},
		.param=NULL
	},
	{
		.desc="SpreadingF",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_3},
		.param=NULL
	},
	{
		.desc="CodingRate",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_4},
		.param=NULL
	},
	{
		.desc="Optimise",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_5},
		.param=NULL
	},
	{
		.desc="Tx Power",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_6},
		.param=NULL
	},
	{
		.desc="Reset",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_7},
		.param=NULL
	},
	{
		.desc="Apply",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_8},
		.param=NULL
	}
};

const struct menu_item_t loraconfmenu_2[10]={
	{
		.desc="7810Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_007<<8))
	},
	{
		.desc="10420Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_010<<8))
	},
	{
		.desc="15630Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_015<<8))
	},
	{
		.desc="20830Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_020<<8))
	},
	{
		.desc="31250Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_031<<8))
	},
	{
		.desc="41670Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_041<<8))
	},
	{
		.desc="62500Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_062<<8))
	},
	{
		.desc="125000Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_125<<8))
	},
	{
		.desc="250000Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_250<<8))
	},
	{
		.desc="500000Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_500<<8))
	}
};

const struct menu_item_t loraconfmenu_3[8]={
	{
		.desc="2^5",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF5<<8))
	},
	{
		.desc="2^6",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF6<<8))
	},
	{
		.desc="2^7",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF7<<8))
	},
	{
		.desc="2^8",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF8<<8))
	},
	{
		.desc="2^9",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF9<<8))
	},
	{
		.desc="2^10",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF10<<8))
	},
	{
		.desc="2^11",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF11<<8))
	},
	{
		.desc="2^12",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF12<<8))
	}
};

const struct menu_item_t loraconfmenu_4[4]={
	{
		.desc="4/5",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_5<<8))
	},
	{
		.desc="4/6",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_6<<8))
	},
	{
		.desc="4/7",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_7<<8))
	},
	{
		.desc="4/8",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_8<<8))
	}
};

uint8_t *loraconf_menumark(uint8_t levels[]) {
	uint8_t *menumark=NULL;
	if (levels[0]==3) {
		menumark=(uint8_t *)malloc(11*sizeof(uint8_t));
		memset(menumark,0,11*sizeof(uint8_t));
		menumark[0]=11;
		menumark[1]=(LoRa_Bandwidth==LORA_BW_007);
		menumark[2]=(LoRa_Bandwidth==LORA_BW_010);
		menumark[3]=(LoRa_Bandwidth==LORA_BW_015);
		menumark[4]=(LoRa_Bandwidth==LORA_BW_020);
		menumark[5]=(LoRa_Bandwidth==LORA_BW_031);
		menumark[6]=(LoRa_Bandwidth==LORA_BW_041);
		menumark[7]=(LoRa_Bandwidth==LORA_BW_062);
		menumark[8]=(LoRa_Bandwidth==LORA_BW_125);
		menumark[9]=(LoRa_Bandwidth==LORA_BW_250);
		menumark[10]=(LoRa_Bandwidth==LORA_BW_500);
	}
	if (levels[0]==4) {
		menumark=(uint8_t *)malloc(9*sizeof(uint8_t));
		memset(menumark,0,9*sizeof(uint8_t));
		menumark[0]=9;
		menumark[1]=(LoRa_SpreadingFactor==LORA_SF5);
		menumark[2]=(LoRa_SpreadingFactor==LORA_SF6);
		menumark[3]=(LoRa_SpreadingFactor==LORA_SF7);
		menumark[4]=(LoRa_SpreadingFactor==LORA_SF8);
		menumark[5]=(LoRa_SpreadingFactor==LORA_SF9);
		menumark[6]=(LoRa_SpreadingFactor==LORA_SF10);
		menumark[7]=(LoRa_SpreadingFactor==LORA_SF11);
		menumark[8]=(LoRa_SpreadingFactor==LORA_SF12);
	}
	return menumark;
}

void* loraconf_config(const void* param) {
	if ((uint32_t)param==LORA_CONF_FREQ) LoRa_Freq=(uint32_t)(menu_numinput(3,3,LoRa_Freq/1000,"MHz")*1000);
	else if ((uint32_t)param==LORA_CONF_OFFSET) LoRa_Offset=(uint32_t)(menu_numinput(3,0,LoRa_Offset,"Hz"));
	else if (((uint8_t*)(&param))[0]==LORA_CONF_BANDWIDTH) LoRa_Bandwidth=((uint8_t*)(&param))[1];
	else if (((uint8_t*)(&param))[0]==LORA_CONF_SPREADINGFACT) LoRa_SpreadingFactor=((uint8_t*)(&param))[1];
	return NULL;
}