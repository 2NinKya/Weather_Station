#include "stm32f10x.h"                  // Device header
#include "AD.H"
#include "OLED.H"
#include "Delay.h"
#include "One_Wire.H"

uint8_t Buf[5];
void DHT11_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line13 | EXTI_Line14;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;	
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI15_10_IRQHandler(void)
{
	OLED_Init();
	AD_Init();
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == RESET)
	{
			OLED_Clear();
			OLED_ShowString(1,1,"Air Quality:");
			OLED_ShowNum(2,1,AD_GetValue(),4);
			Delay_s(1);
			OLED_Clear();	
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == RESET)
	{
		DHT11_Start();
		if(DHT11_ACK() == 0)
		{
			Buf[0]=DHT11_ReadByte();
			Buf[1]=DHT11_ReadByte();
			Buf[2]=DHT11_ReadByte();
			Buf[3]=DHT11_ReadByte();
			Buf[4]=DHT11_ReadByte();
		}
		if(Buf[0]+Buf[1]+Buf[2]+Buf[3]==Buf[4])
		{
			OLED_Clear();
			OLED_ShowString(1,1,"Humi:  %");
			OLED_ShowString(2,1,"Temp:  C");
			OLED_ShowNum(1,6,Buf[0],2);
			OLED_ShowNum(2,6,Buf[2],2);	
			Delay_s(1);				
		}
		DHT11_Stop();
	}
}

