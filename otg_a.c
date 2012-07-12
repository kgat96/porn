#include "jz4770.h"

#define BIT22 	        (1 << 22)
#define USBPCR_POR	BIT22
#define USB_REG_DEVCTL	(USB_BASE + 0x60)
/* O1: PF18 */
/* #define ID_R()	REG_GPIO_PXPIN(5) & (1 << (18))		/\* 0 or none 0 *\/ */

/* NPM701: PE14 */
#define ID_R()	REG_GPIO_PXPIN(4) & (1 << (14))		/* 0 or none 0 */

/////////////////////////
#define usb_set_index(i)	(REG8(USB_REG_INDEX) = (i))

static __inline__ u8 readb(u32 port)
{
	return *(volatile u8 *)port;
}

static __inline__ u16 readw(u32 port)
{
	return *(volatile u16 *)port;
}

static __inline__ void writeb(u32 port, u8 val)
{
	*(volatile u8 *)port = val;
}

static __inline__ void writew(u32 port, u16 val)
{
	*(volatile u16 *)port = val;
}

static __inline__ void setb(u32 port, u8 val)
{
	volatile u8 *ioport = (volatile u8 *)(port);
	*ioport = (*ioport) | val;
}

static __inline__ void setw(u32 port, u16 val)
{
	volatile u16 *ioport = (volatile u16 *)(port);
	*ioport = (*ioport) | val;
}

static __inline__ void clearb(u32 port, u8 val)
{
	volatile u8 *ioport = (volatile u8 *)(port);
	*ioport = (*ioport) & ~val;
}

static __inline__ void clearw(u32 port, u16 val)
{
	volatile u16 *ioport = (volatile u16 *)(port);
	*ioport = (*ioport) & ~val;
}

/////////////////////////
static void dump_musb_reg(void);

/***********************************************************************/
#define MUSB_INDEX	USB_REG_INDEX
#define MUSB_TXMAXP	(USB_BASE + 0x10)
#define MUSB_TXCSR	(USB_BASE + 0x12)
#define MUSB_RXMAXP	(USB_BASE + 0x14)
#define MUSB_RXCSR	(USB_BASE + 0x16)
#define MUSB_RXCOUNT	(USB_BASE + 0x18)
#define MUSB_TXTYPE	(USB_BASE + 0x1a)
#define MUSB_TXINTERVAL	(USB_BASE + 0x1b)
#define MUSB_RXTYPE	(USB_BASE + 0x1c)
#define MUSB_RXINTERVAL	(USB_BASE + 0x1d)
#define MUSB_FIFOSIZE	(USB_BASE + 0x1f)

void bulk_msg_tx(unsigned char *buf, int len)
{
	int temp;
	volatile u8 *fifo = USB_FIFO_EP1;
	
	writeb(USB_REG_INDEX, 1);
	
	for (temp = 0; temp < len; temp++)
		*fifo = buf[temp];
	
	/* TxPktRdy */
	setw(MUSB_TXCSR, 0x01);
	while (readw(MUSB_TXCSR) & 0x01);
}

void bulk_msg_rx(void)
{
	int temp, len;
	volatile u8 *fifo = USB_FIFO_EP1;
	
	writeb(USB_REG_INDEX, 1);
	
	/* ReqPkt */
	setw(MUSB_RXCSR, 0x0020);
	prints("ep1 rcv byte len:");
	while (!(readw(MUSB_RXCSR) & 0x01));
		
	len = readw(MUSB_RXCOUNT);
	print_hex16(len);
	prints(", data:");
	for (temp = 0; temp < len; temp++)
		print_hex8(*fifo);
	prints("\n");

	clearw(MUSB_RXCSR, 0x01);	/* clear RxPktRdy */
}

void bulk_init(void)
{
	writeb(MUSB_INDEX, 1);
	writew(MUSB_TXMAXP, 512);
	writeb(MUSB_TXTYPE, 0x22);
	writew(MUSB_RXMAXP, 512);
	writeb(MUSB_RXTYPE, 0x21);
	
	writeb(USB_BASE + 0x80 + 8, 2);	/* txaddr */
	writeb(USB_BASE + 0x82 + 8, 0);	/* txHubaddr */
	writeb(USB_BASE + 0x83 + 8, 0);	/* txHubPort */
	writeb(USB_BASE + 0x84 + 8, 2);	/* rxaddr */
	writeb(USB_BASE + 0x86 + 8, 0);	/* rxHubaddr */
	writeb(USB_BASE + 0x87 + 8, 0);	/* rxHubPort */
}

