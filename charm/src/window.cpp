#include "window.h"
#include "system.h"

#define BOX_TOPLEFT 201
#define BOX_TOPRIGHT 187
#define BOX_BOTTOMLEFT 200
#define BOX_BOTTOMRIGHT 188
#define BOX_LATSIDE 186
#define BOX_VERTSIDE 205

Window::Window()
{
	
}

Window::Window(int x, int y, int width, int height)
{
	this->vga = (char*) 0xB8000;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->cursorX = 0;
	this->cursorY = 0;
	
	for(int boxY = 0; boxY < height; boxY++)
	{
		if(boxY == 0)
		{
			vga[coordToOffset(x, y)] = BOX_TOPLEFT;
			for(int boxX = 1; boxX < width - 1; boxX++)
			{
				vga[coordToOffset(x + boxX, y)] = BOX_VERTSIDE;
			}
			vga[coordToOffset(x + width - 1, y)] = BOX_TOPRIGHT;
		}
		else if(boxY == height - 1)
		{
			for(int boxY = 1; boxY < height - 1; boxY++)
			{
				vga[coordToOffset(x, y + boxY)] = BOX_LATSIDE;
				vga[coordToOffset(x + width - 1, y + boxY)] = BOX_LATSIDE;
			}
		}
		else
		{
			vga[coordToOffset(x, y + height - 1)] = BOX_BOTTOMLEFT;
			for(int boxX = 1; boxX < width - 1; boxX++)
			{
				vga[coordToOffset(x + boxX, y + height - 1)] = BOX_VERTSIDE;
			}
			vga[coordToOffset(x + width - 1, y + height - 1)] = BOX_BOTTOMRIGHT;
		}
	}
}

void Window::operator<<(char* data)
{
	printString(data);
}

void Window::operator<<(uint8_t data)
{
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char out[] = {0, 0};
	for(int i = 0; i < 2; i++)
	{
		out[0] = digits[data & 0x0F];
		printString(out);
		data = data >> 4;
	}
}

void Window::operator<<(uint16_t data)
{
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char out[] = {0, 0};
	for(int i = 0; i < 4; i++)
	{
		out[0] = digits[data & 0x0F];
		printString(out);
		data = data >> 4;
	}
}

void Window::operator<<(uint32_t data)
{
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char out[] = {0, 0};
	for(int i = 0; i < 6; i++)
	{
		out[0] = digits[data & 0x0F];
		printString(out);
		data = data >> 4;
	}
}

void Window::operator<<(uint64_t data)
{
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	char out[] = {0, 0};
	for(int i = 0; i < 16; i++)
	{
		out[0] = digits[data & 0x0F];
		printString(out);
		data = data >> 4;
	}
}

void Window::putChar(char c)
{
	if(c == '\n')
	{
		newline();
	}
	else
	{
		vga[coordToOffset(cursorX + 1 + this->x, cursorY + 1 + this->y)] = c;
		if(cursorX < width - 3)
		{
			cursorX++;
		}
		else
		{
			newline();
		}
	}
}

void Window::clear()
{
	for(int x = 1; x < width - 1; x++)
	{
		for(int y = 1; y < height - 1; y++)
		{
			vga[coordToOffset(x + this->x, y + this->y)] = ' ';
		}
	}
	
}

int Window::coordToOffset(int x, int y)
{
	return (80*y + x) * 2;
}

void Window::printString(char* data)
{
	for(int i = 0; data[i] != 0; i++)
	{
		if(data[i] == '\n')
		{
			newline();
		}
		else
		{
			vga[coordToOffset(cursorX + 1 + this->x, cursorY + 1 + this->y)] = data[i];
			if(cursorX < width - 3)
			{
				cursorX++;
			}
			else
			{
				newline();
			}
		}
	}
}

void Window::newline()
{
	if(cursorY < (height - 3))
	{
		cursorY++;
		cursorX = 0;
	}
	else
	{
		for(int i = x + 1; i < (x + width - 1); i++)
		{
			for(int j = y + 1; j < (y + height - 2); j++)
			{
				vga[coordToOffset(i, j)] = vga[coordToOffset(i, j+1)];
			}
		}
		for(int i = x + 1; i < (x + width - 1); i++)
		{
			vga[coordToOffset(i, y + height - 2)] = ' ';
		}
		cursorX = 0;
	}
}
