#ifndef DISPLAY_H
#define DISPLAY_H

#include "window.h"
#include "console.h"

class Display
{
public:
	friend class System;

	Display();
	
	void initialize();
	
	void clear();
	
	void putChar(char c);
	void print(char* msg);
	void print(uint8_t msg);
	void print(uint16_t msg);
	void print(uint32_t msg);
	void print(uint64_t msg);
	
	void printError(char* msg);
	void printError(uint8_t msg);
	void printError(uint16_t msg);
	void printError(uint32_t msg);
	void printError(uint64_t msg);
			
private:

	Window info;
	Console console;
	
};

#endif
