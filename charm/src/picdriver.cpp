#include "picdriver.h"
#include "pio.h"

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

PICDriver::PICDriver()
{
	
}

PICDriver::PICDriver(int offset)
{
	this->offset = 0x20;
}

void PICDriver::update()
{
	char mask1 = inb(PIC1_DATA);
	char mask2 = inb(PIC2_DATA);
	
	outb(PIC1_COMMAND, 0x11);
	outb(PIC2_COMMAND, 0x11);
	outb(PIC1_DATA, offset);
	outb(PIC2_DATA, offset + 8);
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, 1);
	outb(PIC2_DATA, 1);
	
	outb(PIC1_DATA, 0xFD);
	outb(PIC2_DATA, 0xFF);
}
