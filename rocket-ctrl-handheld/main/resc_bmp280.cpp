#include "resc_bmp280.hpp"

Adafruit_BMP280 BMP280(&I2C2);

void bmp280_init() {
//	I2C2.begin(I2C2_SDA, I2C2_SCL, 100000);
	I2C2.setPins(I2C2_SDA, I2C2_SCL);

	if (BMP280.begin(0x76,0x58)) {
//		BME680.setTemperatureOversampling(BME680_OS_8X);
//		BME680.setHumidityOversampling(BME680_OS_4X);
//		BME680.setPressureOversampling(BME680_OS_4X);
//		BME680.setIIRFilterSize(BME680_FILTER_SIZE_3);
		disp.write(">>> BMP280 online\n");
		disp.display();
		// Sensor Standby
		BMP280.setSampling(Adafruit_BMP280::BMP280_MODE_SLEEP,
                    Adafruit_BMP280::BMP280_SAMPLING_NONE,
                    Adafruit_BMP280::BMP280_SAMPLING_NONE,
                    Adafruit_BMP280::BMP280_FILTER_OFF,
                    Adafruit_BMP280::BMP280_STANDBY_MS_4000);
	}
	else {
		disp.write(">>> BMP280 failure\n");
		disp.display();
	}
}