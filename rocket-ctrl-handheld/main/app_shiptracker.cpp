#include "app_shiptracker.hpp"

#define PARAM_SIZE sizeof(uint8_t)+sizeof(struct packet_ctrl_t)

const size_t packet_header_size=sizeof(uint8_t)*3+sizeof(uint16_t);

const struct menu_page_t shipctrlmenu_p={6,shipctrlmenu,NULL,0};
const struct menu_page_t shipctrlmenu_p_0={4,shipctrlmenu_0,NULL,0};
const struct menu_page_t shipctrlmenu_p_1={3,shipctrlmenu_1,NULL,0};
const struct menu_page_t shipctrlmenu_p_2={3,shipctrlmenu_2,NULL,0};
const struct menu_page_t shipctrlmenu_p_3={3,shipctrlmenu_3,NULL,0};
const struct menu_page_t shipctrlmenu_p_4={1,shipctrlmenu_4,NULL,0};
const struct menu_page_t shipctrlmenu_p_5={1,shipctrlmenu_5,NULL,0};

const struct menu_item_t shipctrlmenu[6]={
	{
		.desc="LED",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&shipctrlmenu_p_0},
		.param=NULL
	},
	{
		.desc="GPS",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&shipctrlmenu_p_1},
		.param=NULL
	},
	{
		.desc="LoRa",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&shipctrlmenu_p_2},
		.param=NULL
	},
	{
		.desc="Display",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&shipctrlmenu_p_3},
		.param=NULL
	},
	{
		.desc="Ignition",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&shipctrlmenu_p_4},
		.param=NULL
	},
	{
		.desc="Poweroff",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&shipctrlmenu_p_5},
		.param=NULL
	}
};

const struct menu_item_t shipctrlmenu_0[4]={
	{
		.desc="On",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_LED_ON
	},
	{
		.desc="Off",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_LED_OFF
	},
	{
		.desc="Flash",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_LED_FLASH
	},
	{
		.desc="Heartbeat",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_LED_HEARTBEAT
	}
};

const struct menu_item_t shipctrlmenu_1[3]={
	{
		.desc="Power On",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_GPSD_ON
	},
	{
		.desc="Power Off",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_GPSD_OFF
	},
	{
		.desc="Frequency",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=NULL},
		.param=NULL
	}
};

const struct menu_item_t shipctrlmenu_2[3]={
	{
		.desc="Tx On",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_LORA_ON
	},
	{
		.desc="Tx Off",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_LORA_OFF
	},
	{
		.desc="Interval",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=NULL},
		.param=NULL
	}
};

const struct menu_item_t shipctrlmenu_3[3]={
	{
		.desc="Power On",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_DISP_ON
	},
	{
		.desc="Power Off",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_DISP_OFF
	},
	{
		.desc="Content",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=NULL},
		.param=NULL
	}
};

const struct menu_item_t shipctrlmenu_4[1]={
	{
		.desc="Confirm",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_IGNITION
	}
};

const struct menu_item_t shipctrlmenu_5[1]={
	{
		.desc="Confirm",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&func_tracker_shipctrl_sendcommand},
		.param=(void*)LORAGPS_CTRL_POWR_OFF
	}
};

void* func_tracker_shipctrl_sendcommand(const void* param) {
	struct packet_frame_t pk_comm;
	pk_comm.magicnum=LORAGPS_HHLD_HEAD;
	pk_comm.pid=0;
	pk_comm.cid=LORAGPS_HANDHELDID;
//	memcpy(&(pk_comm.sig), param, sizeof(uint8_t)+sizeof(struct packet_ctrl_t));
// Hacky copy. Possible only with little endian
	memcpy(&(pk_comm.sig), &param, sizeof(void*));
	LoRa.transmit((uint8_t *)&pk_comm, packet_header_size+sizeof(struct packet_ctrl_t),0,LoRa_TXpower,WAIT_TX);
	return NULL;
}

