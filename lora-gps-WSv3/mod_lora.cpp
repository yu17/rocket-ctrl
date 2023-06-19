#include "mod_lora.h"

//LoRa Modem Parameters
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

const size_t packet_header_size=sizeof(uint8_t)*3+sizeof(uint16_t);

struct packet_frame_t pk_frame;
struct packet_time_t pk_time;
struct packet_cord_t pk_cord;
struct packet_motn_t pk_motn;
struct packet_accr_t pk_accr;
struct packet_pose_t pk_pose;
struct packet_ctrl_t pk_ctrl;

TaskHandle_t Task_Comm;
TaskHandle_t Task_Broadcast;

bool broadcast_bg_runflag=0;

void func_shipctrl_rx(void *param) {
	struct packet_frame_t pk_probe;
	while (1) {
		PacketLen=LoRa.receive((uint8_t *)&pk_probe, sizeof(struct packet_frame_t), 0, WAIT_RX);
		PacketRSSI=LoRa.readPacketRSSI();
		PacketSNR=LoRa.readPacketSNR();
		PacketID=pk_probe.pid;
		if (PacketLen && pk_probe.magicnum==LORAGPS_MAG_HEAD) {
			switch (pk_probe.sig) {
				case LORAGPS_CTRL_POWR_OFF:
					digitalWrite(Vext,HIGH);
					rtc_gpio_init(GPIO_NUM_0);
					rtc_gpio_pullup_en(GPIO_NUM_0);
					esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,LOW);
					esp_deep_sleep_start();
					break;
				case LORAGPS_CTRL_LED_ON:
					digitalWrite(LED,HIGH);
					break;
				case LORAGPS_CTRL_LED_OFF:
					digitalWrite(LED,LOW);
					break;
				case LORAGPS_CTRL_GPSD_ON:
					vext_powered=1;
					digitalWrite(Vext,LOW);
					func_GPS_enable();
					break;
				case LORAGPS_CTRL_GPSD_OFF:
					func_GPS_disable();
					digitalWrite(Vext,HIGH);
					vext_powered=0;
					break;
				case LORAGPS_CTRL_LORA_ON:
					func_shipinfo_broadcast_enable();
					break;
				case LORAGPS_CTRL_LORA_OFF:
					func_shipinfo_broadcast_disable();
					break;
				case LORAGPS_CTRL_DISP_ON:
					disp_pwron();
					break;
				case LORAGPS_CTRL_DISP_OFF:
					disp_pwroff();
					break;
			}
		}
	}
}

void func_shipinfo_update() {
	pk_time.hour=GPS.time.hour();
	pk_time.minute=GPS.time.minute();
	pk_time.second=GPS.time.second();
	pk_cord.latitude=GPS.location.lat();
	pk_cord.longitude=GPS.location.lng();
	pk_cord.altitude=GPS.altitude.meters();
	pk_motn.speed=GPS.speed.mps();
	pk_motn.course=GPS.course.value();

//	gyro.read();
//	pk_accr.acc_x=gyro.getAccelX();
//	pk_accr.acc_y=gyro.getAccelY();
//	pk_accr.acc_z=gyro.getAccelZ();
//	pk_accr.gyro_x=gyro.getGyroX();
//	pk_accr.gyro_y=gyro.getGyroY();
//	pk_accr.gyro_z=gyro.getGyroZ();
//
//	pk_pose.temperature=gyro.getTemperature();
//	pk_pose.agl_x=gyro.getAngleX();
//	pk_pose.agl_y=gyro.getAngleY();
//	pk_pose.agl_z=gyro.getAngleZ();
//	pk_pose.pitch=gyro.getPitch();
//	pk_pose.roll=gyro.getRoll();
//	pk_pose.yaw=gyro.getYaw();
}

void func_shipinfo_broadcast_daemon(void* param) {
	pk_frame.magicnum=LORAGPS_MAG_HEAD;
	pk_frame.cid=LORAGPS_CLIENTID;
	pk_frame.pid=0;
	while (1) {
		func_shipinfo_update();

		pk_frame.sig=LORAGPS_INFO_TIME;
		pk_frame.pid++;
		memcpy(pk_frame.fields,&pk_time,sizeof(struct packet_time_t));
		LoRa.transmit((uint8_t *)&pk_frame,packet_header_size+sizeof(struct packet_time_t),0,LoRa_TXpower,WAIT_TX);
		delay(100);

		pk_frame.sig=LORAGPS_INFO_CORD;
		pk_frame.pid++;
		memcpy(pk_frame.fields,&pk_cord,sizeof(struct packet_cord_t));
		LoRa.transmit((uint8_t *)&pk_frame,packet_header_size+sizeof(struct packet_cord_t),0,LoRa_TXpower,WAIT_TX);
		delay(100);

		pk_frame.sig=LORAGPS_INFO_MOTN;
		pk_frame.pid++;
		memcpy(pk_frame.fields,&pk_motn,sizeof(struct packet_motn_t));
		LoRa.transmit((uint8_t *)&pk_frame,packet_header_size+sizeof(struct packet_motn_t),0,LoRa_TXpower,WAIT_TX);
		delay(100);

		pk_frame.sig=LORAGPS_INFO_ACCR;
		pk_frame.pid++;
		memcpy(pk_frame.fields,&pk_accr,sizeof(struct packet_accr_t));
		LoRa.transmit((uint8_t *)&pk_frame,packet_header_size+sizeof(struct packet_accr_t),0,LoRa_TXpower,WAIT_TX);
		delay(100);

		pk_frame.sig=LORAGPS_INFO_POSE;
		pk_frame.pid++;
		memcpy(pk_frame.fields,&pk_pose,sizeof(struct packet_pose_t));
		LoRa.transmit((uint8_t *)&pk_frame,packet_header_size+sizeof(struct packet_pose_t),0,LoRa_TXpower,WAIT_TX);
		delay(400);
	}
}

void func_lora_setup() {
	SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);
	while (!LoRa.begin(SS_LoRa,RST_LoRa,BUSY_LoRa,DIO1_LoRa,SW_LoRa,DEVICE_SX1262)) delay(2000);
	LoRa.setupLoRa(LoRa_Freq, LoRa_Offset, LoRa_SpreadingFactor, LoRa_Bandwidth, LoRa_CodeRate, LoRa_Optimisation);

	xTaskCreate(func_shipctrl_rx,"Comm RX",100000,NULL,0,&Task_Comm);
}

void func_shipinfo_broadcast_enable() {
	broadcast_bg_runflag=1;
	xTaskCreate(func_shipinfo_broadcast_daemon,"Shipinfo Broadcast",100000,NULL,0,&Task_Broadcast);
}

void func_shipinfo_broadcast_disable() {
	vTaskDelete(Task_Broadcast);
	broadcast_bg_runflag=0;
}