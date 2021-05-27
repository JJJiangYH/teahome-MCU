#include  "timeControl.h"	
#include  "i2c.h"
#include "uart.h"
#include "oled.h"

volatile uint8	hour, minute, second;	       //RTC变量，时，分，秒
volatile uint8	myweek, day, month, year;	   //RTC变量，周，天，月，年
uint8	time[6];
uint8	date[9];
static uint8  LED8[8];		                 //显示存放的时，分，秒信息
static uint8  DAY[2];		                   //显示存放的天数信息
static uint8  MONTH[2];		                 //显示存放的月份信息
static uint8  YEAR[2];		                 //显示存放的年份信息
static uint8  ErrFlag;		                 //错误标志位

/**************************************************************************************
功能描述：向指定地址写数据（PCF8563）
入口参数：uint8 addr, uint8 *p, uint8 number
返回值：无
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
功能描述：向指定地址读取数据（PCF8563）
入口参数：uint8 addr, uint8 *p, uint8 number
返回值：无
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
功能描述：读RTC函数
入口参数：无
返回值：无
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
功能描述：写RTC函数
入口参数：无
返回值：无
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
功能描述：PCF8563时钟初始化
入口参数：无
返回值：无
 **************************************************************************************/
void PCF8563_Init(void)
{
	ErrFlag = 0;
	ReadRTC();          //读PCF8563时钟值，即读出时、分、秒

	if (second >= 60)	ErrFlag = 1;	   //读出的秒超出范围，判断出错
	if (minute >= 60)	ErrFlag = 1;	   //读出的分超出范围，判断出错
	if (hour >= 24)	ErrFlag = 1;	   //读出的时超出范围，判断出错
	if (myweek >= 7)	ErrFlag = 1;	 //读出的周超出范围，判断出错
	if (day > 31)	ErrFlag = 1;	   //读出的天超出范围，判断出错
	if (month >= 12)	ErrFlag = 1;	   //读出的月超出范围，判断出错
	if (year > 99)	ErrFlag = 1;	   //读出的年超出范围，判断出错

		//year = 21;
		//month = 5;
		//day = 6;
		//myweek = 2;
		//hour = 20;
		//minute = 30;
		//second = 0;
		//WriteRTC();             //把新的时钟值写进PCF8563模块
}

/**************************************************************************************
功能描述：PCF8563时钟信息处理
入口参数：无
返回值：无
 **************************************************************************************/
void PCF8563_TEST(void)
{
	memset(time, 0, 6);                   //将temp数组初始化（清零）
	memset(date, 0, 9);                   //将date数组初始化（清零）

	ReadRTC();			                      //读PCF8563时钟值，即读出时、分、秒

	if (hour >= 10)	time[0] = hour / 10 + 0x30;
	else			time[0] = 0 + 0x30;
	time[1] = hour % 10 + 0x30;
	time[2] = 10 + 0x30;            // 10+48值为58  对应ASCII码：
	time[3] = minute / 10 + 0x30;
	time[4] = minute % 10 + 0x30;
	
	date[0] = year / 10 + 0x30;
	date[1] = year % 10 + 0x30;
	date[2] = -3 + 0x30;             // 10+48值为58  对应ASCII码-
	date[3] = month / 10 + 0x30;
	date[4] = month % 10 + 0x30;
	date[5] = -3 + 0x30;             // 10+48值为58  对应ASCII码-
	date[6] = day / 10 + 0x30;
	date[7] = day % 10 + 0x30;

	//实时时钟时、分、秒信息串口显示
	SendStringByUart1("The time is:");
	SendStringByUart1(time);
	SendStringByUart1("\r\n");
	SendStringByUart1("The date is:");
	SendStringByUart1(date);
	SendStringByUart1("\r\n");	            //发送换行符	
}
/*********************************END FILE********************************************/

