/*
 * main.c
 *
 * Main routine of the firmware.
 *
 * Copyright (C) 2008 Ingenic Semiconductor Inc.
 *
 */
#include "jz4770.h"

/* 600M --> (1÷600000000)×(40×15) = 1uS */
void udelay(volatile unsigned int delays)
{
	volatile unsigned int temp;
	for ( ; delays > 0; delays--) {
		for (temp = 0; temp < 40; temp++)
			;
	}
}
void mdelay(volatile unsigned int delays)
{
	for ( ; delays > 0; delays--) {
		udelay(1000);
	}
}

static void led_flash(void)
{
	do {
		mdelay(500);
		__gpio_port_as_output0(4, 26);	/* light on */
		mdelay(500);
		__gpio_port_as_output1(4, 26);	/* light off */
	} while (1);
}

void c_main(void)
{
	/* PE26 -> LED yellow
	 * A B C D E F
	 * 0 1 2 3 4 5 */
	//__gpio_port_as_output0(4, 26);	/* light on */
	
	
	__gpio_as_uart2();
	__cpm_start_uart2();
	
	serial_init();
	
	prints("\n\nThis is Duke!\n");
	
	pll_init_4770();		/* init pll */
	
	prints("Charlene Choi + Gillian Chung = Twins!\n");


	act8600_setting();
	
	otg_func();
	//led_flash();
	
	while(1);
	
	/* serial_puts("pll init\n"); */
	/* pll_init_4770(); */
	/* sdram_init_4770(); */

	/* serial_puts("Setup fw args finish!\n"); */
}


