/****************************************Copyright (c)****************************************************
**                                        
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			
** Last modified Date:          
** Last Version:		   
** Descriptions:							
**--------------------------------------------------------------------------------------------------------
**--------------------------------------------------------------------------------------------------------
** Modified by:			JiangYuHang
** Modified date:		2021-3-28
** Version:				  1.0
** Descriptions:	  
** Rechecked by:		
**********************************************************************************************************/

#include	"uart.h"	
#include	"ds18b20.h" 
#include 	"oled.h"
#include	"15W4KxxS4.h"
#include	"oledfont.h"
#include  "delay.h"
#include  "timeControl.h"	
#include "Math.h"
#include	"led.h"

uint8 state_all=0;

/***************************************************************************
 * ��  �� : ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************/
int main() {
	uint8 i = 0;
	uint8 lastTime = 0;
	uint8 lastTem[6];
	uint8 isEquals = 1;
	int conTemp = 0;
	int boiled = 0;
	uint16 ht = 0;
	uint8 ht_char[7];

	memset(lastTem, 0, 6);
	memset(ht_char, 0, 7);

	//ʱ��ģ���ʼ��
	P3M1 &= 0xFC;	P3M0 &= 0xFC;	                  //����P3.0~P3.1Ϊ׼˫���

	Uart1_Init();                                 //����1��ʼ�� 	
	Uart4_Init();         												//����4��ʼ��
	EA = 1;                                       //ʹ�����ж�
	delay_ms(10);                                 //��ʼ������ʱ

	PCF8563_Init();                               //PCF8563ʱ�ӳ�ʼ�� 

	//�¶ȳ�ʼ��
	P3M1 &= 0xBF;	P3M0 &= 0xBF;	                  //����P3.6Ϊ׼˫���

	DATA_PIN = 1;                                   //�������� 
	delay_ms(10);                                 //��ʼ������ʱ

	//OLED��ʼ��
	P0M1 &= 0xCF;	P0M0 &= 0xCF;	     //����P0.4,P0.5Ϊ׼˫���
	P2M1 &= 0xE5;	P2M0 &= 0xE5;	       //����P2.1,P2.3��P2.4Ϊ׼˫��� 
	RES = 0;                            //��OLED����λ���ŵĿ��ƣ��͵�ƽ��Ч
	delay_ms(500);                    //����ʱ�����٣������ڸ�λ��ʱ��
	RES = 1;                            //��OLED����λ���ŵĿ���
	OLED_Init();                      //��OLED����ʼ��  
	OLED_Fill(0, 0x00);                  //���� 

	P0M1 &= 0x3F;	P0M0 &= 0x3F;	 //����P0.6��P0.7Ϊ׼˫���
	P5M1 &= 0xF3;	P5M0 &= 0xF3;
	
	//���ȴ���
	sprintf(ht_char, "HT:%02d", ht);
	LCD_P8x16Str(44, 6, ht_char);
	
	while (1) {
		DS18B20_TEST();       //�����¶Ȳ����ڴ�ӡ
		isEquals = 1;
		for (i = 0; i < 7; i++) {
			if (lastTem[i] != temp[i]) isEquals = 0;
		}
		LCD_P8x16Str(4, 2, "TempNow");
		if (isEquals == 0) {
			OLED_Fill_Row(4, 0x00);
			OLED_Fill_Row(5, 0x00);
			LCD_P8x16Str(12, 4, temp);
			for (i = 0; i < 6; i++) {
				lastTem[i] = temp[i];
			}
			isEquals = 1;
		}

		//���ú���
		sprintf(constantTemp, "%03d", ctemp);

		LCD_P8x16Str(4 + 64, 2, "ConTemp");
		LCD_P8x16Str(64 + 20, 4, constantTemp);

		//��ʾ״̬
		OLED_Fill_Row(0, 0x00);
		OLED_Fill_Row(1, 0x00);
		if (state_all == 1) {
			LCD_P8x16Str(32, 0, "State:ON");
		}
		else {
			LCD_P8x16Str(27, 0, "State:OFF");
		}
		
		delay_ms(1000);

		HC08_Tx_Puts();   //��Ƭ�����յ�HC08�������������ָʾ�ƶ����������ڷ��ظ�HC08ģ��
	}
}