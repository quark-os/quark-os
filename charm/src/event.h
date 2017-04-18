#ifndef EVENT_H
#define EVENT_H

#define EVENT_UNDEF 0
#define EVENT_KEYBOARD 1
#define EVENT_COMMAND 2

class Event
{
public:

	Event();
	
	int getID();
			
protected:

	int id;
	
};

#endif
