#include "resc_lora.hpp"

uint32_t LoRa_Freq = 915000000;				//frequency of transmissions in hertz
uint32_t LoRa_Offset = 0;					//offset frequency for calibration purposes
uint8_t LoRa_Bandwidth = LORA_BW_250;		//LoRa bandwidth
uint8_t LoRa_SpreadingFactor = LORA_SF11;	//LoRa spreading factor
uint8_t LoRa_CodeRate = LORA_CR_4_8;		//LoRa coding rate
uint8_t LoRa_Optimisation = LDRO_AUTO;		//low data rate optimisation setting, normally set to auto
int8_t LoRa_TXpower = 22;					//LoRa transmit power in dBm;for SX1262, SX1268 power range is +22dBm to -9dBm

SX126XLT LoRa;
int8_t PacketRSSI;
int8_t PacketSNR;
uint8_t PacketLen;
uint8_t PacketID;
uint8_t *PacketBuffer[255];

const struct menu_page_t loraconfmenu_p={8,loraconfmenu,NULL,0};
const struct menu_page_t loraconfmenu_p_bandwidth={10,loraconfmenu_bandwidth,&loraconf_menumark,LORA_CONF_BANDWIDTH};
const struct menu_page_t loraconfmenu_p_sf={8,loraconfmenu_sf,&loraconf_menumark,LORA_CONF_SPREADINGFACT};
const struct menu_page_t loraconfmenu_p_cr={4,loraconfmenu_cr,&loraconf_menumark,LORA_CONF_CODERATE};
const struct menu_page_t loraconfmenu_p_optim={3,loraconfmenu_optim,&loraconf_menumark,LORA_CONF_OPTIM};
const struct menu_page_t loraconfmenu_p_presets={8,loraconfmenu_presets,&loraconf_menumark,LORA_CONF_PRESETS};

const struct menu_item_t loraconfmenu[8]={
	{
		.desc="Frequency",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)LORA_CONF_FREQ
	},
	{
		.desc="Offset",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)LORA_CONF_OFFSET
	},
	{
		.desc="Bandwidth",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_bandwidth},
		.param=NULL
	},
	{
		.desc="SpreadingF",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_sf},
		.param=NULL
	},
	{
		.desc="CodingRate",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_cr},
		.param=NULL
	},
	{
		.desc="Optimise",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_optim},
		.param=NULL
	},
	{
		.desc="Tx Power",
		.enter_behavior=1,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)LORA_CONF_TXPOWER
	},
	{
		.desc="Presets",
		.enter_behavior=0,
		.drop_menu=0,
		.routine={.page=&loraconfmenu_p_presets},
		.param=NULL
	}
};

const struct menu_item_t loraconfmenu_bandwidth[10]={
	{
		.desc="7810Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_007<<8))
	},
	{
		.desc="10420Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_010<<8))
	},
	{
		.desc="15630Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_015<<8))
	},
	{
		.desc="20830Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_020<<8))
	},
	{
		.desc="31250Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_031<<8))
	},
	{
		.desc="41670Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_041<<8))
	},
	{
		.desc="62500Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_062<<8))
	},
	{
		.desc="125000Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_125<<8))
	},
	{
		.desc="250000Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_250<<8))
	},
	{
		.desc="500000Hz",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_BANDWIDTH+(LORA_BW_500<<8))
	}
};

const struct menu_item_t loraconfmenu_sf[8]={
	{
		.desc="2^5",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF5<<8))
	},
	{
		.desc="2^6",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF6<<8))
	},
	{
		.desc="2^7",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF7<<8))
	},
	{
		.desc="2^8",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF8<<8))
	},
	{
		.desc="2^9",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF9<<8))
	},
	{
		.desc="2^10",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF10<<8))
	},
	{
		.desc="2^11",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF11<<8))
	},
	{
		.desc="2^12",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_SPREADINGFACT+(LORA_SF12<<8))
	}
};

const struct menu_item_t loraconfmenu_cr[4]={
	{
		.desc="4/5",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_5<<8))
	},
	{
		.desc="4/6",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_6<<8))
	},
	{
		.desc="4/7",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_7<<8))
	},
	{
		.desc="4/8",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_CODERATE+(LORA_CR_4_8<<8))
	}
};

