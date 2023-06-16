#include <Arduino.h>
#include <driver/rtc_io.h>
#include <driver/temp_sensor.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#include <TinyGPS++.h>

#include <SPI.h>
#include <SX126XLT.h>

// ----- OLED -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define SCREEN_I2C_SDA 17
#define SCREEN_I2C_SCL 18

// ----- BME680 -----
#define BME680_I2C_SDA 3
#define BME680_I2C_SCL 2

// ----- PRGSW/Keyboard -----
#define PRGSW_PIN 0

// ----- LoRa -----
#define LORA_DEVICE DEVICE_SX1262               //we need to define the device we are using

// ----- Comm Protocol -----
#define LORAGPS_MAG_HEAD 0x89
#define LORAGPS_TYP_TIME 0x30
#define LORAGPS_TYP_CORD 0x32
#define LORAGPS_TYP_MOTN 0x34
#define LORAGPS_TYP_ACCR 0x36
#define LORAGPS_TYP_POSE 0x38

// ----- Settings -----
#define SYS_GPS_1_1 0x22
#define SYS_GPS_1_2 0x23
#define SYS_GPS_2_1 0x24
#define SYS_GPS_2_2 0x25
#define SYS_GPS_3_1 0x26
#define SYS_GPS_3_2 0x27
#define SYS_GPS_3_3 0x28
#define SYS_GPS_3_4 0x29
#define SYS_VOLT_1 0x00
#define SYS_VOLT_2 0x01
#define SYS_TEMP_1 0x02
#define SYS_TEMP_2 0x03

// ----- MISC -----
#define TICKINT 10
#define BUFFER_SIZE 128

// ----- OLED -----
TwoWire I2C_SSD1306=TwoWire(0);
Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SSD1306, RST_OLED);
bool disp_flag_on;
uint8_t disp_brightness;

// ----- BME680 -----
TwoWire I2C_BME680=TwoWire(1);
Adafruit_BME680 BME680(&I2C_BME680);

// ----- Battery Voltage -----
TaskHandle_t Task_batvolt;
bool batvolt_flag_enabled;
float batvolt_value;

// ----- Serial/GPS -----
TinyGPSPlus GPS;
TaskHandle_t Task_GPS;
bool GPS_bg_runflag;

// ----- PRGSW/Keyboard -----
bool PRGSW_def,PRGSW_act;

