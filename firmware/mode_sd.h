#ifndef __MODE_SD_H
#define __MODE_SD_H

#include "command.h"

extern const char help_sdinit[];
extern const char help_sdbench[];

unsigned char CommandParserSD(char *buffer,unsigned char size);
unsigned char CommandParserSDInit(char *buffer,unsigned char size);
unsigned char CommandParserSDErase(char *buffer,unsigned char size);
unsigned char CommandParserSDErase2(char *buffer,unsigned char size);
unsigned char CommandParserSDErase3(char *buffer,unsigned char size);
unsigned char CommandParserSDWrite(char *buffer,unsigned char size);
unsigned char CommandParserSDRead(char *buffer,unsigned char size);
unsigned char CommandParserSDStream(char *buffer,unsigned char size);
unsigned char CommandParserSDVolume(char *buffer,unsigned char size);
unsigned char CommandParserSDFormat(char *buffer,unsigned char size);
unsigned char CommandParserSDLogTest(char *buffer,unsigned char size);
unsigned char CommandParserSDLogTest2(char *buffer,unsigned char size);
unsigned char CommandParserSDBench(char *buffer,unsigned char size);
unsigned char CommandParserSDBench2(char *buffer,unsigned char size);
unsigned char CommandParserSDBench_t1(char *buffer,unsigned char size);
unsigned char CommandParserSDBench_t2(char *buffer,unsigned char size);

void mode_sd(void);


#endif
