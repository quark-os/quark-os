#include "interruptcontroller.h"

IDT InterruptController::idt;

PICDriver InterruptController::pic;

void InterruptController::init(void* idtLocation)
{
	idt = IDT(idtLocation);
	pic = PICDriver(0x20);
	for(int i = 0; i < 256; i++)
	{
		idt.writeDescriptor(i, (void*) 0);
	}
	idt.update();
	pic.update();
}


void InterruptController::addInterrupt(uint8_t id, void* offset)
{
	idt.writeDescriptor(id, offset);
}

void InterruptController::addInterrupt(uint8_t id, uint16_t tss)
{
	idt.writeDescriptor(id, tss);
}
