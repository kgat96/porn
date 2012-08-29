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
// File Name	: my4770.c
// Author		: Kage
// Version 		: V0.01
// Create Date	: 
// Description 	: The file is used to define my function
// Changes		: 
//--------------------------------------------------------------------------
#include "jz4770.h"
#include "my4770.h"
#include "uart.h"

#if 0
#define Index_Invalidate_I      0x00
#define Index_Writeback_Inv_D   0x01
#define Index_Invalidate_SI     0x02
#define Index_Writeback_Inv_SD  0x03
#define Index_Load_Tag_I	0x04
#define Index_Load_Tag_D	0x05
#define Index_Load_Tag_SI	0x06
#define Index_Load_Tag_SD	0x07
#define Index_Store_Tag_I	0x08
#define Index_Store_Tag_D	0x09
#define Index_Store_Tag_SI	0x0A
#define Index_Store_Tag_SD		0x0B
#define Create_Dirty_Excl_D		0x0d
#define Create_Dirty_Excl_SD	0x0f
#define Hit_Invalidate_I	0x10
#define Hit_Invalidate_D	0x11
#define Hit_Invalidate_SI	0x12
#define Hit_Invalidate_SD	0x13
#define Fill			0x14
#define Hit_Writeback_Inv_D	0x15
					/* 0x16 is unused */
#define Hit_Writeback_Inv_SD	0x17
#define Hit_Writeback_I		0x18
#define Hit_Writeback_D		0x19
					/* 0x1a is unused */
#define Hit_Writeback_SD	0x1b
					/* 0x1c is unused */
					/* 0x1e is unused */
#define Hit_Set_Virtual_SI	0x1e
#define Hit_Set_Virtual_SD	0x1f

#define CFG_DCACHE_SIZE		16384
#define CFG_ICACHE_SIZE		16384
#define CFG_CACHELINE_SIZE	32

#define K0BASE				0x80000000

void flush_icache_all(void)
{
	unsigned int addr, t = 0;

	asm volatile ("mtc0 $0, $28"); /* Clear Taglo */
	asm volatile ("mtc0 $0, $29"); /* Clear TagHi */

	for (addr = K0BASE; addr < K0BASE + CFG_ICACHE_SIZE;
	     addr += CFG_CACHELINE_SIZE) {
		asm volatile (
			".set mips3\n\t"
			" cache %0, 0(%1)\n\t"
			".set mips2\n\t"
			:
			: "I" (Index_Store_Tag_I), "r"(addr));
	}

	/* invalicate btb */
	asm volatile (
		".set mips32\n\t"
		"mfc0 %0, $16, 7\n\t"
		"nop\n\t"
		"ori %0,2\n\t"
		"mtc0 %0, $16, 7\n\t"
		".set mips2\n\t"
		:
		: "r" (t));
}

void flush_dcache_all(void)
{
	unsigned int addr;

	for (addr = K0BASE; addr < K0BASE + CFG_DCACHE_SIZE; 
	     addr += CFG_CACHELINE_SIZE) {
		asm volatile (
			".set mips3\n\t"
			" cache %0, 0(%1)\n\t"
			".set mips2\n\t"
			:
			: "I" (Index_Writeback_Inv_D), "r"(addr));
	}

	asm volatile ("sync");
}

void flush_cache_all(void)
{
	flush_dcache_all();
	flush_icache_all();
}
#endif

/* 600M --> (1÷600000000)×(40×15) = 1uS */
void delayus(v32 delays)
{
	v32 temp;
	for ( ; delays > 0; delays--) {
		for (temp = 0; temp < 75; temp++);
	}
}

void delayms(v32 delays)
{
	for ( ; delays > 0; delays--) {
		delayus(1000);
	}
}

void init_pll(void)
{

	prints((u8 *)"\n\nMcu: init pll0 & pll1");

	rCPCCR = 0x43431530;	// PLL0 984MHz

	rCPPCR = 0xd2008120;	// PLL1 432MHz

	rCPPCR1 = 0x23000085;

	delayus(100);

	while(!(rCPPSR & 0x3c000000));
	
	sendhex32((u8 *)"\nrCPCCR ", rCPCCR);
	sendhex32((u8 *)"\nrCPPR ", rCPPCR);
	sendhex32((u8 *)"\nrCPPCR1 ", rCPPCR1);

	rCLKGR = 0x2e5da680;
	rCLKGR1 = 0x6dff;
	rOPCR = 0x100015c4;

	sendhex32((u8 *)"\nrCLKGR ", rCLKGR);
	sendhex32((u8 *)"\nrCLKGR1 ", rCLKGR1);
	sendhex32((u8 *)"\nrOPCR ", rOPCR);
}

