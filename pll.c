#include "jz4770.h"

#define MHZ (1000 * 1000)
#define PLL_OUT_MAX 1200		/* 1200MHz. */

static inline unsigned int pll_calc_m_n_od(unsigned int speed, unsigned int xtal)
{
	unsigned int CFG_EXTAL_CLK;
	unsigned int CFG_PLL_OUT;
	unsigned int PLL_NO;
	unsigned int NR;
	unsigned int NO;
	unsigned int PLL_MO;
	unsigned int NF;
	unsigned int FOUT;
	unsigned int PLL_BS;
	unsigned int PLL_OD;
	unsigned int CPCCR_M_N_OD;


	CFG_EXTAL_CLK = xtal/1000000;
	CFG_PLL_OUT = speed/1000000;

	if (CFG_PLL_OUT > PLL_OUT_MAX)
		prints("PLL output can NOT more than 1200MHZ");
	else if (CFG_PLL_OUT > 600) {
		PLL_BS	=	1; 
		PLL_OD	=	0; 
	}
	else if (CFG_PLL_OUT > 300) {
		PLL_BS	 =      0; 
		PLL_OD	 =      0; 
	}
	else if (CFG_PLL_OUT > 155) {
		PLL_BS	 =      0;
		PLL_OD	 =      1;
	}
	else if (CFG_PLL_OUT > 76) {
		PLL_BS	 =      0; 
		PLL_OD	 =      2; 
	}
	else if (CFG_PLL_OUT > 47) {
		PLL_BS	 =      0; 
		PLL_OD	 =      3; 
	}
	else {
		prints("PLL ouptput can NOT less than 48");
		goto __ERROR_PLL;
	}
	PLL_NO = 0;
	NR  = (PLL_NO + 1);
	NO  = (0x1 << PLL_OD);
	PLL_MO =	(((CFG_PLL_OUT / CFG_EXTAL_CLK) * NR * NO) - 1);
	NF = (PLL_MO + 1);
	FOUT = (CFG_EXTAL_CLK * NF / NR / NO);
	PLL_BS = 1;

	if ((CFG_EXTAL_CLK / NR > 50) || (CFG_EXTAL_CLK / NR < 10)) {
		prints("Can NOT set the value to PLL_N: ");
		print_hex32(NR);
		goto __ERROR_PLL;
	}
	
	if ((PLL_MO > 127) || (PLL_MO < 1)) {
		prints("Can NOT set the value to PLL_M:");
		print_hex32(PLL_MO);
		goto __ERROR_PLL;
	}

	if ((FOUT * NO) < 500)
		PLL_BS = 0;
	else if ((FOUT * NO) > 600)
		PLL_BS == 1;
	if ((FOUT * NO) > PLL_OUT_MAX || ((FOUT * NO) < 300)) {
		prints("The PLL is outof range");
		goto __ERROR_PLL;
	}
	CPCCR_M_N_OD =	((PLL_MO << 24) | (PLL_NO << 18) | (PLL_OD << 16) | (PLL_BS << 31));
		 
	return CPCCR_M_N_OD;
__ERROR_PLL:
	while(1);
}

/* divisors, 
 * for jz4770 ,C:H0:P:C1:H2:H1.
 * DIV should be one of [1, 2, 3, 4, 6, 8, 12, 16, 24, 32]
 */
#define DIV_1 0
#define DIV_2 1
#define DIV_3 2
#define DIV_4 3
#define DIV_6 4
#define DIV_8 5
#define DIV_12 6
#define DIV_16 7
#define DIV_24 8
#define DIV_32 9

#define DIV(a,b,c,d,e,f)					\
({								\
	unsigned int retval;					\
	retval = (DIV_##a << CPM_CPCCR_CDIV_BIT)   |		\
		 (DIV_##b << CPM_CPCCR_H0DIV_BIT)  |		\
		 (DIV_##c << CPM_CPCCR_PDIV_BIT)   |		\
		 (DIV_##d << CPM_CPCCR_C1DIV_BIT)  |		\
		 (DIV_##e << CPM_CPCCR_H2DIV_BIT)  |		\
		 (DIV_##f << CPM_CPCCR_H1DIV_BIT);		\
	retval;							\
})

#define CFG_EXTAL		12000000
#define CFG_CPU_SPEED		600000000

/* TODO: pll_init() need modification. */
void pll_init_4770()
{
	register unsigned int cfcr, plcr1;

	unsigned int div;
	int pllout2;

	div = DIV(1,4,8,2,4,4);
	cfcr = 	div;
	cfcr |= CPM_CPCCR_CE;

	pllout2 = (cfcr & CPM_CPCCR_PCS) ? (CFG_CPU_SPEED/2) : CFG_CPU_SPEED;

	plcr1 = pll_calc_m_n_od(CFG_CPU_SPEED, CFG_EXTAL);
	plcr1 |= (0x20 << CPM_CPPCR_PLLST_BIT)	/* PLL stable time */
		| CPM_CPPCR_PLLEN;		/* enable PLL */

	prints("\nCFG_CPU_SPEED = ");
	print_hex32(CFG_CPU_SPEED);
	prints("\nCFG_EXTAL = ");
	print_hex32(CFG_EXTAL);
	
	/* init PLL */
	prints("\ncfcr = ");
	print_hex32(cfcr);
	prints("\nplcr1 = ");
	print_hex32(plcr1);

	REG_CPM_CPCCR = cfcr;
	REG_CPM_CPPCR = plcr1;
	
	while(!(REG_CPM_CPPSR & (1 << 29))); 
	
	/*wait for pll output stable ...*/
	while (!(REG_CPM_CPPCR & CPM_CPPCR_PLLS));

	prints("\nREG_CPM_CPCCR = ");
	print_hex32(REG_CPM_CPCCR);
	prints("\nREG_CPM_CPPCR = ");
	print_hex32(REG_CPM_CPPCR);
	prints("\n");
}
