#ifndef __MPU_H
#define __MPU_H


#include "cpu.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "i2c.h"

/*
 Structure for MPU data
 Memory organisation on AVR (GCC 7.2):
 struct start: 0x40df
	acc: 0x40df 0x40e1 0x40e3
	gyr: 0x40e5 0x40e7 0x40e9
	mag: 0x40eb 0x40ed 0x40ef 0x40f1
	temp: 0x40f2
	time: 0x40f4
	packetctr: 0x40f8
	
	Offsets: 
		ax: 0
		ay:	2
		az:	4
		gx: 6
		gy: 8
		gz: 10
		mx: 12
		my: 14
		mz: 16
		ms: 18
		temp: 19
		time: 21
		packetctr: 25
		
	
*/
typedef struct {
	signed short ax,ay,az;
	signed short gx,gy,gz;
	signed short mx,my,mz;
	unsigned char ms;
	signed short temp;
	unsigned long int time;
	unsigned long packetctr;
} MPUMOTIONDATA;


typedef struct {
	float yaw,pitch,roll;		// Aerospace
	float alpha,x,y,z;			// Quaternion debug
	float q0,q1,q2,q3;			// Quaternion
} MPUMOTIONGEOMETRY;

#include "mpu_geometry.h"





// AK8963 address:0X0C
#define MAG_ADDRESS 0X0C
#define MPU_ADDRESS 105

#define MPU_R_SMPLRT_DIV 			0x19
#define MPU_R_CONFIG				0x1A
#define MPU_R_GYROCONFIG			0x1B
#define MPU_R_ACCELCONFIG			0x1C
#define MPU_R_ACCELCONFIG2			0x1D
#define MPU_R_LPODR					0x1E
#define MPU_R_FIFOEN 				0x23
#define MPU_R_INTERRUPTPIN			0x37
#define MPU_R_INTERRUPTENABLE 		0x38
#define MPU_R_USR_CTRL	 			106
#define MPU_R_PWR_MGMT_1 			0x6B
#define MPU_R_PWR_MGMT_2			0x6C
#define MPU_R_WHOAMI					0x75

#define MPU_R_I2C_SLV0_ADDR		37
#define MPU_R_I2C_SLV0_REG		38
#define MPU_R_I2C_SLV0_CTRL		39
#define MPU_R_I2C_SLV0_DO 99

#define MPU_R_I2C_SLV4_ADDR		49
#define MPU_R_I2C_SLV4_REG		50
#define MPU_R_I2C_SLV4_DO		51
#define MPU_R_I2C_SLV4_CTRL		52
#define MPU_R_I2C_SLV4_DI 		53
#define MPU_R_INT_STATUS		58

#define MPU_R_I2C_MST_CTRL		36
#define MPU_R_I2C_MST_STATUS	54
#define MPU_R_I2C_MST_DELAY_CTRL 103

#define MPU_LPODR_500 11
#define MPU_LPODR_250 10
#define MPU_LPODR_125 9
#define MPU_LPODR_62 8
#define MPU_LPODR_31 7
#define MPU_LPODR_16 6
#define MPU_LPODR_8 5
#define MPU_LPODR_4 4
#define MPU_LPODR_2 3
#define MPU_LPODR_1 2

#define MPU_ACC_LPF_460 0
#define MPU_ACC_LPF_184 1
#define MPU_ACC_LPF_92 2
#define MPU_ACC_LPF_41 3
#define MPU_ACC_LPF_20 4
#define MPU_ACC_LPF_10 5
#define MPU_ACC_LPF_5 6

#define MPU_GYR_LPF_250 0
#define MPU_GYR_LPF_184 1
#define MPU_GYR_LPF_92 2
#define MPU_GYR_LPF_41 3
#define MPU_GYR_LPF_20 4
#define MPU_GYR_LPF_10 5
#define MPU_GYR_LPF_5 6
#define MPU_GYR_LPF_3600 7

// Gyro scale
#define MPU_GYR_SCALE_250 0
#define MPU_GYR_SCALE_500 1
#define MPU_GYR_SCALE_1000 2
#define MPU_GYR_SCALE_2000 3

