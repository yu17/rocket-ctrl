#include "resc_gps.hpp"

#include <Arduino.h>

// ----- GPS -----
TinyGPSPlus GPS;
TaskHandle_t Task_GPS;
bool GPS_bg_runflag;

void GPS_init() {
	digitalWrite(Vext,LOW);
	Serial1.begin(9600,SERIAL_8N1,48,47);
	xTaskCreatePinnedToCore(func_GPS_update,"GPS Parse",100000,NULL,0,&Task_GPS,0);
	GPS_bg_runflag=true;
	disp.write(">>> GPS online\n");
	disp.display();
}

// ----- GPS background functions -----
void func_GPS_update(void *param) {
	while (1) {
		while (Serial1.available()>0) GPS.encode(Serial1.read());
		delay(100);
	}
}
