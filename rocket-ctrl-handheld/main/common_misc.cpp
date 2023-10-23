#include "common_misc.hpp"

// ----- Secondary I2C for temp/pressure sensor -----
TwoWire I2C2=TwoWire(1);

// ----- Common buffer for text printing -----
uint16_t TICK;
char buffer[BUFFER_SIZE];
uint8_t buffer_pt;

// ----- Vext power flag -----
uint8_t Vext_enabled;

// ----- Battery Voltage -----
bool batvolt_flag_enabled;
float batvolt_value;

// ----- Animation Handle -----
uint8_t animation_running=0;
TaskHandle_t Task_animation;

// ----- battery voltage update functions -----
void func_batvolt_update() {
	batvolt_value=((float)analogRead(A0))/(float)4095*3.6*(float)490/(float)100;
}

void *func_setbrightness(const void *param) {
	disp.setTextSize(1);
	enum JOY_DISCRETE joy;
	uint8_t brightness_cache=disp_brightness;
	while (1) {
		disp.clearDisplay();
		disp.fillRect(12,18,104,24,SSD1306_INVERSE);
		disp.fillRect(14,20,100,20,SSD1306_INVERSE);
		disp.fillRect(14,20,brightness_cache*4/10,20,SSD1306_INVERSE);
		if (brightness_cache<30) disp.setCursor(58, 48);
		else if (brightness_cache==250) disp.setCursor(52, 48);
		else disp.setCursor(55, 48);
		sprintf(buffer,"%d%%",brightness_cache*4/10);
		disp.write(buffer);
		disp.display();
		joy=joy_read(4);
		switch (joy) {
			case U:
			case D:
				disp_setbrightness(disp_brightness);
				return NULL;
				break;
			case L:
				if (brightness_cache>0) {
					brightness_cache-=10;
					disp_setbrightness(brightness_cache);
				}
				break;
			case R:
				if (brightness_cache<250) {
					brightness_cache+=10;
					disp_setbrightness(brightness_cache);
				}
				break;
			case B:
				disp_brightness=brightness_cache;
				return NULL;
				break;
			default:
				break;
		}
	}
	return NULL;
}

void *func_quick_settings(const void *param) {
	if ((uint32_t)param==SYS_GPS_1_1) {
		digitalWrite(Vext,LOW);
		Vext_enabled=1;
	}
	else if ((uint32_t)param==SYS_GPS_1_2) {
		digitalWrite(Vext,HIGH);
		Vext_enabled=0;
	}
	else if ((uint32_t)param==SYS_GPS_2_1 && !GPS_bg_runflag) {
		xTaskCreate(func_GPS_update,"GPS Parse",8000,NULL,0,&Task_GPS);
		GPS_bg_runflag=true;
	}
	else if ((uint32_t)param==SYS_GPS_2_2 && GPS_bg_runflag) {
		vTaskDelete(Task_GPS);
		GPS_bg_runflag=false;
	}
	else if ((uint32_t)param==SYS_VOLT_1 && !batvolt_flag_enabled) {
		batvolt_flag_enabled=true;
	}
	else if ((uint32_t)param==SYS_VOLT_2 && batvolt_flag_enabled) {
		batvolt_flag_enabled=false;
	}
	return NULL;
}

void *func_deepsleep(const void *param) {
	digitalWrite(Vext,HIGH);
	rtc_gpio_init(GPIO_NUM_0);
	rtc_gpio_pullup_en(GPIO_NUM_0);
	esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,LOW);
	esp_deep_sleep_start();
}

void func_animation_hline_worker(void *param){
	uint16_t *lineparam=(uint16_t*)param;
	if (lineparam[3])
		disp.fillRect(0,lineparam[0],DISPLAY_WIDTH,lineparam[1],SSD1306_INVERSE);
		//for (uint16_t i=lineparam[0];i<lineparam[0]+lineparam[1];i++) disp.drawFastHLine(0,i,DISPLAY_WIDTH,SSD1306_INVERSE);
	disp.display();
	if (lineparam[2]) {
		uint16_t framecount=lineparam[2]/20;
		uint16_t framewidth=(128/framecount)+1;
		for (uint16_t i=0;i<DISPLAY_WIDTH && animation_running;i+=framewidth) {
			for (uint16_t j=0;j<framewidth && i+j<DISPLAY_WIDTH;j++) 
				disp.drawFastVLine(i+j,lineparam[0],lineparam[1],SSD1306_INVERSE);
			disp.display();
			delay(20);
		}
	}
	else {
		uint16_t frameleft=0;
		uint16_t frameright=24;
		disp.fillRect(0,lineparam[0],24,lineparam[1],SSD1306_INVERSE);
		disp.display();
		while (animation_running) {
			for (uint16_t i=frameleft;i<(frameleft+4)%128;i++) disp.drawFastVLine(i,lineparam[0],lineparam[1],SSD1306_INVERSE);
			for (uint16_t i=frameright;i<(frameright+4)%128;i++) disp.drawFastVLine(i,lineparam[0],lineparam[1],SSD1306_INVERSE);
			disp.display();
			frameleft=(frameleft+4)%128;
			frameright=(frameright+4)%128;
			delay(20);
		}
	}
	free(lineparam);
	animation_running=0;
	disp.display();
	vTaskDelete(NULL);
}

// Draw horizontal animation async for duration ms
void func_animation_hline(uint16_t y, uint16_t h, uint16_t duration, uint16_t inverted, uint8_t command) {
	if (command==ANIME_START && !animation_running) {
		animation_running=1;
		uint16_t *lineparam=(uint16_t *)malloc(sizeof(uint16_t)*4);
		lineparam[0]=y;
		lineparam[1]=h;
		lineparam[2]=duration;
		lineparam[3]=inverted;
		//xTaskCreatePinnedToCore(&func_animation_hline_worker,"Animation_HLine",4000,lineparam,0,&Task_animation,0);
		xTaskCreate(&func_animation_hline_worker,"Animation_HLine",4000,lineparam,0,&Task_animation);
	}
	else if (command==ANIME_INTERRUPT) animation_running=0;
}