/////////////////////////
void ctrl_msg(int need_read, unsigned char *buf)
{
	int temp, len;
	volatile u8 *fifo = USB_FIFO_EP0;
	
	writeb(USB_REG_INDEX, 0);
	
	for (temp = 0; temp < 8; temp++)
		*fifo = buf[temp];
	
	if (need_read) {
		/* SetupPkt & TxPktRdy */
		setw(USB_REG_CSR0, 0x08 | 0x02);
		while (readw(USB_REG_CSR0) & 0x02);

		/* input pkt is data */
		/* ReqPkt & StatusPkt*/
		setw(USB_REG_CSR0, 0x20 | 0x40);
		//do {
			prints("ep0 rcv byte len:");
			while (!(readw(USB_REG_CSR0) & 0x01));

			len = readb(USB_BASE + 0x18);
			print_hex8(len);
			prints(", data:");
			for (temp = 0; temp < len; temp ++)
				print_hex8(*fifo);
			prints("\n");

			clearw(USB_REG_CSR0, 0x01);	/* clear RxPktRdy */
		//} while (!len);

		/* TxPktRdy */
		setw(USB_REG_CSR0, 0x02);
		while (readw(USB_REG_CSR0) & 0x02);
	} else {
		/* SetupPkt & TxPktRdy */
		setw(USB_REG_CSR0, 0x08 | 0x02);
		while (readw(USB_REG_CSR0) & 0x02);
		
		/* input pkt is zero handshake */
		prints("ctrl msg handshake ");
		/* ReqPkt */
		setw(USB_REG_CSR0, 0x20);
		while (!(readw(USB_REG_CSR0) & 0x01));
		
		len = readb(USB_BASE + 0x18);
		if (!len)
			prints("ok\n");
		else
			prints("err\n");
		
		clearw(USB_REG_CSR0, 0x01);	/* clear RxPktRdy */
	}
}

void change_addr_reg_ep0(unsigned char addr)
{
	writeb(USB_BASE + 0x80, addr);	/* txaddr */
	writeb(USB_BASE + 0x82, 0);	/* txHubaddr */
	writeb(USB_BASE + 0x83, 0);	/* txHubPort */
	writeb(USB_BASE + 0x84, addr);	/* rxaddr */
	writeb(USB_BASE + 0x86, 0);	/* rxHubaddr */
	writeb(USB_BASE + 0x87, 0);	/* rxHubPort */
}

void usb_device_reset(void)
{
	setb(USB_REG_POWER, 8);		/* reset */
	mdelay(30);
	clearb(USB_REG_POWER, 8);	/* nreset */
	mdelay(90);
}

/***********************************************************************/

