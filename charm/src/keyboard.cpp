#include "keyboard.h"
#include "keyboardtranslator.h"
#include "pio.h"
#include "system.h"
#include "eventkeyboard.h"

bool Keyboard::keyflags[128];

void Keyboard::initialize()
{
	
}

bool Keyboard::getKeyState(uint8_t keycode)
{
	return keyflags[keycode];
}

void Keyboard::scan()
{
	uint8_t scancode[8] = {0};
	int scancodeLength = 0;
	bool pressed = true;

	uint8_t byte = 0;
	do
	{
		byte = readByte();
		if(byte != 0)
		{
			scancode[scancodeLength] = byte;
			scancodeLength++;
		}
	} while(byte != 0);
	
	if(scancode[0] == 0xF0)
	{
		
		pressed = false;
	}
	else if(scancode[0] == 0xE0)
	{
		if(scancode[1] == 0xF0)
		{
			pressed = false;
		}
	}
	
	if(scancodeLength > 0)
	{
		uint8_t lookup = scancode[scancodeLength - 1] + (scancode[0] == 0xE0 ? 0x80 : 0x00);
		EventKeyboard* event = new EventKeyboard(KeyboardTranslator::lookupToKeycode(lookup), pressed);
		System::eventSystem.sendEvent(event);
	}
}

uint8_t Keyboard::readByte()
{
	if(waitForBuffer(BUFFER_OUTPUT, true))
	{
		uint8_t b = inb(0x60);
		return b;
	}
	else
		return 0;
}

bool Keyboard::waitForBuffer(uint8_t buffer, bool state)
{
	int counter = 0;
	uint8_t status = 0x00;
	do
	{
		status = inb(0x64);
		io_wait();
		counter++;
		if(counter > 16)
			return false;
	} while((status & buffer) != (state ? buffer : 0));
	return true;
}

bool Keyboard::waitForAck()
{
	if(waitForBuffer(BUFFER_OUTPUT, false))
	{
		uint8_t response = inb(0x60);
		if(response == 0xFA)
			return true;
		else
			return false;
	}
	else
	{
		System::display.printError("Keyboard timeout\n");
		return false;
	}
}
