#ifndef conf_lora
#define conf_lora 1.0

#include <SPI.h>
#include <SX126XLT.h>

#include "common_menu.hpp"

#define LORA_CONF_FREQ 0x01
#define LORA_CONF_OFFSET 0x02
#define LORA_CONF_BANDWIDTH 0x03
#define LORA_CONF_SPREADINGFACT 0x04
#define LORA_CONF_CODERATE 0x05
#define LORA_CONF_OPTIM 0x06
#define LORA_CONF_TXPOWER 0x07

// ----- LoRa -----
#define LORA_DEVICE DEVICE_SX1262

extern uint32_t LoRa_Freq;				//frequency of transmissions in hertz
extern uint32_t LoRa_Offset;			//offset frequency for calibration purposes
extern uint8_t LoRa_Bandwidth;			//LoRa bandwidth
extern uint8_t LoRa_SpreadingFactor;	//LoRa spreading factor
extern uint8_t LoRa_CodeRate;			//LoRa coding rate
extern uint8_t LoRa_Optimisation;		//low data rate optimisation setting, normally set to auto
extern int8_t LoRa_TXpower;				//LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

extern SX126XLT LoRa;
extern int8_t PacketRSSI;
extern int8_t PacketSNR;
extern uint8_t PacketLen;
extern uint8_t PacketID;
extern uint8_t *PacketBuffer[255];

// ----- LoRa configuration menu -----
extern const struct menu_page_t loraconfmenu_p,
	loraconfmenu_p_2,
	loraconfmenu_p_3,
	loraconfmenu_p_4,
	loraconfmenu_p_5,
	loraconfmenu_p_7,
	loraconfmenu_p_8;

extern const struct menu_item_t loraconfmenu[9],
	loraconfmenu_2[10],
	loraconfmenu_3[8],
	loraconfmenu_4[4],
	loraconfmenu_5[3];

void* loraconf_enter(const void *param);

int8_t loraconf_menumark(const uint8_t param);

void* loraconf_config(const void* param);

#endif