#ifndef __IDT_H
#define __IDT_H

#define IDT_SIZE 256

#include "idtdescriptor.h"

#include <stdint.h>

class IDT
{
public:

	IDT();
	
	IDT(void* idtLocation);
	
	void update();
	
	void writeDescriptor(uint8_t index, void* offset);
	
	IDTDescriptor getDescriptor(uint8_t index);
	
	uint64_t getData(uint8_t index);
	
private:

	uint64_t* idt;

};

extern "C"
{
	void loadIDT(uint16_t size, uint32_t location);
}

#endif
