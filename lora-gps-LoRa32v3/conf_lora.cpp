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

struct menuitem_t *loraconf_menu(uint8_t levels[]) {
	struct menuitem_t *menu=NULL;
	if (levels[0]==0) {
		menu=(struct menuitem_t *)malloc(sizeof(struct menuitem_t));
		//1-Freq
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="Frequency";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		item->prev=NULL;
		//2-Offset
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="Offset";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
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
		item->desc="CodeRate";
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
}

//void* loraconf_config(void* param) {
