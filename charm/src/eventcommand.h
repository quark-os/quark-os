#ifndef EVENTCOMMAND_H
#define EVENTCOMMAND_H

#include "event.h"

class EventCommand: public Event
{
public:

	EventCommand();
	
	EventCommand(char* command);
	
	char* getCommand();
	
private:

	char command[64];
	
};

#endif
