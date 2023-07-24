#include "conf_lora.h"

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

void* loraconf_enter(void *param) {menu_exec(&loraconf_menu,&loraconf_menumark);return NULL;}

struct menuitem_t *loraconf_menu(uint8_t levels[]) {
	struct menuitem_t *menu=NULL;
	if (levels[0]==0) {
		menu=(struct menuitem_t *)malloc(sizeof(struct menuitem_t));
		//1-Freq
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="Frequency";
		item->enter_behavior=1;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t));
		*((uint8_t*)(item->param))=LORA_CONF_FREQ;
		item->prev=NULL;
		//2-Offset
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="Offset";
		item->enter_behavior=1;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t));
		*((uint8_t*)(item->param))=LORA_CONF_OFFSET;
		//3-Bandwidth
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=3;
		item->desc="Bandwidth";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		//4-SpreadingFactor
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=4;
		item->desc="SpreadingF";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		//5-CodeRate
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=5;
		item->desc="CodingRate";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		//6-Optimisation
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=6;
		item->desc="Optimise";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		//7-TX Power
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=7;
		item->desc="Tx Power";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		//8-Reset
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=8;
		item->desc="Reset";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		//9-Apply
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=9;
		item->desc="Apply";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		item->next=NULL;
	}
	else if (levels[0]==3) {
		//3-Bandwidth
		menu=(struct menuitem_t *)malloc(sizeof(struct menuitem_t));
		//3-1-7810Hz
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="7810Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_007;
		item->prev=NULL;
		//3-2-10420Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="10420Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_010;
		//3-3-15630Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=3;
		item->desc="15630Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_015;
		//3-4-20830Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=4;
		item->desc="20830Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_020;
		//3-5-31250Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=5;
		item->desc="31250Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_031;
		//3-6-41670Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=6;
		item->desc="41670Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_041;
		//3-7-62500Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=7;
		item->desc="62500Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_062;
		//3-8-125000Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=8;
		item->desc="125000Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_125;
		//3-9-250000Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=9;
		item->desc="250000Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_250;
		//3-10-500000Hz
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=10;
		item->desc="500000Hz";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_BANDWIDTH;
		((uint8_t*)(item->param))[1]=LORA_BW_500;
		item->next=NULL;
	}
	else if (levels[0]==4) {
		//4-SpreadingFactor
		menu=(struct menuitem_t *)malloc(sizeof(struct menuitem_t));
		//4-1-SF5
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="2^5";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF5;
		item->prev=NULL;
		//4-2-SF6
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="2^6";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF6;
		//4-3-SF7
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=3;
		item->desc="2^7";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF7;
		//4-4-SF8
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=4;
		item->desc="2^8";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF8;
		//4-5-SF9
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=5;
		item->desc="2^9";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF9;
		//4-6-SF10
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=6;
		item->desc="2^10";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF10;
		//4-7-SF11
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=7;
		item->desc="2^11";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF11;
		//4-8-SF12
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=8;
		item->desc="2^12";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_SPREADINGFACT;
		((uint8_t*)(item->param))[1]=LORA_SF12;
		item->next=NULL;
	}
	else if (levels[0]==5) {
		//5-CodingRate
		menu=(struct menuitem_t *)malloc(sizeof(struct menuitem_t));
		//5-1-4/5
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="4/5";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_CODERATE;
		((uint8_t*)(item->param))[1]=LORA_CR_4_5;
		item->prev=NULL;
		//5-2-4/6
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="4/6";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_CODERATE;
		((uint8_t*)(item->param))[1]=LORA_CR_4_6;
		//5-3-4/7
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=3;
		item->desc="4/7";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_CODERATE;
		((uint8_t*)(item->param))[1]=LORA_CR_4_7;
		//5-4-4/8
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=4;
		item->desc="4/8";
		item->enter_behavior=2;
		item->drop_menu=0;
		item->enter=&loraconf_config;
		item->param=malloc(sizeof(uint8_t)*2);
		((uint8_t*)(item->param))[0]=LORA_CONF_CODERATE;
		((uint8_t*)(item->param))[1]=LORA_CR_4_8;
		item->next=NULL;
	}
	return menu;
}

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

void* loraconf_config(void* param) {
	if (*((uint8_t*)param)==LORA_CONF_FREQ) LoRa_Freq=(uint32_t)(menu_numinput(3,3,LoRa_Freq/1000,"MHz")*1000);
	else if (*((uint8_t*)param)==LORA_CONF_OFFSET) LoRa_Offset=(uint32_t)(menu_numinput(3,0,LoRa_Offset,"Hz"));
	else if (((uint8_t*)param)[0]==LORA_CONF_BANDWIDTH) LoRa_Bandwidth=((uint8_t*)param)[1];
	else if (((uint8_t*)param)[0]==LORA_CONF_SPREADINGFACT) LoRa_SpreadingFactor=((uint8_t*)param)[1];
	return NULL;
}