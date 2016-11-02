#ifndef __INIT_H
#define __INIT_H

extern unsigned char init_ddra;
extern unsigned char init_porta;
extern unsigned char init_ddrb;
extern unsigned char init_portb;


void init_ports(void);
void init_timers(void);
void init_module(void);
void init_spi(void);


#if BOOTLOADER==0
void init_lcd(void);
void deinit_lcd(void);
#endif

#endif 