/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "stm32f10x.h"
#include "stm32f10x_fsmc.h"
//#include "rtc_driver.h"

#ifdef  RT_USING_RTC
#include "rtc.h"
#endif

#ifdef  RT_USING_ALARM
#include "alarm.h"
#endif

#ifdef RT_USING_DFS
#include "spi_flash_w25qxx.h"
#endif
#include <drivers/mmcsd_core.h>
#include "board.h"


void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	    /*
     * set priority group:
     * 2 bits for pre-emption priority
     * 2 bits for subpriority
     */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


/**
 * This is the timer interrupt service routine.
 *
 */
void rt_hw_timer_handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

/**
 * This function will initial STM32 board.
 */
void rt_hw_board_init()
{
	SystemInit();
	/* NVIC Configuration */
	NVIC_Configuration();

    /* Configure the SysTick */
    SysTick_Config( SystemCoreClock / RT_TICK_PER_SECOND );

    rt_hw_usart_init();
#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
}

void rt_platform_init(void)
{
#ifdef RT_USING_RTC
    rt_hw_rtc_init();
	#ifdef RT_USING_ALARM	
	rt_alarm_system_init(); 
	#endif		
	  /* 放在finsh中去设置初始时间 
	  set_date(2013,4,28);
	  set_time(10,8,24);
	  */
	  list_date();	
	
#endif /* RT_USING_RTC */	
#ifdef RT_USING_SPI
	{
		extern void rt_hw_spi1_init(void);
		rt_hw_spi1_init();
	}
    

#ifdef RT_USING_DFS
       w25qxx_init("flash0", "spi10");
#endif /* RT_USING_DFS */

#ifdef RT_USING_RTGUI
    /* initilize touch panel */
    //rtgui_touch_hw_init("spi11");
#endif /* RT_USING_RTGUI */
#endif /* RT_USING_SPI */

#ifdef RT_USING_USB_HOST
    /* register stm32 usb host controller driver */
    //rt_hw_susb_init();
#endif

#ifdef RT_USING_DFS
    /* initilize sd card */
#ifdef RT_USING_SDIO
     rt_mmcsd_core_init();
     rt_mmcsd_blk_init();
	{
		extern rt_int32_t stm32f1xx_sdio_init(void);
		stm32f1xx_sdio_init();
	}	
     rt_thread_delay(RT_TICK_PER_SECOND);
#else
     rt_hw_sdcard_init();
#endif
#endif /* RT_USING_DFS */

#ifdef RT_USING_RTGUI
    /* initilize ili9320 lcd controller */
    //lcd_9320_init();

    /* initilize key module */
    //rt_hw_key_init();
#endif /* RT_USING_RTGUI */

    rt_thread_delay(50);
    rt_device_init_all();
}
