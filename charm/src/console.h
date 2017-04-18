#ifndef CONSOLE_H
#define CONSOLE_H

#include "window.h"
#include "event.h"
#include "eventlistener.h"

class Console: public Window, public EventListener
{
public:

		Console();
		
		Console(int x, int y, int width, int height);
		
		virtual void process(Event* event);
			
private:
	
	int bufferLength;
	
	char commandBuffer[256];
	
};

#endif
