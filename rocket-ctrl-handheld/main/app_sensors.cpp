#include "app_sensors.hpp"

void *app_sensors(const void *param) {
	enum JOY_DISCRETE joy;
	disp.setTextSize(1);//21 characters per line
	TICK=0;
	int heading_ccw;
	int16_t rawx,rawy,rawz,rawt;
	uint8_t page=0;
	while (1) {
		if (!TICK%90) {
			if (page==0) {
				heading_ccw=(compass.readHeading()+compass_offset)%360;
				BME680.beginReading();
				BME680.endReading();
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
				sprintf(buffer,"Heading = %d",compass_course_clkflip(heading_ccw));
				disp.write(buffer);
				disp.display();
			}
			else if (page==1) {
				compass.readRaw(&rawx,&rawy,&rawz,&rawt);
				heading_ccw=(compass.readHeading()+compass_offset)%360;
				disp.clearDisplay();
				disp.setCursor(2, 0);
				sprintf(buffer,"X = %d",rawx);
				disp.write(buffer);
				disp.setCursor(2, 12+1);
				sprintf(buffer,"Y = %d",rawy);
				disp.write(buffer);
				disp.setCursor(2, 24+2);
				sprintf(buffer,"Z = %d",rawz);
				disp.write(buffer);
				disp.setCursor(2, 36+3);
				sprintf(buffer,"T = %d",rawt);
				disp.write(buffer);
				disp.setCursor(2,48+4);
				sprintf(buffer,"> %d",compass_course_clkflip(heading_ccw));
				disp.write(buffer);
				compass_draw_graph(96,32,30,heading_ccw);
				compass_draw_arrow(96,32,18,heading_ccw);
				disp.display();
			}
		}
		joy=joy_read(0);
		switch (joy) {
			case U:
				if (page>0) page--;
				break;
			case D:
				if (page<1) page++;
				break;
			case L:
				return NULL;
			default:
				break;
		}
		TICK++;
		delay(TICKINT);
	}
	return NULL;
}

