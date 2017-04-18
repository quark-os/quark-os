#ifndef EVENTKEYBOARD_H
#define EVENTKEYBOARD_H

#include "event.h"
#include <stdint.h>

class EventKeyboard: public Event
{
public:

	EventKeyboard();
	
	EventKeyboard(uint8_t keycode, bool state);
	
	uint8_t getKeycode();
	
	bool isPressed();
			
private:

	uint8_t keycode;
	
	bool state;
	
};

#endif
