#ifndef resc_gps
#define resc_gps 1.0

#include <TinyGPS++.h>

// ----- GPS -----
extern TinyGPSPlus GPS;
extern TaskHandle_t Task_GPS;
extern bool GPS_bg_runflag;

void func_GPS_update(void *param);

#endif