#include "jz4770.h"
#include "my4770.h"
#include "uart.h"

//FIXME: add uart send function Kage 7/17/12
//		 fix some function -- kage 7/19/12

const u8 HexTable[16] = {"0123456789ABCDEF"};

u32 UART_BASE = UART0_BASE + 2 * 0x1000;
u32 CFG_EXTAL = 12 * 1000000;				//	EXCLK

static void serial_setbrg_4770(void)
{
	v8 *uart_lcr = (v8 *)(UART_BASE + OFF_LCR);
	v8 *uart_dlhr = (v8 *)(UART_BASE + OFF_DLHR);
	v8 *uart_dllr = (v8 *)(UART_BASE + OFF_DLLR);
	u32 baud_div, tmp;

	baud_div = (CFG_EXTAL / 16 / 57600);
	//baud_div = (CFG_EXTAL / 16 / CONFIG_BAUDRATE);

	tmp = *uart_lcr;
	tmp |= UART_LCR_DLAB;
	*uart_lcr = tmp;

	*uart_dlhr = (baud_div >> 8) & 0xff;
	*uart_dllr = baud_div & 0xff;

	tmp &= ~UART_LCR_DLAB;
	*uart_lcr = tmp;
}

int serial_tstc (void)
{
	v8 *uart_lsr = (v8 *)(UART_BASE + OFF_LSR);

	if (*uart_lsr & UART_LSR_DR) {
		/* Data in rfifo */
		return (1);
	}
	return 0;
}

void serial_putc(u8 c)
{
	v8 *uart_lsr = (v8 *)(UART_BASE + OFF_LSR);
	v8 *uart_tdr = (v8 *)(UART_BASE + OFF_TDR);

	if (c == '\n') serial_putc ('\r');

	/* Wait for fifo to shift out some bytes */
	while ( !((*uart_lsr & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60) );

	*uart_tdr = (u8)c;
}

void prints(u8 *s)
{
	if(s == NULL){
		//prints("Null");
		return;
	}

	while (*s) {
		serial_putc (*s++);
	}
}

void sendhex16(u32 dat)
{
	u8 i;

	//prints("0x");
	i = (dat>>12)&0xf;
	serial_putc(*(HexTable+i));
	i = (dat>>8)&0xf;
   	serial_putc(*(HexTable+i));

	i = (dat>>4)&0xf;
	serial_putc(*(HexTable+i));
	i = dat&0xf;
   	serial_putc(*(HexTable+i));
	serial_putc(' ');
}

void sendhex32(u8 *p, u32 dat)
{
	u8 i;

	prints(p);
	i = (dat>>28)&0xf;
	serial_putc(*(HexTable+i));
	i = (dat>>24)&0xf;
   	serial_putc(*(HexTable+i));

	i = (dat>>20)&0xf;
	serial_putc(*(HexTable+i));
	i = (dat>>16)&0xf;
   	serial_putc(*(HexTable+i));

	i = (dat>>12)&0xf;
	serial_putc(*(HexTable+i));
	i = (dat>>8)&0xf;
   	serial_putc(*(HexTable+i));

	i = (dat>>4)&0xf;
	serial_putc(*(HexTable+i));
	i = dat&0xf;
   	serial_putc(*(HexTable+i));
	serial_putc(' ');
}

int uart_getc(void)
{
	v8 *uart_rdr = (v8 *)(UART_BASE + OFF_RDR);

	while (!serial_tstc());

	return *uart_rdr;
}

void init_uart(void)
{
	__gpio_as_uart2();
	__cpm_start_uart2();

	v8 *uart_fcr = (v8 *)(UART_BASE + OFF_FCR);
	v8 *uart_lcr = (v8 *)(UART_BASE + OFF_LCR);
	v8 *uart_ier = (v8 *)(UART_BASE + OFF_IER);
	v8 *uart_sircr = (v8 *)(UART_BASE + OFF_SIRCR);

	/* Disable port interrupts while changing hardware */
	*uart_ier = 0;

	/* Disable UART unit function */
	*uart_fcr = ~UART_FCR_UUE;

	/* Set both receiver and transmitter in UART mode (not SIR) */
	*uart_sircr = ~(SIRCR_RSIRE | SIRCR_TSIRE);

	/* Set databits, stopbits and parity. (8-bit data, 1 stopbit, no parity) */
	*uart_lcr = UART_LCR_WLEN_8 | UART_LCR_STOP_1;

	serial_setbrg_4770();

	/* Enable UART unit, enable and clear FIFO */
	*uart_fcr = UART_FCR_UUE | UART_FCR_FE | UART_FCR_TFLS | UART_FCR_RFLS;
}





