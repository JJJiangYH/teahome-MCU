#include  "timeControl.h"	
#include  "i2c.h"
#include "uart.h"
#include "oled.h"

volatile uint8	hour, minute, second;	       //RTC������ʱ���֣���
volatile uint8	myweek, day, month, year;	   //RTC�������ܣ��죬�£���
uint8	time[6];
uint8	date[9];
static uint8  LED8[8];		                 //��ʾ��ŵ�ʱ���֣�����Ϣ
static uint8  DAY[2];		                   //��ʾ��ŵ�������Ϣ
static uint8  MONTH[2];		                 //��ʾ��ŵ��·���Ϣ
static uint8  YEAR[2];		                 //��ʾ��ŵ������Ϣ
static uint8  ErrFlag;		                 //�����־λ

/**************************************************************************************
������������ָ����ַд���ݣ�PCF8563��
��ڲ�����uint8 addr, uint8 *p, uint8 number
����ֵ����
 **************************************************************************************/
void WriteNbyte(uint8 addr, uint8* p, uint8 number)			/*	WordAddress,First Data Address,Byte lenth	*/
															//F0=0,right, F0=1,error
{
	I2C_Start();
	I2C_SendByte(SLAW);
	I2C_Check_ACK();
	if (!F0)
	{
		I2C_SendByte(addr);
		I2C_Check_ACK();
		if (!F0)
		{
			do
			{
				I2C_SendByte(*p);		p++;
				I2C_Check_ACK();
				if (F0)	break;
			} while (--number);
		}
	}
	I2C_Stop();
}

/**************************************************************************************
������������ָ����ַ��ȡ���ݣ�PCF8563��
��ڲ�����uint8 addr, uint8 *p, uint8 number
����ֵ����
 **************************************************************************************/
void ReadNbyte(uint8 addr, uint8* p, uint8 number)		/*	WordAddress,First Data Address,Byte lenth	*/
{
	I2C_Start();
	I2C_SendByte(SLAW);
	I2C_Check_ACK();
	if (!F0)
	{
		I2C_SendByte(addr);
		I2C_Check_ACK();
		if (!F0)
		{
			I2C_Start();
			I2C_SendByte(SLAR);
			I2C_Check_ACK();
			if (!F0)
			{
				do
				{
					*p = I2C_ReceiveByte();	p++;
					if (number != 1)		I2C_Ack();	//send ACK
				} while (--number);
				I2C_NoAck();			//send no ACK
			}
		}
	}
	I2C_Stop();
}

/**************************************************************************************
������������RTC����
��ڲ�������
����ֵ����
 **************************************************************************************/
void	ReadRTC(void)
{
	uint8	tmp[7];

	ReadNbyte(2, tmp, 7);
	second = ((tmp[0] >> 4) & 0x07) * 10 + (tmp[0] & 0x0f);
	minute = ((tmp[1] >> 4) & 0x07) * 10 + (tmp[1] & 0x0f);
	hour = ((tmp[2] >> 4) & 0x03) * 10 + (tmp[2] & 0x0f);
	day = ((tmp[3] >> 4) & 0x03) * 10 + (tmp[3] & 0x0f);
	myweek = (tmp[4] & 0x07);
	month = ((tmp[5] >> 4) & 0x01) * 10 + (tmp[5] & 0x0f);
	year = ((tmp[6] >> 4) & 0x0f) * 10 + (tmp[6] & 0x0f);
}

/***************************************************************************************
����������дRTC����
��ڲ�������
����ֵ����
***************************************************************************************/
void	WriteRTC(void)
{
	uint8	tmp[7];

	tmp[0] = ((second / 10) << 4) + (second % 10);
	tmp[1] = ((minute / 10) << 4) + (minute % 10);
	tmp[2] = ((hour / 10) << 4) + (hour % 10);
	tmp[3] = ((day / 10) << 4) + (day % 10);
	tmp[4] = (myweek % 10);
	tmp[5] = ((month / 10) << 4) + (month % 10);
	tmp[6] = ((year / 10) << 4) + (year % 10);
	WriteNbyte(2, tmp, 7);
}

/**************************************************************************************
����������PCF8563ʱ�ӳ�ʼ��
��ڲ�������
����ֵ����
 **************************************************************************************/
void PCF8563_Init(void)
{
	ErrFlag = 0;
	ReadRTC();          //��PCF8563ʱ��ֵ��������ʱ���֡���

	if (second >= 60)	ErrFlag = 1;	   //�������볬����Χ���жϳ���
	if (minute >= 60)	ErrFlag = 1;	   //�����ķֳ�����Χ���жϳ���
	if (hour >= 24)	ErrFlag = 1;	   //������ʱ������Χ���жϳ���
	if (myweek >= 7)	ErrFlag = 1;	 //�������ܳ�����Χ���жϳ���
	if (day > 31)	ErrFlag = 1;	   //�������쳬����Χ���жϳ���
	if (month >= 12)	ErrFlag = 1;	   //�������³�����Χ���жϳ���
	if (year > 99)	ErrFlag = 1;	   //�������곬����Χ���жϳ���

		//year = 21;
		//month = 5;
		//day = 6;
		//myweek = 2;
		//hour = 20;
		//minute = 30;
		//second = 0;
		//WriteRTC();             //���µ�ʱ��ֵд��PCF8563ģ��
}

/**************************************************************************************
����������PCF8563ʱ����Ϣ����
��ڲ�������
����ֵ����
 **************************************************************************************/
void PCF8563_TEST(void)
{
	memset(time, 0, 6);                   //��temp�����ʼ�������㣩
	memset(date, 0, 9);                   //��date�����ʼ�������㣩

	ReadRTC();			                      //��PCF8563ʱ��ֵ��������ʱ���֡���

	if (hour >= 10)	time[0] = hour / 10 + 0x30;
	else			time[0] = 0 + 0x30;
	time[1] = hour % 10 + 0x30;
	time[2] = 10 + 0x30;            // 10+48ֵΪ58  ��ӦASCII�룺
	time[3] = minute / 10 + 0x30;
	time[4] = minute % 10 + 0x30;
	
	date[0] = year / 10 + 0x30;
	date[1] = year % 10 + 0x30;
	date[2] = -3 + 0x30;             // 10+48ֵΪ58  ��ӦASCII��-
	date[3] = month / 10 + 0x30;
	date[4] = month % 10 + 0x30;
	date[5] = -3 + 0x30;             // 10+48ֵΪ58  ��ӦASCII��-
	date[6] = day / 10 + 0x30;
	date[7] = day % 10 + 0x30;

	//ʵʱʱ��ʱ���֡�����Ϣ������ʾ
	SendStringByUart1("The time is:");
	SendStringByUart1(time);
	SendStringByUart1("\r\n");
	SendStringByUart1("The date is:");
	SendStringByUart1(date);
	SendStringByUart1("\r\n");	            //���ͻ��з�	
}
/*********************************END FILE********************************************/

