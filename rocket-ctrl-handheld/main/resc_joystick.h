#ifndef resc_joystick
#define resc_joystick 1.0

#include <Arduino.h>
#include "resc_display.h"
#include "common_misc.h"

#define JOY_UD A1
#define JOY_LR A3
#define JOY_BTN 3

#define PRGSW_PIN 0

extern bool PRGSW_def,PRGSW_act;

extern uint16_t JOY_UD_neutral, JOY_UD_up_limit, JOY_UD_up_threshold, JOY_UD_down_limit, JOY_UD_down_threshold;
extern uint16_t JOY_LR_neutral, JOY_LR_left_limit, JOY_LR_left_threshold, JOY_LR_right_limit, JOY_LR_right_threshold;

enum JOY_DISCRETE{N,U,D,L,R,UL,UR,DL,DR,B,P};

extern enum JOY_DISCRETE c;

void func_joystick_debug();

enum JOY_DISCRETE joy_read(uint8_t pollint);

#endif