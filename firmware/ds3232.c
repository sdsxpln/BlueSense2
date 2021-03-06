#include "cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>
#include <string.h>

#include "ds3232.h"
#include "i2c.h"
#include "i2c_int.h"
#include "main.h"
#include "helper.h"
#include "system.h"

/*
	file: ds3232
	
	DS3232 RTC functions
	
	The key functions are:
	
		* ds3232_init:		Initialisation of the DS3232
		* ds3232_readtimeregisters:	Read the DS3232 date/time registers immediately (registers 0-6).
		
	*DS3232 specifics*
	
	The DS3232 second counter is incremented at the same time as the falling edge of INT#/SQW pin.
	
	Upon writing the time with ds3232_writetime the INT#/SQW ping goes low (if it is already low it remains low), and
	the one second square wave starts from the moment of the time write.
	
	*Usage in interrupts*
	
	??
	
*/


/******************************************************************************
	function: ds3232_init
*******************************************************************************	
	Initialisation of the DS3232 with the following:
	- 1 Hz square wave on INTC#/SWQ pin when powered on; disabled (hiZ) when powered off (VBat only)
	- 32 KHz pin disabled when powered on and powered off
	
	Parameters:
		-
	Returns:
		-

******************************************************************************/
void ds3232_init(void)
{
	unsigned char r __attribute__((unused));
	fprintf_P(file_pri,PSTR("DS3232 init... "));
	//fprintf(file_pri,"Before write control\n");
	//_delay_ms(500);
	r = ds3232_write_control(0b00011000);			// Enable oscillator, disable battery backed 1Hz SQW, enable normal square wave 1Hz
	//printf("write control: %02Xh\n",r);
	//fprintf(file_pri,"Before write status\n");
	//_delay_ms(500);
	r = ds3232_write_status(0b00000000);			// Clear stopped flag, clear alarm flag, disable battery-backed 32KHz, disable normal 32KHz
	//printf("write status: %02Xh\n",r);
	fprintf_P(file_pri,PSTR("done\n"));
}


/******************************************************************************
	function: ds3232_readtimeregisters
*******************************************************************************	
	Read the DS3232 date/time registers immediately (registers 0-6).
	
	Parameters:
		time		-	Pointer to a buffer of 7 bytes which will hold the raw time registers
	
	Returns:
		0			-	Success
		1			-	Error
******************************************************************************/
unsigned char ds3232_readtimeregisters(unsigned char *regs)
{
	unsigned char r;
	
	// Perform read.
	r = i2c_readregs(DS3232_ADDRESS,0,7,regs);
	if(r!=0)
	{
		memset(regs,0,7);
	}
	return 0;
}

/*


	Returns the time from DS3232 at the instant the second register changes.
	This can be used to initialise local timers.
	
	time:				7 bytes long buffer that will receive the first 7 time register of DS3232
	
	Return:			zero in case of success, nonzero for i2c error
*/
/*unsigned char ds3232_readtime_sync(unsigned char *time)
{
	unsigned char r;
	unsigned char val1[7];
	
	// Perform initial read.
	r = i2c_readregs(DS3232_ADDRESS,0,7,val1);
	//printf("%d\n",r);
	if(r!=0x00)
		return r;
	//for(int i=0;i<7;i++)
		//printf("%02X ",val1[i]);
			
	// Continuous read until second change
	do
	{
		r = i2c_readregs(DS3232_ADDRESS,0,7,time);
		//printf("%d\n",r);
		if(r!=0x00)
			return r;
		//for(int i=0;i<7;i++)
			//printf("%02X ",time[i]);
	} 
	while(val1[0] == time[0]);
	return 0;
}*/
/*
	Returns the time from DS3232 at the instant the second register changes.
	This can be used to initialise local timers.
	
	This function synchronizes using the square wave signal, which is assumed
	to be handled by an interrupt handler that increments a 
	variable called rtc_time_sec.
	
	time:				7 bytes long buffer that will receive the first 7 time register of DS3232
	
	Return:			zero in case of success, nonzero for i2c error
*/
/*extern unsigned long rtc_time_sec;
unsigned char ds3232_readtime_sqwsync(unsigned char *time)
{
	unsigned char r;
	
	unsigned long st;
	
	// Check current time
	st = rtc_time_sec;
	// Loop until time changes
	while(rtc_time_sec==st);
	// Read the time	
	r = i2c_readregs(DS3232_ADDRESS,0,7,time);
	return r;
}*/





