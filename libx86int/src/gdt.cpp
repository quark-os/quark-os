#include "gdt.h"

GDT::GDT()
{
	
}

GDT::GDT(void* gdtLocation)
{
	gdt = (uint64_t*) gdtLocation;
	for(int i = 0; i < GDT_SIZE; i++)
	{
		((uint64_t*) gdt)[i] = 0;
	}
}

void GDT::update()
{
	loadGDT(GDT_SIZE * GDT_DESC_SIZE - 1, (uint32_t) gdt);
}

void GDT::writeDescriptor(uint8_t index, void* base, void* limit, uint8_t type)
{
	GDTDescriptor descriptor(base, limit, type);
	gdt[index] = descriptor.getData();
}

GDTDescriptor GDT::getDescriptor(uint8_t index)
{
	return GDTDescriptor(gdt[index]);
}

uint64_t GDT::getData(uint8_t index)
{
	return gdt[index];
}