// tracker maintain shipdb
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
		if (PacketLen && pk_probe.magicnum==LORAGPS_TRCK_HEAD) {
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
				case LORAGPS_INFO_TIME:
					memcpy(&(ship->ship_time),pk_probe.fields,sizeof(struct packet_time_t));
					break;
				case LORAGPS_INFO_CORD:
					memcpy(&(ship->ship_cord),pk_probe.fields,sizeof(struct packet_cord_t));
					break;
				case LORAGPS_INFO_MOTN:
					memcpy(&(ship->ship_motn),pk_probe.fields,sizeof(struct packet_motn_t));
					break;
				case LORAGPS_INFO_STAT:
					memcpy(&(ship->ship_stat),pk_probe.fields,sizeof(struct packet_stat_t));
					break;
				case LORAGPS_INFO_ACCR:
					memcpy(&(ship->ship_accr),pk_probe.fields,sizeof(struct packet_accr_t));
					break;
				case LORAGPS_INFO_POSE:
					memcpy(&(ship->ship_pose),pk_probe.fields,sizeof(struct packet_pose_t));
					break;
				default:
					break;
			}
		}
	}
}

// tracker display shipinfo
void func_tracker_shipinfo(struct ship_data_t *ship) {
	bool runflag=1;
	uint8_t page=0;
	enum JOY_DISCRETE joy;
	int heading_ccw;
	while (runflag) {
		disp.clearDisplay();
		disp.setTextSize(1);
		if (page==0) {
			//disp.setCursor(6, 0);
			//sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			//disp.write(buffer);
			disp.setCursor(0, 0);
			sprintf(buffer,"%02d:%02d:%02d",GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(6*15, 0);
			sprintf(buffer,"%.3fV",ship->ship_stat.vbat);
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
			sprintf(buffer,"SAT: %d",ship->ship_stat.SAT);
			disp.write(buffer);
			disp.setCursor(6*11, 48);
			sprintf(buffer,"HDOP: %.*f",ship->ship_stat.HDOP>99?0:ship->ship_stat.HDOP>9?1:2,ship->ship_stat.HDOP);
			disp.write(buffer);
			disp.setCursor(0, 56);
			sprintf(buffer,"COMP: %d",ship->ship_motn.compass);
			disp.write(buffer);
			disp.setCursor(6*11, 56);
			sprintf(buffer,"PKT: %d",PacketID);
			disp.write(buffer);
		}
		else if (page==1) {
			//disp.setCursor(6, 0);
			//sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			//disp.write(buffer);
			disp.setCursor(0, 0);
			sprintf(buffer,"%02d:%02d:%02d",GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(6*15, 0);
			sprintf(buffer,"%.3fV",ship->ship_stat.vbat);
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
			sprintf(buffer,"COMP: %d",ship->ship_motn.compass);
			disp.write(buffer);
			disp.setCursor(6*11, 56);
			sprintf(buffer,"PKT: %d",PacketID);
			disp.write(buffer);
		}
		else if (page==2) {
			heading_ccw=(compass.readHeading()+compass_offset)%360;
			disp.setCursor(0, 0);
			sprintf(buffer,"%02d:%02d:%02d",GPS.time.hour(),GPS.time.minute(),GPS.time.second());
			disp.write(buffer);
			disp.setCursor(0, 8);
			sprintf(buffer,"%9.6f%c",abs(ship->ship_cord.latitude),ship->ship_cord.latitude<0?'S':'N');
			disp.write(buffer);
			disp.setCursor(0, 16);
			if (abs(GPS.location.lng())>=100) sprintf(buffer,"%9.5f%c",abs(ship->ship_cord.longitude),ship->ship_cord.longitude<0?'W':'E');
			else sprintf(buffer,"%9.6f%c",abs(ship->ship_cord.longitude),ship->ship_cord.longitude<0?'W':'E');
			disp.write(buffer);
			disp.setCursor(0, 24);
			sprintf(buffer,"%.2fm",ship->ship_cord.altitude);
			disp.write(buffer);
			disp.setCursor(0, 32);
			sprintf(buffer,"%.2f%c",GPS.courseTo(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude),248);
			disp.write(buffer);
			disp.setCursor(0, 40);
			sprintf(buffer,"DIST: %.1f",GPS.distanceBetween(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude));
			disp.write(buffer);
			disp.setCursor(0, 48);
			sprintf(buffer,"RSSI: %d",PacketRSSI);
			disp.write(buffer);
			disp.setCursor(0, 56);
			sprintf(buffer,"PKT: %d",PacketID);
			disp.write(buffer);
			compass_draw_graph(96,32,30,heading_ccw);
			compass_draw_arrow(96,32,18,heading_ccw-compass_course_clkflip(GPS.courseTo(GPS.location.lat(),GPS.location.lng(),ship->ship_cord.latitude,ship->ship_cord.longitude)));
		}
//		else if (page==1) {
//			disp.setCursor(6, 0);
//			sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",GPS.date.year(),GPS.date.month(),GPS.date.day(),GPS.time.hour(),GPS.time.minute(),GPS.time.second());
//			disp.write(buffer);
//			disp.setCursor(0, 8);
//			sprintf(buffer,"Accel/Gyro:");
//			disp.write(buffer);
//			disp.setCursor(0,16);
//			sprintf(buffer,"%7.2f",ship->ship_accr.acc_x);
//			disp.write(buffer);
//			disp.setCursor(6*7,16);
//			sprintf(buffer,"%7.2f",ship->ship_accr.acc_y);
//			disp.write(buffer);
//			disp.setCursor(6*14,16);
//			sprintf(buffer,"%7.2f",ship->ship_accr.acc_z);
//			disp.write(buffer);
//			disp.setCursor(0,24);
//			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_x);
//			disp.write(buffer);
//			disp.setCursor(6*7,24);
//			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_y);
//			disp.write(buffer);
//			disp.setCursor(6*14,24);
//			sprintf(buffer,"%7.2f",ship->ship_accr.gyro_z);
//			disp.write(buffer);
//			disp.setCursor(0,32);
//			sprintf(buffer,"Orientation:");
//			disp.write(buffer);
//			disp.setCursor(0,40);
//			sprintf(buffer,"%7.2f",ship->ship_pose.agl_x);
//			disp.write(buffer);
//			disp.setCursor(6*7,40);
//			sprintf(buffer,"%7.2f",ship->ship_pose.agl_y);
//			disp.write(buffer);
//			disp.setCursor(6*14,40);
//			sprintf(buffer,"%7.2f",ship->ship_pose.agl_z);
//			disp.write(buffer);
//			disp.setCursor(0,48);
//			sprintf(buffer,"%7.2f",ship->ship_pose.pitch);
//			disp.write(buffer);
//			disp.setCursor(6*7,48);
//			sprintf(buffer,"%7.2f",ship->ship_pose.roll);
//			disp.write(buffer);
//			disp.setCursor(6*14,48);
//			sprintf(buffer,"%7.2f",ship->ship_pose.yaw);
//			disp.write(buffer);
//			disp.setCursor(0,56);
//			sprintf(buffer,"Temperature: %.2f%cC",ship->ship_pose.temperature,248);
//			disp.write(buffer);
//		}
		disp.display();
		joy=joy_read(0);
		switch (joy) {
			case U:
				if (page>0) page--;
				break;
			case D:
				if (page<2) page++;
				break;
			case L:
				runflag=0;
				break;
			case R:
				menu_exec(&shipctrlmenu_p);
				break;
			default:
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
	enum JOY_DISCRETE joy;
	bool runflag=1;
	while (runflag) {
		if (!ship && shiplist->ship) {
			ship=shiplist->ship;
		}
		func_tracker_shiplist_render(ship, pos, 2);
		joy=joy_read(0);
		switch (joy) {
			case U:
				if (ship && ship->prev) {
					ship=ship->prev;
					if (pos>1 || (pos==1 && !ship->prev)) pos--;
				}
				break;
			case D:
				if (ship && ship->next) {
					ship=ship->next;
					if (pos<2 || (pos==2 && !ship->next)) pos++;
				}
				break;
			case L:
				runflag=0;
				break;
			case R:
				if (ship) func_tracker_shipinfo(ship);
				break;
			case B:
				uint8_t sig[sizeof(uint8_t)+sizeof(struct packet_ctrl_t)];
				sig[0]=LORAGPS_CTRL_LORA_OFF;
				func_tracker_shipctrl_sendcommand(&sig);
				delay(100);
				sig[0]=LORAGPS_CTRL_LORA_ON;
				func_tracker_shipctrl_sendcommand(&sig);
				break;
			default:
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

void *app_tracker(const void *param) {
	TaskHandle_t Task_LoRa;
	struct ship_list_t *shiplist=(struct ship_list_t *)malloc(sizeof(struct ship_list_t));
	shiplist->ship=NULL;
	xTaskCreatePinnedToCore(func_tracker_shiplist_update,"Ship Scan",100000,shiplist,0,&Task_LoRa,0);
	func_tracker_selship(shiplist);
	vTaskDelete(Task_LoRa);
	func_tracker_destroy_shiplist(shiplist);
	return NULL;
}