/******************************************************************************
	ds3232_readtime_conv
*******************************************************************************	
	
	Return value:
		0:			success
		1:			error
******************************************************************************/
/*unsigned char ds3232_readtime_conv(unsigned char *hour,unsigned char *min,unsigned char *sec)
{
	unsigned char val[7];
	unsigned char r;
	
	r = ds3232_readtime(val);
	if(r!=0)
		return r;
		
	ds3232_convtime(val,hour,min,sec);

	return 0;
}*/



/******************************************************************************
	function: ds3232_sync_fall
*******************************************************************************	
	Waits until a falling edge of the DS3232 SQW pin. 
	
	The DS3232 time registers are updated on the falling edge.
	
	Waiting for a falling edge leaves 1 second to read the time info before the 
	time registers change again.
	
	Parameters:
		-

	Returns:
		-

******************************************************************************/
void ds3232_sync_fall(void)
{
	unsigned char pc,pn;		// pc: current pin state, pn: new pin state
	// Wait for the falling edge of the RTC int
	pc=system_getrtcint();
	while( !( ((pn=system_getrtcint())!=pc) && pn==0) )		// Loop until pn!=pc and pn==0
		pc=pn;
}
/******************************************************************************
	function: ds3232_sync_rise
*******************************************************************************	
	Waits until a rising edge of the DS3232 SQW pin. 
	
	The DS3232 time registers are updated on the falling edge.
	
	Waiting for a rising edge leaves 500ms to read the time info before the time
	registers change.
			
	Parameters:
		-

	Returns:
		-

******************************************************************************/
void ds3232_sync_rise(void)
{
	unsigned char pc,pn;		// pc: current pin state, pn: new pin state
	// Wait for the rising edge of the RTC int
	pc=system_getrtcint();
	while( !( ((pn=system_getrtcint())!=pc) && pn==1) )		// Loop until pn!=pc and pn==1
		pc=pn;
	return;
}

/******************************************************************************
	function: ds3232_readtime_conv_int
*******************************************************************************	
	Returns the DS3232 time in hh:mm.ss format, optionally with synchronising return.
	
	In synchronised mode (sync=1), this function waits for a change of second and then reads and returns the date/time.
	In immediate mode (sync=0) this function immediately reads and returns the date/time.
	
	Use the synchronised mode when the precise moment at which the time is read is important. This
	mode introduces a latency of up to one second, as it must wait for a second change before returning.
	
	If a pointer to the variables receiving the return values is null that value is not returned.
		
	Parameters:
		sync		-	0: to read and return the time immediately, 1 to read and return the time at a change of seconds.
		hour		-	Pointer to the variable holding the returned hours (pass 0 if not needed)
		min			-	Pointer to the variable holding the returned min (pass 0 if not needed)
		sec			-	Pointer to the variable holding the returned sec (pass 0 if not needed)
		day			-	Pointer to the variable holding the returned day (pass 0 if not needed)
		month		-	Pointer to the variable holding the returned month (pass 0 if not needed)
		year		-	Pointer to the variable holding the returned year (pass 0 if not needed)

	Returns:
		0			-	Success, the returned time is provided in the variables hour, min, sec.
		1			-	Error
******************************************************************************/
unsigned char ds3232_readdatetime_conv_int(unsigned char sync, unsigned char *hour,unsigned char *min,unsigned char *sec,unsigned char *day,unsigned char *month,unsigned char *year)
{
	unsigned char regs[7];
	
	if(sync)
	{
		// Wait for the falling edge of the RTC int
		ds3232_sync_fall();
	}
	// Read time
	ds3232_readtimeregisters(regs);
	ds3232_convtime(regs,hour,min,sec);
	ds3232_convdate(regs,day,month,year);
		
	return 0;
}


/******************************************************************************
	ds3232_readdate_conv
*******************************************************************************	
	
	Return value:
		0:			success
		1:			error
******************************************************************************/
unsigned char ds3232_readdate_conv(unsigned char *date,unsigned char *month,unsigned char *year)
{
	unsigned char r;
	unsigned char data[4];
	//unsigned char day;
	
	// Perform read.
	r = i2c_readregs(DS3232_ADDRESS,3,4,data);
	if(r!=0)
	{
		*date=*month=*year=0;
		return r;
	}
		
	//day = data[0];
	*date = ds3232_bcd2dec(data[1]);
	*month = ds3232_bcd2dec(data[2]);
	*year = ds3232_bcd2dec(data[3]);
	
	return 0;
}


