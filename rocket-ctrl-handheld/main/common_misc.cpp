#include "common_misc.hpp"

TwoWire I2C2=TwoWire(1);

uint8_t TICK;
char buffer[BUFFER_SIZE];
uint8_t buffer_pt;

// ----- Battery Voltage -----
TaskHandle_t Task_batvolt;
bool batvolt_flag_enabled;
float batvolt_value;

// ----- battery voltage background functions -----
void func_batvolt_update(void *param) {
	int raw;
	while (1) {
		raw=analogRead(A0);
		batvolt_value=(float)raw/(float)4095*3.6*(float)490/(float)100;
		delay(1000);
	}
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
	if (*((int*)param)==SYS_GPS_1_1) digitalWrite(Vext,LOW);
	else if (*((int*)param)==SYS_GPS_1_2) digitalWrite(Vext,HIGH);
	else if (*((int*)param)==SYS_GPS_2_1 && !GPS_bg_runflag) {
		xTaskCreatePinnedToCore(func_GPS_update,"GPS Parse",100000,NULL,0,&Task_GPS,0);
		GPS_bg_runflag=true;
	}
	else if (*((int*)param)==SYS_GPS_2_2 && GPS_bg_runflag) {
		vTaskDelete(Task_GPS);
		GPS_bg_runflag=false;
	}
	else if (*((int*)param)==SYS_VOLT_1 && !batvolt_flag_enabled) {
		xTaskCreate(func_batvolt_update,"Battery Voltage",10000,NULL,0,&Task_batvolt);
		batvolt_flag_enabled=true;
	}
	else if (*((int*)param)==SYS_VOLT_2 && batvolt_flag_enabled) {
		vTaskDelete(Task_batvolt);
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
//	rtc_gpio_deinit(GPIO_NUM_0);
//	pinMode(PRGSW_PIN,INPUT);
//	digitalWrite(Vext,LOW);
}