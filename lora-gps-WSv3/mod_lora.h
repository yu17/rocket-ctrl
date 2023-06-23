#ifndef mod_lora
#define mod_lora 1.0

#include <Arduino.h>
#include <driver/rtc_io.h>

#include <SPI.h>
#include <SX126XLT.h>

#include "common_misc.h"
#include "mod_disp.h"
#include "mod_gps.h"
#include "mod_compass.h"

#define LORA_DEVICE DEVICE_SX1262               //we need to define the device we are using

// ----- Comm Protocol -----

#define LORAGPS_CLIENTID 0x1001

#define LORAGPS_HHLD_HEAD 0xA0
#define LORAGPS_TRCK_HEAD 0xA2

#define LORAGPS_INFO_TIME 0x30
#define LORAGPS_INFO_CORD 0x32
#define LORAGPS_INFO_MOTN 0x34
#define LORAGPS_INFO_STAT 0x36
#define LORAGPS_INFO_ACCR 0x38
#define LORAGPS_INFO_POSE 0x3A

#define LORAGPS_CTRL_POWR_ON 0x60
#define LORAGPS_CTRL_POWR_OFF 0x61
#define LORAGPS_CTRL_LED_ON 0x62
#define LORAGPS_CTRL_LED_OFF 0x63
#define LORAGPS_CTRL_LED_FLASH 0x64
#define LORAGPS_CTRL_LED_HEARTBEAT 0x65
#define LORAGPS_CTRL_GPSD_ON 0x70
#define LORAGPS_CTRL_GPSD_OFF 0x71
#define LORAGPS_CTRL_GPSD_FREQ 0x72
#define LORAGPS_CTRL_LORA_ON 0x80
#define LORAGPS_CTRL_LORA_OFF 0x81
#define LORAGPS_CTRL_LORA_INT 0x82
#define LORAGPS_CTRL_DISP_ON 0x90
#define LORAGPS_CTRL_DISP_OFF 0x91

#define LORAGPS_CTRL_PARAMLEN 4


//LoRa Modem Parameters
extern const uint32_t LoRa_Freq;				//frequency of transmissions in hertz
extern const uint32_t LoRa_Offset;				//offset frequency for calibration purposes
extern const uint8_t LoRa_Bandwidth;			//LoRa bandwidth
extern const uint8_t LoRa_SpreadingFactor;		//LoRa spreading factor
extern const uint8_t LoRa_CodeRate;				//LoRa coding rate
extern const uint8_t LoRa_Optimisation;			//low data rate optimisation setting, normally set to auto
extern const int8_t LoRa_TXpower;				//LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

extern SX126XLT LoRa;

extern int8_t PacketRSSI;
extern int8_t PacketSNR;
extern uint8_t PacketLen;
extern uint8_t PacketID;
extern uint8_t *PacketBuffer[255];

extern const size_t packet_header_size;

struct packet_frame_t{
	uint8_t magicnum;
	uint8_t pid;
	uint16_t cid;
	uint8_t sig;
	uint8_t fields[240];
};

struct packet_time_t{
	uint8_t hour,minute,second;
};

struct packet_cord_t{
	double latitude,longitude,altitude;
};

struct packet_motn_t{
	double speed;
	int32_t course,compass;
};

struct packet_stat_t{
	float vbat,HDOP;
	uint8_t SAT;
};

struct packet_accr_t{
	float acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z;
};

struct packet_pose_t{
	float temperature,agl_x,agl_y,agl_z,pitch,roll,yaw;
};

struct packet_ctrl_t{
	uint8_t parameter[LORAGPS_CTRL_PARAMLEN];
};

extern struct packet_frame_t pk_frame;
extern struct packet_time_t pk_time;
extern struct packet_cord_t pk_cord;
extern struct packet_motn_t pk_motn;
extern struct packet_stat_t pk_stat;
extern struct packet_accr_t pk_accr;
extern struct packet_pose_t pk_pose;
extern struct packet_ctrl_t pk_ctrl;

extern TaskHandle_t Task_Comm;
extern TaskHandle_t Task_Broadcast;

extern bool broadcast_bg_runflag;

void func_shipctrl_rx(void *param);

void func_shipinfo_update();

void func_shipinfo_broadcast_daemon(void *param);

void func_lora_setup();

void func_shipinfo_broadcast_enable();

void func_shipinfo_broadcast_disable();

#endif