#include "Console.h"

template Console& Console::operator<< <int>(int);
template Console& Console::operator<< <unsigned int>(unsigned int);
template Console& Console::operator<< <long int>(long int);
template Console& Console::operator<< <long unsigned int>(long unsigned int);

Console::Console()
{
	vga = (char*) 0xB8000;
	width = 80;
	height = 25;
	cursorX = 0;
	cursorY = 0;
}

Console::Console(char* vga, int width, int height)
{
	this->vga = vga;
	this->width = width;
	this->height = height;
	this->cursorX = cursorX;
	this->cursorY = cursorY;
}

Console& Console::operator<<(const char* string)
{
	print(string);
	return *this;
}  
	
Console& Console::operator<<(void* ptr)
{
	char string[sizeof(void*) * 2]; 
	intToString((uint32_t) ptr, string, 16, true);
	print(string);
	return *this;
}

void Console::clear()
{
	for(int x = 0; x < width; x++) 
	{
		for(int y = 0; y < height; y++)
		{
			vga[2 * (y * width + x)] = ' ';
		}
	}
	cursorX = 0;
	cursorY = 0;
}

void Console::print(char* string)
{
	for(int i = 0; string[i] != 0; i++)
	{
		if(string[i] == '\n')
		{
			newline();
		}
		else
		{ 
			vga[2 * (cursorY * width + cursorX)] = string[i];
			cursorX++;
			if(cursorX == width)
			{
				cursorX = 0;
				newline();
			}
		}
	}
}

void Console::newline()
{
	cursorX = 0;
	cursorY++;
	if(cursorY == height)
	{
		cursorY--;
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++) 
			{
				if(y < height - 1)
				{
					vga[2 * (y * width + x)] = vga[2 * ((y+1) * width + x)];
				}
				else
				{
					vga[2 * (y * width + x)] = ' ';
				}
			}
		}
	}
}