void musb_isr(void) 	/* simulate interrupt */
{
	int temp;
	volatile u8 *fifo = USB_FIFO_EP0;
	
	volatile u8 intr_usb = readb(USB_REG_INTRUSB); /* & 0x7  mask SOF */
	volatile u16 intr_tx  = readw(UDC_BASE + 0x02);
	volatile u16 intr_rx = readw(UDC_BASE + 0x04);

	if (!(intr_usb || intr_tx || intr_rx))
		return;
	
	prints("intr usb tx rx:");
	print_hex8(intr_usb);
	print_hex16(intr_tx);
	print_hex16(intr_rx);
	prints("\n");
	
	if (intr_usb & USB_INTR_RESUME) {
		prints("USB resume\n");
	}
	
	if (intr_usb & 0x04) {
		prints("USB babble\n");
	}

	if (intr_usb & USB_INTR_SUSPEND) {
		prints("USB suspend\n");
	}
	
	if (intr_usb & 0x08) {
		prints("USB SOF\n");
	}
	
	if (intr_usb & 0x10) {
		prints("USB Conn\n");
		static usb_conn = 0;
		if (!usb_conn) {
			usb_conn = 1;
			usb_device_reset();
			change_addr_reg_ep0(0);
			/* prints("\n* get device descriptor 18 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x00\x01\x00\x00\x40\x00"); */
			
			prints("\n* set addr = 2 *\n");
			ctrl_msg(0, "\x00\x05\x02\x00\x00\x00\x00\x00");
			change_addr_reg_ep0(2);
			
			prints("\n* get device descriptor 64 *\n");
			ctrl_msg(1, "\x80\x06\x00\x01\x00\x00\x12\x00");

			/* /\* get configuration 1 *\/ */
			
			/* prints("\n* get configuration descriptor 9 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x00\x02\x00\x00\x09\x00"); */
			
			/* prints("\n* get configuration descriptor 39 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x00\x02\x00\x00\x27\x00"); */

			/* /\* get configuration 2 *\/ */
			
			/* prints("\n* get configuration descriptor 9 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x01\x02\x00\x00\x09\x00"); */
			
			/* prints("\n* get configuration descriptor 39 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x01\x02\x00\x00\x27\x00"); */

			/* /\* string *\/ */
			
			/* prints("\n* get string lang IDs 255 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x00\x03\x00\x00\xff\x00"); */
			
			/* prints("\n* get string iProduct 255 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x02\x03\x09\x04\xff\x00"); */
			
			/* prints("\n* get string iManufacturer 255 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x01\x03\x09\x04\xff\x00"); */
			
			/* prints("\n* get string iSerialNumber 255 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x05\x03\x09\x04\xff\x00"); */

			/* mdelay(100); */
			/* prints("\n* set Configuration = 1 *\n"); */
			/* ctrl_msg(0, "\x00\x09\x01\x00\x00\x00\x00\x00"); */
			
			/* prints("\n* get string iConfiguration 255 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x03\x03\x09\x04\xff\x00"); */
			
			/* prints("\n* get string iInterface 255 *\n"); */
			/* ctrl_msg(1, "\x80\x06\x04\x03\x09\x04\xff\x00"); */
			
			/* prints("\n* class request IN 1 *\n"); */
			/* ctrl_msg(1, "\xa1\xfe\x00\x00\x00\x00\x01\x00"); */

			/* bulk send */
			/* bulk_init(); */
			/* prints("\n* bulk send *\n"); */
			/* bulk_msg_tx("\x55\x53\x42\x43\x01\x00\x00\x00" */
			/* 	    "\x24\x00\x00\x00\x80\x00\x06\x12" */
			/* 	    "\x00\x00\x00\x24\x00\x00\x00\x00" */
			/* 	    "\x00\x00\x00\x00\x00\x00\x00", 31); */
			
			/* prints("\n* bulk read *\n"); */
			/* bulk_msg_rx(); */
			
			/* prints("\n* bulk read *\n"); */
			/* bulk_msg_rx(); */
		}
	}
	
	if (intr_usb & 0x20) {
		prints("USB Discon\n");
	}
	
	if (intr_usb & 0x40) {
		prints("USB Sess Req\n");
	}
	
	if (intr_usb & 0x80) {
		prints("USB VBus Error\n");
	}

	/******************************************/

	/* ep0 intr */
	/* if (intr_tx & 0x01) { */
	/* 	/\* RxPKtRdy *\/ */
	/* 	if (readw(USB_REG_CSR0) & 0x01) { */
	/* 		int len = readb(USB_BASE + 0x18); */
	/* 		prints("ep0 rcv byte len:"); */
	/* 		print_hex8(len); */
	/* 		prints(", data:"); */
	/* 		for (temp = 0; temp < len; temp ++) */
	/* 			print_hex8(*fifo); */
	/* 		prints("\n"); */
	/* 	} */
	/* } */

	prints("\n");
	return;
}

static void id_interrupt(void)	/* simulate interrupt */
{
	static volatile int session_on = 1;
	
	if (ID_R()) {
		if (session_on) {
			session_on = 0;
			otg_power_off();
			prints("power off\n");
		}
	} else {
		if (!session_on) {
			session_on = 1;

			mdelay(500);
			setb(USB_REG_DEVCTL, 1);	/*session start*/
			mdelay(5); /* =144 wireless mouse ok; ZTE MF637U 3G bad */
			otg_power_on();
			prints("session start, and power on.\n");
		}
	}
}


