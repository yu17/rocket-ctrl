#ifndef conf_lora
#define conf_lora 1.0

#include <SPI.h>
#include <SX126XLT.h>

// ----- LoRa -----
#define LORA_DEVICE DEVICE_SX1262

extern const uint32_t LoRa_Freq;           //frequency of transmissions in hertz
extern const uint32_t LoRa_Offset;                      //offset frequency for calibration purposes
extern const uint8_t LoRa_Bandwidth;          //LoRa bandwidth
extern const uint8_t LoRa_SpreadingFactor;       //LoRa spreading factor
extern const uint8_t LoRa_CodeRate;           //LoRa coding rate
extern const uint8_t LoRa_Optimisation;         //low data rate optimisation setting, normally set to auto
extern const int8_t LoRa_TXpower;                      //LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

extern SX126XLT LoRa;
extern int8_t PacketRSSI;
extern int8_t PacketSNR;
extern uint8_t PacketLen;
extern uint8_t PacketID;
extern uint8_t *PacketBuffer[255];

#endif