// Acc scale
#define MPU_ACC_SCALE_2		0
#define MPU_ACC_SCALE_4		1
#define MPU_ACC_SCALE_8		2
#define MPU_ACC_SCALE_16	3


// Non-volatile (EEPROM) storage
#define CONFIG_ADDR_MAG_BIASXL (CONFIG_ADDR_MPU_SETTINGS+0)
#define CONFIG_ADDR_MAG_BIASXH (CONFIG_ADDR_MPU_SETTINGS+1)
#define CONFIG_ADDR_MAG_BIASYL (CONFIG_ADDR_MPU_SETTINGS+2)
#define CONFIG_ADDR_MAG_BIASYH (CONFIG_ADDR_MPU_SETTINGS+3)
#define CONFIG_ADDR_MAG_BIASZL (CONFIG_ADDR_MPU_SETTINGS+4)
#define CONFIG_ADDR_MAG_BIASZH (CONFIG_ADDR_MPU_SETTINGS+5)
#define CONFIG_ADDR_MAG_SENSXL (CONFIG_ADDR_MPU_SETTINGS+6)
#define CONFIG_ADDR_MAG_SENSXH (CONFIG_ADDR_MPU_SETTINGS+7)
#define CONFIG_ADDR_MAG_SENSYL (CONFIG_ADDR_MPU_SETTINGS+8)
#define CONFIG_ADDR_MAG_SENSYH (CONFIG_ADDR_MPU_SETTINGS+9)
#define CONFIG_ADDR_MAG_SENSZL (CONFIG_ADDR_MPU_SETTINGS+10)
#define CONFIG_ADDR_MAG_SENSZH (CONFIG_ADDR_MPU_SETTINGS+11)
#define CONFIG_ADDR_MAG_CORMOD (CONFIG_ADDR_MPU_SETTINGS+12)
#define CONFIG_ADDR_ACC_SCALE (CONFIG_ADDR_MPU_SETTINGS+13)
#define CONFIG_ADDR_GYRO_SCALE (CONFIG_ADDR_MPU_SETTINGS+14)

#define CONFIG_ADDR_BETA (CONFIG_ADDR_MPU_SETTINGS+20)
#define CONFIG_ADDR_BETA1 (CONFIG_ADDR_MPU_SETTINGS+21)
#define CONFIG_ADDR_BETA2 (CONFIG_ADDR_MPU_SETTINGS+22)
#define CONFIG_ADDR_BETA3 (CONFIG_ADDR_MPU_SETTINGS+23)



#if HWVER==1
#include "mpu-i2c.h"
#endif
#if (HWVER==4) || (HWVER==5) || (HWVER==6) || (HWVER==7)
#include "mpu-usart0.h"
#endif




extern unsigned char sample_mode;

// Conversion from gyro raw readings to radians per second
#ifdef __cplusplus
extern float mpu_gtorps;
#else
extern _Accum mpu_gtorps;
#endif

//typedef void (*MPU_READ_CALLBACK7)(unsigned char status,unsigned char error,signed short ax,signed short ay,signed short az,signed short gx,signed short gy,signed short gz,signed short temp);
//typedef void (*MPU_READ_CALLBACK3)(unsigned char status,unsigned char error,signed short x,signed short y,signed short z);





//unsigned long mpu_estimatefifofillrate(unsigned char fenflag);
//unsigned short mpu_estimateodr(void);



// Interrupt driven
void mpu_get_agt_int_init(void);
unsigned short mpu_getfifocnt_int(void);
unsigned char mpu_get_agt_int(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz,signed short *temp);
/*unsigned char mpu_get_agt_int_cb(MPU_READ_CALLBACK7 cb);
unsigned char mpu_get_a_int(signed short *ax,signed short *ay,signed short *az);
unsigned char mpu_get_a_int_cb(MPU_READ_CALLBACK3 cb);
unsigned char mpu_get_g_int(signed short *gx,signed short *gy,signed short *gz);
unsigned char mpu_get_g_int_cb(MPU_READ_CALLBACK3 cb);*/


extern unsigned char __mpu_sample_softdivider_ctr,__mpu_sample_softdivider_divider;



void mpu_isr(void);



