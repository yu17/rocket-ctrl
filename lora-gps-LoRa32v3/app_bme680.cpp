#include "app_bme680.h"

void *app_bme680(void *param) {
	char key;
	disp.setTextSize(1);//21 characters per line
	TICK=0;
	while (1) {
		if (!TICK%90) {
			BME680.performReading();
			disp.clearDisplay();
			disp.setCursor(6, 0+4);
			sprintf(buffer,"Temp  = %.3f%cC",BME680.temperature,248);
			disp.write(buffer);
			disp.setCursor(6, 16+4);
			sprintf(buffer,"Pres  = %.2fhPa",BME680.pressure/100.0);
			disp.write(buffer);
			disp.setCursor(6, 32+4);
			sprintf(buffer,"Humid = %.3f%%",BME680.humidity);
			disp.write(buffer);
			disp.display();
		}
		key=kbd_read(0);
		switch (key) {
			case 'C':
				return NULL;
			case 'D':
				return NULL;
		}
		TICK++;
		delay(TICKINT);
	}
	return NULL;
}