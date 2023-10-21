#ifndef resc_bme680
#define resc_bme680 1.0


#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_BMP280.h>

#include "common_misc.hpp"
#include "resc_joystick.hpp"
#include "resc_display.hpp"

extern Adafruit_BMP280 BMP280;

void bmp280_init();

#endif