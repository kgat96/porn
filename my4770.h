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
// File Name	: my4770.h
// Author		: Kage
// Version 		: V0.01
// Create Date	: 
// Description 	: The file is used to define my function
// Changes		: 
//--------------------------------------------------------------------------

#ifndef __MY4770_H__
#define __MY4770_H__

#ifndef __JZ4770_H__
	#error	"### please include jz477.h before ###"
#endif

#define NULL (void *)0

//#define REG8(addr)	*((v8 *)(addr))
//#define REG16(addr)	*((v16 *)(addr))
//#define REG32(addr)	*((v32 *)(addr))

#define rCPCCR		(*(v32 *)0xb0000000)
#define rCPPCR		(*(v32 *)0xb0000010)
#define rCPPCR1		(*(v32 *)0xb0000030)
#define rCPPSR		(*(v32 *)0xb0000014)

#define rCLKGR		(*(v32 *)0xb0000020)
#define rCLKGR1		(*(v32 *)0xb0000028)
#define rOPCR		(*(v32 *)0xb0000024)

#define DDR_BASE_ADDR 0xb3020000

#define rDSTATUS	(*(v32 *)(DDR_BASE_ADDR + 0))
#define rDCFG		(*(v32 *)(DDR_BASE_ADDR + 0x4))
#define rDCTRL		(*(v32 *)(DDR_BASE_ADDR + 0x8))
#define rDLMR		(*(v32 *)(DDR_BASE_ADDR + 0xc))
#define rDTIMING1	(*(v32 *)(DDR_BASE_ADDR + 0x10))
#define rDTIMING2	(*(v32 *)(DDR_BASE_ADDR + 0x14))
#define rDREFCNT	(*(v32 *)(DDR_BASE_ADDR + 0x18))
#define rDDQS		(*(v32 *)(DDR_BASE_ADDR + 0x1c))
#define rDDQSADJ	(*(v32 *)(DDR_BASE_ADDR + 0x20))
#define rDMMAP0		(*(v32 *)(DDR_BASE_ADDR + 0x24))
#define rDMMAP1		(*(v32 *)(DDR_BASE_ADDR + 0x28))
#define rDDELAYCTRL	(*(v32 *)(DDR_BASE_ADDR + 0x2c))
#define rPMEMCTRL3	(*(v32 *)(DDR_BASE_ADDR + 0x5c))

#define LCD_BASE_ADDR 0xb3050000

#define rLCDCFG		(*(v32 *)(LCD_BASE_ADDR + 0x0))
#define rLCDCTRL	(*(v32 *)(LCD_BASE_ADDR + 0x30))
#define rLCDOSDC	(*(v32 *)(LCD_BASE_ADDR + 0x100))
#define rLCDOSDCTRL (*(v32 *)(LCD_BASE_ADDR + 0x104))
#define rLCDALPHA	(*(v32 *)(LCD_BASE_ADDR + 0x118))
#define rLCDRGBC	(*(v32 *)(LCD_BASE_ADDR + 0x90))
#define rLCDVAT		(*(v32 *)(LCD_BASE_ADDR + 0xc))
#define rLCDDAH		(*(v32 *)(LCD_BASE_ADDR + 0x10))
#define rLCDDAV		(*(v32 *)(LCD_BASE_ADDR + 0x14))
#define rLCDSIZE0	(*(v32 *)(LCD_BASE_ADDR + 0x128))
#define rLCDSIZE0_P2	(*(v32 *)(LCD_BASE_ADDR + 0x1f4))

#define rLCDSIZE1	(*(v32 *)(LCD_BASE_ADDR + 0x12c))
#define rLCDVSYNC	(*(v32 *)(LCD_BASE_ADDR + 0x4))
#define rLCDHSYNC	(*(v32 *)(LCD_BASE_ADDR + 0x8))
#define rLCDDA0		(*(v32 *)(LCD_BASE_ADDR + 0x40))
#define rLCDDA1		(*(v32 *)(LCD_BASE_ADDR + 0x50))
#define rLCDDA0_P2	(*(v32 *)(LCD_BASE_ADDR + 0x1c0))

