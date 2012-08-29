/*
 * arch/mips/mach-jz4760/include/mach/chip-dmac.h
 *
 * JZ4760 DMAC register definition.
 *
 * Copyright (C) 2010 Ingenic Semiconductor Co., Ltd.
 */

#ifndef __CHIP_DMAC_H__
#define __CHIP_DMAC_H__

#define DMAC_BASE	0xB3420000

/*************************************************************************
 * DMAC (DMA Controller)
 *************************************************************************/

#define MAX_DMA_NUM 	12  /* max 12 channels */
#define MAX_BDMA_NUM	3   /* max 3  channels */
#define HALF_DMA_NUM	6   /* the number of one dma controller's channels */

/* m is the DMA controller index (0, 1), n is the DMA channel index (0 - 11) */

#define DMAC_DSAR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x00 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA source address */
#define DMAC_DTAR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x04 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA target address */
#define DMAC_DTCR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x08 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA transfer count */
#define DMAC_DRSR(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x0c + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA request source */
#define DMAC_DCCSR(n) (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x10 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA control/status */
#define DMAC_DCMD(n)  (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x14 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA command */
#define DMAC_DDA(n)   (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0x18 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x20)) /* DMA descriptor address */
#define DMAC_DSD(n)   (DMAC_BASE + ((n)/HALF_DMA_NUM*0x100 + 0xc0 + ((n)-(n)/HALF_DMA_NUM*HALF_DMA_NUM) * 0x04)) /* DMA Stride Address */

#define DMAC_DMACR(m)	(DMAC_BASE + 0x0300 + 0x100 * (m))              /* DMA control register */
#define DMAC_DMAIPR(m)	(DMAC_BASE + 0x0304 + 0x100 * (m))              /* DMA interrupt pending */
#define DMAC_DMADBR(m)	(DMAC_BASE + 0x0308 + 0x100 * (m))              /* DMA doorbell */
#define DMAC_DMADBSR(m)	(DMAC_BASE + 0x030C + 0x100 * (m))              /* DMA doorbell set */
#define DMAC_DMACKE(m)  (DMAC_BASE + 0x0310 + 0x100 * (m))
#define DMAC_DMACKES(m) (DMAC_BASE + 0x0314 + 0x100 * (m))
#define DMAC_DMACKEC(m) (DMAC_BASE + 0x0318 + 0x100 * (m))

#define REG_DMAC_DSAR(n)	REG32(DMAC_DSAR((n)))
#define REG_DMAC_DTAR(n)	REG32(DMAC_DTAR((n)))
#define REG_DMAC_DTCR(n)	REG32(DMAC_DTCR((n)))
#define REG_DMAC_DRSR(n)	REG32(DMAC_DRSR((n)))
#define REG_DMAC_DCCSR(n)	REG32(DMAC_DCCSR((n)))
#define REG_DMAC_DCMD(n)	REG32(DMAC_DCMD((n)))
#define REG_DMAC_DDA(n)		REG32(DMAC_DDA((n)))
#define REG_DMAC_DSD(n)         REG32(DMAC_DSD(n))
#define REG_DMAC_DMACR(m)	REG32(DMAC_DMACR(m))
#define REG_DMAC_DMAIPR(m)	REG32(DMAC_DMAIPR(m))
#define REG_DMAC_DMADBR(m)	REG32(DMAC_DMADBR(m))
#define REG_DMAC_DMADBSR(m)	REG32(DMAC_DMADBSR(m))
#define REG_DMAC_DMACKE(m)      REG32(DMAC_DMACKE(m))
#define REG_DMAC_DMACKES(m)     REG32(DMAC_DMACKES(m))
#define REG_DMAC_DMACKEC(m)     REG32(DMAC_DMACKEC(m))

// DMA request source register
#define DMAC_DRSR_RS_BIT		0
#define DMAC_DRSR_RS_MASK		(0x3f << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_AUTO		(0x08 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_TSSIIN		(0x09 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_EXT        (0x0c << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART3OUT	(0x0e << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART3IN	(0x0f << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART2OUT	(0x10 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART2IN	(0x11 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART1OUT	(0x12 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART1IN	(0x13 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART0OUT	(0x14 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_UART0IN	(0x15 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_SSI0OUT	(0x16 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_SSI0IN		(0x17 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_AICOUT		(0x18 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_AICIN		(0x19 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_MSC0OUT	(0x1a << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_MSC0IN		(0x1b << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_TCU		(0x1c << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_SADC		(0x1d << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_MSC1OUT	(0x1e << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_MSC1IN		(0x1f << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_SSI1OUT	(0x20 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_SSI1IN		(0x21 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_PMOUT		(0x22 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_PMIN		(0x23 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_MSC2OUT	(0x24 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_MSC2IN		(0x25 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_I2C0OUT    (0x28 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_I2C0IN     (0x29 << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_I2C1OUT    (0x2a << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_I2C1IN     (0x2b << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_I2C2OUT    (0x2e << DMAC_DRSR_RS_BIT)
#define DMAC_DRSR_RS_I2C2IN     (0x2f << DMAC_DRSR_RS_BIT)

