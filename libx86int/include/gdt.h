#ifndef __GDT_H
#define __GDT_H

#define KERNEL_CODE_SEL 0x9A
#define KERNEL_DATA_SEL 0x92
#define TSS_DESC 0x89

#define GDT_SIZE 256
#define GDT_DESC_SIZE 8

#include "gdtdescriptor.h"

#include <stdint.h>

class GDT
{
public:

	GDT();
	
	GDT(void* gdtLocation);
	
	void update();
	
	void writeDescriptor(uint8_t index, void* base, void* limit, uint8_t type);
	
	GDTDescriptor getDescriptor(uint8_t index);
	
	uint64_t getData(uint8_t index);
	
private:

	uint64_t* gdt;

};

extern "C"
{
	void loadGDT(uint16_t size, uint32_t location);
}

#endif

