#ifndef SYSTEM_H
#define SYSTEM_H

#include "display.h"
#include "ps2controller.h"
#include "eventsystem.h"
#include <heap.h>
#include <stdint.h>

class System
{
public:

	static EventSystem eventSystem;

	static Display display;
	
	static PS2Controller ps2;
	
	static Heap heap;

	static void initialize();
	
	static void run();
	
	static void panic();
			
private:
	
	
	
};

#endif
