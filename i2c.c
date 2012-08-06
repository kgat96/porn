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
// File Name	: i2c.c
// Author		: Kage
// Version 		: V0.01
// Create Date	: 
// Description 	: The file is used to define i2c function
// Changes		: 
//--------------------------------------------------------------------------
#include "jz4770.h"
#include "my4770.h"
#include "i2c.h"
#include "uart.h"

#define EEPROM_ADDR 0xa7

#define send_i2c(a)			{\
	while(!(rI2C0ST & 0x2));\
	rI2C0DC = a;}while(0)

void init_i2c(void)
{
	prints((u8 *)"\nMcu: init i2c");

	// init gpio PWM
	REG32(0xb0010318) = 0xc0000000;		// PDINTc
	REG32(0xb0010328) = 0xc0000000;		// PDMSKc
	REG32(0xb0010338) = 0xc0000000;		// PDPAT1c
	REG32(0xb0010348) = 0xc0000000;		// PDPAT0c
	
	sendhex32("\n", REG32(0xb0010310));
	sendhex32(NULL, REG32(0xb0010320));
	sendhex32(NULL, REG32(0xb0010330));
	sendhex32(NULL, REG32(0xb0010340));

	rI2C0CON = 0xe3;
//  rI2C0TAR = 0x5;
    rI2C0SAR = 0x55;
//  rI2C0DC = 0x234;
    rI2C0SHCNT = 0x234;
    rI2C0SLCNT = 0x2a0;
    rI2C0FHCNT = 0x3c;
    rI2C0FLCNT = 0x82;
//  rI2C0INTST = 0x0;
	rI2C0INTM = 0x0;

	rI2C0RXTL = 0xa;
    rI2C0TXTL = 0x0;

	rI2C0DMACR = 0;
	rI2C0DMATDLR = 0;
	rI2C0DMARDLR = 0;
	rI2C0SDASU = 0x33;
	rI2C0ACKGC = 1;
//	rI2C0ENBST = 1;
	rI2C0SDAHD = 0x131;

	rI2C0ENB = 1;
	delayus(10);

#if 1
	rI2C0TAR = 0x57;
	rI2C0CON = 0xe3;

	send_i2c(0x10);

	rI2C0TAR = 0x57;
	rI2C0CON = 0xe3;

	send_i2c(0x01);
	send_i2c(0x02);
	send_i2c(0x03);
	send_i2c(0x04);
	send_i2c(0x05);

	rI2C0CON = 0x63;
#else
	rI2C0TAR = 0x57;
	rI2C0DC = 0x00;
	rI2C0DC = 0x0e;
	
	rI2C0DC = 0x11;
	rI2C0DC = 0x22;
	rI2C0DC = 0x33;
	rI2C0DC = 0x44;
	rI2C0DC = 0x55;

	rI2C0CON = 0x63;
#endif
	sendhex32((u8 *)"\nrI2C0CON ", rI2C0CON);

	sendhex32((u8 *)"\nrI2C0ABTSRC ", rI2C0ABTSRC);
}
