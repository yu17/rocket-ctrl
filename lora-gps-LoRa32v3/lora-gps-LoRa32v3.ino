#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#include "conf_lora.h"
#include "resc_display.h"
#include "resc_keyboard.h"
#include "resc_gps.h"
#include "common_misc.h"
#include "common_menu.h"
#include "app_shiptracker.h"
#include "app_gps.h"
#include "app_bme680.h"
#include "app_mainmenu.h"

#define LORA_DEVICE DEVICE_SX1262               //we need to define the device we are using

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
		menu_exec(mainmenu_load);
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
