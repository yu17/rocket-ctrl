#include "app_shiptracker.h"

//tracker maintain shipdb
void func_tracker_shiplist_update(void *param) {
	if (!param) return;
	struct ship_list_t *shiplist=(struct ship_list_t *)param;
	struct ship_data_t *ship=shiplist->ship;
	struct packet_frame_t pk_probe;
	while (1) {
		PacketLen=LoRa.receive((uint8_t *)&pk_probe, sizeof(struct packet_frame_t), 0, WAIT_RX);
		PacketRSSI=LoRa.readPacketRSSI();
		PacketSNR=LoRa.readPacketSNR();
		PacketID=pk_probe.pid;
		if (PacketLen && pk_probe.magicnum==LORAGPS_MAG_HEAD) {
			while (ship && ship->ship_cid!=pk_probe.cid) ship=ship->next;
			if (!ship) {
				ship=(struct ship_data_t *)malloc(sizeof(struct ship_data_t));
				ship->ship_cid=pk_probe.cid;
				ship->next=shiplist->ship;
				if (shiplist->ship) shiplist->ship->prev=ship;
				ship->prev=NULL;
				shiplist->ship=ship;
			}
			switch (pk_probe.sig) {
				case LORAGPS_TYP_TIME:
					memcpy(&(ship->ship_time),pk_probe.fields,sizeof(struct packet_time_t));
					break;
				case LORAGPS_TYP_CORD:
					memcpy(&(ship->ship_cord),pk_probe.fields,sizeof(struct packet_cord_t));
					break;
				case LORAGPS_TYP_MOTN:
					memcpy(&(ship->ship_motn),pk_probe.fields,sizeof(struct packet_motn_t));
					break;
				case LORAGPS_TYP_ACCR:
					memcpy(&(ship->ship_accr),pk_probe.fields,sizeof(struct packet_accr_t));
					break;
				case LORAGPS_TYP_POSE:
					memcpy(&(ship->ship_pose),pk_probe.fields,sizeof(struct packet_pose_t));
					break;
			}
		}
	}
}

// tracker display shipinfo
void func_tracker_shipinfo(struct ship_data_t *ship) {
	bool runflag=1;
	uint8_t page=0;
	char key;
	while (runflag) {
		disp.clearDisplay();
		disp.setTextSize(1);
		if (page==0) {
			disp.setCursor(6, 0);
			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(0, 8);
			sprintf(buffer,"%9.6f%c",abs(ship->ship_cord.latitude),ship->ship_cord.latitude<0?'S':'N');
			disp.write(buffer);
			disp.setCursor(6*11, 8);
			if (abs(GPS.location.lng())>=100) sprintf(buffer,"%9.5f%c",abs(ship->ship_cord.longitude),ship->ship_cord.longitude<0?'W':'E');
			else sprintf(buffer,"%9.6f%c",abs(ship->ship_cord.longitude),ship->ship_cord.longitude<0?'W':'E');
			disp.write(buffer);
			disp.setCursor(6*2, 16);
			sprintf(buffer,"%.1fm/s",ship->ship_motn.speed);
			disp.write(buffer);
			disp.setCursor(6*9, 16);
			sprintf(buffer,"(%.1fkm/h)",ship->ship_motn.speed*3.6);
			disp.write(buffer);
			disp.setCursor(0, 24);
			sprintf(buffer,"%.2f%c",ship->ship_motn.course/100.0,248);
			disp.write(buffer);
			disp.setCursor(6*9, 24);
			sprintf(buffer,"%s",GPS.cardinal(ship->ship_motn.course/100.0));
			disp.write(buffer);
			disp.setCursor(6*13, 24);
			sprintf(buffer,"%.2fm",ship->ship_cord.altitude);
			disp.write(buffer);
			disp.setCursor(0, 32);
			sprintf(buffer,"CORS: %.2f%c",GPS.courseTo(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude),248);
			disp.write(buffer);
			disp.setCursor(6*15, 32);
			sprintf(buffer,"%s",GPS.cardinal(GPS.courseTo(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude)));
			disp.write(buffer);
			disp.setCursor(0, 40);
			sprintf(buffer,"DIST: %.1f",GPS.distanceBetween(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude));
			disp.write(buffer);
			disp.setCursor(0, 48);
			sprintf(buffer,"SNR: %d",PacketSNR);
			disp.write(buffer);
			disp.setCursor(6*11, 48);
			sprintf(buffer,"RSSI: %d",PacketRSSI);
			disp.write(buffer);
			disp.setCursor(0, 56);
			sprintf(buffer,"PKT: %d",PacketID);
			disp.write(buffer);
		}
		else if (page==1) {
			disp.setCursor(6, 0);
			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(0, 8);
			sprintf(buffer,"Accel/Gyro:");
			disp.write(buffer);
			disp.setCursor(0,16);
			sprintf(buffer,"%7.2f",ship->ship_accr.acc_x);
			disp.write(buffer);
			disp.setCursor(6*7,16);
			sprintf(buffer,"%7.2f",ship->ship_accr.acc_y);
			disp.write(buffer);
			disp.setCursor(6*14,16);
			sprintf(buffer,"%7.2f",ship->ship_accr.acc_z);
			disp.write(buffer);
			disp.setCursor(0,24);
			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_x);
			disp.write(buffer);
			disp.setCursor(6*7,24);
			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_y);
			disp.write(buffer);
			disp.setCursor(6*14,24);
			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_z);
			disp.write(buffer);
			disp.setCursor(0,32);
			sprintf(buffer,"Orientation:");
			disp.write(buffer);
			disp.setCursor(0,40);
			sprintf(buffer,"%7.2f",ship->ship_pose.agl_x);
			disp.write(buffer);
			disp.setCursor(6*7,40);
			sprintf(buffer,"%7.2f",ship->ship_pose.agl_y);
			disp.write(buffer);
			disp.setCursor(6*14,40);
			sprintf(buffer,"%7.2f",ship->ship_pose.agl_z);
			disp.write(buffer);
			disp.setCursor(0,48);
			sprintf(buffer,"%7.2f",ship->ship_pose.pitch);
			disp.write(buffer);
			disp.setCursor(6*7,48);
			sprintf(buffer,"%7.2f",ship->ship_pose.roll);
			disp.write(buffer);
			disp.setCursor(6*14,48);
			sprintf(buffer,"%7.2f",ship->ship_pose.yaw);
			disp.write(buffer);
			disp.setCursor(0,56);
			sprintf(buffer,"Temperature: %.2f%cC",ship->ship_pose.temperature,248);
			disp.write(buffer);
		}
		disp.display();
		key=kbd_read(0);
		switch (key) {
			case 'A':
				if (page>0) page--;
				break;
			case 'B':
				if (page<1) page++;
				break;
			case 'C':
			case 'D':
				runflag=0;
				break;
		}
		delay(TICKINT);
	}
}