const uint8_t KBD_ROW[4]={42,41,40,39};
const uint8_t KBD_COL[4]={34,33,47,48};
const char KBD_GRID[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
char c;

// ----- LoRa -----
const uint32_t LoRa_Freq = 915000000;           //frequency of transmissions in hertz
const uint32_t LoRa_Offset = 0;                      //offset frequency for calibration purposes
const uint8_t LoRa_Bandwidth = LORA_BW_125;          //LoRa bandwidth
const uint8_t LoRa_SpreadingFactor = LORA_SF7;       //LoRa spreading factor
const uint8_t LoRa_CodeRate = LORA_CR_4_5;           //LoRa coding rate
const uint8_t LoRa_Optimisation = LDRO_AUTO;         //low data rate optimisation setting, normally set to auto
const int8_t LoRa_TXpower = 10;                      //LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

SX126XLT LoRa;
int8_t PacketRSSI;
int8_t PacketSNR;
uint8_t PacketLen;
uint8_t *PacketBuffer[255];

// ----- Comm Protocol -----
struct packet_frame_t{
	uint8_t magicnum;
	uint8_t sig;
	uint16_t cid;
	uint8_t fields[250];
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

// ----- Menu -----
struct menuitem_t{
	uint8_t id;
	const char *desc;
	bool enter_behavior;//0 - Menu; 1 - Function
	bool drop_menu;
	void* (*enter)(void*);
	void *param;
	struct menuitem_t *prev,*next;
};

struct menuitem_t *mainmenu;

// ----- MISC -----
uint8_t TICK;
char buffer[BUFFER_SIZE];
uint8_t buffer_pt;

// ----- display functions -----

void disp_switch(){
	if (disp_flag_on) disp.ssd1306_command(SSD1306_DISPLAYOFF);
	else disp.ssd1306_command(SSD1306_DISPLAYON);
	disp_flag_on=!disp_flag_on;
}

void disp_swdisplay(Adafruit_SSD1306 *disp){
	if (disp_flag_on) disp->display();
}

void disp_setbrightness(uint8_t value){
	disp.ssd1306_command(SSD1306_SETCONTRAST);
	disp.ssd1306_command(value);
}

// ----- keyboard functions -----

char kbd_read(uint8_t pollint){
	PRGSW_act=0;
	char key=0;
	uint8_t keyi=0,keyj=0;
	while (!PRGSW_act) {
		for (uint8_t i=0;i<4;i++) {
			digitalWrite(KBD_ROW[i],LOW);
			for (uint8_t j=0;j<4;j++) 
				if (digitalRead(KBD_COL[j])==LOW) {
					digitalWrite(KBD_ROW[i],HIGH);
					key=KBD_GRID[i][j];
					keyi=i;
					keyj=j;
					PRGSW_act=1;
					break;
				}
			digitalWrite(KBD_ROW[i],HIGH);
		}
		if (digitalRead(PRGSW_PIN)!=PRGSW_def) {
			key='P';
			PRGSW_act=1;
		}
		if (pollint) delay(pollint);
		else break;
	}
	while (PRGSW_act) {
		if (key=='P' && digitalRead(PRGSW_PIN)==PRGSW_def) PRGSW_act=0;
		else if (key!='P') {
			digitalWrite(KBD_ROW[keyi],LOW);
			if (digitalRead(KBD_COL[keyj])==HIGH) PRGSW_act=0;
		}
		if (pollint) delay(pollint);
		else delay(4);
	}
	return key;
}

// ----- GPS background functions -----
void func_GPS_update(void *param) {
	while (1) {
		while (Serial1.available()>0) GPS.encode(Serial1.read());
		delay(100);
	}
}

// ----- battery voltage background functions -----
void func_batvolt_update(void *param) {
	int raw;
	while (1) {
		raw=analogRead(A0);
		batvolt_value=(float)raw/(float)4095*3.6*(float)490/(float)100;
		delay(1000);
	}
}

// ----- main menu functions -----

void menu_render(struct menuitem_t *item, uint8_t pos, uint8_t scale){
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	disp.setTextSize(scale);
	disp.setCursor(6*scale, 8*scale*pos);
	//disp.write(item->desc, 32-11*scale);
	disp.write(item->desc);
	uint8_t pospt=pos;
	struct menuitem_t *itempt=item;
	while (pospt>0 && itempt->prev) {
		pospt--;
		itempt=itempt->prev;
		disp.setCursor(6*scale, 8*scale*pospt);
		disp.write(itempt->desc);
	}
	pospt=pos;
	itempt=item;
	while (pospt<(11-scale*4) && itempt->next) {
		pospt++;
		itempt=itempt->next;
		disp.setCursor(6*scale, 8*scale*pospt);
		disp.write(itempt->desc);
	}
	disp.setCursor(0, 8*scale*pos);
	disp.write(">");
	disp.display();
}

// Setup static menu entries
struct menuitem_t *menu_load(uint8_t levels[]){
	struct menuitem_t *menu=NULL;
	if (levels[0]==0) {
		// Main menu
		menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		//1-Trackers
		struct menuitem_t *item=menu;
		item->id=1;
		item->desc="Trackers";
		item->enter_behavior=1;
		item->drop_menu=1;
		item->enter=&app_tracker;
		item->param=NULL;
		item->prev=NULL;
		//2-LocalGPS
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=2;
		item->desc="LocalGPS";
		item->enter_behavior=1;
		item->drop_menu=1;
		item->enter=&app_gps;
		item->param=NULL;
		//3-Sensors
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=3;
		item->desc="Sensors";
		item->enter_behavior=1;
		item->drop_menu=1;
		item->enter=&app_bme680;
		item->param=NULL;
		//4-Settings
		item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
		item->next->prev=item;
		item=item->next;
		item->id=4;
		item->desc="Settings";
		item->enter_behavior=0;
		item->drop_menu=0;
		item->enter=NULL;
		item->param=NULL;
		item->next=NULL;
	}
	else if (levels[0]==4) {
		if (levels[1]==0) {
			// 4-Settings
			menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			//4-1-System
			struct menuitem_t *item=menu;
			item->id=1;
			item->desc="System";
			item->enter_behavior=0;
			item->drop_menu=0;
			item->enter=NULL;
			item->param=NULL;
			item->prev=NULL;
			//4-2-GPS
			item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			item->next->prev=item;
			item=item->next;
			item->id=2;
			item->desc="GPS";
			item->enter_behavior=0;
			item->drop_menu=0;
			item->enter=NULL;
			item->param=NULL;
			//4-3-LoRa
			item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			item->next->prev=item;
			item=item->next;
			item->id=3;
			item->desc="LoRa";
			item->enter_behavior=0;
			item->drop_menu=0;
			item->enter=NULL;
			item->param=NULL;
			//4-4-Sleep
			item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
			item->next->prev=item;
			item=item->next;
			item->id=4;
			item->desc="Sleep";
			item->enter_behavior=1;
			item->drop_menu=1;
			item->enter=&func_deepsleep;
			item->param=NULL;
			item->next=NULL;
		}
		else if (levels[1]==1) {
			if (levels[2]==0) {
				//4-1-System
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-1-1-Display
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Display";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_setbrightness;
				item->param=NULL;
				item->prev=NULL;
				//4-1-2-Background Voltage Update
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Bat Volt";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				//4-1-3-Background Chip Temperature Update
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=3;
				item->desc="Chip Temp";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				item->next=NULL;
			}
			else if (levels[2]==2) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-1-2-1-Background Voltage Update Enabled
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Enable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_VOLT_1;
				item->prev=NULL;
				//4-1-2-2-Background Voltage Update Disabled
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Disable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_VOLT_2;
				item->next=NULL;
			}
			else if (levels[2]==3) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-1-3-1-Background Chip Temperature Update Enabled
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Enable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_TEMP_1;
				item->prev=NULL;
				//4-1-3-2-Background Chip Temperature Update Disabled
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Disable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_TEMP_2;
				item->next=NULL;
			}
		}
		else if (levels[1]==2) {
			if (levels[2]==0) {
				//4-2-GPS
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-1-GPS Power
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Power";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				item->prev=NULL;
				//4-2-2-GPS Background Update
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Parser";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				//4-2-3-GPS Update Frequency
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=3;
				item->desc="Frequency";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=NULL;
				item->param=NULL;
				item->next=NULL;
			}
			else if (levels[2]==1) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-1-1-GPS Power On
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="On";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_1_1;
				item->prev=NULL;
				//4-2-1-2-GPS Power Off
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Off";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_1_2;
				item->next=NULL;
			}
			else if (levels[2]==2) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-2-1-GPS Background Update Enabled
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="Enable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_2_1;
				item->prev=NULL;
				//4-2-2-2-GPS Background Update Disabled
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="Disable";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_2_2;
				item->next=NULL;
			}
			else if (levels[2]==3) {
				menu=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				//4-2-3-1-GPS Freq 0.5Hz
				struct menuitem_t *item=menu;
				item->id=1;
				item->desc="0.5 Hz";
				item->enter_behavior=1;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_3_1;
				item->prev=NULL;
				//4-2-3-2-GPS Freq 1Hz
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=2;
				item->desc="1 Hz";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_3_2;
				//4-2-3-3-GPS Freq 2Hz
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=3;
				item->desc="2 Hz";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_3_3;
				//4-2-3-4-GPS Freq 5Hz
				item->next=(struct menuitem_t*)malloc(sizeof(struct menuitem_t));
				item->next->prev=item;
				item=item->next;
				item->id=4;
				item->desc="5 Hz";
				item->enter_behavior=0;
				item->drop_menu=0;
				item->enter=&func_quick_settings;
				item->param=malloc(sizeof(int));
				*((int*)(item->param))=SYS_GPS_3_4;
				item->next=NULL;
			}
		}
	}
	return menu;
}

