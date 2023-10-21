#include "app_sensors.hpp"

void *app_sensors(const void *param) {
	enum JOY_DISCRETE joy;
	disp.setTextSize(1);//21 characters per line
	TICK=0;
	int heading_ccw;
	int16_t rawx,rawy,rawz,rawt;
	uint8_t page=0;

	// Config sensor
	BME280.setSampling(Adafruit_BME280::BME280_MODE_NORMAL,
                    Adafruit_BME280::BME280_SAMPLING_X2,  // temperature
                    Adafruit_BME280::BME280_SAMPLING_X16, // pressure
                    Adafruit_BME280::BME280_SAMPLING_X1,  // humidity
                    Adafruit_BME280::BME280_FILTER_X16,
                    Adafruit_BME280::BME280_STANDBY_MS_0_5);
    // suggested rate is 25Hz
    // 1 + (2 * T_ovs) + (2 * P_ovs + 0.5) + (2 * H_ovs + 0.5)
    // T_ovs = 2
    // P_ovs = 16
    // H_ovs = 1
    // = 40ms (25Hz)
    // with standby time that should really be 24.16913... Hz
    int TICKSAMPLE = 41;

	while (1) {
		if (!TICK%90) {
			if (page==0) {
				heading_ccw=(compass.readHeading()+compass_offset)%360;
				disp.clearDisplay();
				disp.setCursor(6, 0+4);
				sprintf(buffer,"Temp  = %.3f%cC",BME280.readTemperature(),248);
				disp.write(buffer);
				disp.setCursor(6, 16+4);
				sprintf(buffer,"Pres  = %.2fhPa",BME280.readPressure()/100.0);
				disp.write(buffer);
				disp.setCursor(6, 32+4);
				sprintf(buffer,"Humid = %.3f%%",BME280.readHumidity());
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
		delay(TICKSAMPLE);
	}
	BME280.setSampling(Adafruit_BME280::BME280_MODE_SLEEP,
                    Adafruit_BME280::BME280_SAMPLING_NONE,
                    Adafruit_BME280::BME280_SAMPLING_NONE,
                    Adafruit_BME280::BME280_SAMPLING_NONE,
                    Adafruit_BME280::BME280_FILTER_OFF,
                    Adafruit_BME280::BME280_STANDBY_MS_1000);
	return NULL;
}

