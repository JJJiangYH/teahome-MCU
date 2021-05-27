#ifndef __PCF8563_H_
#define __PCF8563_H_

#include  "delay.h"
#include <string.h>       

#define DIS_DOT		0x20
#define DIS_BLACK	0x10
#define DIS_		0x11

#define SLAW	0xA2
#define SLAR	0xA3

extern  uint8	time[6];
extern uint8	date[9];
extern volatile uint8	hour, minute, second;
extern volatile uint8	myweek, day, month, year;

extern  void  PCF8563_Init(void);
extern  void	WriteRTC(void);
extern  void	ReadRTC(void);
extern  void PCF8563_TEST(void);

#endif
