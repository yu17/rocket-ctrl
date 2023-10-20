#include "mod_gps.hpp"

TinyGPSPlus GPS;
TaskHandle_t Task_GPS;
bool GPS_bg_runflag=0;

void func_GPS_update(void *param) {
	while (1) {
		while (Serial1.available()>0) GPS.encode(Serial1.read());
		delay(100);
	}
}

void func_GPS_enable() {
	if (!GPS_bg_runflag) {
		digitalWrite(Vext,LOW);
		delay(100);
		Serial1.begin(9600,SERIAL_8N1,34,33);
		xTaskCreate(func_GPS_update,"GPS update",100000,NULL,0,&Task_GPS);
		GPS_bg_runflag=1;
	}
}

void func_GPS_disable() {
	if (GPS_bg_runflag) {
		vTaskDelete(Task_GPS);
		GPS_bg_runflag=0;
		delay(500);
		Serial1.end();
		digitalWrite(Vext,HIGH);
	}
}