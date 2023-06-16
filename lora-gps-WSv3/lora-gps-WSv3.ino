#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <GY521.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <TinyGPS++.h>

#include <SPI.h>
#include <SX126XLT.h>

// ----- OLED -----
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
#define SCREEN_I2C_SDA 17
#define SCREEN_I2C_SCL 18

//TwoWire I2C_SSD1306=TwoWire(0);
//Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SSD1306, RST_OLED);
bool disp_flag_on;

// ----- PRGSW -----
#define PRGSW_PIN 0
bool PRGSW_def,PRGSW_act;

// ----- Serial/GPS -----
TinyGPSPlus GPS;

// ----- LoRa -----
#define LORA_DEVICE DEVICE_SX1262               //we need to define the device we are using
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];
//LoRa Modem Parameters
const uint32_t LoRa_Freq = 915000000;           //frequency of transmissions in hertz
const uint32_t LoRa_Offset = 0;                      //offset frequency for calibration purposes
const uint8_t LoRa_Bandwidth = LORA_BW_125;          //LoRa bandwidth
const uint8_t LoRa_SpreadingFactor = LORA_SF7;       //LoRa spreading factor
const uint8_t LoRa_CodeRate = LORA_CR_4_5;           //LoRa coding rate
const uint8_t LoRa_Optimisation = LDRO_AUTO;         //low data rate optimisation setting, normally set to auto
const int8_t LoRa_TXpower = 10;                      //LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

const uint16_t packet_delay = 1000;             //mS delay between packets

SX126XLT LoRa;

// ----- GY521 -----
#define GY521_I2C_SDA 46
#define GY521_I2C_SCL 45
TwoWire I2C_GY521=TwoWire(0);
GY521 gyro(0x68, &I2C_GY521);

// ----- MISC -----
uint8_t TICK;
char c;

// ----- Comm Protocol -----
#define LORAGPS_CLIENTID 0x1001

#define LORAGPS_MAG_HEAD 0x89
#define LORAGPS_TYP_TIME 0x30
#define LORAGPS_TYP_CORD 0x32
#define LORAGPS_TYP_MOTN 0x34
#define LORAGPS_TYP_ACCR 0x36
#define LORAGPS_TYP_POSE 0x38

struct packet_frame_t{
	uint8_t magicnum;
	uint8_t sig;
	uint16_t cid;
	uint8_t fields[250];
} pk_frame;

struct packet_time_t{
	uint8_t hour,minute,second;
} pk_time;

struct packet_cord_t{
	double latitude,longitude,altitude;
} pk_cord;

struct packet_motn_t{
	double speed;
	int32_t course;
} pk_motn;

struct packet_accr_t{
	float acc_x,acc_y,acc_z,gyro_x,gyro_y,gyro_z;
} pk_accr;

struct packet_pose_t{
	float temperature,agl_x,agl_y,agl_z,pitch,roll,yaw;
} pk_pose;

//void GY521_update(){


//void disp_switch(){
//	if (disp_flag_on) disp.ssd1306_command(SSD1306_DISPLAYOFF);
//	else disp.ssd1306_command(SSD1306_DISPLAYON);
//	disp_flag_on=!disp_flag_on;
//}
//
//void disp_swdisplay(Adafruit_SSD1306 *disp){
//	if (disp_flag_on) disp->display();
//}

void setup() {
	Serial.begin(115200);
	TICK=0;
	// OLED -- initialization
//	I2C_SSD1306.begin(SCREEN_I2C_SDA,SCREEN_I2C_SCL,100000);
//	Serial.println(">>> Display trial");
//	while (!disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) delay(2000);
//	Serial.println(">>> Display inited");
//	disp.clearDisplay();
//	disp_flag_on=1;
//	// text output initialization routine
//	disp.setTextSize(1);
//	disp.setTextColor(SSD1306_WHITE);
//	disp.setCursor(0, 0);
//	disp.cp437(true);
//	disp.write(">>> Display inited\n");
//	disp.display();
	// LED -- turn off on startup
	pinMode(LED,OUTPUT);
//	disp.write(">>> LED test\n");
//	disp.display();
	digitalWrite(LED,HIGH);
	delay(500);
	digitalWrite(LED,LOW);
	Serial.println(">>> LED test");
	// PRGSW -- detect default voltage and reset
	pinMode(PRGSW_PIN,INPUT);
	PRGSW_def=digitalRead(PRGSW_PIN);
	PRGSW_act=0;
//	if (PRGSW_def) disp.write(">>> PRGSW HIGH\n");
//	else disp.write(">>> PRGSW LOW\n");
//	disp.display();
	// Serial/GPS -- Todo: GPS module communication
	pinMode(36,OUTPUT);
	digitalWrite(36,LOW);
	Serial1.begin(9600,SERIAL_8N1,34,33);
//	disp.write(">>> GPS online\n");
//	disp.display();
	Serial.println(">>> GPS online");
	// LoRa -- initialization
	SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);
	while (!LoRa.begin(SS_LoRa,RST_LoRa,BUSY_LoRa,DIO1_LoRa,SW_LoRa,DEVICE_SX1262)) delay(2000);
	LoRa.setupLoRa(LoRa_Freq, LoRa_Offset, LoRa_SpreadingFactor, LoRa_Bandwidth, LoRa_CodeRate, LoRa_Optimisation);
