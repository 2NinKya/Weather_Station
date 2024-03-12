#include "stm32f10x.h"                 
#include "Delay.h"

/**
   *@brief 只在One_Wire中使用，用于切换IO的开漏输出模式
   *@param	无
   *@retval	无
   */
void DHT11_InitOut(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
/**
   *@brief 只在One_Wire中使用，用于切换IO的上拉输入模式
   *@param 无
   *@retval	无
   */
void DHT11_InitIn(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
/**
   *@brief 	主机发送，让DHT11开启发送数据
   *@param	无
   *@retval	无
   */
void DHT11_Start(void)
{
	DHT11_InitOut();
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	Delay_ms(30);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	Delay_us(30);
}
/**
   *@brief 接收DHT11发送的响应
   *@param	无
   *@retval	响应，接入时返回0，未接入时返回1
   */
uint8_t DHT11_ACK(void)
{	
	DHT11_InitIn();
	uint8_t Ack=1;									
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0)
	{
		Ack=0;
	}
	Delay_us(100);
	return Ack;	
}
/**
   *@brief 	读取8位数据，
   *@param	无
   *@retval	八位
   */
uint8_t DHT11_ReadByte(void)
{
	DHT11_InitIn();	
	uint8_t i, Byte=0;
	for(i=0;i<8;i++)
	{	
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 1);
		Delay_us(78);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 1)
		{
			Byte|=(uint8_t)(0x01<<(7-i));
		}
		else
		{
			Byte&=(uint8_t)~(0x01<<(7-i));
		}
	}
	return Byte;
}
/**
   *@brief	
   *@param
   *@retval
   */
void DHT11_Stop(void)
{
	DHT11_InitOut();
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
