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