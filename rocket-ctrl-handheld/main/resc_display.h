#ifndef resc_display
#define resc_display 1.0

#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----- OLED -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define SCREEN_I2C_SDA 17
#define SCREEN_I2C_SCL 18

extern TwoWire I2C_SSD1306;
extern Adafruit_SSD1306 disp;
extern bool disp_flag_on;
extern uint8_t disp_brightness;

void disp_init();

void disp_setup();

void disp_switch();

void disp_swdisplay(Adafruit_SSD1306 *disp);

void disp_setbrightness(uint8_t value);

#endif