#
# Copyright (C) 2006 Ingenic Semiconductor Inc.
#
# Makefile for the JZ4770 internal bootrom code.
#

#CROSS	:= mipsel-linux-
CROSS	:= mips-linux-gnu-

CFLAGS	:= -O2 -mips32 -fno-pic -mno-abicalls -EL -g -Wall
#CFLAGS	:= -O2 -mips32 -fno-pic -EL -g -Wall -mabi=32
LDFLAGS	:= -nostdlib -T target.ld -EL

OBJS1	= head.o main.o uart.o my4770.o act8600.o i2c.o

all: fw_ddr1.bin
#	cat mbr fw_ddr1.bin > all.bin
	cat fw_ddr1.bin > all.bin
	ingenic-boot --boot=all.bin

fw_ddr1.bin: fw_ddr1.elf
	$(CROSS)objcopy -O binary $< $@
	$(CROSS)objdump -D $< > fw.dump
	$(CROSS)objdump -h $< > fw.map
	$(CROSS)nm -n $< > System.map
fw_ddr1.elf: $(OBJS1)
	$(CROSS)ld $(LDFLAGS) $(OBJS1) -o $@

.c.o:
	$(CROSS)gcc $(CFLAGS) -c $< -o $@
.S.o:
	$(CROSS)gcc $(CFLAGS) -c $< -o $@
clean:
	rm -fr *.o *.elf *.bin *.dump *.map