// tracker display shiplist
void func_tracker_shiplist_render(struct ship_data_t *ship, uint8_t pos, uint8_t scale) {
	// Scale=1 ==> line space=16pixels, 8 entries per page, 21 characters per line
	// Scale=2 ==> line space=32pixels, 4 entries per page, 10 characters per line
	disp.clearDisplay();
	struct ship_data_t *shippt=ship;
	uint8_t count=0;
	while (shippt) {
		count++;
		shippt=shippt->next;
	}
	disp.setTextSize(1);
	disp.setCursor(128-8*6, 64-8);
	sprintf(buffer,"SHIPS:%2d",count);
	disp.write(buffer);
	if (count) {
		disp.setTextSize(scale);
		disp.setCursor(6*scale, 8*scale*pos);
		sprintf(buffer,"0X%04x",ship->ship_cid);
		disp.write(buffer);
		uint8_t pospt=pos;
		shippt=ship;
		while (pospt>0 && shippt->prev) {
			pospt--;
			shippt=shippt->prev;
			disp.setCursor(6*scale, 8*scale*pospt);
			sprintf(buffer,"0X%04x",shippt->ship_cid);
			disp.write(buffer);
		}
		pospt=pos;
		shippt=ship;
		while (pospt<(11-scale*4) && shippt->next) {
			pospt++;
			shippt=shippt->next;
			disp.setCursor(6*scale, 8*scale*pospt);
			sprintf(buffer,"0X%04x",shippt->ship_cid);
			disp.write(buffer);
		}
		disp.setCursor(0, 8*scale*pos);
		disp.write(">");
	}
	disp.display();
}

void func_tracker_selship(struct ship_list_t *shiplist) {
	uint8_t pos=0;
	struct ship_data_t *ship=shiplist->ship;
	char key;
	bool runflag=1;
	while (runflag) {
		if (!ship && shiplist->ship) {
			ship=shiplist->ship;
		}
		func_tracker_shiplist_render(ship, pos, 2);
		key=kbd_read(0);
		switch (key) {
			case 'A':
				if (ship && ship->prev) {
					ship=ship->prev;
					if (pos>1 || (pos==1 && !ship->prev)) pos--;
				}
				break;
			case 'B':
				if (ship && ship->next) {
					ship=ship->next;
					if (pos<2 || (pos==2 && !ship->next)) pos++;
				}
				break;
			case 'C':
				runflag=0;
				break;
			case 'D':
				if (ship) func_tracker_shipinfo(ship);
				break;
		}
		delay(TICKINT);
	}
}

void func_tracker_destroy_shiplist(struct ship_list_t *shiplist) {
	struct ship_data_t *ship=shiplist->ship;
	if (!ship) return;
	while (ship->next) {
		ship=ship->next;
		free(ship->prev);
	}
	free(ship);
}

void *app_tracker(void *param) {
	TaskHandle_t Task_LoRa;
	struct ship_list_t *shiplist=(struct ship_list_t *)malloc(sizeof(struct ship_list_t));
	shiplist->ship=NULL;
	xTaskCreatePinnedToCore(func_tracker_shiplist_update,"Ship Scan",100000,shiplist,0,&Task_LoRa,0);
	func_tracker_selship(shiplist);
	vTaskDelete(Task_LoRa);
	func_tracker_destroy_shiplist(shiplist);
	return NULL;
}