/*
 * File      : startup.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-08-31     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "stm32f10x.h"
#include "board.h"

#ifdef RT_USING_RTC
#include "rtc_driver.h"
#endif 


extern int  rt_application_init(void);
#ifdef RT_USING_FINSH
extern void finsh_system_init(void);
extern void finsh_set_device(const char* device);
#endif

#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define STM32_SRAM_BEGIN    (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="HEAP"
#define STM32_SRAM_BEGIN    (__segment_end("HEAP"))
#else
extern int __bss_end;
#define STM32_SRAM_BEGIN    (&__bss_end)
#endif

void up_mcu_show(void)
{
	uint8_t * sn = (uint8_t *)0x1FFFF7E8;
    uint32_t i;	
	
	rt_kprintf("\n\r UP MCU 工作室   \r\n");
	rt_kprintf(" 魔笛stm32 RT-thread体验平台 \r\n");
	rt_kprintf(" QQ群：258043068   \r\n");
	rt_kprintf("      \r\n");
	
	// show SN
    rt_kprintf("CPU SN: ");
    for(i=0;i<12;i++)
    {
        rt_kprintf("%02X",*sn++);
    }
   // rt_kprintf("\r\n");
}

void assert_failed(u8* file, u32 line)
{
	rt_kprintf("\n\r Wrong parameter value detected on\r\n");
	rt_kprintf("       file  %s\r\n", file);
	rt_kprintf("       line  %d\r\n", line);

	while (1) ;
}

/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
	/* 初始化硬件平台相关：时钟设置、中断设置、系统滴答设置、串口设置 */
	rt_hw_board_init();

	
	up_mcu_show();
	/* 打印RT-Thread版本信息 */
	rt_show_version();

	/* init tick */
	rt_system_tick_init();

	/* 内核对象初始化 */
	rt_system_object_init();

	/* 系统定时器初始化 */
	rt_system_timer_init();

	/* 如果使用动态内存分配，则配置之 */
#if STM32_EXT_SRAM
	fsmc_bus_init();	
    FSMC_SRAM_Init();
	sram_test();
    rt_system_heap_init((void*)STM32_EXT_SRAM_BEGIN,
                        (void*)STM32_EXT_SRAM_END);
#else
    rt_system_heap_init((void*)STM32_SRAM_BEGIN, (void*)STM32_SRAM_END);
#endif /* STM32_EXT_SRAM */

   
	/* 系统调度器初始化 */
	rt_system_scheduler_init();


	/* 系统设备对象初始化 */
	rt_device_init_all();

	/* 用户应用初始化 */
	rt_application_init();


  	/* 初始化软件定时器 */
  	rt_system_timer_thread_init();

	/* 初始化空闲线程 */
	rt_thread_idle_init();

	/* 开始线程调度 此后便进入各个线程的无限循环 */
	rt_system_scheduler_start();

	/* never reach here */
	return ;
}

int main(void)
{
	/* disable interrupt first */
	rt_hw_interrupt_disable();

	/* startup RT-Thread RTOS */
	rtthread_startup();

	return 0;
}

/*@}*/
