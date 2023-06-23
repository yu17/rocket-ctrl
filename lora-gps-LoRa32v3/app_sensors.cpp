#include "app_sensors.h"

void *app_sensors(void *param) {
	enum JOY_DISCRETE joy;
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
			disp.setCursor(6, 48+4);
			sprintf(buffer,"Heading = %d",compass.readHeading());
			disp.write(buffer);
			disp.display();
		}
		joy=joy_read(0);
		switch (joy) {
			case L:
				return NULL;
			case R:
				return NULL;
		}
		TICK++;
		delay(TICKINT);
	}
	return NULL;
}

