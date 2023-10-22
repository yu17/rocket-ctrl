#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "conf_lora.hpp"
#include "resc_display.hpp"
//#include "resc_keyboard.h"
#include "resc_joystick.hpp"
#include "resc_gps.hpp"
#include "common_misc.hpp"
#include "common_menu.hpp"
#include "app_shiptracker.hpp"
#include "app_gps.hpp"
#include "app_sensors.hpp"
#include "app_mainmenu.hpp"

#define LORA_DEVICE DEVICE_SX1262               //we need to define the device we are using

extern "C" void app_main() {
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
	// BME280
	bmp280_init();
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
	lora_init();
	delay(500);
	disp.clearDisplay();
	disp.display();

	while (1) {
		c=joy_read(0);
		if (c==P) disp_switch();
		if (c==R && disp_flag_on) {
			menu_exec(&mainmenu_p);
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
}
