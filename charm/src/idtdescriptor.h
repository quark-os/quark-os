#ifndef __IDTDESC_H
#define __IDTDESC_H

#include <stdint.h>

class IDTDescriptor
{
public:

	IDTDescriptor();
	
	IDTDescriptor(void* offset);
	
	IDTDescriptor(uint64_t data);
	
	uint64_t getData();
	
	void* getOffset();

	void setOffset(void* offset);

private:
	
	uint16_t offset_lo;
	uint16_t selector;
	uint8_t zero;
	uint8_t type;
	uint16_t offset_hi;
	
};

#endif
