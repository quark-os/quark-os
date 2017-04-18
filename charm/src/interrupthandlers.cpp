#include "interrupts.h"
#include "pio.h"
#include "keyboardtranslator.h"
#include "keyboardevent.h"
#include "keyboard.h"

extern "C"
{
	
	void handleKeyboardEvent()
	{
		Keyboard::scan();
	}
	
}
