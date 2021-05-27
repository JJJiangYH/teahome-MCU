#ifndef __I2C_H
#define __I2C_H			 
#include "delay.h"

/******************************************************************************
引脚别名定义
*******************************************************************************/			
sbit	SDA	= P5^3;	   //定义SDA  
sbit	SCL	= P5^2;	   //定义SCL

extern bit I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern void I2C_Check_ACK(void);
extern void I2C_SendByte(uint8 SendByte);
extern uint8 I2C_ReceiveByte(void);


#endif 

