#include "stm32f10x.h"                 
#include "DS1302.H"
#include "OLED.H"
#include "AD.H"
#include "Key.h"
#include "Buzzer.h"
#include "IR.H"
#include "Delay.h"
#include "One_Wire.H"

int main(void)
{
	OLED_Init();

	
	DS1302_Init();
	Key_Init();
	AD_Init();
	Buzzer_Init();
	DS1302_SetTime();
	while(1)
	{
		DS1302_ShowTime();
		MQ135_Show();
		DHT11_Show();	
	}


