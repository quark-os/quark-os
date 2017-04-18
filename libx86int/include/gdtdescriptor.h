#ifndef __GDTDESC_H
#define __GDTDESC_H

#define FLAG_DEFAULT 0xC0	// Uses page-level granularity and is a 32-bit descriptor

#include <stdint.h>

class GDTDescriptor
{
public:

	GDTDescriptor();
	
	GDTDescriptor(void* base, void* limit, uint8_t type);
		
	GDTDescriptor(uint64_t data);
	
	uint64_t getData();

private:
	
	uint32_t base;
	uint32_t limit;
	uint8_t access;
	uint8_t flags;
	
};

#endif