void menu_destroy(struct menuitem_t *item) {
	while (item->prev) item=item->prev;
	while (item->next) {
		item=item->next;
		if (item->prev->param) free(item->prev->param);
		free(item->prev);
	}
	free(item);
}

// main menu program
uint8_t func_menu(){
	uint8_t menu_stack[10];
	memset(&menu_stack,0,10*sizeof(uint8_t));
	uint8_t stackpt=0;
	uint8_t pos=0;
	struct menuitem_t *item=menu_load(menu_stack);
	void* (*enter)(void*)=NULL;
	void* param=NULL;
	char key;
	while (item) {
		menu_render(item, pos, 2);
		key=kbd_read(4);
		switch (key) {
			case 'A':
				if (item->prev) {
					item=item->prev;
					if (pos>1 || (pos==1 && !item->prev)) pos--;
				}
				break;
			case 'B':
				if (item->next) {
					item=item->next;
					if (pos<2 || (pos==2 && !item->next)) pos++;
				}
				break;
			case 'C':
				if (stackpt) {
					stackpt--;
					pos=menu_stack[stackpt];
					menu_stack[stackpt]=0;
					menu_destroy(item);
					item=menu_load(menu_stack);
					while (item->id!=pos && item->next) item=item->next;
					if (pos==1) pos=0;
					else pos=1;
				}
				else {
					menu_destroy(item);
					item=NULL;
				}
				break;
			case 'D':
				if (item->enter_behavior) {
					if (item->drop_menu) {
						enter=item->enter;
						param=item->param;
						menu_destroy(item);
						item=NULL;
					}
					else (*(item->enter))(item->param);
				}
				else {
					menu_stack[stackpt++]=item->id;
					menu_destroy(item);
					item=menu_load(menu_stack);
					pos=0;
				}
				break;
			default:
				break;
		}
	}
	disp.clearDisplay();
	disp.display();
	if (enter) (*enter)(param);
	return 0;
}

