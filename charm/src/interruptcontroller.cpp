#include "interruptcontroller.h"
#include "interrupts.h"

IDT InterruptController::idt;

PICDriver InterruptController::pic;

void InterruptController::init(void* idtLocation)
{
	idt = IDT(idtLocation);
	pic = PICDriver(0x20);
	for(int i = 0; i < 256; i++)
	{
		if(i >= 0x20 && i < 0x30)
		{
			idt.writeDescriptor(i, pic_routine);
		}
		else
		{
			idt.writeDescriptor(i, test_handler);
		}
	}
	idt.writeDescriptor(0x08, double_fault);
	idt.writeDescriptor(0x0A, unhandled_exception);
	idt.writeDescriptor(0x0B, unhandled_exception);
	idt.writeDescriptor(0x0C, unhandled_exception);
	idt.writeDescriptor(0x0D, unhandled_exception);
	idt.writeDescriptor(0x0E, unhandled_exception);
	idt.writeDescriptor(0x11, unhandled_exception);
	idt.writeDescriptor(0x1E, unhandled_exception);
	idt.writeDescriptor(0x21, keyboard_isr);
	idt.update();
	pic.update();
}
