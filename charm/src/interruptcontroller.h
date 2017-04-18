#ifndef __INTCTRL_H
#define __INTCTRL_H

#include "idt.h"
#include "picdriver.h"

class InterruptController
{
public:

	static void init(void* idtLocation);
	
private:

	static IDT idt;
	
	static PICDriver pic;

};

#endif
