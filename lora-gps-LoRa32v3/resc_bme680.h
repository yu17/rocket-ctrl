#ifndef resc_bme680
#define resc_bme680 1.0


#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <Adafruit_BME680.h>

#include "common_misc.h"
#include "resc_joystick.h"
#include "resc_display.h"

extern Adafruit_BME680 BME680;

void bme680_init();

#endif