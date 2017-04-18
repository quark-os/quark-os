#include "event.h"

Event::Event()
{
	id = EVENT_UNDEF;
}

int Event::getID()
{
	return id;
}
