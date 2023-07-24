#include "common_menu.h"

#define SYMB_TICK_W 12
#define SYMB_TICK_H 16
static const unsigned char SYMB_TICK[] = {
	B00000000, B00000000,
	B00000000, B00010000,
	B00000000, B00110000,
	B00000000, B00110000,
	B00000000, B01100000,
	B00000000, B01100000,
	B00000000, B11000000,
	B00000000, B11000000,
	B00000001, B10000000,
	B00000001, B10000000,
	B10000011, B00000000,
	B11000011, B00000000,
	B11100110, B00000000,
	B01110110, B00000000,
	B00111100, B00000000,
	B00011100, B00000000};

#define SYMB_UPTRG_W 12
#define SYMB_UPTRG_H 6
static const unsigned char SYMB_UPTRG[] = {
	B00000110, B00000000,
	B00001111, B00000000,
	B00011111, B10000000,
	B00111111, B11000000,
	B01111111, B11100000,
	B11111111, B11110000};

#define SYMB_DOWNTRG_W 12
#define SYMB_DOWNTRG_H 6
static const unsigned char SYMB_DOWNTRG[] = {
	B11111111, B11110000,
	B01111111, B11100000,
	B00111111, B11000000,
	B00011111, B10000000,
	B00001111, B00000000,
	B00000110, B00000000};



void menu_render(struct menuitem_t *item, uint8_t pos, uint8_t scale) {
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	disp.setTextSize(scale);
	disp.setCursor(0, 8*scale*pos);
	//disp.write(item->desc, 32-11*scale);
	disp.write(item->desc);
	uint8_t pospt=pos;
	struct menuitem_t *itempt=item;
	while (pospt>0 && itempt->prev) {
		pospt--;
		itempt=itempt->prev;
		disp.setCursor(0, 8*scale*pospt);
		disp.write(itempt->desc);
	}
	pospt=pos;
	itempt=item;
	while (pospt<(11-scale*4) && itempt->next) {
		pospt++;
		itempt=itempt->next;
		disp.setCursor(0, 8*scale*pospt);
		disp.write(itempt->desc);
	}
	//disp.setCursor(0, 8*scale*pos);
	//disp.write(">");
	for (uint8_t i=8*scale*pos;i<8*scale*(pos+1);i++) disp.drawFastHLine(0,i,DISPLAY_WIDTH,SSD1306_INVERSE);
	disp.display();
}

void menu_render_animation(uint8_t pos, uint8_t scale, uint8_t speed) {
	for (uint8_t i=8*scale*pos;i<8*scale*(pos+1);i++) disp.drawFastHLine(0,i,DISPLAY_WIDTH,SSD1306_INVERSE);
	disp.display();
	for (uint8_t i=0;i<DISPLAY_WIDTH;i+=speed) {
		for (uint8_t j=0;j<speed && i+j<DISPLAY_WIDTH;j++) 
			disp.drawFastVLine(i+j,8*scale*pos,8*scale,SSD1306_INVERSE);
		disp.display();
	}
}

void menu_destroy(struct menuitem_t *item) {
	while (item->prev) item=item->prev;
	while (item->next) {
		item=item->next;
		if (item->prev->param) free(item->prev->param);
		free(item->prev);
	}
	free(item);
}

uint8_t menu_exec(struct menuitem_t* (*menu_loader)(uint8_t[]), uint8_t* (*menu_marking)(uint8_t[])) {
	uint8_t menu_stack[10];
	memset(&menu_stack,0,10*sizeof(uint8_t));
	uint8_t stackpt=0;
	uint8_t pos=0;
	struct menuitem_t *item=menu_loader(menu_stack);
	//uint8_t *
	void* (*enter)(void*)=NULL;
	void* param=NULL;
	enum JOY_DISCRETE joy;
	while (item) {
		menu_render(item, pos, 2);
		joy=joy_read(4);
		switch (joy) {
			case U:
				if (item->prev) {
					item=item->prev;
					if (pos>1 || (pos==1 && !item->prev)) pos--;
				}
				break;
			case D:
				if (item->next) {
					item=item->next;
					if (pos<2 || (pos==2 && !item->next)) pos++;
				}
				break;
			case L:
				if (stackpt) {
					stackpt--;
					pos=menu_stack[stackpt];
					menu_stack[stackpt]=0;
					menu_destroy(item);
					item=menu_loader(menu_stack);
					while (item->id!=pos && item->next) item=item->next;
					if (pos==1) pos=0;
					else pos=1;
				}
				else {
					menu_destroy(item);
					item=NULL;
				}
				break;
			case R:
				if (item->enter_behavior) {
					if (item->enter_behavior==2) menu_render_animation(pos,2,4);
					if (item->drop_menu) {
						enter=item->enter;
						param=item->param;
						menu_destroy(item);
						item=NULL;
					}
					else (*(item->enter))(item->param);
				}
				else {
					menu_stack[stackpt++]=item->id;
					menu_destroy(item);
					item=menu_loader(menu_stack);
					pos=0;
				}
				break;
			default:
				break;
		}
	}
	disp.clearDisplay();
	disp.display();
	if (enter) (*enter)(param);
	return 0;
}