// DMA channel control/status register
#define DMAC_DCCSR_NDES		(1 << 31) /* descriptor (0) or not (1) ? */
#define DMAC_DCCSR_DES8    	(1 << 30) /* Descriptor 8 Word */
#define DMAC_DCCSR_DES4    	(0 << 30) /* Descriptor 4 Word */
#define DMAC_DCCSR_CDOA_BIT	16        /* copy of DMA offset address */
#define DMAC_DCCSR_CDOA_MASK	(0xff << DMAC_DCCSR_CDOA_BIT)

#define DMAC_DCCSR_AR		(1 << 4)  /* address error */
#define DMAC_DCCSR_TT		(1 << 3)  /* transfer terminated */
#define DMAC_DCCSR_HLT		(1 << 2)  /* DMA halted */
#define DMAC_DCCSR_EN		(1 << 0)  /* channel enable bit */

// DMA channel command register 
#define DMAC_DCMD_EACKS_LOW  	(1 << 31) /* External DACK Output Level Select, active low */
#define DMAC_DCMD_EACKS_HIGH  	(0 << 31) /* External DACK Output Level Select, active high */
#define DMAC_DCMD_EACKM_WRITE 	(1 << 30) /* External DACK Output Mode Select, output in write cycle */
#define DMAC_DCMD_EACKM_READ 	(0 << 30) /* External DACK Output Mode Select, output in read cycle */
#define DMAC_DCMD_ERDM_BIT      28        /* External DREQ Detection Mode Select */
#define DMAC_DCMD_ERDM_MASK     (0x03 << DMAC_DCMD_ERDM_BIT)
#define DMAC_DCMD_ERDM_LOW      (0 << DMAC_DCMD_ERDM_BIT)
#define DMAC_DCMD_ERDM_FALL     (1 << DMAC_DCMD_ERDM_BIT)
#define DMAC_DCMD_ERDM_HIGH     (2 << DMAC_DCMD_ERDM_BIT)
#define DMAC_DCMD_ERDM_RISE     (3 << DMAC_DCMD_ERDM_BIT)
//#define DMAC_DCMD_BLAST		(1 << 25) /* BCH last */
#define DMAC_DCMD_SAI		(1 << 23) /* source address increment */
#define DMAC_DCMD_DAI		(1 << 22) /* dest address increment */
#define DMAC_DCMD_RDIL_BIT	16        /* request detection interval length */
#define DMAC_DCMD_RDIL_MASK	(0x0f << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_IGN	(0 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_2	(0x01 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_4	(0x02 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_8	(0x03 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_12	(0x04 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_16	(0x05 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_20	(0x06 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_24	(0x07 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_28	(0x08 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_32	(0x09 << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_48	(0x0a << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_60	(0x0b << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_64	(0x0c << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_124	(0x0d << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_128	(0x0e << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_RDIL_200	(0x0f << DMAC_DCMD_RDIL_BIT)
#define DMAC_DCMD_SWDH_BIT	14  /* source port width */
#define DMAC_DCMD_SWDH_MASK	(0x03 << DMAC_DCMD_SWDH_BIT)
#define DMAC_DCMD_SWDH_32	(0 << DMAC_DCMD_SWDH_BIT)
#define DMAC_DCMD_SWDH_8	(1 << DMAC_DCMD_SWDH_BIT)
#define DMAC_DCMD_SWDH_16	(2 << DMAC_DCMD_SWDH_BIT)
#define DMAC_DCMD_DWDH_BIT	12  /* dest port width */
#define DMAC_DCMD_DWDH_MASK	(0x03 << DMAC_DCMD_DWDH_BIT)
#define DMAC_DCMD_DWDH_32	(0 << DMAC_DCMD_DWDH_BIT)
#define DMAC_DCMD_DWDH_8	(1 << DMAC_DCMD_DWDH_BIT)
#define DMAC_DCMD_DWDH_16	(2 << DMAC_DCMD_DWDH_BIT)
#define DMAC_DCMD_DS_BIT	8  /* transfer data size of a data unit */
#define DMAC_DCMD_DS_MASK	(0x07 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_DS_32BIT	(0 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_DS_8BIT	(1 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_DS_16BIT	(2 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_DS_16BYTE	(3 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_DS_32BYTE	(4 << DMAC_DCMD_DS_BIT)
#define DMAC_DCMD_DS_64BYTE	(5 << DMAC_DCMD_DS_BIT)

