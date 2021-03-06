/*
   NTSensors - Firmware
   Copyright (C) 2009:
         Daniel Roggen, droggen@gmail.com

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef __MAIN_H
#define __MAIN_H




/*
	Device specific elements here.
*/


#include <stdio.h>
#include "command.h"



/*
	Firmware state. 
	
	Power-up default is IDLE
	In IDLE: upon BT connection go to STREAMING
	In STREAMING: upon BT disconnection go to IDLE
*/
#define FIRMWARE_IDLE 0
#define FIRMWARE_STREAMING 1
#define FIRMWARE_LOGGING 2
#define FIRMWARE_DOWNLOADING 3
extern unsigned char firmware_state;

typedef struct {
	unsigned char valid;
	unsigned char offh,offm,offs,offday,offmonth,offyear;
	unsigned char onh,onm,ons,onday,onmonth,onyear;
	unsigned long offcharge,oncharge;
	unsigned short offvoltage,onvoltage;
	unsigned long offtime,ontime;
	char str[5][64];
} _POWERUSE_OFF;
extern _POWERUSE_OFF _poweruse_off;

extern FILE *file_bt,*file_usb,*file_fb,*file_dbg,*file_pri;
extern unsigned char system_devname[];

#define CONFIG_ADDR_ENABLE_ID 0
#define CONFIG_ADDR_ENABLE_TIMESTAMP 1
#define CONFIG_ADDR_ENABLE_BATTERY 2
#define CONFIG_ADDR_ENABLE_ACCELERATION 3
#define CONFIG_ADDR_ENABLE_GYROSCOPE 4
#define CONFIG_ADDR_ENABLE_CHECKSUM 5
#define CONFIG_ADDR_DATA_FORMAT 6
//#define CONFIG_ADDR_SENSORSR 10
#define CONFIG_ADDR_STREAM_FORMAT 11
#define CONFIG_ADDR_ENABLE_TEMPERATURE 12
#define CONFIG_ADDR_STREAM_BINARY 13
//#define CONFIG_ADDR_ADC_MASK 14
//#define CONFIG_ADDR_ADC_PERIOD0 15
//#define CONFIG_ADDR_ADC_PERIOD1 16
//#define CONFIG_ADDR_ADC_PERIOD2 17
//#define CONFIG_ADDR_ADC_PERIOD3 18
#define CONFIG_ADDR_ENABLE_LCD 19
#define CONFIG_ADDR_TS_PERIOD0 20
#define CONFIG_ADDR_TS_PERIOD1 21
#define CONFIG_ADDR_TS_PERIOD2 22
#define CONFIG_ADDR_TS_PERIOD3 23
#define CONFIG_ADDR_STREAM_LABEL 24
#define CONFIG_ADDR_STREAM_PKTCTR 25
#define CONFIG_ADDR_ENABLE_INFO 26



// Boot script - reserve 100 bytes
#define CONFIG_ADDR_SCRIPTSTART 100
#define CONFIG_ADDR_SCRIPTLEN COMMANDMAXSIZE
#define CONFIG_ADDR_SCRIPTEND (CONFIG_ADDR_SCRIPTSTART+CONFIG_ADDR_SCRIPTLEN-1)

// Storing number of boots
#define STATUS_ADDR_NUMBOOT0	300
#define STATUS_ADDR_NUMBOOT1	301
#define STATUS_ADDR_NUMBOOT2	302
#define STATUS_ADDR_NUMBOOT3	303

// Storing charge status upon soft off
#define STATUS_ADDR_OFFCURRENT_VALID 500
#define STATUS_ADDR_OFFCURRENT_CHARGE0 501
#define STATUS_ADDR_OFFCURRENT_CHARGE1 502
#define STATUS_ADDR_OFFCURRENT_CHARGE2 503
#define STATUS_ADDR_OFFCURRENT_CHARGE3 504
#define STATUS_ADDR_OFFCURRENT_VOLTAGE0 505
#define STATUS_ADDR_OFFCURRENT_VOLTAGE1 506
#define STATUS_ADDR_OFFCURRENT_H 510
#define STATUS_ADDR_OFFCURRENT_M 511
#define STATUS_ADDR_OFFCURRENT_S 512
#define STATUS_ADDR_OFFCURRENT_DAY 513
#define STATUS_ADDR_OFFCURRENT_MONTH 514
#define STATUS_ADDR_OFFCURRENT_YEAR 515
#define STATUS_ADDR_OFFCURRENT_TIME 516
#define STATUS_ADDR_OFFCURRENT_TIME_1 517
#define STATUS_ADDR_OFFCURRENT_TIME_2 518
#define STATUS_ADDR_OFFCURRENT_TIME_3 519

// MPU related settings - MPU requires ~50 bytes of non-volatile storage
#define CONFIG_ADDR_MPU_SETTINGS 600


extern unsigned char config_enable_id,config_enable_acceleration,config_enable_gyroscope,config_enable_checksum,config_data_format;
extern unsigned char config_sensorsr;
extern volatile unsigned long motion_int_ctr, motion_int_ctrds;
extern unsigned char do_packet;

extern unsigned long int system_perf;




extern unsigned char sharedbuffer[];
extern unsigned char system_mode;

// System status
extern volatile signed short system_temperature;
extern volatile unsigned long int system_temperature_time;

signed short system_gettemperature(void);
unsigned long system_gettemperaturetime(void);

unsigned char system_sampletemperature(unsigned char p);
void system_sampletemperature_cb(unsigned char s,signed short t);



unsigned char bootloaderhook_dbg(unsigned char c);
unsigned char bootloaderhook_bluetooth(unsigned char c);


extern volatile unsigned char bluetoothrts;


/**
	\brief Clears statistics about motion sampling
	
	Resets all statistics related to motion sampling
**/
void main_samplestatclear(void);

unsigned short Battery_ADC2mv(unsigned short adc);
void isr_motionint_sample(void);
char ui_shouldexit(FILE *file);
unsigned long main_perfbench(unsigned long mintime);



#endif
