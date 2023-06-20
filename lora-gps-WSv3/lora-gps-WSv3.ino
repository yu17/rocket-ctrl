#include <Arduino.h>
#include <driver/rtc_io.h>

#include <Wire.h>
#include <GY521.h>

#include "common_misc.h"
#include "mod_disp.h"
#include "mod_gps.h"
#include "mod_lora.h"
#include "mod_compass.h"


// ----- GY521 -----
//#define GY521_I2C_SDA 46
//#define GY521_I2C_SCL 45
//TwoWire I2C_GY521=TwoWire(0);
//GY521 gyro(0x68, &I2C_GY521);

void setup() {
	Serial.begin(115200);
	pinMode(Vext,OUTPUT);
	digitalWrite(Vext,LOW);
	vext_powered=1;
	TICK=0;
	disp_init();
	// LED -- turn off on startup
	pinMode(LED,OUTPUT);
	disp.write(">>> LED test\n");
	disp.display();
	digitalWrite(LED,HIGH);
	delay(500);
	digitalWrite(LED,LOW);
	// PRGSW -- detect default voltage and reset
	pinMode(PRGSW_PIN,INPUT);
	PRGSW_def=digitalRead(PRGSW_PIN);
	PRGSW_act=0;
	if (PRGSW_def) disp.write(">>> PRGSW HIGH\n");
	else disp.write(">>> PRGSW LOW\n");
	disp.display();
	// Serial/GPS -- Todo: GPS module communication
	func_GPS_enable();
	disp.write(">>> GPS online\n");
	disp.display();
	// LoRa -- initialization
	func_lora_setup();
	func_shipinfo_broadcast_enable();
	disp.write(">>> LoRa online\n");
	disp.display();
	func_compass_init();
	disp.write(">>> Compass online\n");
	disp.display();
//	I2C_GY521.begin(GY521_I2C_SDA,GY521_I2C_SCL,100000);
//	if (gyro.begin()) Serial.println(">>> GY521 online");
//	else Serial.println(">>> GY521 failure");
//	gyro.setAccelSensitivity(3);
//	gyro.setGyroSensitivity(1);
	delay(500);
	disp_pwroff();
}

void loop() {
	TICK++;
	func_batvolt_update(NULL);
	sleep(1);
}
