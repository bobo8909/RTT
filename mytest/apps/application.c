#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_DFS
/* dfs init */
#include <dfs_init.h>
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#include <dfs_posix.h>
#endif


#include "components.h"

#include "led.h"
#include "ds18b20.h"
extern void telnet_srv(void);

extern struct netif * netif_list;

//ALIGN(RT_ALIGN_SIZE)  

void rt_init_thread_entry(void* parameter)
{
#ifdef RT_USING_LWIP
    /* initialize eth interface 
    rt_hw_stm32_eth_init();*/
	 /* register ethernetif device */
    rt_hw_dm9000_init();
#endif

#ifdef RT_USING_I2C
    rt_i2c_core_init();
    //rt_hw_i2c_init();
#endif

#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

    rt_platform_init();
#if 1
    /* Filesystem Initialization */
#ifdef RT_USING_DFS
    {    
        rt_bool_t mount_flag = RT_FALSE;
       
        if (dfs_mount("flash0", "/", "elm", 0, 0) == 0)
        {
            mount_flag = RT_TRUE;
        }
        else
        {
            rt_kprintf("flash0 mount to / failed!, fatmat and try again!\r\n");

            /* fatmat filesystem. */
            dfs_mkfs("elm", "flash0");

            /* re-try mount. */
            if (dfs_mount("flash0", "/", "elm", 0, 0) == 0)
            {
                mount_flag = RT_TRUE;
            }
			else
			{
				rt_kprintf("flash0 mount to / failed!\r\n");
			}	
        }
				
        if(mount_flag == RT_TRUE)
        {
           rt_kprintf("flash0 mount to / ok \r\n");

            /* download resource from www.rt-thread.org */
            {
             //   resource_download();  
            }
						
			#if (RT_DFS_ELM_USE_LFN != 0) && (defined RT_DFS_ELM_CODE_PAGE_FILE)
            {
                extern void ff_convert_init(void);
                ff_convert_init();
            }
		    #endif	
		
            if (dfs_mount("sd0", "/SD", "elm", 0, 0) == 0)//  /SD
            {
                rt_kprintf("sd0 mount to /SD ok! \r\n");   
            }
            else
            {
                rt_kprintf("sd0 mount to /SD failed!\r\n");
            }
	
        }
        else
        {
            rt_kprintf("flash0 mount to / failed!\r\n");
        }
    }
#endif/* RT_USING_DFS */

#endif 
#if 0
if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)//	/SD
{
	rt_kprintf("sd0 mount to / ok! \r\n"); 
	#if (RT_DFS_ELM_USE_LFN != 0) && (defined RT_DFS_ELM_CODE_PAGE_FILE)
	{
		extern void ff_convert_init(void);
		ff_convert_init();
	}
	#endif				
}
else
{
	 rt_kprintf("sd0 mount to / failed!\r\n");
}
while( !(netif_list->flags & NETIF_FLAG_UP)) 
{	/*等待网络准备好*/
	rt_thread_delay( RT_TICK_PER_SECOND);
			
}	
rt_kprintf(" link ok \r\n");
//list_if();
#endif

#if defined(RT_USING_DFS)&&defined(RT_USING_LWIP)&&defined(RT_USING_DFS_NFS)
{
	/*NFS initialization*/	
	rt_kprintf("beginning init DFS..");
	nfs_init();

	if(dfs_mount(RT_NULL,"/NFS","nfs",0,RT_NFS_HOST_EXPORT)==0)
		rt_kprintf("NFS initialized!\n");
	else
		rt_kprintf("NFS initialized failed!\n");
	
}
#endif

#ifdef RT_USING_USB_DEVICE
    /* usb device controller driver initilize */
    rt_hw_usbd_init();

    rt_usb_device_init("usbd");
#endif /* RT_USING_USB_DEVICE */	

//	telnet_srv();
//	tcpclient("192.168.1.103",4663);

}


int rt_application_init()
{
	
	rt_thread_t init_thread;

	init_thread = rt_thread_create("init",
								rt_init_thread_entry, RT_NULL,
								2048, 8, 20);

	if (init_thread != RT_NULL)
		rt_thread_startup(init_thread);

	init_thread =rt_thread_create("led",
								led_thread_entry,RT_NULL,
								1024,4,21);

	if (init_thread != RT_NULL)
		rt_thread_startup(init_thread);

	init_thread=rt_thread_create("DS18B20",DS18B20_thread_entry,RT_NULL,512,6,18);
	if (init_thread !=RT_NULL)
		rt_thread_startup(init_thread);

  	init_thread=rt_thread_create("uartprint",UartPrint_thread_entry,RT_NULL,512,8,21);
		rt_thread_startup(init_thread);
//	demo_thread_creat();
	return 0;
}


