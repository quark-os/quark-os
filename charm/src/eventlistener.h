#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "event.h"

class EventListener
{
public:

	EventListener();
	
	virtual void process(Event* event) = 0;
			
private:
	
};

#endif
