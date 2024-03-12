#include "stm32f10x.h"                  // Device header
#include "OLED.H"
#include "Delay.h"
#include "AD.H"
#include "One_Wire.H"
#include "IR.H"

uint8_t Buf[5];
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}

void MQ135_Show(void)
{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == RESET)
	{
		OLED_Clear();
		OLED_ShowString(1,1,"AQI:");					//Air Quality Index
		OLED_ShowString(2,1,"GOOD 0--25");
		OLED_ShowString(3,1,"Moderate 26--77");
		OLED_ShowString(4,1,"Unhealthy 78--99");
		OLED_ShowNum(1,5,AD_GetValue()*100/4096,2);	
		Delay_s(2);
		OLED_Clear();
	}
}

void DHT11_Show(void)
{
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

