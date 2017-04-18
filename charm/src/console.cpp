#include "console.h"
#include "keyboardtranslator.h"
#include "keyboard.h"
#include "system.h"
#include "eventkeyboard.h"
#include "eventcommand.h"

Console::Console()
	: Window(), EventListener()
{
	bufferLength = 0;
	for(int i = 0; i < 256; i++)
		commandBuffer[i] = 0;
}

Console::Console(int x, int y, int width, int height)
	: Window(x, y, width, height), EventListener()
{
	bufferLength = 0;
	for(int i = 0; i < 256; i++)
		commandBuffer[i] = 0;
}

void Console::process(Event* event)
{
	if(event->getID() == EVENT_KEYBOARD)
	{
		EventKeyboard* keyboardEvent = event;
		char c = KeyboardTranslator::keycodeToAscii(Keyboard::getKeyState(0x33) ? keyboardEvent->getKeycode() + 0x60 : keyboardEvent->getKeycode());
		if(keyboardEvent->isPressed() && c != '\0')
		{
			if(c == '\n')
			{
				newline();
				commandBuffer[bufferLength] = c;
				System::eventSystem.sendEvent(new EventCommand(commandBuffer));
			}
			else
			{
				putChar(c);
				commandBuffer[bufferLength] = c;
				bufferLength++;
			}
			// Handle other keys...
		}
	}
}


