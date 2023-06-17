#ifndef common_misc
#define common_misc 1.0

#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

#include "conf_global.h"
#include "resc_display.h"
#include "resc_keyboard.h"
#include "common_menu.h"
#include "app_shiptracker.h"
#include "app_gps.h"
#include "app_bme680.h"
#include "app_mainmenu.h"

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