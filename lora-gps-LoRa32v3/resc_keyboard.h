#ifndef resc_keyboard
#define resc_keyboard 1.0

#include <Arduino.h>

// ----- PRGSW/Keyboard -----
#define PRGSW_PIN 0

extern bool PRGSW_def,PRGSW_act;

extern const uint8_t KBD_ROW[4];
extern const uint8_t KBD_COL[4];
extern const char KBD_GRID[4][4];
extern char c;

char kbd_read(uint8_t pollint);

#endif