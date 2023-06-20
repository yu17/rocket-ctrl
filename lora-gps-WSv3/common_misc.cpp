#include "common_misc.h"

bool PRGSW_def,PRGSW_act;

bool vext_powered;
uint8_t TICK;
char c;

char buffer[BUFFER_SIZE];

bool batvolt_flag_enabled;
float batvolt_value;

void func_batvolt_update(void *param) {
	int raw;
	raw=analogRead(A0);
	batvolt_value=(float)raw/(float)4095*16.4349;
}