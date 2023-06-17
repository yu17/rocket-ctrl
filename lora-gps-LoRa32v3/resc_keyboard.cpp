#include "resc_keyboard.h"

bool PRGSW_def,PRGSW_act;

const uint8_t KBD_ROW[4]={42,41,40,39};
const uint8_t KBD_COL[4]={34,33,47,48};
const char KBD_GRID[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
char c;

char kbd_read(uint8_t pollint) {
	PRGSW_act=0;
	char key=0;
	uint8_t keyi=0,keyj=0;
	while (!PRGSW_act) {
		for (uint8_t i=0;i<4;i++) {
			digitalWrite(KBD_ROW[i],LOW);
			for (uint8_t j=0;j<4;j++) 
				if (digitalRead(KBD_COL[j])==LOW) {
					digitalWrite(KBD_ROW[i],HIGH);
					key=KBD_GRID[i][j];
					keyi=i;
					keyj=j;
					PRGSW_act=1;
					break;
				}
			digitalWrite(KBD_ROW[i],HIGH);
		}
		if (digitalRead(PRGSW_PIN)!=PRGSW_def) {
			key='P';
			PRGSW_act=1;
		}
		if (pollint) delay(pollint);
		else break;
	}
	while (PRGSW_act) {
		if (key=='P' && digitalRead(PRGSW_PIN)==PRGSW_def) PRGSW_act=0;
		else if (key!='P') {
			digitalWrite(KBD_ROW[keyi],LOW);
			if (digitalRead(KBD_COL[keyj])==HIGH) PRGSW_act=0;
		}
		if (pollint) delay(pollint);
		else delay(4);
	}
	return key;
}