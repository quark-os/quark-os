#include "idtdescriptor.h"

IDTDescriptor::IDTDescriptor()
{
	offset_hi = 0;
	offset_lo = 0;
	type = 0;
	zero = 0;
	offset_hi = 0;
}

IDTDescriptor::IDTDescriptor(void* offset)
{
	offset_lo = (uint16_t) offset;
	offset_hi = ((uint32_t) offset) >> 16;
	type = 0x8E;
	zero = 0;
	selector = 0x0008;
}

IDTDescriptor::IDTDescriptor(uint16_t tss)
{
	offset_lo = 0;
	offset_hi = 0;
	type = 0x85;
	zero = 0;
	selector = tss;
}

IDTDescriptor::IDTDescriptor(uint64_t data)
{
	
}

uint64_t IDTDescriptor::getData()
{
	uint64_t offsetHigh = offset_hi;
	uint64_t type = this->type;
	uint64_t selector = this->selector;
	
	return offset_lo + (selector << 16) + (type << 40) + (offsetHigh << 48);
}

void* IDTDescriptor::getOffset()
{
	uint32_t offset_hi32 = offset_hi;
	offset_hi32 = offset_hi32 >> 16;
	return (void*)(offset_hi32 + offset_lo);
}

void IDTDescriptor::setOffset(void* offset)
{
	offset_lo = (uint16_t) offset;
	offset_hi = ((uint16_t) offset) >> 16;
}
