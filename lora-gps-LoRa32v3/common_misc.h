#ifndef common_misc
#define common_misc 1.0

#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#include "resc_display.h"
#include "resc_keyboard.h"
#include "common_menu.h"
#include "app_shiptracker.h"
#include "app_gps.h"
#include "app_bme680.h"
#include "app_mainmenu.h"

// ----- Settings -----
#define SYS_GPS_1_1 0x22
#define SYS_GPS_1_2 0x23
#define SYS_GPS_2_1 0x24
#define SYS_GPS_2_2 0x25
#define SYS_GPS_3_1 0x26
#define SYS_GPS_3_2 0x27
#define SYS_GPS_3_3 0x28
#define SYS_GPS_3_4 0x29
#define SYS_VOLT_1 0x00
#define SYS_VOLT_2 0x01
#define SYS_TEMP_1 0x02
#define SYS_TEMP_2 0x03

// ----- MISC -----
#define TICKINT 10
#define BUFFER_SIZE 128

// ----- BME680 -----
#define BME680_I2C_SDA 3
#define BME680_I2C_SCL 2

extern uint8_t TICK;
extern char buffer[BUFFER_SIZE];
extern uint8_t buffer_pt;

// ----- BME680 -----
extern TwoWire I2C_BME680;
extern Adafruit_BME680 BME680;

// ----- Battery Voltage -----
extern TaskHandle_t Task_batvolt;
extern bool batvolt_flag_enabled;
extern float batvolt_value;

void func_batvolt_update(void *param);

void *func_setbrightness(void *param);

void *func_quick_settings(void *param);

void *func_deepsleep(void *param);

#endif