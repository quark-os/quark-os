#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "event.h"
#include "eventlistener.h"

class EventSystem
{
public:

	EventSystem();
	
	void sendEvent(Event* event);
	
	int addListener(EventListener* listener);
	
	void removeListener(int index);
	
	void handleEvents();
			
private:
	
	EventListener* listeners[64];
	
	Event* eventQueue[64];
	
	int eventQueueLength;
	
};

#endif