// ----- misc functions -----

void *func_setbrightness(void *param) {
	disp.setTextSize(1);
	char key;
	uint8_t brightness_cache=disp_brightness;
	while (1) {
		disp.clearDisplay();
		disp.fillRect(12,18,104,24,SSD1306_INVERSE);
		disp.fillRect(14,20,100,20,SSD1306_INVERSE);
		disp.fillRect(14,20,brightness_cache*4/10,20,SSD1306_INVERSE);
		if (brightness_cache<30) disp.setCursor(58, 48);
		else if (brightness_cache==250) disp.setCursor(52, 48);
		else disp.setCursor(55, 48);
		sprintf(buffer,"%d%%",brightness_cache*4/10);
		disp.write(buffer);
		disp.display();
		key=kbd_read(4);
		switch (key) {
			case 'A':
				if (brightness_cache<250) {
					brightness_cache+=10;
					disp_setbrightness(brightness_cache);
				}
				break;
			case 'B':
				if (brightness_cache>0) {
					brightness_cache-=10;
					disp_setbrightness(brightness_cache);
				}
				break;
			case 'C':
				disp_setbrightness(disp_brightness);
				return NULL;
				break;
			case 'D':
				disp_brightness=brightness_cache;
				return NULL;
				break;
		}
	}
	return NULL;
}

void *func_quick_settings(void *param) {
	if (*((int*)param)==SYS_GPS_1_1) digitalWrite(Vext,LOW);
	else if (*((int*)param)==SYS_GPS_1_2) digitalWrite(Vext,HIGH);
	else if (*((int*)param)==SYS_GPS_2_1 && !GPS_bg_runflag) {
		xTaskCreatePinnedToCore(func_GPS_update,"GPS Parse",100000,NULL,0,&Task_GPS,0);
		GPS_bg_runflag=true;
	}
	else if (*((int*)param)==SYS_GPS_2_2 && GPS_bg_runflag) {
		vTaskDelete(Task_GPS);
		GPS_bg_runflag=false;
	}
	else if (*((int*)param)==SYS_VOLT_1 && !batvolt_flag_enabled) {
		xTaskCreate(func_batvolt_update,"Battery Voltage",10000,NULL,0,&Task_batvolt);
		batvolt_flag_enabled=true;
	}
	else if (*((int*)param)==SYS_VOLT_2 && batvolt_flag_enabled) {
		vTaskDelete(Task_batvolt);
		batvolt_flag_enabled=false;
	}
	else if (*((int*)param)==SYS_TEMP_1 && !esp32_tsensor_flag_enabled) {
		temp_sensor_start();
		xTaskCreate(func_esp32_tsensor_update,"esp32 internal tsensor",10000,NULL,0,&Task_esp32_tsensor);
		esp32_tsensor_flag_enabled=true;
	}
	else if (*((int*)param)==SYS_TEMP_2 && esp32_tsensor_flag_enabled) {
		vTaskDelete(Task_esp32_tsensor);
		temp_sensor_stop();
		esp32_tsensor_flag_enabled=false;
	}
	return NULL;
}

