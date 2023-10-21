#ifndef resc_bme680
#define resc_bme680 1.0


#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_BME280.h>

#include "common_misc.hpp"
#include "resc_joystick.hpp"
#include "resc_display.hpp"

extern Adafruit_BME280 BME280;

void bme280_init();

#endif