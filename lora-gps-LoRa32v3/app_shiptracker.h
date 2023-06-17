#ifndef app_shiptracker
#define app_shiptracker 1.0
#include <Arduino.h>
#include <driver/rtc_io.h>


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <SPI.h>
#include <SX126XLT.h>

#include "common_misc.h"
#include "conf_global.h"
#include "conf_lora.h"
#include "resc_display.h"
#include "resc_keyboard.h"
#include "resc_gps.h"

// ----- Comm Protocol -----
struct packet_frame_t{
	uint8_t magicnum;
	uint8_t sig;
	uint16_t cid;
	uint8_t pid;
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
	int32_t course;
};

struct packet_accr_t{
	float acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z;
};

struct packet_pose_t{
	float temperature,agl_x,agl_y,agl_z,pitch,roll,yaw;
};

// ----- Ship Tracking Database -----
struct ship_data_t{
	uint16_t ship_cid;
	struct packet_time_t ship_time;
	struct packet_cord_t ship_cord;
	struct packet_motn_t ship_motn;
	struct packet_accr_t ship_accr;
	struct packet_pose_t ship_pose;
	struct ship_data_t *prev,*next;
};

struct ship_list_t{
	struct ship_data_t *ship;
};

void func_tracker_shiplist_update(void *param);

void func_tracker_shipinfo(struct ship_data_t *ship);

void func_tracker_shiplist_render(struct ship_data_t *ship, uint8_t pos, uint8_t scale);

void func_tracker_selship(struct ship_list_t *shiplist);

void func_tracker_destroy_shiplist(struct ship_list_t *shiplist);

void *app_tracker(void *param);

#endif