#define DMAC_DCMD_STDE	        (1 << 2)  /* Stride enable */
#define DMAC_DCMD_TIE		(1 << 1)  /* DMA transfer interrupt enable */
#define DMAC_DCMD_LINK		(1 << 0)  /* descriptor link enable */

// DMA descriptor address register
#define DMAC_DDA_BASE_BIT	12  /* descriptor base address */
#define DMAC_DDA_BASE_MASK	(0x0fffff << DMAC_DDA_BASE_BIT)
#define DMAC_DDA_OFFSET_BIT	4   /* descriptor offset address */
#define DMAC_DDA_OFFSET_MASK	(0x0ff << DMAC_DDA_OFFSET_BIT)

// DMA stride address register
#define DMAC_DSD_TSD_BIT        16  /* target stride address */
#define DMAC_DSD_TSD_MASK      	(0xffff << DMAC_DSD_TSD_BIT)
#define DMAC_DSD_SSD_BIT        0  /* source stride address */
#define DMAC_DSD_SSD_MASK      	(0xffff << DMAC_DSD_SSD_BIT)

// DMA control register
#define DMAC_DMACR_FMSC		(1 << 31)  /* MSC Fast DMA mode */
#define DMAC_DMACR_FSSI		(1 << 30)  /* SSI Fast DMA mode */
#define DMAC_DMACR_FTSSI	(1 << 29)  /* TSSI Fast DMA mode */
#define DMAC_DMACR_FUART	(1 << 28)  /* UART Fast DMA mode */
#define DMAC_DMACR_FAIC		(1 << 27)  /* AIC Fast DMA mode */
#define DMAC_DMACR_PR_BIT	8  /* channel priority mode */
#define DMAC_DMACR_PR_MASK	(0x03 << DMAC_DMACR_PR_BIT)
#define DMAC_DMACR_PR_012345	(0 << DMAC_DMACR_PR_BIT)
#define DMAC_DMACR_PR_120345	(1 << DMAC_DMACR_PR_BIT)
#define DMAC_DMACR_PR_230145	(2 << DMAC_DMACR_PR_BIT)
#define DMAC_DMACR_PR_340125	(3 << DMAC_DMACR_PR_BIT)
#define DMAC_DMACR_HLT		(1 << 3)  /* DMA halt flag */
#define DMAC_DMACR_AR		(1 << 2)  /* address error flag */
#define DMAC_DMACR_DMAE		(1 << 0)  /* DMA enable bit */

// DMA doorbell register
#define DMAC_DMADBR_DB5		(1 << 5)  /* doorbell for channel 5 */
#define DMAC_DMADBR_DB4		(1 << 4)  /* doorbell for channel 4 */
#define DMAC_DMADBR_DB3		(1 << 3)  /* doorbell for channel 3 */
#define DMAC_DMADBR_DB2		(1 << 2)  /* doorbell for channel 2 */
#define DMAC_DMADBR_DB1		(1 << 1)  /* doorbell for channel 1 */
#define DMAC_DMADBR_DB0		(1 << 0)  /* doorbell for channel 0 */

// DMA doorbell set register
#define DMAC_DMADBSR_DBS5	(1 << 5)  /* enable doorbell for channel 5 */
#define DMAC_DMADBSR_DBS4	(1 << 4)  /* enable doorbell for channel 4 */
#define DMAC_DMADBSR_DBS3	(1 << 3)  /* enable doorbell for channel 3 */
#define DMAC_DMADBSR_DBS2	(1 << 2)  /* enable doorbell for channel 2 */
#define DMAC_DMADBSR_DBS1	(1 << 1)  /* enable doorbell for channel 1 */
#define DMAC_DMADBSR_DBS0	(1 << 0)  /* enable doorbell for channel 0 */

// DMA interrupt pending register
#define DMAC_DMAIPR_CIRQ5	(1 << 5)  /* irq pending status for channel 5 */
#define DMAC_DMAIPR_CIRQ4	(1 << 4)  /* irq pending status for channel 4 */
#define DMAC_DMAIPR_CIRQ3	(1 << 3)  /* irq pending status for channel 3 */
#define DMAC_DMAIPR_CIRQ2	(1 << 2)  /* irq pending status for channel 2 */
#define DMAC_DMAIPR_CIRQ1	(1 << 1)  /* irq pending status for channel 1 */
#define DMAC_DMAIPR_CIRQ0	(1 << 0)  /* irq pending status for channel 0 */


#endif /* __CHIP_DMAC_H__ */
