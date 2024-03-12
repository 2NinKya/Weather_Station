#include "stm32f10x.h"                  // Device header
#include "DS1302.H"
#include "OLED.H"

/*配置Pin11--CE,Pin10--SCLK,Pin_1--IO 为输入输出模式*/
uint8_t Second,Minute,Hour,Date,Month,Day,Year;

void DS1302_OutInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);				
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void DS1302_InInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


void DS1302_Init(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);		//CE=0
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}
/**
   *@brief 	DS1302写入字节
   *@param	命令，数据	
   *@retval	无
   */
void DS1302_WriteByte(uint8_t Command,uint8_t Data)
{
	DS1302_OutInit();						//全配置成输出
	GPIO_SetBits(GPIOB,GPIO_Pin_11);		//CE=1,开始
	uint8_t i;
	for(i=0;i<8;i++)						
	{
		if(Command & (0x01<<i))				
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
		GPIO_SetBits(GPIOB,GPIO_Pin_10);		//SCLK=1
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);		//SCKL=0
	}
	for(i=0;i<8;i++)
	{
		if(Data & (0x01<<i))
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);		
	}		
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
}

uint8_t DS1302_ReadByte(uint8_t Command)
{
	DS1302_OutInit();
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	uint8_t i,Data=0x00;
	for(i=0;i<8;i++)
	{
		if(Command & (0x01<<i))
		{
			GPIO_SetBits(GPIOB,GPIO_Pin_1);
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		}
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);		
		GPIO_SetBits(GPIOB,GPIO_Pin_10);		
	}
	DS1302_InInit();
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1))
		{
			Data|=(0x01<<i);
		}
	}
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);	
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);	
	return Data;
}
/**
   *@brief 开启写保护和设定时间
   *@param	无	
   *@retval	无
   */
void DS1302_SetTime(void)
{
	DS1302_OutInit();					//先将GPIO口设置为输入模式
	DS1302_WriteByte(0x8E,0x80); 		//写保护
	DS1302_WriteByte(0x80,0x20);		//秒
	DS1302_WriteByte(0x82,0x19);		//分
	DS1302_WriteByte(0x84,0x18);		//时
	DS1302_WriteByte(0x86,0x07);		//日
	DS1302_WriteByte(0x88,0x06);		//月
	DS1302_WriteByte(0x8A,0x02);		//星期
	DS1302_WriteByte(0x8C,0x22);		//年
}
/**
   *@brief 读取寄存器，显示时间，同时进行BCD转十进制，并进行星期转换
   *@param	无	
   *@retval	无
   */
void DS1302_ShowTime(void)
{
		Second=DS1302_ReadByte(0x81);
		Minute=DS1302_ReadByte(0x83);
		Hour=DS1302_ReadByte(0x85);
		Date=DS1302_ReadByte(0x87);
		Month=DS1302_ReadByte(0x89);
		Day=DS1302_ReadByte(0x8B);
		Year=DS1302_ReadByte(0x8D);
	
		OLED_ShowNum(1,1,2000+Year/16*10+Year%16,4);
		OLED_ShowString(1,5,"-");
		OLED_ShowNum(1,6,Month/16*10+Month%16,2);
		OLED_ShowString(1,8,"-");
		OLED_ShowNum(1,9,Date/16*10+Date%16,2);
		OLED_ShowNum(2,7,Second/16*10+Second%16,2);
		OLED_ShowNum(2,4,Minute/16*10+Minute%16,2);
		OLED_ShowString(2,6,"-");		
		OLED_ShowNum(2,1,Hour/16*10+Hour%16,2);	
		OLED_ShowString(2,3,"-");
		OLED_ShowString(2,9,"  ");    					//这个是用来清除的
		switch(Day)
		{
			case 1:OLED_ShowString(1,14,"Mon");break;
			case 2:OLED_ShowString(1,13,"Tues");break;
			case 3:OLED_ShowString(1,14,"Wed");break;
			case 4:OLED_ShowString(1,13,"Thur");break;
			case 5:OLED_ShowString(1,14,"Fri");break;
			case 6:OLED_ShowString(1,14,"Sat");break;
			case 7:OLED_ShowString(1,14,"Sun");break;
		}
}


