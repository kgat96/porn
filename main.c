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

void init_all(void);

void cmain(void)
{
	init_all();
							
	prints((u8 *)"\nMcu: System init OK!");

	while(1){
		delayms(500);
	
	}

}


void init_all(void)
{
	init_uart();
	init_pll();
	act8600_setting();
//	init_ddr();
//	init_lcd();
//	init_i2c();

}






















