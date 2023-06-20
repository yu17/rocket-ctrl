#ifndef mod_disp
#define mod_disp 1.0

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

void disp_init();

void disp_swdisplay();

inline static void disp_pwron() {
	disp_flag_on=1;
	disp.ssd1306_command(SSD1306_DISPLAYON);
}

inline static void disp_pwroff() {
	disp_flag_on=0;
	disp.ssd1306_command(SSD1306_DISPLAYOFF);
}

#endif