void *func_deepsleep(void *param) {
	digitalWrite(Vext,HIGH);
	rtc_gpio_init(GPIO_NUM_0);
	rtc_gpio_pullup_en(GPIO_NUM_0);
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,LOW);
	esp_deep_sleep_start();
//	rtc_gpio_deinit(GPIO_NUM_0);
//	pinMode(PRGSW_PIN,INPUT);
//	digitalWrite(Vext,LOW);
}

//tracker maintain shipdb
void func_tracker_shiplist_update(void *param) {
	if (!param) return;
	struct ship_list_t *shiplist=(struct ship_list_t *)param;
	struct ship_data_t *ship=shiplist->ship;
	struct packet_frame_t pk_probe;
	while (1) {
		PacketLen=LoRa.receive((uint8_t *)&pk_probe, sizeof(struct packet_frame_t), 0, WAIT_RX);
		PacketRSSI=LoRa.readPacketRSSI();
		PacketSNR=LoRa.readPacketSNR();
		if (PacketLen && pk_probe.magicnum==LORAGPS_MAG_HEAD) {
			while (ship && ship->ship_cid!=pk_probe.cid) ship=ship->next;
			if (!ship) {
				ship=(struct ship_data_t *)malloc(sizeof(struct ship_data_t));
				ship->ship_cid=pk_probe.cid;
				ship->next=shiplist->ship;
				if (shiplist->ship) shiplist->ship->prev=ship;
				ship->prev=NULL;
				shiplist->ship=ship;
			}
			switch (pk_probe.sig) {
				case LORAGPS_TYP_TIME:
					memcpy(&(ship->ship_time),pk_probe.fields,sizeof(struct packet_time_t));
					break;
				case LORAGPS_TYP_CORD:
					memcpy(&(ship->ship_cord),pk_probe.fields,sizeof(struct packet_cord_t));
					break;
				case LORAGPS_TYP_MOTN:
					memcpy(&(ship->ship_motn),pk_probe.fields,sizeof(struct packet_motn_t));
					break;
				case LORAGPS_TYP_ACCR:
					memcpy(&(ship->ship_accr),pk_probe.fields,sizeof(struct packet_accr_t));
					break;
				case LORAGPS_TYP_POSE:
					memcpy(&(ship->ship_pose),pk_probe.fields,sizeof(struct packet_pose_t));
					break;
			}
		}
	}
}

