#include "idt.h"

IDT::IDT()
{
	
}

IDT::IDT(void* idtLocation)
{
	idt = (uint64_t*) idtLocation;
	for(int i = 0; i < IDT_SIZE; i++)
	{
		((uint64_t*) idt)[i] = 0;
	}
}

void IDT::update()
{
	loadIDT(IDT_SIZE * 8 - 1, (uint32_t) idt);
}

void IDT::writeDescriptor(uint8_t index, void* offset)
{
	IDTDescriptor descriptor(offset);
	idt[index] = descriptor.getData();
}

void IDT::writeDescriptor(uint8_t index, uint16_t tss)
{
	IDTDescriptor descriptor(tss);
	idt[index] = descriptor.getData();
}

IDTDescriptor IDT::getDescriptor(uint8_t index)
{
	return IDTDescriptor(idt[index]);
}

uint64_t IDT::getData(uint8_t index)
{
	return idt[index];
}

