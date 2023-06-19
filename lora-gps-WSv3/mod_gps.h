#ifndef mod_gps
#define mod_gps 1.0

#include <Arduino.h>
#include <driver/rtc_io.h>
#include <TinyGPS++.h>

extern TinyGPSPlus GPS;
extern TaskHandle_t Task_GPS;
extern bool GPS_bg_runflag;

void func_GPS_update(void *param);

void func_GPS_init();

void func_GPS_enable();

void func_GPS_disable();

#endif