static void dump_musb_reg(void)
{
	/* prints("POWER:"); */
	/* print_hex8(*((unsigned char *) USB_REG_POWER)); */
	prints("DEVCTL:");
	print_hex8(*((unsigned char *) USB_REG_DEVCTL));
	/* prints("\tFRAME:"); */
	/* print_hex16(*((unsigned short *) USB_REG_FRAME)); */
	prints("\n");

	/* simulate interrupt */
	/* musb_isr(); */
	/* id_interrupt(); */
}

void otg_init(void)
{
	/* PHY init */

	/* id gpio pin */
	REG_GPIO_PXINTC(5) = (1 << (18));	/* IO func */
	REG_GPIO_PXMASKS(5) = (1 << (18));	/* IO */
	REG_GPIO_PXPAT1S(5) = (1 << (18));	/* INPUT */
	REG_GPIO_PXPENC(5) = (1 << (18));	/* pull on */

	/* drvvbus pin func 0 */
	REG_GPIO_PXINTC(4) = (1 << (10));	/* IO func */
	REG_GPIO_PXMASKC(4) = (1 << (10));	/* func */
	REG_GPIO_PXPAT1C(4) = (1 << (10));	/* 0 1 */
	REG_GPIO_PXPAT0C(4) = (1 << (10));	/* 0 */

	REG_CPM_OPCR |= 0x80;	/* SPENDN = 1 !!! */
	
	/* enable tx pre-emphasis */
	REG_CPM_USBPCR |= 0x40;

	/* OTG Mode. */
	REG_CPM_USBPCR |= (1 << 31); //work as OTG
	REG_CPM_USBPCR &= ~((1 << 24) | (1 << 23) | (1 << 20));
	REG_CPM_USBPCR |= ((1 << 28) | (1 << 29)); //idpullup always active

	/* phy reset */
	REG_CPM_USBPCR |= USBPCR_POR;
	udelay(30);
	REG_CPM_USBPCR &= ~USBPCR_POR;
	udelay(300);

	/* musb reset */
	writeb(USB_BASE + 0x7f, 3);
	while (readb(USB_BASE + 0x7f));
	udelay(300);

	//REG_CPM_OPCR.7=1

	mdelay(500);
	dump_musb_reg();

	/* Disable interrupts */
	writew(USB_REG_INTRINE, 0xffff);
	writew(USB_REG_INTROUTE, 0xffff);
	writeb(USB_REG_INTRUSBE, 0xff);
	
	writeb(USB_REG_FADDR, 0);
	
	writeb(USB_REG_INDEX, 0);
	writeb(USB_REG_CSR0, 0xc0);
	
	writeb(USB_REG_INDEX, 1);
	writew(USB_REG_INMAXP, 512);
	writew(USB_REG_INCSR, 0x2048);
	writew(USB_REG_OUTMAXP, 512);
	writew(USB_REG_OUTCSR, 0x0090);
}

#if 0
void home_pin_init(void)
{
	/* PD19 */
	REG_GPIO_PXINTS(3) = (1 << (19));	/* interrupt */
	REG_GPIO_PXMASKC(3) = (1 << (19));	/* enable */
	REG_GPIO_PXPAT1S(3) = (1 << (19));	/* edge trigger */
	REG_GPIO_PXPAT0C(3) = (1 << (19));	/* failling edge */

	REG_INTC_IMCR(0) = (1 << (14));		/* gpio3 */

	/* int *regs = 0x10001000; */
	/* for ( ; regs < 0x10001034; regs++) { */
	/* 	prints("int reg dump:"); */
	/* 	print_hex32(regs); */
	/* 	prints(" :"); */
	/* 	print_hex32(*regs); */
	/* 	prints("\n"); */
	/* } */
	prints("press key to interrupt !\n");
	prints("press key to interrupt !\n");
	prints("press key to interrupt !\n");
}
#endif

static void delay_with_id_sense(volatile unsigned int delays)
{
	for ( ; delays > 0; delays--) {
		id_interrupt();
	}
}

void otg_func(void)
{
	otg_init();
	
	for (;;) {
		delay_with_id_sense(3000000);
		dump_musb_reg();
	}
}
