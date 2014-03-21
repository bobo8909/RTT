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
	
	rt_kprintf("\n\r UP MCU ������   \r\n");
	rt_kprintf(" ħ��stm32 RT-thread����ƽ̨ \r\n");
	rt_kprintf(" QQȺ��258043068   \r\n");
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
	/* ��ʼ��Ӳ��ƽ̨��أ�ʱ�����á��ж����á�ϵͳ�δ����á��������� */
	rt_hw_board_init();

	
	up_mcu_show();
	/* ��ӡRT-Thread�汾��Ϣ */
	rt_show_version();

	/* init tick */
	rt_system_tick_init();

	/* �ں˶����ʼ�� */
	rt_system_object_init();

	/* ϵͳ��ʱ����ʼ�� */
	rt_system_timer_init();

	/* ���ʹ�ö�̬�ڴ���䣬������֮ */
#if STM32_EXT_SRAM
	fsmc_bus_init();	
    FSMC_SRAM_Init();
	sram_test();
    rt_system_heap_init((void*)STM32_EXT_SRAM_BEGIN,
                        (void*)STM32_EXT_SRAM_END);
#else
    rt_system_heap_init((void*)STM32_SRAM_BEGIN, (void*)STM32_SRAM_END);
#endif /* STM32_EXT_SRAM */

   
	/* ϵͳ��������ʼ�� */
	rt_system_scheduler_init();


	/* ϵͳ�豸�����ʼ�� */
	rt_device_init_all();

	/* �û�Ӧ�ó�ʼ�� */
	rt_application_init();


  	/* ��ʼ�������ʱ�� */
  	rt_system_timer_thread_init();

	/* ��ʼ�������߳� */
	rt_thread_idle_init();

	/* ��ʼ�̵߳��� �˺���������̵߳�����ѭ�� */
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
