#ifndef __One_Wire_H
#define __One_Wire_H

void DHT11_Start(void);
uint8_t DHT11_ACK(void);
uint8_t DHT11_ReadByte(void);
void DHT11_Stop(void);
#endif