// Data buffers
// 32 buffers works on all cards which are U-1 or faster without data loss at 500Hz LBW and 500Hz HBW. Also works at 1KHz, although the effective sample rate is 800Hz.
#define MPU_MOTIONBUFFERSIZE 64		
//#define MPU_MOTIONBUFFERSIZE 32
//#define MPU_MOTIONBUFFERSIZE 16
//#define MPU_MOTIONBUFFERSIZE 8
//#define MPU_MOTIONBUFFERSIZE 4
//#define MPU_MOTIONBUFFERSIZE 128
extern MPUMOTIONDATA mpu_data[];
extern volatile unsigned long __mpu_data_packetctr_current;
extern volatile unsigned char mpu_data_rdptr,mpu_data_wrptr;

extern volatile MPUMOTIONDATA _mpumotiondata_test;


// Magnetometer Axis Sensitivity Adjustment
extern unsigned char _mpu_mag_asa[3];
extern signed short _mpu_mag_calib_max[3];
extern signed short _mpu_mag_calib_min[3];
extern signed short _mpu_mag_bias[3];
extern signed short _mpu_mag_sens[3];
extern unsigned char _mpu_mag_correctionmode;

extern unsigned char __mpu_autoread;
extern unsigned char _mpu_current_motionmode;

// Automatic read statistic counters
extern unsigned long mpu_cnt_int, mpu_cnt_sample_tot, mpu_cnt_sample_succcess, mpu_cnt_sample_errbusy, mpu_cnt_sample_errfull;
extern unsigned long mpu_cnt_spurious;

extern unsigned char _mpu_kill;
extern unsigned short _mpu_samplerate;
extern float _mpu_beta;

void __mpu_read_cb(void);

// Motion data buffers
unsigned char mpu_data_isfull(void);
//unsigned char mpu_data_isempty(void);
unsigned char mpu_data_level(void);
unsigned char mpu_data_getnext_raw(MPUMOTIONDATA &data);
unsigned char mpu_data_getnext(MPUMOTIONDATA &data,MPUMOTIONGEOMETRY &geometry);
void _mpu_data_wrnext(void);
void _mpu_data_rdnext(void);

void mpu_clearstat(void);
void mpu_clearbuffer(void);
void mpu_getstat(unsigned long *cnt_int, unsigned long *cnt_sample_tot, unsigned long *cnt_sample_succcess, unsigned long *cnt_sample_errbusy, unsigned long *cnt_sample_errfull);

void mpu_mode_accgyro(unsigned char gdlpe,unsigned char gdlpoffhbw,unsigned char gdlpbw,unsigned char adlpe,unsigned char adlpbw,unsigned char divider);
void mpu_mode_gyro(unsigned char gdlpe,unsigned char gdlpoffhbw,unsigned char gdlpbw,unsigned char divider);
void mpu_mode_acc(unsigned char dlpenable,unsigned char dlpbw,unsigned char divider);
void mpu_mode_lpacc(unsigned char lpodr);
void mpu_mode_off(void);

void _mpu_enableautoread(void);
void _mpu_disableautoread(void);


void mpu_init(void);
void mpu_get_agt(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz,signed short *temp);
void mpu_get_agmt(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz,signed short *mx,signed short *my,signed short *mz,unsigned char *ms,signed short *temp);
void mpu_get_g(signed short *gx,signed short *gy,signed short *gz);
void mpu_get_a(signed short *ax,signed short *ay,signed short *az);
/*unsigned char mpu_get_agt_int_cb(MPU_READ_CALLBACK7 cb);
unsigned char mpu_get_a_int_cb(MPU_READ_CALLBACK3 cb);
unsigned char mpu_get_g_int_cb(MPU_READ_CALLBACK3 cb);*/

unsigned short mpu_getfifocnt(void);
void mpu_fifoenable(unsigned char flags,unsigned char en,unsigned char reset);
void mpu_readallregs(unsigned char *v);
void mpu_fiforead(unsigned char *fifo,unsigned short n);
void mpu_fiforeadshort(short *fifo,unsigned short n);
void mpu_setgyrobias(short bgx,short bgy,short bgz);
void mpu_setaccodr(unsigned char odr);
void mpu_setacccfg2(unsigned char cfg);
void mpu_setusrctrl(unsigned char cfg);
void mpu_reset(void);
void mpu_setsrdiv(unsigned char div);
void mpu_setgyrosamplerate(unsigned char fchoice,unsigned char dlp);
void mpu_setaccsamplerate(unsigned char fchoice,unsigned char dlp);
void mpu_set_interrutenable(unsigned char wom,unsigned char fifo,unsigned char fsync,unsigned char datardy);

