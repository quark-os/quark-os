#include "gdtdescriptor.h"

GDTDescriptor::GDTDescriptor()
{
	base = 0;
	limit = 0;
	access = 0;
	flags = 0;
}

GDTDescriptor::GDTDescriptor(void* base, void* limit, uint8_t type)
{
	this->base = base;
	this->limit = limit;
	this->access = type;
	this->flags = ((uint32_t) limit) > 65535 ? FLAG_DEFAULT : 0x40;
}

GDTDescriptor::GDTDescriptor(uint64_t data)
{
	
}

uint64_t GDTDescriptor::getData()
{
	uint8_t target[8];
	uint32_t limit = this->limit;
	if (limit > 65536) {
        limit = limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
	
	target[0] = this->limit & 0xFF;
    target[1] = (this->limit >> 8) & 0xFF;
    target[6] |= (this->limit >> 16) & 0xF;
 
    target[2] = this->base & 0xFF;
    target[3] = (this->base >> 8) & 0xFF;
    target[4] = (this->base >> 16) & 0xFF;
    target[7] = (this->base >> 24) & 0xFF;
 
    target[5] = this->access;
    
    uint64_t data[8] = {0};
    for(int i = 0; i < 8; i++)
    {
		data[i] = target[i];
	}
	
	return data[0] 
		+ data[1] * 0x100 
		+ data[2] * 0x10000 
		+ data[3] * 0x1000000 
		+ data[4] * 0x100000000 
		+ data[5] * 0x10000000000
		+ data[6] * 0x1000000000000
		+ data[7] * 0x100000000000000;
}