/******************************************************************************
	ds3232_readdate_conv_int
*******************************************************************************	
	
	I2C transactional
	
	Return value:
		0:			success
		1:			error
******************************************************************************/
unsigned char ds3232_readdate_conv_int(unsigned char *date,unsigned char *month,unsigned char *year)
{
	unsigned char r;
	unsigned char data[4];
	//unsigned char day;
	
	// Perform read.
	r = i2c_readregs_int(DS3232_ADDRESS,3,4,data);
	if(r!=0)
	{
		*date=*month=*year=0;
		return r;
	}
		
	//day = data[0];
	*date = ds3232_bcd2dec(data[1]);
	*month = ds3232_bcd2dec(data[2]);
	*year = ds3232_bcd2dec(data[3]);
	
	return 0;
}

unsigned char ds3232_readtime_sync_conv(unsigned char *hour,unsigned char *min,unsigned char *sec)
{
	unsigned char val[7];
	unsigned char r;
	
	r = ds3232_readtime_sync(val);
	if(r!=0)
		return r;
		
	ds3232_convtime(val,hour,min,sec);

	return 0;
}

unsigned char ds3232_bcd2dec(unsigned char bcd)
{
	return (bcd&0xf) + (bcd>>4)*10;
}

/******************************************************************************
	function:	ds3232_convtime
*******************************************************************************
	Convert the time (register 0-2) of DS3232 into hours, minutes and seconds.
	
	The pointers to the return variable can be null, in which case the corresponding
	parameters is not returned.
	
	
	Parameters:
		val			-		Pointer to register 0-6 of date/time
		hour		-		Pointer to the variable receiving the hours, or 0 if unneeded
		min			-		Pointer to the variable receiving the minutes, or 0 if unneeded
		sec			-		Pointer to the variable receiving the seconds, or 0 if unneeded
		
	Returns:
		-
******************************************************************************/
void ds3232_convtime(unsigned char *val,unsigned char *hour,unsigned char *min,unsigned char *sec)
{
	// Do the conversion
	if(sec)
		*sec = ds3232_bcd2dec(val[0]);
	if(min)
		*min = ds3232_bcd2dec(val[1]);
	if(hour)
	{
		if(val[2]&0x40)
		{
			// 12hr
			*hour = (val[2]&0xf);
			if(val[2]&0x20)
				*hour+=12;
		}
		else
		{
			// 24hr
			*hour = ds3232_bcd2dec(val[2]);
		}
	}
}
/******************************************************************************
	function:	ds3232_convdate
*******************************************************************************
	Convert the date (register 3-6) of DS3232 into day, month, year.
	
	The pointers to the return variable can be null, in which case the corresponding
	parameters is not returned.
		
	Parameters:
		val			-		Pointer to register 0-6 of date/time
		day			-		Pointer to the variable receiving the day, or 0 if unneeded
		month		-		Pointer to the variable receiving the month, or 0 if unneeded
		year		-		Pointer to the variable receiving the year, or 0 if unneeded
		
	Returns:
		-
******************************************************************************/
void ds3232_convdate(unsigned char *val,unsigned char *day,unsigned char *month,unsigned char *year)
{
	if(day)
		*day = ds3232_bcd2dec(val[4]);
	if(month)
		*month = ds3232_bcd2dec(val[5]);
	if(year)
		*year = ds3232_bcd2dec(val[6]);
	
}


/******************************************************************************
	ds3232_readtemp
*******************************************************************************	
	Reads the ds3232 temperature.
	
	Returns temperature*100 (i.e. 2125 is 21.25�C)
	
	Return value:
		0:			success
		1:			error
******************************************************************************/
unsigned char ds3232_readtemp(signed short *temp)
{
	unsigned char r;
	unsigned char t[2];
	
	// Perform read.
	r = i2c_readregs(DS3232_ADDRESS,0x11,2,t);
	if(r!=0)
	{
		*temp=0;
		return r;
	}
	
	
	// Convert temperature
	*temp = t[0]*100;
	*temp += (t[1]>>6)*25;
	
	//printf("%02d %02d -> %04d\n",t[0],t[1],*temp);
	
	return 0;
}



/******************************************************************************
	ds3232_readtemp_int_cb
*******************************************************************************	
	Reads the ds3232 temperature in background using the I2C transactional 
	mechanism.
	
	Calls the provided callback with status (0=success) and temperature*100 
	(i.e. 2125 is 21.25�C) when the reading is completed.
	
	Return value:
		0:			successfully issued a conversion
		1:			error
******************************************************************************/
unsigned char ds3232_readtemp_int_cb(void (*cb)(unsigned char,signed short))
{
	unsigned char r = i2c_readregs_int_cb(DS3232_ADDRESS,0x11,2,0,ds3232_readtemp_int_cb_cb,(void*)cb);
	if(r)
		return 1;
			
	return 0;	
}