#define rLCDPCFG	(*(v32 *)(LCD_BASE_ADDR + 0x2c0))

#define rLCDIPUR	(*(v32 *)(LCD_BASE_ADDR + 0x11c))
#define rLCDSA0		(*(v32 *)(LCD_BASE_ADDR + 0x44))
#define rLCDCMD0	(*(v32 *)(LCD_BASE_ADDR + 0x4c))
#define rLCDSTATE	(*(v32 *)(LCD_BASE_ADDR + 0x34))

#define rLCDOSDS	(*(v32 *)(LCD_BASE_ADDR + 0x108))
#define rLCDBGC		(*(v32 *)(LCD_BASE_ADDR + 0x10c))
#define rLCDKEY0	(*(v32 *)(LCD_BASE_ADDR + 0x110))
#define rLCDKEY1	(*(v32 *)(LCD_BASE_ADDR + 0x114))

#define rLCDXYP0	(*(v32 *)(LCD_BASE_ADDR + 0x120))
#define rLCDXYP0_P2	(*(v32 *)(LCD_BASE_ADDR + 0x1f0))
#define rLCDXYP1	(*(v32 *)(LCD_BASE_ADDR + 0x124))

#define TCU_BASE_ADDR 0xb0002000
#define rTCUTCSR0	(*(v32 *)(TCU_BASE_ADDR + 0x4c))
#define rTCUTCSR1	(*(v32 *)(TCU_BASE_ADDR + 0x5c))


/*
 * Macros to access the system control coprocessor
 */

#define __read_32bit_c0_register(source, sel)				\
({ int __res;								\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mfc0\t%0, " #source "\n\t"			\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mfc0\t%0, " #source ", " #sel "\n\t"		\
			".set\tmips0\n\t"				\
			: "=r" (__res));				\
	__res;								\
})

#define __read_64bit_c0_register(source, sel)				\
({ unsigned long long __res;						\
	if (sizeof(unsigned long) == 4)					\
		__res = __read_64bit_c0_split(source, sel);		\
	else if (sel == 0)						\
		__asm__ __volatile__(					\
			".set\tmips3\n\t"				\
			"dmfc0\t%0, " #source "\n\t"			\
			".set\tmips0"					\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips64\n\t"				\
			"dmfc0\t%0, " #source ", " #sel "\n\t"		\
			".set\tmips0"					\
			: "=r" (__res));				\
	__res;								\
})

#define __write_32bit_c0_register(register, sel, value)			\
do {									\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mtc0\t%z0, " #register "\n\t"			\
			: : "Jr" ((unsigned int)(value)));		\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mtc0\t%z0, " #register ", " #sel "\n\t"	\
			".set\tmips0"					\
			: : "Jr" ((unsigned int)(value)));		\
} while (0)

#define __write_64bit_c0_register(register, sel, value)			\
do {									\
	if (sizeof(unsigned long) == 4)					\
		__write_64bit_c0_split(register, sel, value);		\
	else if (sel == 0)						\
		__asm__ __volatile__(					\
			".set\tmips3\n\t"				\
			"dmtc0\t%z0, " #register "\n\t"			\
			".set\tmips0"					\
			: : "Jr" (value));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips64\n\t"				\
			"dmtc0\t%z0, " #register ", " #sel "\n\t"	\
			".set\tmips0"					\
			: : "Jr" (value));				\
} while (0)

#define __read_ulong_c0_register(reg, sel)				\
	((sizeof(unsigned long) == 4) ?					\
	(unsigned long) __read_32bit_c0_register(reg, sel) :		\
	(unsigned long) __read_64bit_c0_register(reg, sel))

#define __write_ulong_c0_register(reg, sel, val)			\
do {									\
	if (sizeof(unsigned long) == 4)					\
		__write_32bit_c0_register(reg, sel, val);		\
	else								\
		__write_64bit_c0_register(reg, sel, val);		\
} while (0)


void delayus(v32 delays);
void delayms(v32 delays);
void init_pll(void);
void init_ddr(void);
void init_lcd(void);

#endif

