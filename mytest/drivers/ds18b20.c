#include "ds18b20.h"
#include "stm32f10x.h"
#include "rtthread.h"

#define DS18B20GPIO GPIOD
#define DS18B20GPIO_PIN GPIO_Pin_7
void us_delay( int us)
{ 	//us *= 12;
	while(us--);
}
unsigned int  ds18b20_reset(void)
{
	unsigned int flag;
//	GPIO_Configuration();
	GPIO_SetBits(DS18B20GPIO, DS18B20GPIO_PIN);
	us_delay(MS_1);//1ms
	GPIO_ResetBits(DS18B20GPIO, DS18B20GPIO_PIN);
	us_delay(US_480);//480us
	GPIO_SetBits(DS18B20GPIO, DS18B20GPIO_PIN);
	us_delay(US_60);//60us
	flag=GPIO_ReadInputDataBit( DS18B20GPIO, DS18B20GPIO_PIN);
	while(GPIO_ReadInputDataBit( DS18B20GPIO, DS18B20GPIO_PIN)){
//		j++;
//		if(j>=2000)
//		break;
	}
	us_delay(US_240);
	GPIO_SetBits(DS18B20GPIO, DS18B20GPIO_PIN);
	return flag;
}


u8 ds18b20_read_byte(void)
{
	u8 i;
	u8 data=0;
	GPIO_SetBits(DS18B20GPIO,DS18B20GPIO_PIN);
	us_delay(MS_1);
	for(i=0;i<8;i++)
	{	
		data>>=1;
		GPIO_ResetBits( DS18B20GPIO, DS18B20GPIO_PIN);
		us_delay(US_1);//1us
		GPIO_SetBits(DS18B20GPIO,DS18B20GPIO_PIN);
		us_delay(US_13);//15us
		if(GPIO_ReadInputDataBit( DS18B20GPIO, DS18B20GPIO_PIN))
		data|=0x80;//读1
		else
		data&=0x7f;//读0
		us_delay(US_45);
		GPIO_SetBits(DS18B20GPIO,DS18B20GPIO_PIN);

	}
	return data;
}

void ds18b20_write_byte(u8 dat)
{
	u8 i;
	u8 one_bit;
	DS18B20GPIO_Configuration();

	for(i = 0; i < 8; i ++)
	{
		GPIO_SetBits(DS18B20GPIO,DS18B20GPIO_PIN);
		us_delay(MS_1);
		one_bit = dat & 0x01;
		dat = dat >> 1;
		GPIO_ResetBits( DS18B20GPIO, DS18B20GPIO_PIN);
		us_delay(US_15);//15us
		if(one_bit)//写1
		{
			GPIO_SetBits(DS18B20GPIO, DS18B20GPIO_PIN);
			us_delay(US_45); //45us
		}
		else//写0
		{
			GPIO_ResetBits(DS18B20GPIO,DS18B20GPIO_PIN);
			us_delay(US_45) ; //45us
			GPIO_SetBits(DS18B20GPIO, DS18B20GPIO_PIN);
		}
//	us_delay(100) ;
//		dat = dat >> 1;

	}
}

uint16_t get_temperature(void)
{
//unsigned int temp,templ,temph;//必须为16位
   uint8_t templ,temph;
   uint16_t temp;
   ds18b20_reset();
   ds18b20_write_byte(0xcc);
   ds18b20_write_byte(0x44);//温度转换命令
   us_delay(MS_1);
   ds18b20_reset();
   ds18b20_write_byte(0xcc);
   ds18b20_write_byte(0xbe);//读温度命令
   
   templ=ds18b20_read_byte();//读取温度
   temph=ds18b20_read_byte();
   ds18b20_reset();
 
//   temp=(temph<<8)|templ;
   if(temph&0x80)
   {
		temp=temph<<8|templ;
   		temp=~temp+1;
   }
   else
   		temp=temph<<8|templ;
   return temp;
}

void DS18B20GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure ;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(DS18B20GPIO, &GPIO_InitStructure);
}

float avrtemp=0;
float temp=0;
char tempbuf[10]={0};
//static rt_base_t level;
static u8 dispbuf[16];
//static rt_uint8_t DS18B20_stack[512];
//static struct rt_thread DS18B20_thread;
//static rt_uint8_t DS18B20;
void DS18B20_thread_entry(void *parameter)
{		
	u8 i=0;
	DS18B20GPIO_Configuration();
	rt_kprintf("ds18b20 init\n");

	while(1)
	{
		for(i=0;i<10;i++)
		{
			temp=get_temperature();
			temp=temp*0.0625;
			tempbuf[i]=temp;
			if(i==9)
			{
				i=0;
				break;
			}
			rt_thread_delay(1);
		}
		for(i=0;i<10;i++)
		avrtemp+=tempbuf[i];
		avrtemp=avrtemp/10;
		rt_thread_delay(5);
	}
}

void UartPrint_thread_entry(void *parameter)
{
	while(1)
		{
			sprintf((char*)dispbuf,"T1:%3.1f\n",avrtemp);
		rt_kprintf("%s",(char*)dispbuf);	
		rt_thread_delay(1000);
	}
}
