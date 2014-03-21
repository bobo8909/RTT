
#ifndef __BOARD_H__
#define __BOARD_H__


// <e>Use external SRAM memory on the board
// 	<i>Enable External SRAM memory
#define STM32_EXT_SRAM          0
//	<o>Begin Address of External SRAM
#define STM32_EXT_SRAM_BEGIN    0x68000000 /* the begining address of external SRAM */
//	<o>End Address of External SRAM
//		<i>Default: 0x68080000
#define STM32_EXT_SRAM_END      0x68080000 /* the end address of external SRAM */

// <o> Internal SRAM memory size[Kbytes] <8-64>
//	<i>Default: 64
#define STM32_SRAM_SIZE         64
#define STM32_SRAM_END          (0x20000000 + STM32_SRAM_SIZE * 1024)

// <o> Console on USART: <0=> no console <1=>USART 1 <2=>USART 2 <3=> USART 3
// 	<i>Default: 1
#define RT_USING_UART1

// <o> Ethernet Interface: <0=> Microchip ENC28J60 <1=> Davicom DM9000A
#define STM32_ETH_IF			1


void rt_hw_board_init(void);
void rt_platform_init(void);
void rt_hw_usart_init(void);

/* SD Card init function */
void rt_hw_sdcard_init(void);
void rt_hw_dm9000_init(void);

#endif

