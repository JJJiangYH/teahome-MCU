#include  "ds18b20.h" 
#include  "uart.h"	
#include 	"oled.h"

static uint8 ReadTempFlag;
uint8 temp[6];
uint8 constantTemp[3];
float fTemp;

/******************************************************************************
 * 描  述 : ds18b20初始化
 * 入  参 : 无
 * 返回值 : 初始化成功:0
 *          初始化失败:1
 *****************************************************************************/
uint8 Init_Ds18b20(void)
{
    uint8 Status = 0;
    uint16 CONT_1 = 0;
    uint8 Flag_1 = 1;

    DATA_PIN = 1;            //主机拉高
     delay_10us(26);          //延时约260us
    DATA_PIN = 0;            //主机拉低
    delay_10us(75);          //延时约750us 大于 480us 小于960us
    DATA_PIN = 1;            //主机拉高
    while((DATA_PIN)&&(Flag_1 == 1))       //等待ds18b20响应，具有防止超时功能
    {                                      //等待约60ms左右
        CONT_1++;
        delay_10us(1);                     //延时约10us
        if(CONT_1 > 8000)Flag_1 = 0;
        Status = DATA_PIN;
    }
    DATA_PIN = 1;               //主机拉高
    delay_10us(10);             //延时约100us
    memset(constantTemp, 0, 3);
    
    return Status;       //返回初始化状态
}

/******************************************************************************
 * 描  述 : Ds18b20写操作：写入1个字节
 * 入  参 : 无
 * 返回值 : 无
 *****************************************************************************/
void Ds18b20Write(uint8 dat)
{
    uint16 i;
    
    for(i=0;i<8;i++)
    {
        if((dat & 0x01))
        {
            DATA_PIN = 0;       //主机拉低
            Delay1us();         //延时约1us
            DATA_PIN = 1;       //主机拉高
            delay_10us(6);      //延时约60us
        }
        else
        {
            DATA_PIN = 0;      //主机拉低
            delay_10us(6);     //延时约60us
            DATA_PIN = 1;      //主机拉高
            Delay1us();        //延时约1us
        }
        dat >>= 1;
    }
}

/******************************************************************************
 * 描  述 : Ds18b20读操作：读出1个字节
 * 入  参 : 无
 * 返回值 : 读取的1字节数据
 *****************************************************************************/
uint8 Ds18b20Read(void)
{
    uint8 Value = 0x00;
    uint16 i;

    DATA_PIN = 1;           //主机拉高
    delay_10us(1);          //延时约10us
    for(i=0;i<8;i++)
    {
        Value >>= 1; 
        DATA_PIN = 0;       //主机拉低，给脉冲信号
        Delay1us();         //延时约1us
        DATA_PIN = 1;       //主机拉高，给脉冲信号
        Delay1us();         //延时约1us
        if(DATA_PIN) Value |= 0x80;
        delay_10us(6);     //延时约60us
    } 
    return Value;
}

/******************************************************************************
 * 描  述 : 温度读取函数 带1位小数位
 * 入  参 : 无
 * 返回值 : 温度值
 *****************************************************************************/
float floatReadDs18B20(void)
{
    uint8 V1, V2;     //定义高低8位 缓冲
    uint16 temp;     //定义温度缓冲寄存器
    float fValue;

    Init_Ds18b20();
    Ds18b20Write(0xcc);    // 跳过读序号列号的操作
    Ds18b20Write(0x44);    // 启动温度转换

    Init_Ds18b20();
    Ds18b20Write(0xcc);    //跳过读序号列号的操作 
    Ds18b20Write(0xbe);    //读取温度寄存器等（共可读9个寄存器） 前两个就是温度

    V1 = Ds18b20Read();    //温度值低位
    V2 = Ds18b20Read();    //温度值高位

    ReadTempFlag = 0;    //非负温温时，将ReadTempFlag标志位赋值为0
    temp = ((V2 << 8) | V1);

    fValue = temp * 0.0625;   //计算温度值

    return fValue;          //返回温度值
}

/**************************************************************************************
 * 描  述 : 串口实时显示温湿度
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void DS18B20_TEST(void)
{
    memset(temp, 0, sizeof(temp));                  //将temp数组初始化（清零）

    fTemp = floatReadDs18B20();                    //温度读取函数

    if (fTemp > 100)sprintf(temp, "%03.01f", fTemp);                //浮点数转成字符串 
    else sprintf(temp, "%02.02f", fTemp);
	
		IE2 &= 0xEF; 			                     //串口4中断关闭
	
    //串口打印温度值
    SendStringByUart4(temp);
		Uart4_Init();                          //串口4初始化，并打开串口4中断	
}
/*********************************END FILE*************************************/