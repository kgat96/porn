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
#include "dma.h"

#define EEPROM_ADDR 0xa7

#define send_i2c(a)			{\
	while(!(rI2C0ST & 0x2));\
	rI2C0DC = a;}while(0)

static unsigned int i2c_rdq[3] = {
	0x29,                 /* 6'b101001 */
	0x2b,                 /* 6'b101011 */
	0x3b                  /* 6'b111011 */
};

u8 buf[20];
v8 vv = 0;

void init_i2c(void)
{
	prints((u8 *)"\nMcu: init i2c\n");

	int tmp = __read_32bit_c0_register($30,0);
	sendhex32(NULL, tmp);
	
	tmp = __read_32bit_c0_register($14 ,0);
	sendhex32(NULL, tmp);
//		prints((u8 *)"\n");

	REG32(0xb000100c) = 0x01800002;				// int enable
	sendhex32(NULL, REG32(0xb0001004));
	// init gpio PWM
	REG32(0xb0010318) = 0xc0000000;		// PDINTc
	REG32(0xb0010328) = 0xc0000000;		// PDMSKc
	REG32(0xb0010338) = 0xc0000000;		// PDPAT1c
	REG32(0xb0010348) = 0xc0000000;		// PDPAT0c

	rI2C0CON = 0xe5;
//  rI2C0TAR = 0x5;
    rI2C0SAR = 0x55;
//  rI2C0DC = 0x234;
    rI2C0SHCNT = 615;
    rI2C0SLCNT = 615;
    rI2C0FHCNT = 615;	// the same set to low speed
    rI2C0FLCNT = 615;
//  rI2C0INTST = 0x0;
	rI2C0INTM = 0x0;	// interrupt mask

	rI2C0RXTL = 0x1;
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
//	rI2C0CON = 0xe3;

/* init DMA */
	//dma_cache_inv((unsigned long)buf, length * sizeof(unsigned char));
	REG_DMAC_DMACKES(0) = 1;

	delayms(10);
	REG_DMAC_DCCSR(0) = 0;
	REG_DMAC_DRSR(0) = i2c_rdq[0];
	REG_DMAC_DSAR(0) = I2C0_BASE_ADDR + 0x10;
//	REG_DMAC_DTAR(0) = (unsigned long)buf;
	REG_DMAC_DTAR(0) = 0xa0004000;

	//REG_DMAC_DTCR(0) = 0;
	REG32(0xb3420008) = 1;
//	REG_DMAC_DCMD(0) = 2;

//	REG_DMAC_DCMD(0) = 
		DMAC_DCMD_DAI | DMAC_DCMD_SWDH_8 | DMAC_DCMD_DWDH_8 | DMAC_DCMD_DS_8BIT | 2;

	REG_DMAC_DCCSR(0) = DMAC_DCCSR_NDES | DMAC_DCCSR_EN;

	REG_DMAC_DMACR(0) = DMAC_DMACR_DMAE; /* global DMA enable bit */

	rI2C0DMARDLR = 2;
	rI2C0DMACR = 1;

	sendhex32("\nDma:", REG32(0xb0001010));
	sendhex32(NULL, REG32(0xb3420304));
	sendhex32(NULL, REG32(0xb3420010));

	send_i2c(0x0);

//	rI2C0TAR = 0x57;
//	rI2C0CON = 0xe3;

	send_i2c(0x100);
	send_i2c(0x100);
	send_i2c(0x100);
	send_i2c(0x100);
	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);
//	send_i2c(0x100);

	rI2C0CON = 0x63;
//while(!(rI2C0ST & 8));
	prints((u8 *)"WWWWWW");
	delayms(50);
//	sendhex32(NULL, REG32(0xb0001010));
//	delayms(500);
//sendhex32((u8 *)"\n ", rI2C0ST);
//sendhex32((u8 *)"\n5", rI2C0DC);
//sendhex32((u8 *)"\n5", rI2C0DC);	sendhex32(NULL, REG32(0xb0001010));

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
