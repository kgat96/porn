/*
 * Include file for Ingenic Semiconductor's JZ4770 uart.
 */
#ifndef __UART_H__
#define __UART_H__

#ifndef __JZ4770_H__
	#error	"please include jz477.h before this file"
#endif

//void sendhex32(u32 dat);
void sendhex32(u8 *p, u32 dat);
void sendhex16(u32 dat);
void prints(u8 *s);
void init_uart(void);

#endif

