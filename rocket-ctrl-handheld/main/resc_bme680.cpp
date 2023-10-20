#include "resc_bme680.hpp"

Adafruit_BME680 BME680(&I2C2);

void bme680_init() {
	I2C2.begin(I2C2_SDA, I2C2_SCL, 100000);
	if (BME680.begin()) {
		BME680.setTemperatureOversampling(BME680_OS_8X);
		BME680.setHumidityOversampling(BME680_OS_4X);
		BME680.setPressureOversampling(BME680_OS_4X);
		BME680.setIIRFilterSize(BME680_FILTER_SIZE_3);
		disp.write(">>> BME680 online\n");
		disp.display();
	}
	else {
		disp.write(">>> BME680 failure\n");
		disp.display();
	}
}