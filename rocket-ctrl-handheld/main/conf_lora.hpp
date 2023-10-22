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
#define LORA_CONF_PRESETS 0x08

#define LORA_CONF_PRESET_SHORT_FAST 0x10
#define LORA_CONF_PRESET_SHORT_SLOW 0x11
#define LORA_CONF_PRESET_MEDIUM_FAST 0x12
#define LORA_CONF_PRESET_MEDIUM_SLOW 0x13
#define LORA_CONF_PRESET_LONG_FAST 0x14
#define LORA_CONF_PRESET_LONG_MODERATE 0x15
#define LORA_CONF_PRESET_LONG_SLOW 0x16
#define LORA_CONF_PRESET_VERYLONG_SLOW 0x17


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
	loraconfmenu_p_bandwidth,
	loraconfmenu_p_sf,
	loraconfmenu_p_cr,
	loraconfmenu_p_optim,
	loraconfmenu_p_presets;

extern const struct menu_item_t loraconfmenu[8],
	loraconfmenu_bandwidth[10],
	loraconfmenu_sf[8],
	loraconfmenu_cr[4],
	loraconfmenu_optim[3],
	loraconfmenu_presets[8];

uint8_t lora_init();

void* loraconf_enter(const void *param);

int8_t loraconf_menumark(const uint8_t param);

void* loraconf_config(const void* param);

#endif