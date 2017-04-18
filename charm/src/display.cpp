#include "display.h"
#include "system.h"
#include "keyboard.h"

Display::Display()
{
	clear();
	console = Console(0, 1, 40, 24);
	info = Window(40, 1, 40, 24);
}

void Display::initialize()
{
	System::eventSystem.addListener(&console);
}

void Display::clear()
{
	char* vga = (char*) 0xB8000;
	for(int i = 0; i < 81 * 24; i++)
	{
		vga[i*2] = 0;
		vga[i*2+1] = 0x07;
	}
}

void Display::putChar(char c)
{
	info.putChar(c);
}

void Display::print(char* msg)
{
	info << msg;
}

void Display::print(uint8_t msg)
{
	info << msg;
}

void Display::print(uint16_t msg)
{
	info << msg;
}

void Display::print(uint32_t msg)
{
	info << msg;
}

void Display::print(uint64_t msg)
{
	info << msg;
}

void Display::printError(char* msg)
{
	info << msg;
}

void Display::printError(uint8_t msg)
{
	info << msg;
}

void Display::printError(uint16_t msg)
{
	info << msg;
}

void Display::printError(uint32_t msg)
{
	info << msg;
}

void Display::printError(uint64_t msg)
{
	info << msg;
}
