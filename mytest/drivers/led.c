/**********************************************************************************************************
*	                                  
*	模块名称 : LED驱动模块
*	文件名称 : led.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期        作者           说明
*		
*		v1.0    2012-11-19  wangkai        在官方基础上做调整,led当简易流水灯
*
*	Copyright (C), 2010-2011, UP MCU 工作室
*   淘宝店：   http://shop73275611.taobao.com
*   QQ交流群： 258043068
*
**********************************************************************************************************/
#include <rtthread.h>
#include <stm32f10x.h>
#include "led.h"

// led define

#define led1_rcc                    RCC_APB2Periph_GPIOF
#define led1_gpio                   GPIOF
#define led1_pin                    (GPIO_Pin_6)

#define led2_rcc                    RCC_APB2Periph_GPIOF
#define led2_gpio                   GPIOF
#define led2_pin                    (GPIO_Pin_7)

#define led3_rcc                    RCC_APB2Periph_GPIOF
#define led3_gpio                   GPIOF
#define led3_pin                    (GPIO_Pin_8)

#define led4_rcc                    RCC_APB2Periph_GPIOF
#define led4_gpio                   GPIOF
#define led4_pin                    (GPIO_Pin_9)


const rt_uint8_t ledTable[4][4]=
{
	{0x00, 0x01, 0x02, 0x03},
	{0x01, 0x00, 0x02, 0x03},
	{0x02, 0x01, 0x00, 0x03},
	{0x03, 0x01, 0x02, 0x00},
};

void led_thread_entry(void* parameter)
{
    rt_uint8_t count=0;
    rt_hw_led_init();

    while (1)
    {
        if (++count>=4) count = 0;
		
        rt_hw_led_on(ledTable[count][0]);
		    rt_hw_led_off(ledTable[count][1]);
		    rt_hw_led_off(ledTable[count][2]);
		    rt_hw_led_off(ledTable[count][3]);		
        rt_thread_delay( RT_TICK_PER_SECOND/2 ); /* sleep 0.5 second and switch to other thread */
    }
}

void rt_hw_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(led1_rcc|led2_rcc|led3_rcc|led4_rcc,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin   = led1_pin;
    GPIO_Init(led1_gpio, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = led2_pin;
    GPIO_Init(led2_gpio, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin   = led3_pin;
    GPIO_Init(led1_gpio, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = led4_pin;
    GPIO_Init(led2_gpio, &GPIO_InitStructure);
}

void rt_hw_led_off(rt_uint32_t n)
{
    switch (n)
    {
    case 0:
        GPIO_SetBits(led1_gpio, led1_pin);
        break;
    case 1:
        GPIO_SetBits(led2_gpio, led2_pin);
        break;
    case 2:
        GPIO_SetBits(led1_gpio, led3_pin);
        break;
    case 3:
        GPIO_SetBits(led2_gpio, led4_pin);
        break;	
    default:
        break;
    }
}

void rt_hw_led_on(rt_uint32_t n)
{
    switch (n)
    {
    case 0:
        GPIO_ResetBits(led1_gpio, led1_pin);
        break;
    case 1:
        GPIO_ResetBits(led2_gpio, led2_pin);
        break;
    case 2:
        GPIO_ResetBits(led1_gpio, led3_pin);
        break;
    case 3:
        GPIO_ResetBits(led2_gpio, led4_pin);
        break;	
    default:
        break;
    }
}


#ifdef RT_USING_FINSH
#include <finsh.h>
static rt_uint8_t led_inited = 0;
void led(rt_uint32_t led, rt_uint32_t value)
{
    /* init led configuration if it's not inited. */
    if (!led_inited)
    {
        rt_hw_led_init();
        led_inited = 1;
    }

    if ( led == 0 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(0);
            break;
        case 1:
            rt_hw_led_on(0);
            break;
        default:
            break;
        }
    }

    if ( led == 1 )
    {
        /* set led status */
        switch (value)
        {
        case 0:
            rt_hw_led_off(1);
            break;
        case 1:
            rt_hw_led_on(1);
            break;
        default:
            break;
        }
    }
}

FINSH_FUNCTION_EXPORT(led, set led[0 - 1] on[1] or off[0].)
#endif

