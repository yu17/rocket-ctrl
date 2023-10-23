#ifndef common_misc
#define common_misc 1.0

#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

#include "resc_display.hpp"
#include "resc_joystick.hpp"
#include "common_menu.hpp"
#include "app_shiptracker.hpp"
#include "app_gps.hpp"
#include "app_sensors.hpp"
#include "app_mainmenu.hpp"

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

// ----- Animation Command -----
#define ANIME_START 0xB0
#define ANIME_PROGRESS 0xB1
#define ANIME_INTERRUPT 0xB2

// ----- Secondary I2C -----
#define I2C2_SDA 33
#define I2C2_SCL 34

// ----- MISC -----
#define TICKINT 10
#define BUFFER_SIZE 128

extern TwoWire I2C2;

extern uint16_t TICK;
extern char buffer[BUFFER_SIZE];
extern uint8_t buffer_pt;

// ----- Battery Voltage -----
extern bool batvolt_flag_enabled;
extern float batvolt_value;

// ----- Animation Handle -----
extern uint8_t animation_running;
extern TaskHandle_t Task_animation;

void func_batvolt_update();

void *func_setbrightness(const void *param);

void *func_quick_settings(const void *param);

void *func_deepsleep(const void *param);

void func_animation_hline_worker(void *param);
void func_animation_hline(uint16_t y, uint16_t h, uint16_t duration, uint16_t inverted, uint8_t command);

#endif