const struct menu_item_t loraconfmenu_optim[3]={
	{
		.desc="Off",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_OPTIM+(LDRO_OFF<<8))
	},
	{
		.desc="On",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_OPTIM+(LDRO_ON<<8))
	},
	{
		.desc="Auto",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_OPTIM+(LDRO_AUTO<<8))
	}
};

const struct menu_item_t loraconfmenu_presets[8]={
	{
		.desc="ShortFast",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_SHORT_FAST<<8))
	},
	{
		.desc="ShortSlow",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_SHORT_SLOW<<8))
	},
	{
		.desc="MedmFast",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_MEDIUM_FAST<<8))
	},
	{
		.desc="MedmSlow",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_MEDIUM_SLOW<<8))
	},
	{
		.desc="LongFast",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_LONG_FAST<<8))
	},
	{
		.desc="LongMedm",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_LONG_MODERATE<<8))
	},
	{
		.desc="LongSlow",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_LONG_SLOW<<8))
	},
	{
		.desc="VeryLong",
		.enter_behavior=2,
		.drop_menu=0,
		.routine={.func=&loraconf_config},
		.param=(void*)(LORA_CONF_PRESETS+(LORA_CONF_PRESET_VERYLONG_SLOW<<8))
	}
};

uint16_t DEVID;

uint8_t lora_init() {
	SPI.begin(SCK_LoRa,MISO_LoRa,MOSI_LoRa,SS_LoRa);
	while (!LoRa.begin(SS_LoRa,RST_LoRa,BUSY_LoRa,DIO1_LoRa,SW_LoRa,DEVICE_SX1262)) delay(2000);
	LoRa.setupLoRa(LoRa_Freq, LoRa_Offset, LoRa_SpreadingFactor, LoRa_Bandwidth, LoRa_CodeRate, LoRa_Optimisation);
	//uint8_t mac[8];
	//esp_read_mac(mac,ESP_MAC_BT);
	DEVID=0xAB12;
	disp.write(">>> LoRa online\n");
	disp.display();
	return 0;
}

void* loraconf_enter(const void *param) {
	menu_exec(&loraconfmenu_p);
	// Apply settings after the configuration menu returns
	LoRa.setupLoRa(LoRa_Freq, LoRa_Offset, LoRa_SpreadingFactor, LoRa_Bandwidth, LoRa_CodeRate, LoRa_Optimisation);
	return NULL;
}

int8_t loraconf_menumark(const uint8_t param) {
	if (param==LORA_CONF_BANDWIDTH) {
		switch (LoRa_Bandwidth) {
			case LORA_BW_007:
				return 0;
			case LORA_BW_010:
				return 1;
			case LORA_BW_015:
				return 2;
			case LORA_BW_020:
				return 3;
			case LORA_BW_031:
				return 4;
			case LORA_BW_041:
				return 5;
			case LORA_BW_062:
				return 6;
			case LORA_BW_125:
				return 7;
			case LORA_BW_250:
				return 8;
			case LORA_BW_500:
				return 9;
		}
	}
	else if (param==LORA_CONF_SPREADINGFACT) {
		switch (LoRa_SpreadingFactor) {
			case LORA_SF5:
				return 0;
			case LORA_SF6:
				return 1;
			case LORA_SF7:
				return 2;
			case LORA_SF8:
				return 3;
			case LORA_SF9:
				return 4;
			case LORA_SF10:
				return 5;
			case LORA_SF11:
				return 6;
			case LORA_SF12:
				return 7;
		}
	}
	else if (param==LORA_CONF_CODERATE) {
		switch (LoRa_CodeRate) {
			case LORA_CR_4_5:
				return 0;
			case LORA_CR_4_6:
				return 1;
			case LORA_CR_4_7:
				return 2;
			case LORA_CR_4_8:
				return 3;
		}
	}
	else if (param==LORA_CONF_OPTIM) {
		switch (LoRa_Optimisation) {
			case LDRO_OFF:
				return 0;
			case LDRO_ON:
				return 1;
			case LDRO_AUTO:
				return 2;
		}
	}
	else if (param==LORA_CONF_PRESETS)
		if (LoRa_CodeRate==LORA_CR_4_8) {
			if (LoRa_SpreadingFactor==LORA_SF7 && LoRa_Bandwidth==LORA_BW_500) return 0;
			else if (LoRa_SpreadingFactor==LORA_SF8 && LoRa_Bandwidth==LORA_BW_500) return 1;
			else if (LoRa_SpreadingFactor==LORA_SF9 && LoRa_Bandwidth==LORA_BW_250) return 2;
			else if (LoRa_SpreadingFactor==LORA_SF10 && LoRa_Bandwidth==LORA_BW_250) return 3;
			else if (LoRa_SpreadingFactor==LORA_SF11 && LoRa_Bandwidth==LORA_BW_250) return 4;
			else if (LoRa_SpreadingFactor==LORA_SF11 && LoRa_Bandwidth==LORA_BW_125) return 5;
			else if (LoRa_SpreadingFactor==LORA_SF12 && LoRa_Bandwidth==LORA_BW_125) return 6;
			else if (LoRa_SpreadingFactor==LORA_SF12 && LoRa_Bandwidth==LORA_BW_062) return 7;
		}
	return -1;
}

