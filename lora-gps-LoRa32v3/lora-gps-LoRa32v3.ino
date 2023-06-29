#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#include "conf_lora.h"
#include "resc_display.h"
//#include "resc_keyboard.h"
#include "resc_joystick.h"
#include "resc_gps.h"
#include "common_misc.h"
#include "common_menu.h"
#include "app_shiptracker.h"
#include "app_gps.h"
#include "app_sensors.h"
#include "app_mainmenu.h"

#define LORA_DEVICE DEVICE_SX1262               //we need to define the device we are using

void setup() {
	// Power on Vext
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW);
	TICK=0;
	Serial.begin(115200);
	// OLED -- initialization
	disp_init();
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
	bme680_init();
	// Compass
	compass_init();
	// Battery Voltage
	xTaskCreate(func_batvolt_update,"Battery Voltage",10000,NULL,0,&Task_batvolt);
	batvolt_flag_enabled=true;
	// Serial/GPS -- Todo: GPS module communication
	GPS_init();
	// Keyboard
	//kbd_init();
	// Joystick
	pinMode(JOY_BTN,INPUT_PULLUP);
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
	Serial.println(menu_numinput(3,1,1234,NULL));
	Serial.println(menu_numinput(4,3,1234,NULL));
	Serial.println(menu_numinput(5,4,1234,NULL));
	sleep(1000);
}

void loop() {
	c=joy_read(0);
	if (c==P) disp_switch();
	if (c==R && disp_flag_on) {
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
