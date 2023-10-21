#include "resc_bme280.hpp"

Adafruit_BME280 BME280;

void bme280_init() {
	I2C2.begin(I2C2_SDA, I2C2_SCL, 100000);
	if (BME280.begin(0x77,&I2C2)) {
//		BME680.setTemperatureOversampling(BME680_OS_8X);
//		BME680.setHumidityOversampling(BME680_OS_4X);
//		BME680.setPressureOversampling(BME680_OS_4X);
//		BME680.setIIRFilterSize(BME680_FILTER_SIZE_3);
		disp.write(">>> BME280 online\n");
		disp.display();
		// Sensor Standby
		BME280.setSampling(Adafruit_BME280::BME280_MODE_SLEEP,
                    Adafruit_BME280::BME280_SAMPLING_NONE,
                    Adafruit_BME280::BME280_SAMPLING_NONE,
                    Adafruit_BME280::BME280_SAMPLING_NONE,
                    Adafruit_BME280::BME280_FILTER_OFF,
                    Adafruit_BME280::BME280_STANDBY_MS_1000);
	}
	else {
		disp.write(">>> BME280 failure\n");
		disp.display();
	}
}