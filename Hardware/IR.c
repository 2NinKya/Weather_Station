#include "stm32f10x.h"                  // Device header
#include "OLED.H"
#include "Delay.h"
#include "AD.H"
#include "One_Wire.H"
#include "IR.H"
#include "DS1302.H"

uint16_t Counter;

void IR_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15);	//这个是用来配置某个引脚输入为中断，此处A15

	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line15;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//优先级分组
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
}


uint16_t Get_Counte(void)
{
	return Counter;
}

void EXTI15_10_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line15) == SET)		//判断中断标志位（判断是否产生中断）
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
		Counter++;
		if(Counter == 3){Counter=0;}
		else if(Counter == 1);
		else if(Counter == 2)
		{
			OLED_Clear();
			OLED_ShowString(1,1,"AQI:");					//Air Quality Index
			OLED_ShowString(2,1,"GOOD 0--25");
			OLED_ShowString(3,1,"Moderate 26--77");
			OLED_ShowString(4,1,"Unhealthy 78--99");
			OLED_ShowNum(1,5,AD_GetValue()*100/4096,2);			
		}
	}
}

