/*
 * head.S
 *
 * Entry point of the firmware.
 * The firmware code are executed in the ICache.
 *
 * Copyright (C) 2012, Kage Shen <kgat96@gmial.com>
 * Copyright (C) 2006 Ingenic Semiconductor Inc.
 *
 */

#define JZ_MSCBOOT_CFG       0x4d53504c
	
	.set noreorder
	.globl _start
	.extern cmain
	.extern cc1
	.extern cc2
	.extern cc3
	.globl _irq
	.extern cisr
	.text

	.org 0x180
_irq:
//	mfc0	$26, $12
//	and		$26, 0xffbff000
//	or		$26, 0x0400
//	mtc0	$26, $12
//	nop

//	la $26, 0
//	la $27, 0xb0050030
//	sw $26, 0($27)

//	nop
	jal cisr
//	nop
//	jal cc2
//	nop
//	jal cc3
//	nop

//	mfc0	$26, $12
//	and		$26, 0xffbff001
//	or		$26, 0x0401
//	mtc0	$26, $12

	nop
	eret
	.org 0x200
_start:
	.word	JZ_MSCBOOT_CFG
	//----------------------------------------------------
	// setup stack, jump to C code
	//----------------------------------------------------
	la		$29, 0x80004000		// sp
	mfc0	$8, $12
	and		$8, 0xffbff009
	or		$8, 0x0409
	mtc0	$8, $12

	mfc0	$8, $13
	and		$8, 0xff7fffff
	or		$8, 0x0
	mtc0	$8, $13

	mfc0	$8, $15,1
	and		$8, 0xfff
	or		$8, 0x0000000
	mtc0	$8, $15,1

	j		cmain
	nop

	.set reorder

