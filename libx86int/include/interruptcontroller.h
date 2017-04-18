#ifndef __INTCTRL_H
#define __INTCTRL_H

#include "idt.h"
#include "picdriver.h"

class InterruptController
{
public:

	static void init(void* idtLocation);
	
	static void addInterrupt(uint8_t id, void* offset);
	
	static void addInterrupt(uint8_t id, uint16_t tss);
	
private:

	static IDT idt;
	
	static PICDriver pic;

};

#endif
