#include "jz4770.h"
#include "my4770.h"
#include "uart.h"


/* O1: SDA --> PD5
 * O1: SCK --> PD4
 *
 * A B C D E F
 * 0 1 2 3 4 5 */
/* #define SDA_R()	REG_GPIO_PXPIN(3) & (1 << (5))		/\* 0 or none 0 *\/ */
/* #define SDA_1()	REG_GPIO_PXPAT1S(3) = (1 << (5))	/\* input *\/ */
/* #define SDA_0()	REG_GPIO_PXPAT1C(3) = (1 << (5))	/\* output 0 *\/ */
/* #define SCK_1()	REG_GPIO_PXPAT1S(3) = (1 << (4))	/\* input *\/ */
/* #define SCK_0()	REG_GPIO_PXPAT1C(3) = (1 << (4))	/\* output 0 *\/ */

/* NPM701 SDA --> PE25
 * NPM701 SCK --> PE28
 */

//	pisces 1.1	SDA --> PE30	Fixme Kage
//				SCK --> PE31

#define gI2C	4
#define gSDA	30
#define gSCK	31

#define SDA_R()	REG_GPIO_PXPIN(gI2C) & (1 << (gSDA))		/* 0 or none 0 */
#define SDA_1()	REG_GPIO_PXPAT1S(gI2C) = (1 << (gSDA))		/* input */
#define SDA_0()	REG_GPIO_PXPAT1C(gI2C) = (1 << (gSDA))		/* output 0 */
#define SCK_1()	REG_GPIO_PXPAT1S(gI2C) = (1 << (gSCK))		/* input */
#define SCK_0()	REG_GPIO_PXPAT1C(gI2C) = (1 << (gSCK))		/* output 0 */

static void act8600_pin_init(void)
{
	/* SCK -> OUT open-drain */
	REG_GPIO_PXINTC(gI2C) = (1 << (gSCK));	/* IO */
	REG_GPIO_PXMASKS(gI2C) = (1 << (gSCK));	/* IO */
	REG_GPIO_PXPAT1S(gI2C) = (1 << (gSCK));	/* INPUT */
	REG_GPIO_PXPAT0C(gI2C) = (1 << (gSCK));	/* 0 for future use */

	/* SDA -> OUT open-drain */
	REG_GPIO_PXINTC(gI2C) = (1 << (gSDA));	/* IO */
	REG_GPIO_PXMASKS(gI2C) = (1 << (gSDA));	/* IO */
	REG_GPIO_PXPAT1S(gI2C) = (1 << (gSDA));	/* INPUT */
	REG_GPIO_PXPAT0C(gI2C) = (1 << (gSDA));	/* 0 for future use */
}


static unsigned char act8600_byte_r(void)
{
	unsigned char data;
	int temp;
	for (temp = 0; temp < 8; temp++) {
		delayus(100);
		SCK_1();
		delayus(100);
		data = data << 1;	/* first shift is no use */
		if (SDA_R())
			data += 1;
		SCK_0();
	}
	delayus(50);
	SDA_0();			/* handshake */
	delayus(50);
	SCK_1();
	delayus(100);
	SCK_0();
	delayus(50);
	SDA_1();
	delayus(50);
	return data;
}

static int act8600_byte_w(unsigned char data)
{
	int temp, retval = 0;
	for (temp = 0; temp < 8; temp ++) {
		delayus(50);
		if (data & (0x80 >> temp))
			SDA_1();
		else
			SDA_0();
		delayus(50);
		SCK_1();
		delayus(100);
		SCK_0();
	}
	delayus(50);
	SDA_1();
	/* handshake */
	delayus(50);
	SCK_1();
	delayus(100);
	if (SDA_R())
		retval = -1;
	SCK_0();
	delayus(100);
	return retval;
}

static int act8600_write_reg(unsigned char reg, unsigned char data)
{
	int ret;
	
	delayus(500);
	SDA_0();
	delayus(200);
	SCK_0();
	delayus(200);
	ret = act8600_byte_w(0xb4);	/* bit8 = 0, nW */
	if (ret)
		goto err;
	delayus(200);
	ret = act8600_byte_w(reg);
	if (ret)
		goto err;
	delayus(200);
	ret = act8600_byte_w(data);
	if (ret)
		goto err;
	delayus(200);
	goto end;
err:
	prints((u8 *)"ERR: act8600 handshake err\n");
end:
	SDA_0();
	delayus(200);
	SCK_1();
	delayus(200);
	SDA_1();
	delayus(200);
	return ret;
}

static int act8600_read_reg(unsigned char reg, unsigned char *val)
{
	int ret;
	/******************************* write *******************************/
	delayus(500);
	SDA_0();
	delayus(200);
	SCK_0();
	delayus(200);
	ret = act8600_byte_w(0xb4);	/* bit8 = 0, nW */
	if (ret)
		goto err;
	delayus(200);
	ret = act8600_byte_w(reg);
	if (ret)
		goto err;
	delayus(200);
/* end */
	SDA_0();
	delayus(200);
	SCK_1();
	delayus(200);
	SDA_1();
	delayus(200);
	/* return ret; */

	/******************************* read *******************************/
	delayus(500);
	SDA_0();
	delayus(200);
	SCK_0();
	delayus(200);
	/* */
	ret = act8600_byte_w(0xb5);	/* bit8 = 1, R */
	if (ret)
		goto err;
	delayus(200);
	*val = act8600_byte_r();
	delayus(200);
	goto end;
err:
	prints((u8 *)"ERR: act8600 handshake err\n");
end:
	SDA_0();
	delayus(200);
	SCK_1();
	delayus(200);
	SDA_1();
	delayus(200);
	return ret;
}

void otg_power_on(void)
{
	act8600_write_reg(0xb0, 0x80);	/* q1 on */
	//act8600_write_reg(0xb0, 0x40);	/* q2 on */
}

void otg_power_off(void)
{
	//act8600_write_reg(0xb0, 0x80);	/* q1 on */
	act8600_write_reg(0xb0, 0x00);	/* off */
}

void lcd_power_config(u32 a)		// config for pisces
{	
	u8 val1, val2;

	if(a){
		act8600_write_reg(0x60, 0x39);	/* on */
		act8600_write_reg(0x61, 0x81);	/* on */
	}else{
		act8600_write_reg(0x60, 0x00);	/* off */
		act8600_write_reg(0x61, 0x00);	/* on */
	}
	act8600_read_reg(0x60, &val1);
	act8600_read_reg(0x61, &val2);

	prints((u8 *)"\nact8600 0x60 & 0x61: ");
	sendhex16(val1);
	sendhex16(val2);
}

void act8600_setting(void)
{
	unsigned char val;
	act8600_pin_init();
	delayus(500);
	prints((u8 *)"\nact8600 test:\nread reg 0x40:");
	act8600_read_reg(0x40, &val);
	sendhex16(val);
	
	//act8600_write_reg(0x40, 0x54); /* 0x54 -> 5.0V */
	act8600_write_reg(0x40, 0x57); /* 0x57 -> 5.3V */
	act8600_write_reg(0x41, 0x80); /* on */
	
	act8600_read_reg(0x40, &val);
	prints((u8 *)"\nread again after write 0x57:");
	sendhex16(val);

	lcd_power_config(1);
}