unsigned char ds3232_readtemp_int_cb_cb(I2C_TRANSACTION *t)
{
	//printf("xx %02x %02x   %02x %02x %02x\n",t->status,t->i2cerror,t->data[0],t->data[1],t->data[2]);
	// Call the final user callback
	signed short temp;
	temp = t->data[0]*100;
	temp += (t->data[1]>>6)*25;
	void (*cb)(unsigned char,signed short);
	cb = (void(*)(unsigned char,signed short))t->user;
	cb(t->status,temp);
	return 0;	
}



/*
	Write val in the DS3232 control register (address 0x0E)
	
	Returns:	zero success
						nonzero error; value indicates I2C error
*/
unsigned char ds3232_write_control(unsigned char val)
{
	return i2c_writereg(DS3232_ADDRESS,0x0E,val);
}

/*
	Write val in the DS3232 status register (address 0x0E)
	
	Returns:	zero success
						nonzero error; value indicates I2C error
*/
unsigned char ds3232_write_status(unsigned char val)
{
	return i2c_writereg(DS3232_ADDRESS,0x0F,val);
}


void ds3232_printreg(FILE *file)
{
		unsigned char r1 __attribute__((unused));
		unsigned char data[32];
	
		fprintf_P(file,PSTR("DS3232 registers:"));
		
		r1 = i2c_readregs(DS3232_ADDRESS,0,16,data);
		r1 = i2c_readregs(DS3232_ADDRESS,16,16,data+16);
		//fprintf_P(file,PSTR("readregs return: %d\n"),r1);
		
		// Pretty print as 2 rows of 16 registers
		for(int j=0;j<2;j++)
		{
			fprintf_P(file,PSTR("\n\t%02X-%02X: "),j*16,j*16+15);
			for(int i=0;i<16;i++)
			{		
				fprintf_P(file,PSTR("%02X "),data[j*16+i]);
			}
		}
		fprintf_P(file,PSTR("\n"));

}



/******************************************************************************
	ds3232_writetime
*******************************************************************************
	Writes the time to the RTC. 
	Range of values:
		sec � [0;59]
		min � [0;59]
		hour � [0;23]
	
	Return value:
		0:			Success
		other:	Error
******************************************************************************/
unsigned char ds3232_writetime(unsigned char hour,unsigned char min,unsigned char sec)
{
	unsigned char v[3];
	
	v[0] = ((sec/10)<<4) + (sec%10);
	v[1] = ((min/10)<<4) + (min%10);
	v[2] = ((hour/10)<<4) + (hour%10);
	//return i2c_writeregs_int(DS3232_ADDRESS,0,v,3);
	return i2c_writeregs(DS3232_ADDRESS,0,v,3);
}

/******************************************************************************
	ds3232_writedate
*******************************************************************************
	Writes the date in day,month,year to the RTC. 
	Range of values:
		day � [1;7]
		date � [1;30]
		month � [1;12]
		year � [0;99]
	
	Return value:
		0:			Success
		other:	Error
******************************************************************************/
unsigned char ds3232_writedate(unsigned char day,unsigned char date,unsigned char month,unsigned char year)
{
	unsigned char r;
	unsigned char v;
	
	// Day
	r = i2c_writereg(DS3232_ADDRESS,3,day);
	if(r!=0) return r;
	// Date
	v = ((date/10)<<4) + (date%10);
	printf("date: %02Xh\n",v);
	r = i2c_writereg(DS3232_ADDRESS,4,v);
	if(r!=0) return r;
	// month
	v = ((month/10)<<4) + (month%10);
	printf("month: %02Xh\n",v);
	r = i2c_writereg(DS3232_ADDRESS,5,v);
	if(r!=0) return r;
	// year
	v = ((year/10)<<4) + (year%10);
	printf("year: %02Xh\n",v);
	r = i2c_writereg(DS3232_ADDRESS,6,v);
	if(r!=0) return r;
	return 0;
}
/******************************************************************************
	ds3232_writedate_int
*******************************************************************************
	Writes the date in day,month,year to the RTC. 
	Range of values:
		day � [1;7]
		date � [1;12]
		month � [1;12]
		year � [0;99]
	
	Return value:
		0:			Success
		other:	Error
******************************************************************************/
unsigned char ds3232_writedate_int(unsigned char day,unsigned char date,unsigned char month,unsigned char year)
{
	unsigned char r;
	unsigned char v[4];
	
	v[0] = day;
	v[1] = ((date/10)<<4) + (date%10);
	v[2] = ((month/10)<<4) + (month%10);
	v[3] = ((year/10)<<4) + (year%10);
	r = i2c_writeregs_int(DS3232_ADDRESS,3,v,4);
	
	return r;	
}



