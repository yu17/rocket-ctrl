#include "mod_disp.hpp"

TwoWire I2C_SSD1306=TwoWire(1);
Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SSD1306, RST_OLED);
bool disp_flag_on;

void disp_init() {
	// OLED -- initialization
	I2C_SSD1306.begin(SCREEN_I2C_SDA,SCREEN_I2C_SCL,100000);
	while (!disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) delay(2000);
	disp.clearDisplay();
	disp_flag_on=1;
	// text output initialization routine
	disp.setTextSize(1);
	disp.setTextColor(SSD1306_WHITE);
	disp.setCursor(0, 0);
	disp.cp437(true);
	disp.write(">>> Display inited\n");
	disp.display();
}

void disp_swdisplay() {
	if (disp_flag_on) disp.display();
}

