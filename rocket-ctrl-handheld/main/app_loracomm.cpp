#include "app_loracomm.hpp"

TaskHandle_t Task_LoRa_listener;
struct contact_list_t contact_list;

void func_loracomm_listener(void *param) {
	struct contact_t *contact;
	struct packet_frame_t packet;
	uint8_t PacketLen=0;
	while (1) {
		PacketLen=LoRa.receiveReliable((uint8_t *)&packet, sizeof(struct packet_frame_t), LORACOMM_FISHNET, RX_WAITTIME_MS, WAIT_RX);
		// Check if the packet has valid device type and is either actually for this device or a broadcast.
		if (!PacketLen || packet.device_type>LORACOMM_DEVTYP_MAX || packet.device_type<LORACOMM_DEVTYP_MIN || packet.source_id==DEVID || (packet.target_id && packet.target_id!=DEVID)) {delay(10);continue;}
		// Check if the packet came from a known sender
		contact=func_contact_list_find(packet.source_id);
		// Add unknown sender to the head of the contact list
		if (!contact) {
			contact=(struct contact_t *)malloc(sizeof(struct contact_t));
			contact->device_type=packet.device_type;
			contact->id=packet.source_id;
			if (!(contact_list.contacts)) {
				contact_list.contacts=contact;
				contact->next=NULL;
				contact->prev=NULL;
			}
			else {
				contact_list.contacts->prev=contact;
				contact->next=contact_list.contacts;
				contact->prev=NULL;
				contact_list.contacts=contact;
			}
			++contact_list.count;
		}
		// Copy new data to contact's storage.
		contact->lastpkt_datalen=PacketLen-LORACOMM_FRAME_LEN;
		contact->lastpkt_RSSI=LoRa.readPacketRSSI();
		contact->lastpkt_SNR=LoRa.readPacketSNR();
		memcpy(&(contact->lastpkt_type),&(packet.packet_type),contact->lastpkt_datalen);
	}
}

// Dieplay available contacts for selection
void func_contact_list_render(struct contact_t *contact, uint8_t pos, uint8_t scale) {
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	if (!(contact_list.contacts)) {
		disp.setTextSize(1);
		disp.setCursor((128-14*6)/2,32-8);
		sprintf(buffer,"Press stick to");
		disp.write(buffer);
		disp.setCursor((128-13*6)/2,32);
		sprintf(buffer,"scan actively");
		disp.write(buffer);
		disp.display();
		return;
	}
//	// Print total
//	disp.setTextSize(1);
//	disp.setCursor(128-8*6, 64-8);
//	sprintf(buffer,"CONTACTS:%2d",contact_list.count);
//	disp.write(buffer);
	// Print highlighted contact
	disp.setTextSize(scale);
	disp.setCursor(6*scale, 8*scale*pos);
	sprintf(buffer,"0X%04x",contact->id);
	disp.write(buffer);
	// Print the remaining contacts
	uint8_t pospt=pos;
	struct contact_t *contactpt=contact;
	while (pospt>0 && contactpt->prev) {
		pospt--;
		contactpt=contactpt->prev;
		disp.setCursor(6*scale, 8*scale*pospt);
		sprintf(buffer,"0X%04x",contactpt->id);
		disp.write(buffer);
	}
	pospt=pos;
	contactpt=contact;
	while (pospt<(11-scale*4) && contactpt->next) {
		pospt++;
		contactpt=contactpt->next;
		disp.setCursor(6*scale, 8*scale*pospt);
		sprintf(buffer,"0X%04x",contactpt->id);
		disp.write(buffer);
	}
//	disp.setCursor(0, 8*scale*pos);
//	disp.write(">");
//	for (uint8_t i=8*scale*pos;i<8*scale*(pos+1);i++) disp.drawFastHLine(0,i,DISPLAY_WIDTH,SSD1306_INVERSE);
	disp.fillRect(0,8*scale*pos,DISPLAY_WIDTH,8*scale,SSD1306_INVERSE);
	disp.display();
}

// Send broadcast message requesting 
void func_loracomm_send_announcement(struct packet_frame_t *packet) {
	func_animation_hline(64-8,8,0,0,ANIME_START);
	packet->target_id=0;
	packet->packet_type=LORACOMM_PKTTYP_ANNC;
	LoRa.transmitReliable((uint8_t*)packet, LORACOMM_FRAME_LEN+sizeof(packet->packet_type)+1, LORACOMM_FISHNET, 0, LoRa_TXpower, WAIT_TX);
	func_animation_hline(64-8,8,0,0,ANIME_INTERRUPT);
}


void func_loracomm_main() {
	uint8_t pos=0;
	struct contact_t *contact=contact_list.contacts;
	enum JOY_DISCRETE joy;
	// Shared packet frame for transmission;
	struct packet_frame_t packet;
	packet.device_type=LORACOMM_DEVTYP_HANDHELD;
	packet.source_id=DEVID;
	bool runflag=1;
	while (runflag) {
		// Check if contact_list has been udpated
		if (!contact && contact_list.contacts) {
			contact=contact_list.contacts;
		}
		func_contact_list_render(contact, pos, 2);
		joy=joy_read(0);
		switch (joy) {
			case U:
				if (contact && contact->prev) {
					contact=contact->prev;
					if (pos>1 || (pos==1 && !contact->prev)) pos--;
				}
				break;
			case D:
				if (contact && contact->next) {
					contact=contact->next;
					if (pos<2 || (pos==2 && !contact->next)) pos++;
				}
				break;
			case L:
				runflag=0;
				break;
			case R:
				if (contact) {
					switch (contact->device_type) {
						case LORACOMM_DEVTYP_GATEWAY:
							plugin_gateway_main(contact);
							break;
					}
				}
				break;
			case B:
				func_loracomm_send_announcement(&packet);
				break;
			default:
				break;
		}
		delay(TICKINT);
	}
}

struct contact_t* func_contact_list_find(uint16_t id) {
	if (!(contact_list.count)) return NULL;
	struct contact_t *contact=contact_list.contacts;
	while (contact->id!=id) {
		if (contact->next) contact=contact->next;
		else return NULL;
	}
	return contact;
}

void func_contact_list_destroy() {
	if (!contact_list.count) return;
	struct contact_t *contact=contact_list.contacts;
	while (contact->next) {
		contact=contact->next;
		free(contact->prev);
	}
	free(contact);
	contact_list.count=0;
	return;
}

void* app_loracomm(const void *param) {
	contact_list.count=0;
	contact_list.contacts=NULL;
	BaseType_t err=xTaskCreate(&func_loracomm_listener,"LoRa_Listener",120000,NULL,0,&Task_LoRa_listener);
	func_loracomm_main();
	vTaskDelete(Task_LoRa_listener);
	func_contact_list_destroy();
	return NULL;
}