//#define CONFIG_DDR
#if 1
static void show_ddr(void)
{
	sendhex32((u8 *)"\nrDSTATUS ",   rDSTATUS    );
	sendhex32((u8 *)"\nrDCFG ",      rDCFG	   );
	sendhex32((u8 *)"\nrDCTRL ",     rDCTRL	   );
	sendhex32((u8 *)"\nrDLMR ",      rDLMR	   );
	sendhex32((u8 *)"\nrDTIMING1 ",  rDTIMING1   );
	sendhex32((u8 *)"\nrDTIMING2 ",  rDTIMING2   );
	sendhex32((u8 *)"\nrDREFCNT ",   rDREFCNT	   );
	sendhex32((u8 *)"\nrDDQS ",      rDDQS	   );
	sendhex32((u8 *)"\nrDDQSADJ ",   rDDQSADJ	   );
	sendhex32((u8 *)"\nrDMMAP0 ",    rDMMAP0	   );
	sendhex32((u8 *)"\nrDMMAP1 ",    rDMMAP1	   );
	sendhex32((u8 *)"\nrDDELAYCTRL ",rDDELAYCTRL );
	sendhex32((u8 *)"\nrPMEMCTRL3 ", rPMEMCTRL3  );
}

static void show_lcd(void)
{
	sendhex32((u8 *)"\nrLCDCFG ",		rLCDCFG		 );
	sendhex32((u8 *)"\nrLCDCTRL ",	rLCDCTRL	 );
	sendhex32((u8 *)"\nrLCDOSDC ",	rLCDOSDC	 );
	sendhex32((u8 *)"\nrLCDOSDCTRL ", rLCDOSDCTRL  );
	sendhex32((u8 *)"\nrLCDALPHA ",	rLCDALPHA	 );
	sendhex32((u8 *)"\nrLCDRGBC ",	rLCDRGBC	 );
	sendhex32((u8 *)"\nrLCDVAT ",		rLCDVAT		 );
	sendhex32((u8 *)"\nrLCDDAH ",		rLCDDAH		 );
	sendhex32((u8 *)"\nrLCDDAV ",		rLCDDAV		 );
	sendhex32((u8 *)"\nrLCDSIZE0 ",	rLCDSIZE0	 );
	sendhex32((u8 *)"\nrLCDSIZE1 ",	rLCDSIZE1	 );
	sendhex32((u8 *)"\nrLCDVSYNC ",	rLCDVSYNC	 );
	sendhex32((u8 *)"\nrLCDHSYNC ",	rLCDHSYNC	 );
	sendhex32((u8 *)"\nrLCDDA0 ",		rLCDDA0		 );
	sendhex32((u8 *)"\nrLCDPCFG ",	rLCDPCFG	 );
	
	sendhex32((u8 *)"\nrLCDSA0 ",	rLCDSA0	 );
	sendhex32((u8 *)"\nrLCDCMD0 ",	rLCDCMD0	 );
	sendhex32((u8 *)"\nrLCDSTATE ",	rLCDSTATE	 );

}

static void test_ddr(u32 dat)
{
	u32 *p = (u32 *)dat;
	u32 i;

	for(i=0; i<1024; i++){
		*(p+i) = i;
	}

	delayms(100);

	for(i=0; i<512; i++){
		//sendhex32("\n0x", *(p+i));
		if(*(p+i) != i){
//			prints((u8 *)"\n### Err: init ddr err! ###");
//			while(1);
			sendhex32((u8 *)"\n", *(p+i));
		}
	}

}