void ds3232_off(void)
{
	unsigned char r;
	unsigned char v[4];
	unsigned char hour,min,sec;
	hour=0;
	min=0;
	sec=0;
	
	fprintf_P(file_pri,PSTR("Register initially\n"));
	ds3232_printreg(file_pri);
	
	r = ds3232_writedate(1,1,1,10);
	fprintf_P(file_pri,PSTR("Write date: %d\n"),r);
	r = ds3232_writetime(hour,min,sec);
	fprintf_P(file_pri,PSTR("Write time: %d\n"),r);
	
	
	// deactivate 1Hz, activate alarm 1
	r = ds3232_write_control(0b00011101);			
	// Clear stopped flag, clear alarm flag, disable battery-backed 32KHz, enable normal 32KHz	
	r = ds3232_write_status(0b00001000);			
	
	// Alarm 1: 0x07-0x0A
	// Time
	sec+=10;
	v[0] = ((sec/10)<<4) + (sec%10);
	v[1] = ((min/10)<<4) + (min%10);
	v[2] = ((hour/10)<<4) + (hour%10);
	r = i2c_writeregs(DS3232_ADDRESS,0x07,v,3);
	fprintf_P(file_pri,PSTR("Write alm time: %d\n"),r);
	r = i2c_writereg(DS3232_ADDRESS,0x0a,1);
	fprintf_P(file_pri,PSTR("Write alm day: %d\n"),r);
	
	ds3232_printreg(file_pri);
	
	/*for(unsigned i=0;i<20;i++)
	{
		fprintf_P(file_pri,PSTR("Register after 0.5*%d. PA6: %d\n"),i,(PINA&0b01000000)?1:0);
		ds3232_printreg(file_pri);
		_delay_ms(500);
	}*/
	
	
	
	/*
	fprintf(file_pri,PSTR("Write date: %d\n"),r);
	fprintf(file_fb,"Before write control\n");
	r = ds3232_write_control(0b00011100);			// Enable timer interrupt with alarm disabled
	printf("write control: %02Xh\n",r);
	*/
	
	
	
	
	//fprintf(file_fb,"Before write status\n");
	//r = ds3232_write_status(0b00001000);			// Clear stopped flag, disable battery-backed 32KHz, enable normal 32KHz
	//printf("write status: %02Xh\n",r);
	//fprintf(file_fb,"RTC done\n");
}
void ds3232_alarm_at(unsigned char date, unsigned char month, unsigned char year,unsigned char hour,unsigned char min,unsigned char sec)
{
	unsigned char v[4];
	unsigned char r __attribute__((unused));
	
	// Alarm 1: 0x07-0x09 is time
	v[0] = ((sec/10)<<4) + (sec%10);
	v[1] = ((min/10)<<4) + (min%10);
	v[2] = ((hour/10)<<4) + (hour%10);
	r = i2c_writeregs(DS3232_ADDRESS,0x07,v,3);
	// Alarm 1: 0x0A-0x0C is date
	v[0] = ((date/10)<<4) + (date%10);
	v[1] = ((month/10)<<4) + (month%10);
	v[2] = ((year/10)<<4) + (year%10);
	r = i2c_writeregs(DS3232_ADDRESS,0x0a,v,3);
	
	r = ds3232_write_control(0b00011101);			// Enable oscillator, disable battery backed 1Hz SQW, enable interrupt, enable alarm 1
	r = ds3232_write_status(0b00000000);			// Clear stopped flag, clear alarm flag, disable battery-backed 32KHz, disable normal 32KHz
	
	ds3232_printreg(file_pri);
}
void ds3232_alarm_in(unsigned short insec)
{
	unsigned char r __attribute__((unused));
	unsigned char hour,min,sec;
	unsigned char date,month,year;
	unsigned short hour2,min2,sec2;
	unsigned char day;
	hour=0;
	min=0;
	sec=0;
	
	
	r = ds3232_readdatetime_conv_int(0,&hour,&min,&sec,&date,&month,&year);
		
	fprintf_P(file_pri,PSTR("Cur: %02d.%02d.%02d %02d:%02d:%02d\n"),date,month,year,hour,min,sec);
	
	day = TimeAddSeconds(hour,min,sec,insec,&hour2,&min2,&sec2);
	
	fprintf_P(file_pri,PSTR("Alm: %02d.%02d.%02d %02d:%02d:%02d (+%d d)\n"),date,month,year,hour2,min2,sec2,day);
	
	if(day)
		fprintf_P(file_pri,PSTR("Cannot set alarm on different day\n"));
	else
		ds3232_alarm_at(date,month,year,hour2,min2,sec2);
}

