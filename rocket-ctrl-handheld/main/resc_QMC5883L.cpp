#include "resc_QMC5883L.hpp"

static void write_register( int addr, int reg, int value ) {
	I2C2.beginTransmission(addr);
	I2C2.write(reg);
	I2C2.write(value);
	I2C2.endTransmission();
}

static int read_register( int addr, int reg, int count ) {
	I2C2.beginTransmission(addr);
	I2C2.write(reg);
	I2C2.endTransmission();
	
	I2C2.requestFrom(addr,count);
	int n = I2C2.available();
	if(n!=count) return 0;

	return n;
}

void QMC5883L::reconfig() {
	write_register(addr,QMC5883L_CONFIG,oversampling|range|rate|mode);  
}

void QMC5883L::reset() {
	write_register(addr,QMC5883L_RESET,0x01);
	reconfig();
}

void QMC5883L::setOversampling( int x ) {
	switch(x) {
		case 512:
			oversampling = QMC5883L_CONFIG_OS512;
			break;
		case 256:
			oversampling = QMC5883L_CONFIG_OS256;
			break;
		case 128:
			oversampling = QMC5883L_CONFIG_OS128;
			break;
		case 64:
			oversampling = QMC5883L_CONFIG_OS64;
			break;
		default:
			break;
	}
	reconfig();
}

void QMC5883L::setRange( int x ) {
	switch(x) {
		case 2:
			range = QMC5883L_CONFIG_2GAUSS;
			break;
		case 8:
			range = QMC5883L_CONFIG_8GAUSS;
			break;
		default:
			break;
	}
	reconfig();
}

void QMC5883L::setSamplingRate( int x ) {
	switch(x) {
		case 10:
			rate = QMC5883L_CONFIG_10HZ;
			break;
		case 50:
			rate = QMC5883L_CONFIG_50HZ;
			break;
		case 100:
			rate = QMC5883L_CONFIG_100HZ;
			break;
		case 200:
			rate = QMC5883L_CONFIG_200HZ;
			break;
		default:
			break;
	}
	reconfig();
}

void QMC5883L::init() {
	/* This assumes the wire library has been initialized. */
	addr = GY271_I2C_ADDR;
	oversampling = QMC5883L_CONFIG_OS512;
	range = QMC5883L_CONFIG_2GAUSS;
	rate = QMC5883L_CONFIG_50HZ;
	mode = QMC5883L_CONFIG_CONT;
	reset();
}

int QMC5883L::ready() {
	if(!read_register(addr,QMC5883L_STATUS,1)) return 0;
	uint8_t status = I2C2.read();
	return status & QMC5883L_STATUS_DRDY; 
}

int QMC5883L::readRaw( int16_t *x, int16_t *y, int16_t *z, int16_t *t ) {
	while(!ready()) {}

	if(!read_register(addr,QMC5883L_X_LSB,6)) return 0;

	*x = I2C2.read() | (I2C2.read()<<8);
	*y = I2C2.read() | (I2C2.read()<<8);
	*z = I2C2.read() | (I2C2.read()<<8);

	return 1;
}

void QMC5883L::resetCalibration() {
	xhigh = yhigh = 0;
	xlow = ylow = 0;
}

int QMC5883L::readHeading() {
	int16_t x, y, z, t;

	if(!readRaw(&x,&y,&z,&t)) return 0;

	/* Update the observed boundaries of the measurements */

	if(x<xlow) xlow = x;
	if(x>xhigh) xhigh = x;
	if(y<ylow) ylow = y;
	if(y>yhigh) yhigh = y;

	/* Bail out if not enough data is available. */
	
	if( xlow==xhigh || ylow==yhigh ) return 0;

	/* Recenter the measurement by subtracting the average */

	x -= (xhigh+xlow)/2;
	y -= (yhigh+ylow)/2;

	/* Rescale the measurement to the range observed. */
	
	float fx = (float)x/(xhigh-xlow);
	float fy = (float)y/(yhigh-ylow);

	int heading = 180.0*atan2(fy,fx)/M_PI;
	if(heading<=0) heading += 360;
	
	return heading;
}

QMC5883L compass;

int compass_offset=-90;

void compass_init() {
	I2C2.begin(I2C2_SDA,I2C2_SCL,100000);
	compass.init();
	compass.setSamplingRate(50);
}

void compass_draw_graph(uint8_t center_x, uint8_t center_y, uint8_t radius, int course_ccw) {
	float course_rad=(course_ccw-90)%360/180.0*M_PI;
	disp.drawCircle(center_x,center_y,radius,SSD1306_INVERSE);
	radius-=4;
	disp.setCursor(center_x+(int)(radius*cos(course_rad))-3,center_y+(int)(radius*sin(course_rad))-4);
	disp.write("N");
	course_rad=(course_ccw-180)%360/180.0*M_PI;
	disp.setCursor(center_x+(int)(radius*cos(course_rad))-3,center_y+(int)(radius*sin(course_rad))-4);
	disp.write("W");
	course_rad=(course_ccw+90)%360/180.0*M_PI;
	disp.setCursor(center_x+(int)(radius*cos(course_rad))-3,center_y+(int)(radius*sin(course_rad))-4);
	disp.write("S");
	course_rad=course_ccw%360/180.0*M_PI;
	disp.setCursor(center_x+(int)(radius*cos(course_rad))-3,center_y+(int)(radius*sin(course_rad))-4);
	disp.write("E");
}

void compass_draw_arrow(uint8_t center_x, uint8_t center_y, uint8_t radius, int course_ccw) {
	float course_rad=(course_ccw-90)%360/180.0*M_PI;
	float arrow_rad=(course_ccw-90-30)%360/180.0*M_PI;
	uint16_t arrow_len=radius*0.4>5?radius*0.4:5;
	disp.drawLine(center_x-(int)(radius*cos(course_rad)),center_y-(int)(radius*sin(course_rad)),center_x+(int)(radius*cos(course_rad)),center_y+(int)(radius*sin(course_rad)),SSD1306_INVERSE);
	disp.drawLine(center_x+(int)(radius*cos(course_rad)),center_y+(int)(radius*sin(course_rad)),center_x+(int)(radius*cos(course_rad))-(int)(arrow_len*cos(arrow_rad)),center_y+(int)(radius*sin(course_rad))-(int)(arrow_len*sin(arrow_rad)),SSD1306_INVERSE);
	arrow_rad=(course_ccw-90+30)%360/180.0*M_PI;
	disp.drawLine(center_x+(int)(radius*cos(course_rad)),center_y+(int)(radius*sin(course_rad)),center_x+(int)(radius*cos(course_rad))-(int)(arrow_len*cos(arrow_rad)),center_y+(int)(radius*sin(course_rad))-(int)(arrow_len*sin(arrow_rad)),SSD1306_INVERSE);
}
