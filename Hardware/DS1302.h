#ifndef __DS1302_H
#define __DS1302_H

void DS1302_OutInit(void);
void DS1302_InInit(void);
void DS1302_WriteByte(uint8_t Command,uint8_t Data);
uint8_t DS1302_ReadByte(uint8_t Command);
void DS1302_Init(void);							//在主函数中只有调用这个三个函数即可显示
void DS1302_SetTime(void);						//其他函数是在子函数中使用
void DS1302_ShowTime(void);						//
#endif
