#include "eventsystem.h"
#include "system.h"

EventSystem::EventSystem()
{
	eventQueueLength = 0;
	for(int i = 0; i < 64; i++)
	{
		listeners[i] = 0;
		eventQueue[i] = 0;
	}
}

void EventSystem::sendEvent(Event* event)
{
	eventQueue[eventQueueLength] = event;
	eventQueueLength++;
}

int EventSystem::addListener(EventListener* listener)
{
	for(int i = 0; i < 64; i++)
	{
		if(listeners[i] == 0)
		{
			listeners[i] = listener;
			return i;
		}
	}
	return -1;
}

void EventSystem::removeListener(int index)
{
	if(0 <= index && index < 64)
	{
		listeners[index] = 0;
	}
}

void EventSystem::handleEvents()
{
	for(int i = 0; i < eventQueueLength; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			if(listeners[j] != 0)
			{
				listeners[j]->process(eventQueue[i]);
			}
		}
		System::heap.free(eventQueue[i]);
	}
	eventQueueLength = 0;
}
