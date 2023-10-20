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

void* loraconf_enter(const void *param);

struct menuitem_t *loraconf_menu(uint8_t levels[]);

uint8_t *loraconf_menumark(uint8_t levels[]);

void* loraconf_config(void* param);

#endif