//	disp.write(">>> LoRa online\n");
//	disp.display();
	Serial.println(">>> LoRa online");
//	I2C_GY521.begin(GY521_I2C_SDA,GY521_I2C_SCL,100000);
//	if (gyro.begin()) Serial.println(">>> GY521 online");
//	else Serial.println(">>> GY521 failure");
//	gyro.setAccelSensitivity(3);
//	gyro.setGyroSensitivity(1);
	delay(500);
	pk_frame.magicnum=LORAGPS_MAG_HEAD;
	pk_frame.cid=LORAGPS_CLIENTID;
}

void loop() {
	TICK++;
	if (digitalRead(PRGSW_PIN)!=PRGSW_def) PRGSW_act=1;
	else if (PRGSW_act) {
		digitalWrite(36,HIGH);
		rtc_gpio_init(GPIO_NUM_0);
		rtc_gpio_pullup_en(GPIO_NUM_0);
		esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,LOW);
		esp_deep_sleep_start();
	}
	if (!TICK%128) {
		while (Serial1.available()>0) GPS.encode(Serial1.read());
		pk_time.hour=GPS.time.hour();
		pk_time.minute=GPS.time.minute();
		pk_time.second=GPS.time.second();
		pk_cord.latitude=GPS.location.lat();
		pk_cord.longitude=GPS.location.lng();
		pk_cord.altitude=GPS.altitude.meters();
		pk_motn.speed=GPS.speed.mps();
		pk_motn.course=GPS.course.value();

//		gyro.read();
//		pk_accr.acc_x=gyro.getAccelX();
//		pk_accr.acc_y=gyro.getAccelY();
//		pk_accr.acc_z=gyro.getAccelZ();
//		pk_accr.gyro_x=gyro.getGyroX();
//		pk_accr.gyro_y=gyro.getGyroY();
//		pk_accr.gyro_z=gyro.getGyroZ();
//
//		pk_pose.temperature=gyro.getTemperature();
//		pk_pose.agl_x=gyro.getAngleX();
//		pk_pose.agl_y=gyro.getAngleY();
//		pk_pose.agl_z=gyro.getAngleZ();
//		pk_pose.pitch=gyro.getPitch();
//		pk_pose.roll=gyro.getRoll();
//		pk_pose.yaw=gyro.getYaw();
	}
	else if (!(TICK%128-20)) {
		pk_frame.sig=LORAGPS_TYP_TIME;
		memcpy(pk_frame.fields,&pk_time,sizeof(struct packet_time_t));
		LoRa.transmit((uint8_t *)&pk_frame,sizeof(struct packet_frame_t),0,LoRa_TXpower,WAIT_TX);
	}
	else if (!(TICK%128-40)) {
		pk_frame.sig=LORAGPS_TYP_CORD;
		memcpy(pk_frame.fields,&pk_cord,sizeof(struct packet_cord_t));
		LoRa.transmit((uint8_t *)&pk_frame,sizeof(struct packet_frame_t),0,LoRa_TXpower,WAIT_TX);
	}
	else if (!(TICK%128-60)) {
		pk_frame.sig=LORAGPS_TYP_MOTN;
		memcpy(pk_frame.fields,&pk_motn,sizeof(struct packet_motn_t));
		LoRa.transmit((uint8_t *)&pk_frame,sizeof(struct packet_frame_t),0,LoRa_TXpower,WAIT_TX);
	}
	else if (!(TICK%128-80)) {
		pk_frame.sig=LORAGPS_TYP_ACCR;
		memcpy(pk_frame.fields,&pk_accr,sizeof(struct packet_accr_t));
		LoRa.transmit((uint8_t *)&pk_frame,sizeof(struct packet_frame_t),0,LoRa_TXpower,WAIT_TX);
	}
	else if (!(TICK%128-100)) {
		pk_frame.sig=LORAGPS_TYP_POSE;
		memcpy(pk_frame.fields,&pk_pose,sizeof(struct packet_pose_t));
		LoRa.transmit((uint8_t *)&pk_frame,sizeof(struct packet_frame_t),0,LoRa_TXpower,WAIT_TX);
	}
	delay(10);
}
