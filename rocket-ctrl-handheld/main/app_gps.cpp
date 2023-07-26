#include "app_gps.h"

void *app_gps(void *param) {
	enum JOY_DISCRETE joy;
	disp.setTextSize(1);//21 characters per line
	TICK=0;
	while (1) {
		if (!TICK%10) {
			disp.clearDisplay();
			disp.setCursor(6, 0+4);
			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);

			disp.setCursor(0, 8+8);
			sprintf(buffer,"%9.6f%c",abs(GPS.location.lat()),GPS.location.rawLat().negative?'S':'N');
			disp.write(buffer);
			disp.setCursor(6*11, 8+8);
			if (abs(GPS.location.lng())>=100) sprintf(buffer,"%9.5f%c",abs(GPS.location.lng()),GPS.location.rawLng().negative?'W':'E');
			else sprintf(buffer,"%9.6f%c",abs(GPS.location.lng()),GPS.location.rawLng().negative?'W':'E');
			disp.write(buffer);
			disp.setCursor(6*2, 16+12);
			sprintf(buffer,"%.1fm/s",GPS.speed.mps());
			disp.write(buffer);
			disp.setCursor(6*9, 16+12);
			sprintf(buffer,"(%.1fkm/h)",GPS.speed.kmph());
			disp.write(buffer);
			disp.setCursor(0, 24+16);
			sprintf(buffer,"%.2f%c",GPS.course.deg(),248);
			disp.write(buffer);
			disp.setCursor(6*9, 24+16);
			sprintf(buffer,"%s",GPS.cardinal(GPS.course.deg()));
			disp.write(buffer);
			disp.setCursor(6*13, 24+16);
			sprintf(buffer,"%.2fm",GPS.altitude.meters());
			disp.write(buffer);
			disp.setCursor(0, 32+20);
			sprintf(buffer,"HDOP: %6.3f",GPS.hdop.hdop());
			disp.write(buffer);
			disp.setCursor(128-6*8, 32+20);
			sprintf(buffer,"SAT: %02d",GPS.satellites.value());
			disp.write(buffer);
			disp.display();
		}
		joy=joy_read(0);
		switch (joy) {
			case L:
				return NULL;
			case R:
				return NULL;
			default:
				break;
		}
		TICK++;
		delay(TICKINT);
	}
	return NULL;
}