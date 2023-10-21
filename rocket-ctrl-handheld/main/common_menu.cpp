#include "common_menu.hpp"


// Bitmap glyphs. Rendered directly as part of some UIs.
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

void menu_render(const struct menu_page_t *page, uint8_t id, uint8_t pos, uint8_t scale, int32_t marking) {
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	disp.setTextSize(scale);
	disp.setCursor(0, 8*scale*pos);
	//disp.write(item->desc, 32-11*scale);
	disp.write(page->items[id].desc);
	if (id==marking) disp.drawCircle(128-4*scale,8*scale*pos+4*scale,4,SSD1306_INVERSE);
	uint8_t pospt=pos;
	uint8_t idpt=id;
	while (pospt>0 && idpt>0) {
		pospt--;
		idpt--;
		disp.setCursor(0, 8*scale*pospt);
		disp.write(page->items[idpt].desc);
		if (idpt==marking) disp.drawCircle(128-4*scale,8*scale*pospt+4*scale,4,SSD1306_INVERSE);
	}
	pospt=pos;
	idpt=id;
	while (pospt<(11-scale*4) && idpt<page->length-1) {
		pospt++;
		idpt++;
		disp.setCursor(0, 8*scale*pospt);
		disp.write(page->items[idpt].desc);
		if (idpt==marking) disp.drawCircle(128-4*scale,8*scale*pospt+4*scale,4,SSD1306_INVERSE);
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

uint8_t menu_exec(const struct menu_page_t *page) {
	struct menu_stack_t menu_stack[MENU_STACK_MAXDEPTH];
	memset(&menu_stack,0,10*sizeof(struct menu_stack_t));
	uint8_t stackpt=0;
	uint8_t pos=0;
	uint8_t id=0;
	void* (*routine)(const void*)=NULL;
	const void* param=NULL;
	enum JOY_DISCRETE joy;
	while (page) {
		if (page->marking) menu_render(page,id,pos,2,(*(page->marking))(page->marking_param));
		else menu_render(page, id, pos, 2, -1);
		joy=joy_read(4);
		switch (joy) {
			case U:
				if (id>0) {
					id--;
					if (pos>1 || (pos==1 && id==0)) pos--;
				}
				break;
			case D:
				if (id<page->length-1) {
					id++;
					if (pos<2 || (pos==2 && id==page->length-1)) pos++;
				}
				break;
			case L:
				if (stackpt) {
					stackpt--;
					page=menu_stack[stackpt].page;
					id=menu_stack[stackpt].item_id;
					menu_stack[stackpt].page=NULL;
					menu_stack[stackpt].item_id=0;
					if (id==0) pos=0;
					else pos=1;
				}
				else {
					page=NULL;
				}
				break;
			case R:
				if (page->items[id].enter_behavior) {
					if (page->items[id].enter_behavior==2) menu_render_animation(pos,2,4);
					if (page->items[id].drop_menu) {
						routine=page->items[id].routine.func;
						param=page->items[id].param;
						page=NULL;
					}
					else (*(page->items[id].routine.func))(page->items[id].param);
				}
				else {
					menu_stack[stackpt].page=page;
					menu_stack[stackpt].item_id=id;
					stackpt++;
					page=page->items[id].routine.page;
					id=0;
					pos=0;
				}
				break;
			default:
				break;
		}
	}
	disp.clearDisplay();
	disp.display();
	if (routine) (*routine)(param);
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
			default:
				break;
		}
	}
}

int8_t menu_rangeinput(int8_t min, int8_t max, int8_t interval, int8_t defval, const char* unit) {
	enum JOY_DISCRETE joy;
	int8_t val=defval;
	while (1) {
		disp.clearDisplay();
		disp.drawBitmap((128-SYMB_UPTRG_W)/2,64/2-16/2-14,SYMB_UPTRG,SYMB_UPTRG_W,SYMB_UPTRG_H,SSD1306_INVERSE);
		disp.drawBitmap((128-SYMB_DOWNTRG_W)/2,64/2+16/2+8,SYMB_DOWNTRG,SYMB_DOWNTRG_W,SYMB_DOWNTRG_H,SSD1306_INVERSE);
		disp.setTextSize(2);
		// Calculate the number of digits so as to determine the coordinates for display
		uint8_t digits=(val<=0);// Add one digit for negative sign; Also add one digit when val=0
		int8_t temp=val;
		while (temp) {digits++;temp/=10;}
		sprintf(buffer,"%d",val);
		disp.setCursor((128-12*digits)/2,24);
		disp.write(buffer);
		disp.setTextSize(1);
		disp.setCursor((128+12*digits)/2+4,32);
		disp.write(unit);
		disp.display();
		joy=joy_read(4);
		switch (joy) {
			case U:
				if (val<=max-interval) val+=interval;
				else val=max;
				break;
			case D:
				if (val>=min+interval) val-=interval;
				else val=min;
				break;
			case L:
				if (val>=min+5*interval) val-=5*interval;
				else val=min;
				break;
			case R:
				if (val<=max-5*interval) val+=5*interval;
				else val=max;
				break;
			case B:
				return val;
				break;
			default:
				break;
		}
	}
	return val;
}