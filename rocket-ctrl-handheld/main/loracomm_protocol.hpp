#ifndef loracomm_protocol
#define loracomm_protocol 1.1

#include <Arduino.h>

// ----- Common Constants -----
#define LORACOMM_FISHNET 0x9830

#define LORACOMM_DEVTYP_HANDHELD 0xA0
#define LORACOMM_DEVTYP_TRACKER 0xA1
#define LORACOMM_DEVTYP_GATEWAY 0xA2
#define LORACOMM_DEVTYP_STATION 0xA3

#define LORACOMM_DEVTYP_MIN 0xA0
#define LORACOMM_DEVTYP_MAX 0xA2

#define LORACOMM_PKTTYP_ANNC 0x20// Announcement of device
#define LORACOMM_PKTTYP_ANAK 0x21// Reply to announcement
#define LORACOMM_PKTTYP_REQ 0x22
#define LORACOMM_PKTTYP_WETHRPT 0x23

#define LORACOMM_FRAME_LEN sizeof(uint8_t)+sizeof(uint16_t)*2
#define LORACOMM_REQ_LEN 4

// ----- Comm Protocol -----
// ----- Packet Frame -----
struct packet_frame_t{
	uint8_t device_type;
	uint16_t source_id;
	uint16_t target_id;
	// Contents below are all considered data
	uint8_t packet_type;
	uint8_t payload[240];
};

// ----- Contacts -----
struct contact_t{
	struct contact_t *prev,*next;
	uint8_t device_type;
	uint16_t id;
	uint8_t lastpkt_datalen;
	int16_t lastpkt_RSSI;
	int8_t lastpkt_SNR;
	uint8_t lastpkt_type;
	uint8_t data[240];
};

// ----- Payloads -----
// ----- Info Request -----
struct payload_req_t{
	uint8_t parameter[LORACOMM_REQ_LEN];
};

// ----- Weather Stations Reports -----
struct payload_wethrpt_t{
	float temperature;
	uint32_t pressure;
	float humidity;
	uint32_t gas_r;
};

// ----- pending modifications -----
struct payload_dat_time_t{
	uint8_t hour,minute,second;
};

struct payload_dat_cord_t{
	double latitude,longitude,altitude;
};

struct payload_dat_motn_t{
	double speed;
	int32_t course,compass;
};

struct payload_dat_stat_t{
	float vbat,HDOP;
	uint8_t SAT;
};

struct payload_dat_accr_t{
	float acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z;
};

struct payload_dat_pose_t{
	float temperature,agl_x,agl_y,agl_z,pitch,roll,yaw;
};

#endif