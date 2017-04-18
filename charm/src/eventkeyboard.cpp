#include "eventkeyboard.h"


EventKeyboard::EventKeyboard()
	: Event()
{
	id = EVENT_KEYBOARD;
}

EventKeyboard::EventKeyboard(uint8_t keycode, bool state)
	: Event()
{
	id = EVENT_KEYBOARD;
	this->keycode = keycode;
	this->state = state;
}

uint8_t EventKeyboard::getKeycode()
{
	return keycode;
}

bool EventKeyboard::isPressed()
{
	return state;
}
