#include "resc_gps.hpp"

#include <Arduino.h>

// ----- GPS -----
TinyGPSPlus GPS;
TaskHandle_t Task_GPS;
bool GPS_bg_runflag;

void GPS_init() {
	digitalWrite(Vext,LOW);
	Serial1.begin(9600,SERIAL_8N1,48,47);
	//BaseType_t err=xTaskCreatePinnedToCore(func_GPS_update,"GPS Parse",8000,NULL,0,&Task_GPS,0);
	BaseType_t err=xTaskCreate(func_GPS_update,"GPS Parse",8000,NULL,0,&Task_GPS);
	if (err!=pdPASS) {
		disp.write(">>> GPS failure\n");
		disp.display();
	}
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
