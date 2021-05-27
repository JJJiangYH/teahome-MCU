#ifndef __UART_H_
#define __UART_H_

#include "delay.h"
#include <stdio.h>
#include <string.h>           // �����ͷ�ļ���,��ʹ��strstr�⺯��
#include <intrins.h>					// �����ͷ�ļ���,��ʹ��_nop_�⺯��

#define Buf4_Max 20

#define S4RI 0x01
#define S4TI 0x02
#define S4_S 0x04

extern uint8 state_all;
extern uint16 ctemp;

extern void Uart1_Init(void);
extern void SendDataByUart1(uint8 dat);
extern void SendStringByUart1(uint8 *s);
extern void SendDataByUart4(uint8 dat);
extern void SendStringByUart4(uint8 *s);

extern void Uart4_Init(void);
extern void HC08_Tx_Puts(void);
#endif
