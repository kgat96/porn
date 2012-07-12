/*
 * head.S
 *
 * Entry point of the firmware.
 * The firmware code are executed in the ICache.
 *
 * Copyright (C) 2006 Ingenic Semiconductor Inc.
 *
 */

#define JZ_MSCBOOT_CFG       0x4d53504c
	
	.set noreorder
	.globl _start
	.extern c_main
	//.globl _irq
	//.extern c_isr
	.text

_start:
	.word	JZ_MSCBOOT_CFG
	//----------------------------------------------------
	// setup stack, jump to C code
	//----------------------------------------------------
	la	$29, 0x80004000		// sp
	//mfc0	$8, $12
	//and	$8, 0xfffffff9
	//or	$8, 0x400001
	//mtc0	$8, $12
	j	c_main
	nop

	// isr addr = 0x8000 0080
	//.org 0x180
//_irq:
	//j	c_isr
	//nop
	//eret
	//nop
	.set reorder
