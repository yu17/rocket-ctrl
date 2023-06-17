#include "conf_lora.h"

const uint32_t LoRa_Freq = 915000000;           //frequency of transmissions in hertz
const uint32_t LoRa_Offset = 0;                      //offset frequency for calibration purposes
const uint8_t LoRa_Bandwidth = LORA_BW_125;          //LoRa bandwidth
const uint8_t LoRa_SpreadingFactor = LORA_SF7;       //LoRa spreading factor
const uint8_t LoRa_CodeRate = LORA_CR_4_5;           //LoRa coding rate
const uint8_t LoRa_Optimisation = LDRO_AUTO;         //low data rate optimisation setting, normally set to auto
const int8_t LoRa_TXpower = 22;                      //LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

SX126XLT LoRa;
int8_t PacketRSSI;
int8_t PacketSNR;
uint8_t PacketLen;
uint8_t PacketID;
uint8_t *PacketBuffer[255];