void mpu_setgyroscale(unsigned char scale);
unsigned char mpu_getgyroscale(void);
void mpu_setaccscale(unsigned char scale);
unsigned char mpu_getaccscale(void);
void mpu_temp_enable(unsigned char enable);
void mpu_clksel(unsigned char clk);
void mpu_mode_gyrostby(unsigned char stby);
void mpu_mode_sleep(unsigned char sleep);
signed short mpu_convtemp(signed short t);
void mpu_set_interruptpin(unsigned char p);
unsigned char mpu_getwhoami(void);

void _mpu_acquirecalib(unsigned char clearbias);
void mpu_calibrate(void);

void _mpu_defaultdlpon(void);
void _mpu_wakefromsleep(void);

//void mpu_testsleep(void);
void mpu_printfifo(FILE *file);

void _mpu_mag_interfaceenable(unsigned char en);
void _mpu_mag_mode(unsigned char mode,unsigned char magdiv);
unsigned char mpu_mag_readreg(unsigned char reg);
void mpu_mag_writereg(unsigned char reg,unsigned char val);
void _mpu_mag_regshadow(unsigned char enable,unsigned char dly,unsigned char regstart,unsigned char numreg);
void _mpu_mag_readasa(void);
void mpu_mag_correct1(signed short mx,signed short my,signed short mz,volatile signed short *mx2,volatile signed short *my2,volatile signed short *mz2);
void mpu_mag_correct2(signed short mx,signed short my,signed short mz,signed short *mx2,signed short *my2,signed short *mz2);
void mpu_mag_correct2_inplace(signed short *mx,signed short *my,signed short *mz);
void mpu_mag_correct2b(signed short mx,signed short my,signed short mz,signed short *mx2,signed short *my2,signed short *mz2);
void mpu_mag_correct2c(signed short mx,signed short my,signed short mz,signed short *mx2,signed short *my2,signed short *mz2);
extern "C" void mpu_mag_correct2_asm(signed short *mx,signed short *my,signed short *mz);
void mpu_mag_calibrate(void);
void mpu_mag_storecalib(void);
void mpu_mag_loadcalib(void);
void mpu_mag_correctionmode(unsigned char mode);
void mpu_kill(unsigned char bitmap);

// Non-volatile parameters
unsigned char mpu_LoadAccScale(void);
unsigned char mpu_LoadGyroScale(void);
void mpu_setandstoregyrocale(unsigned char scale);
void mpu_setandstoreaccscale(unsigned char scale);

void mpu_LoadBeta(void);
void mpu_StoreBeta(float beta);



// Print functions
void mpu_mag_printreg(FILE *file);
void mpu_mag_printcalib(FILE *f);
void mpu_printreg(FILE *file);
void mpu_printextreg(FILE *file);
void mpu_printregdesc(FILE *file);
void mpu_printregdesc2(FILE *file);
void mpu_printstat(FILE *file);

void __mpu_copy_spibuf_to_mpumotiondata_1(unsigned char *spibuf,unsigned char *mpumotiondata);
void __mpu_copy_spibuf_to_mpumotiondata_2(unsigned char *spibuf,unsigned char *mpumotiondata);
void __mpu_copy_spibuf_to_mpumotiondata_3(unsigned char *spibuf,MPUMOTIONDATA *mpumotiondata);
extern "C" void __mpu_copy_spibuf_to_mpumotiondata_asm(unsigned char *spibuf,MPUMOTIONDATA *mpumotiondata);
extern "C" void __mpu_copy_spibuf_to_mpumotiondata_magcor_asm(unsigned char *spibuf,MPUMOTIONDATA *mpumotiondata);
extern "C" void __mpu_copy_spibuf_to_mpumotiondata_magcor_asm_mathias(unsigned char *spibuf,MPUMOTIONDATA *mpumotiondata);

void mpu_benchmark_isr(void);

#endif
