#ifndef conf_global
#define conf_global 1.0

#include <Arduino.h>

// ----- Comm Protocol -----
#define LORAGPS_MAG_HEAD 0x89
#define LORAGPS_TYP_TIME 0x30
#define LORAGPS_TYP_CORD 0x32
#define LORAGPS_TYP_MOTN 0x34
#define LORAGPS_TYP_ACCR 0x36
#define LORAGPS_TYP_POSE 0x38

// ----- Settings -----
#define SYS_GPS_1_1 0x22
#define SYS_GPS_1_2 0x23
#define SYS_GPS_2_1 0x24
#define SYS_GPS_2_2 0x25
#define SYS_GPS_3_1 0x26
#define SYS_GPS_3_2 0x27
#define SYS_GPS_3_3 0x28
#define SYS_GPS_3_4 0x29
#define SYS_VOLT_1 0x00
#define SYS_VOLT_2 0x01
#define SYS_TEMP_1 0x02
#define SYS_TEMP_2 0x03

// ----- MISC -----
#define TICKINT 10
#define BUFFER_SIZE 128

#endif