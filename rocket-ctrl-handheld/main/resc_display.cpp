#include "resc_display.hpp"

TwoWire I2C_SSD1306=TwoWire(0);
Adafruit_SSD1306 disp(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_SSD1306, RST_OLED);
bool disp_flag_on;
uint8_t disp_brightness;

void disp_init() {
	I2C_SSD1306.begin(SCREEN_I2C_SDA,SCREEN_I2C_SCL,100000);
	while (!disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) delay(2000);
	disp.clearDisplay();
	disp_setbrightness(disp_brightness=20);
	disp_flag_on=1;
	// text output initialization routine
	disp.setTextSize(1);
	disp.setTextColor(SSD1306_INVERSE);
	disp.setCursor(0, 0);
	disp.cp437(true);
	disp.write(">>> Display inited\n");
	disp.display();
}

void disp_setup() {
	I2C_SSD1306.begin(SCREEN_I2C_SDA,SCREEN_I2C_SCL,100000);
	while (!disp.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) delay(2000);
	disp.clearDisplay();
	disp_setbrightness(disp_brightness=20);
	disp_flag_on=1;
}

void disp_switch() {
	if (disp_flag_on) {
		disp.ssd1306_command(SSD1306_DISPLAYOFF);
		disp.ssd1306_command(SSD1306_CHARGEPUMP);
		disp.ssd1306_command(0x10);//Turn off charge pump
	}
	else {
		disp.ssd1306_command(SSD1306_CHARGEPUMP);
		disp.ssd1306_command(0x14);//Turn off charge pump
		disp.ssd1306_command(SSD1306_DISPLAYON);
	}
	disp_flag_on=!disp_flag_on;
}

void disp_swdisplay(Adafruit_SSD1306 *disp) {
	if (disp_flag_on) disp->display();
}

void disp_setbrightness(uint8_t value) {
	disp.ssd1306_command(SSD1306_SETCONTRAST);
	disp.ssd1306_command(value);
}