#include "common_menu.h"

void menu_render(struct menuitem_t *item, uint8_t pos, uint8_t scale) {
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	disp.setTextSize(scale);
	disp.setCursor(6*scale, 8*scale*pos);
	//disp.write(item->desc, 32-11*scale);
	disp.write(item->desc);
	uint8_t pospt=pos;
	struct menuitem_t *itempt=item;
	while (pospt>0 && itempt->prev) {
		pospt--;
		itempt=itempt->prev;
		disp.setCursor(6*scale, 8*scale*pospt);
		disp.write(itempt->desc);
	}
	pospt=pos;
	itempt=item;
	while (pospt<(11-scale*4) && itempt->next) {
		pospt++;
		itempt=itempt->next;
		disp.setCursor(6*scale, 8*scale*pospt);
		disp.write(itempt->desc);
	}
	disp.setCursor(0, 8*scale*pos);
	disp.write(">");
	disp.display();
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

uint8_t menu_exec(struct menuitem_t* (*menu_loader)(uint8_t[])) {
	uint8_t menu_stack[10];
	memset(&menu_stack,0,10*sizeof(uint8_t));
	uint8_t stackpt=0;
	uint8_t pos=0;
	struct menuitem_t *item=menu_loader(menu_stack);
	void* (*enter)(void*)=NULL;
	void* param=NULL;
	char key;
	while (item) {
		menu_render(item, pos, 2);
		key=kbd_read(4);
		switch (key) {
			case 'A':
				if (item->prev) {
					item=item->prev;
					if (pos>1 || (pos==1 && !item->prev)) pos--;
				}
				break;
			case 'B':
				if (item->next) {
					item=item->next;
					if (pos<2 || (pos==2 && !item->next)) pos++;
				}
				break;
			case 'C':
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
			case 'D':
				if (item->enter_behavior) {
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