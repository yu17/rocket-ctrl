#ifndef common_misc
#define common_misc 1.0

#include <Arduino.h>

#define PRGSW_PIN 0
#define PIN_IGN 45
#define BUFFER_SIZE 128

extern bool PRGSW_def,PRGSW_act;

extern bool vext_powered;
extern uint8_t TICK;
extern char c;

extern char buffer[BUFFER_SIZE];

extern bool batvolt_flag_enabled;
extern float batvolt_value;

void func_batvolt_update(void *param);

#endif