void* loraconf_config(const void* param) {
	if ((uint32_t)param==LORA_CONF_FREQ) LoRa_Freq=(uint32_t)(menu_numinput(3,3,LoRa_Freq/1000,"MHz")*1000);
	else if ((uint32_t)param==LORA_CONF_OFFSET) LoRa_Offset=(uint32_t)(menu_numinput(3,0,LoRa_Offset,"Hz"));
	else if (((uint8_t*)(&param))[0]==LORA_CONF_BANDWIDTH) LoRa_Bandwidth=((uint8_t*)(&param))[1];
	else if (((uint8_t*)(&param))[0]==LORA_CONF_SPREADINGFACT) LoRa_SpreadingFactor=((uint8_t*)(&param))[1];
	else if (((uint8_t*)(&param))[0]==LORA_CONF_CODERATE) LoRa_CodeRate=((uint8_t*)(&param))[1];
	else if (((uint8_t*)(&param))[0]==LORA_CONF_OPTIM) LoRa_Optimisation=((uint8_t*)(&param))[1];
	else if ((uint32_t)param==LORA_CONF_TXPOWER) LoRa_TXpower=menu_rangeinput(-9,22,1,LoRa_TXpower,"dBm");
	else if (((uint8_t*)(&param))[0]==LORA_CONF_PRESETS) {
		switch (((uint8_t*)(&param))[1]) {
			case LORA_CONF_PRESET_SHORT_FAST:
				LoRa_Bandwidth=LORA_BW_500;
				LoRa_SpreadingFactor=LORA_SF7;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_SHORT_SLOW:
				LoRa_Bandwidth=LORA_BW_500;
				LoRa_SpreadingFactor=LORA_SF8;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_MEDIUM_FAST:
				LoRa_Bandwidth=LORA_BW_250;
				LoRa_SpreadingFactor=LORA_SF9;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_MEDIUM_SLOW:
				LoRa_Bandwidth=LORA_BW_250;
				LoRa_SpreadingFactor=LORA_SF10;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_LONG_FAST:
			default:
				LoRa_Bandwidth=LORA_BW_250;
				LoRa_SpreadingFactor=LORA_SF11;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_LONG_MODERATE:
				LoRa_Bandwidth=LORA_BW_125;
				LoRa_SpreadingFactor=LORA_SF11;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_LONG_SLOW:
				LoRa_Bandwidth=LORA_BW_125;
				LoRa_SpreadingFactor=LORA_SF12;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
			case LORA_CONF_PRESET_VERYLONG_SLOW:
				LoRa_Bandwidth=LORA_BW_062;
				LoRa_SpreadingFactor=LORA_SF12;
				LoRa_CodeRate=LORA_CR_4_8;
				break;
		}
	}
	return NULL;
}