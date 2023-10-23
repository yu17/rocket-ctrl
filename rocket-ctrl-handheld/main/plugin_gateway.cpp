#include "plugin_gateway.hpp"

int8_t plugin_gateway_request_weather(struct contact_t *device, struct payload_wethrpt_t *wethrpt) {
	func_animation_hline(64-8,8,11000,0,ANIME_START);
	struct packet_frame_t packet;
	packet.device_type=LORACOMM_DEVTYP_HANDHELD;
	packet.source_id=DEVID;
	packet.target_id=device->id;
	packet.packet_type=LORACOMM_PKTTYP_REQ;
	packet.payload[0]=LORACOMM_PKTTYP_WETHRPT;
	LoRa.transmitReliable((uint8_t*)&packet, LORACOMM_FRAME_LEN+sizeof(packet.packet_type)+2, LORACOMM_FISHNET, 0, LoRa_TXpower, WAIT_TX);
	uint8_t timeout=4;
	while(!device->lastpkt_updated && timeout) {
		delay(3000);
		timeout--;
	}
	func_animation_hline(64-8,8,15000,0,ANIME_INTERRUPT);
	// Clear bottom screen
	//for (uint8_t i=64-8;i<64;i++) disp.drawFastHLine(0,i,128,SSD1306_BLACK);
	disp.fillRect(0,64-8,DISPLAY_WIDTH,8,SSD1306_BLACK);
	disp.display();
	if (!device->lastpkt_updated) return -1;
	device->lastpkt_updated=0;
	memcpy(wethrpt,device->data,sizeof(struct payload_wethrpt_t));
	return 0;
}

void plugin_gateway_display_weather(struct contact_t *device) {
	enum JOY_DISCRETE joy;
	bool force_update=0;
	struct payload_wethrpt_t wethrpt;
	disp.setTextSize(1);//21 characters per line
	uint16_t TICK=0;
	disp.clearDisplay();
	disp.display();
	while (1) {
		if (!(TICK%6000) || force_update) {
			force_update=0;
			// Print "Updating" at the bottom
			disp.fillRect(0,64-8,DISPLAY_WIDTH,8,SSD1306_BLACK);
			disp.setCursor((128-11*6)/2,64-8);
			disp.write("Updating...");
			disp.display();
			if (plugin_gateway_request_weather(device,&wethrpt)) {
				disp.setCursor(128-11*6, 64-8);
				disp.write("Unreachable");
				disp.display();
			}
			else {
				//func_animation_hline(128-8,8,11000,0,ANIME_INTERRUPT);
				disp.clearDisplay();
				disp.setCursor(6, 0);
				sprintf(buffer,"Temp  = %.3f%cC",wethrpt.temperature,248);
				disp.write(buffer);
				disp.setCursor(6, 8);
				sprintf(buffer,"Pres  = %.2fhPa",wethrpt.pressure/100.0);
				disp.write(buffer);
				disp.setCursor(6, 16);
				sprintf(buffer,"Humid = %.3f%%",wethrpt.humidity);
				disp.write(buffer);
				disp.setCursor(6, 24);
				sprintf(buffer,"Gas = %d",wethrpt.gas_r);
				disp.write(buffer);
				disp.setCursor(6, 32);
				sprintf(buffer,"Est.IAQ = %.3f",log(wethrpt.gas_r)+0.04*wethrpt.humidity);
				disp.write(buffer);
				disp.setCursor(0,40);
				sprintf(buffer,"SNR = %d",device->lastpkt_SNR);
				disp.write(buffer);
				disp.setCursor(6*11,40);
				sprintf(buffer,"RSSI = %d",device->lastpkt_RSSI);
				disp.write(buffer);
				disp.display();
			}
		}
		joy=joy_read(0);
		switch (joy) {
			case U:
				break;
			case D:
				break;
			case L:
				return;
			case B:
				force_update=1;
				break;
			default:
				break;
		}
		TICK++;
		delay(TICKINT);
	}
	return;
}

void plugin_gateway_main(struct contact_t *device) {
	device->lastpkt_updated=0;
	plugin_gateway_display_weather(device);
}