// tracker display shipinfo
void func_tracker_shipinfo(struct ship_data_t *ship) {
	bool runflag=1;
	uint8_t page=0;
	char key;
	while (runflag) {
		disp.clearDisplay();
		disp.setTextSize(1);
		if (page==0) {
			disp.setCursor(6, 0);
			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(0, 8);
			sprintf(buffer,"%9.6f%c",abs(ship->ship_cord.latitude),ship->ship_cord.latitude<0?'S':'N');
			disp.write(buffer);
			disp.setCursor(6*11, 8);
			if (abs(GPS.location.lng())>=100) sprintf(buffer,"%9.5f%c",abs(ship->ship_cord.longitude),ship->ship_cord.longitude<0?'W':'E');
			else sprintf(buffer,"%9.6f%c",abs(ship->ship_cord.longitude),ship->ship_cord.longitude<0?'W':'E');
			disp.write(buffer);
			disp.setCursor(6*2, 16);
			sprintf(buffer,"%.1fm/s",ship->ship_motn.speed);
			disp.write(buffer);
			disp.setCursor(6*9, 16);
			sprintf(buffer,"(%.1fkm/h)",ship->ship_motn.speed*3.6);
			disp.write(buffer);
			disp.setCursor(0, 24);
			sprintf(buffer,"%.2f%c",ship->ship_motn.course/100.0,248);
			disp.write(buffer);
			disp.setCursor(6*9, 24);
			sprintf(buffer,"%s",GPS.cardinal(ship->ship_motn.course/100.0));
			disp.write(buffer);
			disp.setCursor(6*13, 24);
			sprintf(buffer,"%.2fm",ship->ship_cord.altitude);
			disp.write(buffer);
			disp.setCursor(0, 32);
			sprintf(buffer,"CORS: %.2f%c",GPS.courseTo(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude),248);
			disp.write(buffer);
			disp.setCursor(6*15, 32);
			sprintf(buffer,"%s",GPS.cardinal(GPS.courseTo(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude)));
			disp.write(buffer);
			disp.setCursor(0, 40);
			sprintf(buffer,"DIST: %.1f",GPS.distanceBetween(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude));
			disp.write(buffer);
			disp.setCursor(0, 48);
			sprintf(buffer,"SNR: %d",PacketSNR);
			disp.write(buffer);
			disp.setCursor(6*11, 48);
			sprintf(buffer,"RSSI: %d",PacketRSSI);
			disp.write(buffer);
		}
		else if (page==1) {
			disp.setCursor(6, 0);
			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(0, 8);
			sprintf(buffer,"Accel/Gyro:");
			disp.write(buffer);
			disp.setCursor(0,16);
			sprintf(buffer,"%7.2f",ship->ship_accr.acc_x);
			disp.write(buffer);
			disp.setCursor(6*7,16);
			sprintf(buffer,"%7.2f",ship->ship_accr.acc_y);
			disp.write(buffer);
			disp.setCursor(6*14,16);
			sprintf(buffer,"%7.2f",ship->ship_accr.acc_z);
			disp.write(buffer);
			disp.setCursor(0,24);
			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_x);
			disp.write(buffer);
			disp.setCursor(6*7,24);
			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_y);
			disp.write(buffer);
			disp.setCursor(6*14,24);
			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_z);
			disp.write(buffer);
			disp.setCursor(0,32);
			sprintf(buffer,"Orientation:");
			disp.write(buffer);
			disp.setCursor(0,40);
			sprintf(buffer,"%7.2f",ship->ship_pose.agl_x);
			disp.write(buffer);
			disp.setCursor(6*7,40);
			sprintf(buffer,"%7.2f",ship->ship_pose.agl_y);
			disp.write(buffer);
			disp.setCursor(6*14,40);
			sprintf(buffer,"%7.2f",ship->ship_pose.agl_z);
			disp.write(buffer);
			disp.setCursor(0,48);
			sprintf(buffer,"%7.2f",ship->ship_pose.pitch);
			disp.write(buffer);
			disp.setCursor(6*7,48);
			sprintf(buffer,"%7.2f",ship->ship_pose.roll);
			disp.write(buffer);
			disp.setCursor(6*14,48);
			sprintf(buffer,"%7.2f",ship->ship_pose.yaw);
			disp.write(buffer);
			disp.setCursor(0,56);
			sprintf(buffer,"Temperature: %.2f%cC",ship->ship_pose.temperature,248);
			disp.write(buffer);
		}
		disp.display();
		key=kbd_read(0);
		switch (key) {
			case 'A':
				if (page>0) page--;
				break;
			case 'B':
				if (page<1) page++;
				break;
			case 'C':
			case 'D':
				runflag=0;
				break;
		}
		delay(TICKINT);
	}
}

// tracker display shiplist
void func_tracker_shiplist_render(struct ship_data_t *ship, uint8_t pos, uint8_t scale) {
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	struct ship_data_t *shippt=ship;
	uint8_t count=0;
	while (shippt) {
		count++;
		shippt=shippt->next;
	}
	disp.setTextSize(1);
	disp.setCursor(128-8*6, 64-8);
	sprintf(buffer,"SHIPS:%2d",count);
	disp.write(buffer);
	if (count) {
		disp.setTextSize(scale);
		disp.setCursor(6*scale, 8*scale*pos);
		sprintf(buffer,"0X%04x",ship->ship_cid);
		disp.write(buffer);
		uint8_t pospt=pos;
		shippt=ship;
		while (pospt>0 && shippt->prev) {
			pospt--;
			shippt=shippt->prev;
			disp.setCursor(6*scale, 8*scale*pospt);
			sprintf(buffer,"0X%04x",shippt->ship_cid);
			disp.write(buffer);
		}
		pospt=pos;
		shippt=ship;
		while (pospt<(11-scale*4) && shippt->next) {
			pospt++;
			shippt=shippt->next;
			disp.setCursor(6*scale, 8*scale*pospt);
			sprintf(buffer,"0X%04x",shippt->ship_cid);
			disp.write(buffer);
		}
		disp.setCursor(0, 8*scale*pos);
		disp.write(">");
	}
	disp.display();
}