void init_ddr(void)
{
	prints((u8 *)"\nMcu: init ddr");

	rDCTRL	 	 = 0x1;

	rDSTATUS	 = 0x1;
	rDCFG		 = 0xca6f;
//	rDCTRL		 = 0x110;
	rDLMR		 = 0x0;
//	rDTIMING1	 = 0x61337232;
//	rDTIMING2	 = 0xf000f21;
//	rDREFCNT	 = 0x2f2503;
	rDDQS		 = 0x2800000;
	rDDQSADJ	 = 0x2325;
//	rDMMAP0		 = 0x20e0;
//	rDMMAP1		 = 0xff00;
	rDDELAYCTRL	 = 0x8040;
	rPMEMCTRL3	 = 0x20000;

	rDREFCNT	 = 0xff0000;	// auto-refresh counter

	/***** init ddrc registers & ddr memory regs ****/
	/* Wait for number of auto-refresh cycles */
	delayus(100);

	/* Set CKE High */
	rDCTRL = 2;

	/* Wait for number of auto-refresh cycles */
	delayus(10);

	/* PREA */
	rDLMR = 0x1;

	/* Wait for number of auto-refresh cycles */
	delayus(10);

	/* EMR2: extend mode register2 */
	rDLMR = 0x221;

	/* EMR3: extend mode register3 */
	rDLMR = 0x321;

	/* EMR1: extend mode register1 */
	rDLMR = 0x20121;
	/* wait DDR_tMRD */
	delayus(10);
	/* MR - DLL Reset A1A0 burst 2 */
	rDLMR = 0x7420021;
	/* wait DDR_tMRD */
	delayus(10);

	/* PREA */
	rDLMR = 0x1;
	/* Wait for DDR_tRP */
	delayus(10);

	/* AR: auto refresh */
	rDLMR = 0x11;
	/* Wait for DDR_tRP */
	delayus(10);
	/* AR: auto refresh */
	rDLMR = 0x11;
	/* Wait for DDR_tRP */
	delayus(10);

	/* MR - DLL Reset End */
	rDLMR = 0x6420021;

	/* wait 200 tCK */
	delayus(20);

	/* EMR1 - OCD Default */
	rDLMR = 0x3820121;
	/* EMR1 - OCD Exit */
	rDLMR = 0x20121;
	delayus(10);

//	show_ddr();
//	return;

	rDCTRL	 	 = 0x113;

	rDSTATUS	 = 0x1;
	rDCFG		 = 0xca6f;
	rDTIMING1	 = 0x61337232;
	rDTIMING2	 = 0xf080f21;
	rDDQS		 = 0x12910e0c;
	rDDQSADJ	 = 0x2325;
	rDMMAP0		 = 0x20e0;
	rDMMAP1		 = 0xff00;
	rDDELAYCTRL	 = 0x98040;
	rPMEMCTRL3	 = 0x20000;
	rDREFCNT	 = 0x2f2503;	// auto-refresh counter

	rDCTRL	 	 = 0x112;

	rDLMR = 0x11;

//	show_ddr();
	test_ddr(0xA0004000);
}
#endif

#if 0
/*
 * LCD controller stucture for JZSOC: JZ4730 JZ4740
 */

struct jz_fb_dma_des {
	u32	fdadr;		/* Frame descriptor address register */
   	u32 fsadr;		/* Frame source address register */
	u32	fidr;		/* Frame ID register */
	u32 ldcmd;		/* Command register */
	u32 offsize;    /* Stride Offsize(in word) */
	u32 page_width; /* Stride Pagewidth(in word) */
	u32 cmd_num; 	/* Command Number(for SLCD) */
	u32 desc_size; 	/* Foreground Size */
};

struct jz_fb_dse{
	u16 fix_hh;
	u16 fix_vv;

	u16 f0_hh;
	u16 f0_vv;

	u16 f1_hh;
	u16 f1_vv;

	u16 f2_hh;
	u16 f2_vv;

	u32 end_addr;
	u32 da0_addr;
	u32 da1_addr;
	u32 da2_addr;

	struct jz_fb_dma_des *fblow;
	struct jz_fb_dma_des *fbhight;
	struct jz_fb_dma_des *fbpale;
}my_fb_des;

#define LCD_FB_BASE	0xa0004000

void memclean(void *pdat, u32 dat, u32 len)
{
	u32 *p = (u32 *)pdat;
	u32 i;

	for(i=0; i<len; i++){
		*(p+i) = dat;
	}

}

