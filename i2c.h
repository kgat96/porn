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
// File Name	: i2c.h
// Author		: Kage
// Version 		: V0.01
// Create Date	: 
// Description 	: The file is used to define i2c function
// Changes		: 
//--------------------------------------------------------------------------

#ifndef __I2C_H__
#define __I2C_H__

#define I2C0_BASE_ADDR 0xb0050000
#define I2C1_BASE_ADDR 0xb0051000
#define I2C2_BASE_ADDR 0xb0055000

#define rI2C0CON		(*(v32 *)(I2C0_BASE_ADDR + 0x0))	
#define rI2C0TAR		(*(v32 *)(I2C0_BASE_ADDR + 0x4))
#define rI2C0SAR		(*(v32 *)(I2C0_BASE_ADDR + 0x8))
#define rI2C0DC			(*(v32 *)(I2C0_BASE_ADDR + 0x10))
#define rI2C0SHCNT		(*(v32 *)(I2C0_BASE_ADDR + 0x14))
#define rI2C0SLCNT		(*(v32 *)(I2C0_BASE_ADDR + 0x18))
#define rI2C0FHCNT		(*(v32 *)(I2C0_BASE_ADDR + 0x1c))
#define rI2C0FLCNT		(*(v32 *)(I2C0_BASE_ADDR + 0x20))
#define rI2C0INTST		(*(v32 *)(I2C0_BASE_ADDR + 0x2c))
#define rI2C0INTM		(*(v32 *)(I2C0_BASE_ADDR + 0x30))

#define rI2C0RXTL		(*(v32 *)(I2C0_BASE_ADDR + 0x38))
#define rI2C0TXTL		(*(v32 *)(I2C0_BASE_ADDR + 0x3c))
#define rI2C0CINT		(*(v32 *)(I2C0_BASE_ADDR + 0x40))
#define rI2C0CRXUF      (*(v32 *)(I2C0_BASE_ADDR + 0x44))
#define rI2C0CRXOF      (*(v32 *)(I2C0_BASE_ADDR + 0x48))
#define rI2C0CTXOF      (*(v32 *)(I2C0_BASE_ADDR + 0x4c))

#define rI2C0CRXREQ     (*(v32 *)(I2C0_BASE_ADDR + 0x50))
#define rI2C0CTXABT     (*(v32 *)(I2C0_BASE_ADDR + 0x54))
#define rI2C0CRXDN      (*(v32 *)(I2C0_BASE_ADDR + 0x58))
#define rI2C0CACT       (*(v32 *)(I2C0_BASE_ADDR + 0x5c))
#define rI2C0CSTP       (*(v32 *)(I2C0_BASE_ADDR + 0x60))
#define rI2C0CSTT       (*(v32 *)(I2C0_BASE_ADDR + 0x64))
#define rI2C0CGC        (*(v32 *)(I2C0_BASE_ADDR + 0x68))
#define rI2C0ENB        (*(v32 *)(I2C0_BASE_ADDR + 0x6c))

#define rI2C0ST			(*(v32 *)(I2C0_BASE_ADDR + 0x70))
#define rI2C0ABTSRC     (*(v32 *)(I2C0_BASE_ADDR + 0x80))
#define rI2C0DMACR      (*(v32 *)(I2C0_BASE_ADDR + 0x88))
#define rI2C0DMATDLR    (*(v32 *)(I2C0_BASE_ADDR + 0x8c))
#define rI2C0DMARDLR    (*(v32 *)(I2C0_BASE_ADDR + 0x90))
#define rI2C0SDASU      (*(v32 *)(I2C0_BASE_ADDR + 0x94))
#define rI2C0ACKGC      (*(v32 *)(I2C0_BASE_ADDR + 0x98))
#define rI2C0ENBST      (*(v32 *)(I2C0_BASE_ADDR + 0x9c))
#define rI2C0SDAHD      (*(v32 *)(I2C0_BASE_ADDR + 0xd0))

void init_i2c(void);

#endif