int menu_numinput(uint8_t digits_integer, uint8_t digits_decimal, int defval, const char* unit) {
	// Return the selected value as an integer! Make sure to apply the deciaml point.
	enum JOY_DISCRETE joy;
	uint8_t total_digit=digits_integer+digits_decimal;
	if (total_digit>9) return -1;
	uint8_t hspace=total_digit<5?6:total_digit<8?2:0;
	uint8_t current_digit=total_digit;
	int8_t digits[10];
	while (current_digit>0) {
		digits[--current_digit]=defval%10;
		defval/=10;
	}
	uint8_t cord_xsum=total_digit*(12+hspace)+6*(digits_decimal>0)+12+hspace;
	while (1) {
		disp.clearDisplay();
		disp.setTextSize(2);
		for (uint8_t i=0;i<digits_integer;i++) {
			disp.setCursor((128-cord_xsum)/2+(12+hspace)*i,64/2-16/2);
			disp.print(digits[i]);
		}
		if (digits_decimal) {
			disp.setCursor((128-cord_xsum)/2+(12+hspace)*digits_integer,64/2);
			disp.setTextSize(1);
			disp.print('.');
			disp.setTextSize(2);
			for (uint8_t i=0;i<digits_decimal;i++) {
				disp.setCursor((128-cord_xsum)/2+(12+hspace)*digits_integer+6+hspace+(12+hspace)*i,64/2-16/2);
				disp.print(digits[digits_integer+i]);
			}
		}
		disp.drawBitmap((128+cord_xsum)/2-12,64/2-16/2,SYMB_TICK,SYMB_TICK_W,SYMB_TICK_H,SSD1306_INVERSE);
		if (unit) {
			disp.setTextSize(1);
			disp.setCursor((128+cord_xsum)/2-12-hspace-6*strlen(unit),64/2+16/2+16);
			disp.print(unit);
		}

		// draw arrows
		if (current_digit<digits_integer) {
			disp.drawBitmap((128-cord_xsum)/2+(12+hspace)*current_digit,64/2-16/2-14,SYMB_UPTRG,SYMB_UPTRG_W,SYMB_UPTRG_H,SSD1306_INVERSE);
			disp.drawBitmap((128-cord_xsum)/2+(12+hspace)*current_digit,64/2+16/2+8,SYMB_DOWNTRG,SYMB_DOWNTRG_W,SYMB_DOWNTRG_H,SSD1306_INVERSE);
//			disp.drawRect((128-cord_xsum)/2+(12+hspace)*current_digit,64/2-16/2,12,16,SSD1306_INVERSE);
		}
		else if (current_digit<total_digit) {
			disp.drawBitmap((128-cord_xsum)/2+(12+hspace)*current_digit+6+hspace,64/2-16/2-14,SYMB_UPTRG,SYMB_UPTRG_W,SYMB_UPTRG_H,SSD1306_INVERSE);
			disp.drawBitmap((128-cord_xsum)/2+(12+hspace)*current_digit+6+hspace,64/2+16/2+8,SYMB_DOWNTRG,SYMB_DOWNTRG_W,SYMB_DOWNTRG_H,SSD1306_INVERSE);
//			disp.drawRect((128-cord_xsum)/2+(12+hspace)*current_digit+6+hspace,64/2-16/2,12,16,SSD1306_INVERSE);
		}
		else 
			for (uint8_t i=0;i<12;i++) disp.drawFastVLine((128+cord_xsum)/2-12+i,64/2-16/2,16,SSD1306_INVERSE);
		disp.display();

		joy=joy_read(4);
		switch (joy) {
			case U:
				digits[current_digit]++;
				digits[current_digit]%=10;
				break;
			case D:
				digits[current_digit]--;
				digits[current_digit]+=10;
				digits[current_digit]%=10;
				break;
			case L:
				if (current_digit) current_digit--;
				break;
			case R:
				if (current_digit<total_digit) current_digit++;
				else {
					defval=0;
					for (uint8_t i=0;i<total_digit;i++) {
						defval*=10;
						defval+=digits[i];
					}
					return defval;
				}
				break;
		}
	}
}