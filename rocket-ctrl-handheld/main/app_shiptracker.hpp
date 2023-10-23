#ifndef app_shiptracker
#define app_shiptracker 1.0
#include <Arduino.h>
#include <driver/rtc_io.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>
#include <SX126XLT.h>

#include "common_misc.hpp"
#include "resc_lora.hpp"
#include "resc_display.hpp"
#include "resc_joystick.hpp"
#include "resc_gps.hpp"
#include "resc_bmp280.hpp"
#include "resc_QMC5883L.hpp"

// ----- Comm Protocol -----
#define LORAGPS_HANDHELDID 0x0001

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

#define LORAGPS_CTRL_IGNITION 0x50

#define LORAGPS_CTRL_PARAMLEN 4

// ----- Comm Protocol -----
struct packet_frame_t_dep{
	uint8_t magicnum;
	uint8_t pid;
	uint16_t cid;
	uint8_t sig;
	uint8_t fields[240];
};

extern const size_t packet_header_size;

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

// ----- Ship Tracking Database -----
struct ship_data_t{
	uint16_t ship_cid;
	struct packet_time_t ship_time;
	struct packet_cord_t ship_cord;
	struct packet_motn_t ship_motn;
	struct packet_stat_t ship_stat;
	struct packet_accr_t ship_accr;
	struct packet_pose_t ship_pose;
	struct ship_data_t *prev,*next;
};

struct ship_list_t{
	struct ship_data_t *ship;
};

// ----- Shipctrl Menu -----
extern const struct menu_page_t shipctrlmenu_p,
    shipctrlmenu_p_0,
	shipctrlmenu_p_1,
	shipctrlmenu_p_2,
	shipctrlmenu_p_3,
	shipctrlmenu_p_4,
	shipctrlmenu_p_5;

extern const struct menu_item_t shipctrlmenu[6],
    shipctrlmenu_0[4],
	shipctrlmenu_1[3],
	shipctrlmenu_2[3],
	shipctrlmenu_3[3],
	shipctrlmenu_4[1],
	shipctrlmenu_5[1];

void* func_tracker_shipctrl_sendcommand(const void* param);

void func_tracker_shiplist_update(void *param);

void func_tracker_shipinfo(struct ship_data_t *ship);

void func_tracker_shiplist_render(struct ship_data_t *ship, uint8_t pos, uint8_t scale);

void func_tracker_selship(struct ship_list_t *shiplist);

void func_tracker_destroy_shiplist(struct ship_list_t *shiplist);

void *app_tracker(const void *param);

#endif