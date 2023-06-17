#include "resc_gps.h"

#include <Arduino.h>

// ----- GPS -----
TinyGPSPlus GPS;
TaskHandle_t Task_GPS;
bool GPS_bg_runflag;

// ----- GPS background functions -----
void func_GPS_update(void *param) {
	while (1) {
		while (Serial1.available()>0) GPS.encode(Serial1.read());
		delay(100);
	}
}
