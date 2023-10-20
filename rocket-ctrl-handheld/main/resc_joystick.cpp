#include "resc_joystick.hpp"

bool PRGSW_def,PRGSW_act=0;

enum JOY_DISCRETE c;

uint16_t JOY_UD_neutral=1816, JOY_UD_up_limit=473, JOY_UD_up_threshold=1000, JOY_UD_down_limit=3126, JOY_UD_down_threshold=2600;
uint16_t JOY_LR_neutral=1816, JOY_LR_left_limit=425, JOY_LR_left_threshold=1000, JOY_LR_right_limit=3100, JOY_LR_right_threshold=2600;

void func_joystick_debug() {
	int ud,lr,btn;
	pinMode(JOY_BTN,INPUT_PULLUP);
	while (1) {
		disp.clearDisplay();
		ud=analogRead(JOY_UD);
		lr=analogRead(JOY_LR);
		btn=digitalRead(JOY_BTN);
		disp.setCursor(6,16);
		sprintf(buffer,"UD: %d\nLR: %d\nBTN: %d\n",ud,lr,btn);
		disp.write(buffer);
		disp.display();
		delay(10);
	}
}

enum JOY_DISCRETE joy_read(uint8_t pollint) {
	uint16_t ud,lr,btn;
	while (PRGSW_act) {
		ud=analogRead(JOY_UD);
		lr=analogRead(JOY_LR);
		btn=digitalRead(JOY_BTN);
		if (ud>JOY_UD_up_threshold && ud<JOY_UD_down_threshold && lr>JOY_LR_left_threshold && lr<JOY_LR_right_threshold
			&& btn==HIGH && digitalRead(PRGSW_PIN)==PRGSW_def) break;
		delay(pollint);
	}
	PRGSW_act=1;
	while (1) {
		ud=analogRead(JOY_UD);
		lr=analogRead(JOY_LR);
		btn=digitalRead(JOY_BTN);
		if (btn==LOW) return B;
		else if (ud<JOY_UD_up_threshold && lr<JOY_LR_left_threshold) return UL;
		else if (ud<JOY_UD_up_threshold && lr>JOY_LR_right_threshold) return UR;
		else if (ud>JOY_UD_down_threshold && lr<JOY_LR_left_threshold) return UL;
		else if (ud>JOY_UD_down_threshold && lr>JOY_LR_right_threshold) return UR;
		else if (ud<JOY_UD_up_threshold) return U;
		else if (ud>JOY_UD_down_threshold) return D;
		else if (lr<JOY_LR_left_threshold) return L;
		else if (lr>JOY_LR_right_threshold) return R;
		else if (digitalRead(PRGSW_PIN)!=PRGSW_def) return P;
		if (pollint) delay(pollint);
		else break;
	}
	PRGSW_act=0;
	return N;
}