#include "interrupts.h"
#include "pio.h"

extern "C"
{

	void picEOI()
	{
		outb(0xA0, 0x20);
		outb(0x20, 0x20);
	}

}
