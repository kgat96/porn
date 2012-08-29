//--------------------------------------------------------------------------
//	Copyright(c) 2012, Kage <kgat96@gmial.com>
//--------------------------------------------------------------------------
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//--------------------------------------------------------------------------
// File Name	: main.c
// Author		: Kage
// Version 		: V0.01
// Create Date	: 
// Description 	: The file is used to define my function
// Changes		: 
//--------------------------------------------------------------------------

#include "jz4770.h"
#include "my4770.h"
#include "uart.h"
#include "act8600.h"
#include "i2c.h"
#include "dma.h"


extern v8 vv;
extern u8 buf[];

void init_all(void);

void cmain(void)
{
	int tmp;

	init_all();
							
	prints((u8 *)"\nMcu: System init OK!");
	
	while(1){
		delayms(500);
		sendhex32("\nmain while(1):", REG32(0xb0001010));
		sendhex32(NULL, REG32(0xb3420304));
		sendhex32(NULL, REG32(0xb3420010));
		sendhex32(NULL, rI2C0ST);
		if(vv){
			for(tmp =0; tmp < vv; tmp ++)
				sendhex16(*(buf + tmp));
		
		}
	}

}


void init_all(void)
{
	init_uart();
	init_pll();
	act8600_setting();
	init_ddr();
//	init_lcd();
	init_i2c();
//	init_pwm();
}


void cisr(void)
{
	u32 tmp;

	prints((u8 *)"Mcu: isr\n");
	tmp = REG32(0xb0001010);
	
	sendhex32((u8 *)"Pend: ", tmp);

	if(tmp & 0x2){
		prints((u8 *)"\nMcu: i2c0 irq\n");

//	sendhex32((u8 *)"\n5", rI2C0DC);
//	sendhex32((u8 *)"\n5", rI2C0DC);
//	sendhex32((u8 *)"\n5", rI2C0DC);
//	sendhex32((u8 *)"\n5", rI2C0DC);
//	sendhex32((u8 *)"\n5", rI2C0DC);
//	sendhex32((u8 *)"\n5", rI2C0DC);

		rI2C0INTM = 0;

	}

	if(tmp & 0x01000000){
		vv = 6;
		REG_DMAC_DCMD(0) = 0;
	}

//	tmp = __read_32bit_c0_register($13,0);
//	sendhex32(NULL, tmp);
//	tmp = __read_32bit_c0_register($12,0);
//	sendhex32(NULL, tmp);
//	tmp = __read_32bit_c0_register($8,0);
//	sendhex32(NULL, tmp);
}



void cc2(void)
{
	prints((u8 *)"\nMcu: cc2");
//	__asm__ __volatile__(
//		"eret\n\t"
//		);
}
void cc3(void)
{
	prints((u8 *)"\nMcu: cc3");
//	__asm__ __volatile__(
//		"eret\n\t"
//		);
}

void cc1(void)
{
	prints((u8 *)"\nMcu: cc1");
	rI2C0INTM = 0;
//	sendhex32((u8 *)"\nINTM:", rI2C0INTM);
//	__asm__ __volatile__(
//		"eret\n\t"
//		);
}