void func_tracker_selship(struct ship_list_t *shiplist) {
	uint8_t pos=0;
	struct ship_data_t *ship=shiplist->ship;
	char key;
	bool runflag=1;
	while (runflag) {
		if (!ship && shiplist->ship) {
			ship=shiplist->ship;
		}
		func_tracker_shiplist_render(ship, pos, 2);
		key=kbd_read(0);
		switch (key) {
			case 'A':
				if (ship && ship->prev) {
					ship=ship->prev;
					if (pos>1 || (pos==1 && !ship->prev)) pos--;
				}
				break;
			case 'B':
				if (ship && ship->next) {
					ship=ship->next;
					if (pos<2 || (pos==2 && !ship->next)) pos++;
				}
				break;
			case 'C':
				runflag=0;
				break;
			case 'D':
				if (ship) func_tracker_shipinfo(ship);
				break;
		}
		delay(TICKINT);
	}
}

void func_tracker_destroy_shiplist(struct ship_list_t *shiplist) {
	struct ship_data_t *ship=shiplist->ship;
	while (ship->next) {
		ship=ship->next;
		free(ship->prev);
	}
	free(ship);
}

// ----- applications -----

void *app_tracker(void *param) {
	TaskHandle_t Task_LoRa;
	struct ship_list_t *shiplist=(struct ship_list_t *)malloc(sizeof(struct ship_list_t));
	shiplist->ship=NULL;
	xTaskCreatePinnedToCore(func_tracker_shiplist_update,"Ship Scan",100000,shiplist,0,&Task_LoRa,0);
	func_tracker_selship(shiplist);
	vTaskDelete(Task_LoRa);
	func_tracker_destroy_shiplist(shiplist);
	return NULL;
}

void *app_gps(void *param) {
	char key;
	disp.setTextSize(1);//21 characters per line
	TICK=0;
	while (1) {
		if (!TICK%10) {
			disp.clearDisplay();
			disp.setCursor(6, 0+4);
			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);

			disp.setCursor(0, 8+8);
			sprintf(buffer,"%9.6f%c",abs(GPS.location.lat()),GPS.location.rawLat().negative?'S':'N');
			disp.write(buffer);
			disp.setCursor(6*11, 8+8);
			if (abs(GPS.location.lng())>=100) sprintf(buffer,"%9.5f%c",abs(GPS.location.lng()),GPS.location.rawLng().negative?'W':'E');
			else sprintf(buffer,"%9.6f%c",abs(GPS.location.lng()),GPS.location.rawLng().negative?'W':'E');
			disp.write(buffer);
			disp.setCursor(6*2, 16+12);
			sprintf(buffer,"%.1fm/s",GPS.speed.mps());
			disp.write(buffer);
			disp.setCursor(6*9, 16+12);
			sprintf(buffer,"(%.1fkm/h)",GPS.speed.kmph());
			disp.write(buffer);
			disp.setCursor(0, 24+16);
			sprintf(buffer,"%.2f%c",GPS.course.deg(),248);
			disp.write(buffer);
			disp.setCursor(6*9, 24+16);
			sprintf(buffer,"%s",GPS.cardinal(GPS.course.deg()));
			disp.write(buffer);
			disp.setCursor(6*13, 24+16);
			sprintf(buffer,"%.2fm",GPS.altitude.meters());
			disp.write(buffer);
			disp.setCursor(0, 32+20);
			sprintf(buffer,"HDOP: %6.3f",GPS.hdop.hdop());
			disp.write(buffer);
			disp.setCursor(128-6*8, 32+20);
			sprintf(buffer,"SAT: %02d",GPS.satellites.value());
			disp.write(buffer);
			disp.display();
		}
		key=kbd_read(0);
		switch (key) {
			case 'C':
				return NULL;
			case 'D':
				return NULL;
		}
		TICK++;
		delay(TICKINT);
	}
	return NULL;
}

void *app_bme680(void *param) {
	char key;
	disp.setTextSize(1);//21 characters per line
	TICK=0;
	while (1) {
		if (!TICK%90) {
			BME680.performReading();
			disp.clearDisplay();
			disp.setCursor(6, 0+4);
			sprintf(buffer,"Temp  = %.3f%cC",BME680.temperature,248);
			disp.write(buffer);
			disp.setCursor(6, 16+4);
			sprintf(buffer,"Pres  = %.2fhPa",BME680.pressure/100.0);
			disp.write(buffer);
			disp.setCursor(6, 32+4);
			sprintf(buffer,"Humid = %.3f%%",BME680.humidity);
			disp.write(buffer);
			disp.display();
		}
		key=kbd_read(0);
		switch (key) {
			case 'C':
				return NULL;
			case 'D':
				return NULL;
		}
		TICK++;
		delay(TICKINT);
	}
	return NULL;
}