void init_lcd(void)
{
	u32 size;
	u32 tmp;

	prints((u8 *)"\nMcu: init lcd");
	
	my_fb_des.fix_hh = 800;
	my_fb_des.fix_vv = 480;
	
	my_fb_des.f0_hh = 200;
	my_fb_des.f0_vv = 480;
	
	my_fb_des.f1_hh = 800;
	my_fb_des.f1_vv = 120;

	my_fb_des.f2_hh = 800;
	my_fb_des.f2_vv = 480;

	size = my_fb_des.fix_vv * my_fb_des.fix_hh;

	tmp = size * 12  + 3*32;

	my_fb_des.end_addr = LCD_FB_BASE + tmp;
	
	memclean((void *)LCD_FB_BASE, 0, tmp >> 2);

	my_fb_des.fblow = (struct jz_fb_dma_des *)(my_fb_des.end_addr - 3*32);
	my_fb_des.fbhight = (struct jz_fb_dma_des *)(my_fb_des.end_addr - 2*32);
	my_fb_des.fbpale = (struct jz_fb_dma_des *)(my_fb_des.end_addr - 1*32);

	my_fb_des.fbhight->fdadr = ((u32)my_fb_des.fbhight & 0x1fffffff);
	my_fb_des.fbhight->fsadr = (LCD_FB_BASE & 0x1fffffff);
	my_fb_des.fbhight->fidr = 0;
	my_fb_des.fbhight->ldcmd = my_fb_des.f0_vv * my_fb_des.f0_hh;
	my_fb_des.fbhight->page_width = 0;
	my_fb_des.fbhight->desc_size = my_fb_des.f0_hh | (my_fb_des.f0_vv << 16);

	my_fb_des.fblow->fdadr = ((u32)my_fb_des.fblow & 0x1fffffff);
	my_fb_des.fblow->fsadr = ((LCD_FB_BASE + size*4) & 0x1fffffff);
	my_fb_des.fblow->fidr = 1;
	my_fb_des.fblow->ldcmd = my_fb_des.f1_vv * my_fb_des.f1_hh;
	my_fb_des.fblow->page_width = 0;
	my_fb_des.fblow->desc_size = my_fb_des.f1_hh | (my_fb_des.f1_vv << 16);

	my_fb_des.fbpale->fdadr = ((u32)my_fb_des.fbpale & 0x1fffffff);
	my_fb_des.fbpale->fsadr = ((LCD_FB_BASE + size*8) & 0x1fffffff);
	my_fb_des.fbpale->fidr = 2;
	my_fb_des.fbpale->ldcmd = my_fb_des.f2_vv * my_fb_des.f2_hh;
	my_fb_des.fbpale->page_width = 0;
	my_fb_des.fbpale->desc_size = my_fb_des.f2_hh | (my_fb_des.f2_vv << 16);

	my_fb_des.da0_addr = my_fb_des.fbhight->fdadr;
	my_fb_des.da1_addr = my_fb_des.fblow->fdadr;
	my_fb_des.da2_addr = my_fb_des.fbpale->fdadr;

	// init gpio PWM
	REG32(0xb0010438) = 0x00002006;		// PEPAT1C
	REG32(0xb0010444) = 0x00002006;		// PEPAT0S

	// init gpio lcd dl
	REG32(0xb0010138) = 0x0c000000;		// PEPAT1C
	REG32(0xb0010144) = 0x04000000;		// PEPAT0S

	// init gpio glcd
	REG32(0xb0010218) = 0x0fffffff;		// PCINTC
	REG32(0xb0010228) = 0x0fffffff;		// PCMSKC
	REG32(0xb0010238) = 0x0fffffff;		// PCPAT1C
	REG32(0xb0010248) = 0x0fffffff;		// PCPAT0C
	// open lcd clock
	// __cpm_start_lcd();
	
	// set lcd clock

	REG32(0xb0000064) = 0x2000000c;

//	delayms(10);
	rLCDCTRL	= 0x40000005;

	// config 4 xboot
//	rLCDCFG		= 0x12000940;
//  rLCDOSDCTRL = 0x0;
//  rLCDOSDC	= 0x9;
//  rLCDALPHA	= 0xff;
//  rLCDRGBC	= 0x0;
//  rLCDVAT		= 0x3e8029b;
//  rLCDDAH		= 0x580378;
//  rLCDDAV		= 0x27027f;
//  rLCDSIZE0	= 0x2580320;
//	rLCDVSYNC	= 0x3;
//  rLCDHSYNC	= 0x28;
//	rLCDIPUR	= 0x80001000;

	// config 4 lod
//	rLCDCFG		=0x12000940;
//  rLCDOSDC	=0x0;
//  rLCDOSDCTRL =0x9;
//  rLCDALPHA	=0x5f;
//  rLCDRGBC	=0x0;
//  rLCDVAT		=0x420020d;
//  rLCDDAH		=0xd803f8;
//  rLCDDAV		=0x230203;
//	rLCDSIZE0	=0x1e00320;
//	rLCDSIZE1	=0x1e00120;
//  rLCDVSYNC	=0x2;
//  rLCDHSYNC	=0x80;
//	rLCDIPUR	= 0x80001000;
//  rLCDPCFG	=0xc7ff2100;

	rLCDCFG		= 0x10000940;
    rLCDOSDCTRL = 0xd;
	rLCDOSDC	= 0x1d;
    rLCDALPHA	= 0x5f;
    rLCDRGBC	= 0x0;

    rLCDVAT		= 0x420020d;
    rLCDDAH		= 0xd803f8;
    rLCDDAV		= 0x230203;

	rLCDSIZE0	= my_fb_des.f0_hh | (my_fb_des.f0_vv << 16);
	rLCDSIZE1	= my_fb_des.f1_hh | (my_fb_des.f1_vv << 16);
//	rLCDSIZE0_P2 = my_fb_des.f2_hh | (my_fb_des.f2_vv << 16);

    rLCDVSYNC	= 0x2;
    rLCDHSYNC	= 0x80;
	rLCDIPUR	= 0x80001000;
//	rLCDXYP0	= 0x800100;
//  rLCDXYP0_P2 = 0x800100;
	rLCDPCFG	=0xc7ff2100;

	rLCDBGC = 0xffffff;

	rLCDDA0		= my_fb_des.da0_addr;				// address
	rLCDDA1		= my_fb_des.da1_addr;				// address
//	rLCDDA0_P2  = my_fb_des.da2_addr;

	rLCDOSDCTRL = 0x5;
	rLCDCTRL	= 0x4000000d;
	
//	show_lcd();

	u32 *p = (u32 *)LCD_FB_BASE;
	u32 i,j;

	for(i=0; i<240; i++){
		for(j=0; j<800; j++){
			*p++ = 0xff0000;
		}
	}

	for(;i<480; i++){
		for(j=0; j<800; j++){
			*p++ = 0xff00;
		}
	}
	
	for(; i<720; i++){
		for(j=0; j<800; j++){
			*p++ = 0xff;
		}
	}
	
	for(; i<960; i++){
		for(j=0; j<800; j++){
			*p++ = 0xfff0;
		}
	}
//	u32 *p = (u32 *)LCD_FB_BASE;
//	u32 i;
//	for(i=0; i<size; i++){
//		*(p+i) = 0xf0ff00;
//	}
//
//	for(; i<(size*2); i++){
//		*(p+i) = 0xff00;
//	}
//
//	for(; i<(size*3); i++){
//		*(p+i) = 0xff0000;
//	}

}

#endif









void init_pwm(void)
{
	// init gpio func0
	REG32(0xb0010418) = 0x00000003;		// PEPAT1C
	REG32(0xb0010428) = 0x00000003;		// PEPAT0S
	REG32(0xb0010438) = 0x00000003;		// PEPAT1C
	REG32(0xb0010448) = 0x00000003;		// PEPAT0S

	REG32(0xb000205c) = 0x000002a4;		// PEPAT0S
	REG32(0xb0002050) = 0x1d4;			// PEPAT0S
	REG32(0xb0002054) = 0xea;			// PEPAT0S

	REG32(0xb000204c) = 0x000002a4;		// PEPAT0S
	REG32(0xb0002040) = 0xaff;			// PEPAT0S
	REG32(0xb0002044) = 0x5ff;			// PEPAT0S

	REG32(0xb0002014) = 0x0003;			// PEPAT0S
}


