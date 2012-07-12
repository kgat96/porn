#include "jz4770.h"

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
#define SDA_R()	REG_GPIO_PXPIN(4) & (1 << (25))		/* 0 or none 0 */
#define SDA_1()	REG_GPIO_PXPAT1S(4) = (1 << (25))	/* input */
#define SDA_0()	REG_GPIO_PXPAT1C(4) = (1 << (25))	/* output 0 */
#define SCK_1()	REG_GPIO_PXPAT1S(4) = (1 << (28))	/* input */
#define SCK_0()	REG_GPIO_PXPAT1C(4) = (1 << (28))	/* output 0 */

static void act8600_pin_init(void)
{
	/* SCK -> OUT open-drain */
	REG_GPIO_PXINTC(3) = (1 << (4));	/* IO */
	REG_GPIO_PXMASKS(3) = (1 << (4));	/* IO */
	REG_GPIO_PXPAT1S(3) = (1 << (4));	/* INPUT */
	REG_GPIO_PXPAT0C(3) = (1 << (4));	/* 0 for future use */
	/* SDA -> OUT open-drain */
	REG_GPIO_PXINTC(3) = (1 << (5));	/* IO */
	REG_GPIO_PXMASKS(3) = (1 << (5));	/* IO */
	REG_GPIO_PXPAT1S(3) = (1 << (5));	/* INPUT */
	REG_GPIO_PXPAT0C(3) = (1 << (5));	/* 0 for future use */
}


static unsigned char act8600_byte_r(void)
{
	unsigned char data;
	int temp;
	for (temp = 0; temp < 8; temp++) {
		udelay(100);
		SCK_1();
		udelay(100);
		data = data << 1;	/* first shift is no use */
		if (SDA_R())
			data += 1;
		SCK_0();
	}
	udelay(50);
	SDA_0();			/* handshake */
	udelay(50);
	SCK_1();
	udelay(100);
	SCK_0();
	udelay(50);
	SDA_1();
	udelay(50);
	return data;
}

static int act8600_byte_w(unsigned char data)
{
	int temp, retval = 0;
	for (temp = 0; temp < 8; temp ++) {
		udelay(50);
		if (data & (0x80 >> temp))
			SDA_1();
		else
			SDA_0();
		udelay(50);
		SCK_1();
		udelay(100);
		SCK_0();
	}
	udelay(50);
	SDA_1();
	/* handshake */
	udelay(50);
	SCK_1();
	udelay(100);
	if (SDA_R())
		retval = -1;
	SCK_0();
	udelay(100);
	return retval;
}

static int act8600_write_reg(unsigned char reg, unsigned char data)
{
	int ret;
	
	udelay(500);
	SDA_0();
	udelay(200);
	SCK_0();
	udelay(200);
	ret = act8600_byte_w(0xb4);	/* bit8 = 0, nW */
	if (ret)
		goto err;
	udelay(200);
	ret = act8600_byte_w(reg);
	if (ret)
		goto err;
	udelay(200);
	ret = act8600_byte_w(data);
	if (ret)
		goto err;
	udelay(200);
	goto end;
err:
	prints("ERR: act8600 handshake err\n");
end:
	SDA_0();
	udelay(200);
	SCK_1();
	udelay(200);
	SDA_1();
	udelay(200);
	return ret;
}

static int act8600_read_reg(unsigned char reg, unsigned char *val)
{
	int ret;
	/******************************* write *******************************/
	udelay(500);
	SDA_0();
	udelay(200);
	SCK_0();
	udelay(200);
	ret = act8600_byte_w(0xb4);	/* bit8 = 0, nW */
	if (ret)
		goto err;
	udelay(200);
	ret = act8600_byte_w(reg);
	if (ret)
		goto err;
	udelay(200);
/* end */
	SDA_0();
	udelay(200);
	SCK_1();
	udelay(200);
	SDA_1();
	udelay(200);
	/* return ret; */

	/******************************* read *******************************/
	udelay(500);
	SDA_0();
	udelay(200);
	SCK_0();
	udelay(200);
	/* */
	ret = act8600_byte_w(0xb5);	/* bit8 = 1, R */
	if (ret)
		goto err;
	udelay(200);
	*val = act8600_byte_r();
	udelay(200);
	goto end;
err:
	prints("ERR: act8600 handshake err\n");
end:
	SDA_0();
	udelay(200);
	SCK_1();
	udelay(200);
	SDA_1();
	udelay(200);
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

void act8600_setting(void)
{
	unsigned char val;
	act8600_pin_init();
	udelay(500);
	prints("\nact8600 test:\n read reg 0x40:");
	act8600_read_reg(0x40, &val);
	print_hex8(val);
	
	//act8600_write_reg(0x40, 0x54); /* 0x54 -> 5.0V */
	act8600_write_reg(0x40, 0x57); /* 0x57 -> 5.3V */
	act8600_write_reg(0x41, 0x80); /* on */
	
	act8600_read_reg(0x40, &val);
	prints("\nread again after write 0x57:");
	print_hex8(val);
	prints("\n");
}
