#ifndef _ds18b20_H
#define _ds18b20_H

#include "stm32f10x.h"

void us_delay(int us);
unsigned int ds18b20_reset(void);
uint8_t ds18b20_read_byte(void); //读一字节
void ds18b20_write_byte(uint8_t dat); //写一个字节
u16 get_temperature(void);
void DS18B20GPIO_Configuration(void);
void DS18B20_thread_entry(void *parameter);
void UartPrint_thread_entry(void *parameter);

//void LED_Configuration(void);
//void LED_ON(void);
//void LED_OFF(void);
// 
#define US_1	9
#define	US_5	90
#define US_13	132
#define US_15	155
#define US_45	462
#define US_60	620
#define	US_480	4945
#define US_240	2475
#define	MS_1	10274


#endif