// ----- main -----

void setup() {
	TICK=0;
	Serial.begin(115200);
	// OLED -- initialization
	I2C_SSD1306.begin(SCREEN_I2C_SDA,SCREEN_I2C_SCL,100000);
	while (!disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) delay(2000);
	disp.clearDisplay();
	disp_setbrightness(disp_brightness=20);
	disp_flag_on=1;
	// text output initialization routine
	disp.setTextSize(1);
	disp.setTextColor(SSD1306_WHITE);
	disp.setCursor(0, 0);
	disp.cp437(true);
	disp.write(">>> Display inited\n");
	disp.display();
	// LED -- turn off on startup
	pinMode(LED,OUTPUT);
	disp.write(">>> LED test\n");
	disp.display();
	digitalWrite(LED,HIGH);
	delay(500);
	digitalWrite(LED,LOW);
	// PRGSW -- detect default voltage and reset
	rtc_gpio_deinit(GPIO_NUM_0);
	pinMode(PRGSW_PIN,INPUT);
	PRGSW_def=digitalRead(PRGSW_PIN);
	PRGSW_act=0;
	if (PRGSW_def) disp.write(">>> PRGSW HIGH\n");
	else disp.write(">>> PRGSW LOW\n");
	disp.display();
	// BME680
	I2C_BME680.begin(BME680_I2C_SDA,BME680_I2C_SCL,100000);
	if (BME680.begin()) {
		BME680.setTemperatureOversampling(BME680_OS_8X);
		BME680.setHumidityOversampling(BME680_OS_4X);
		BME680.setPressureOversampling(BME680_OS_4X);
		BME680.setIIRFilterSize(BME680_FILTER_SIZE_3);
		disp.write(">>> BME680 online\n");
		disp.display();
	}
	else {
		disp.write(">>> BME680 failure\n");
		disp.display();
	}
	// Battery Voltage
	xTaskCreate(func_batvolt_update,"Battery Voltage",10000,NULL,0,&Task_batvolt);
	batvolt_flag_enabled=true;
	// Serial/GPS -- Todo: GPS module communication
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW);
	Serial1.begin(9600,SERIAL_8N1,46,45);
	xTaskCreatePinnedToCore(func_GPS_update,"GPS Parse",100000,NULL,0,&Task_GPS,0);
	GPS_bg_runflag=true;
	disp.write(">>> GPS online\n");
	disp.display();
	// Keyboard
	for (uint8_t i=0;i<4;i++) {
		pinMode(KBD_ROW[i],OUTPUT);
		digitalWrite(KBD_ROW[i],HIGH);
		pinMode(KBD_COL[i],INPUT_PULLUP);
	}
	disp.write(">>> Keyboard inited\n");
	disp.display();
	// LoRa -- initialization
	disp.display();
	SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);
	while (!LoRa.begin(SS_LoRa,RST_LoRa,BUSY_LoRa,DIO1_LoRa,SW_LoRa,DEVICE_SX1262)) delay(2000);
	LoRa.setupLoRa(LoRa_Freq, LoRa_Offset, LoRa_SpreadingFactor, LoRa_Bandwidth, LoRa_CodeRate, LoRa_Optimisation);
	disp.write(">>> LoRa online\n");
	disp.display();
	delay(500);
	disp.clearDisplay();
	disp.display();
}

void loop() {
	c=kbd_read(0);
	if (c=='P') disp_switch();
	if (c=='D'&&disp_flag_on) {
		func_menu();
		TICK=0;
	}
	if (!TICK%10) {
		disp.clearDisplay();
		disp.setTextSize(1);
		if (batvolt_flag_enabled) {
			disp.setCursor(2, 2);
			sprintf(buffer,"%.3fV",batvolt_value);
			disp.write(buffer);
		}
		sprintf(buffer,"%02d:%02d:%02d\n",GPS.time.hour(),GPS.time.minute(),GPS.time.second());
		disp.setTextSize(2);
		disp.setCursor(12, 24);
		disp.write(buffer);
		disp.display();
	}
	TICK++;
	delay(TICKINT);
}
