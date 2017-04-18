#ifndef __WINDOW_H
#define __WINDOW_H

#include <stdint.h>

#include "stream.h"

class Window : public Stream
{
public:

	Window();
	
	Window(int x, int y, int width, int height);
	
	void operator<<(char* data);
	
	void operator<<(uint8_t data);
	
	void operator<<(uint16_t data);
	
	void operator<<(uint32_t data);
	
	void operator<<(uint64_t data);
	
	void putChar(char c);
	
	void clear();
	
protected:

	char* vga;

	int x, y;
	
	int width, height;

	int cursorX, cursorY;
	
	int coordToOffset(int x, int y);
	
	void printString(char* data);
	
	void newline();

};

#endif
