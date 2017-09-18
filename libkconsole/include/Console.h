#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdint.h>

class Console
{
public:

	Console();
	
	Console(char* vga, int width, int height);

	template <class T>
	Console& operator<<(T number)
	{
		char string[sizeof(T) * 8];
		intToString(number, string);
		print(string);
		return *this;
	}
	
	Console& operator<<(const char* string);
	
	Console& operator<<(void* ptr);
	
	void memdump(void* location, uint32_t size);
	
	void clear();
	
protected:

	char* vga;
	
	int cursorX, cursorY;
	
	int width, height;
	
	void print(char* string);
	
	void newline();
		
	template<class Num>
	void intToString(Num number, char* destination, int base = 10, bool trailingZeroes = false, bool bigEndian = true)
	{
		if(base != 2 && base != 10 && base != 16)
		{
			print("[WARNING] Unsupported base used; aborting conversion\n");
			destination[0] = 0;
			return;
		}
		
		char* digits = "0123456789ABCDEF";
		int i = 0;
		while((number > 0 || trailingZeroes) && i < (base == 16 ? (sizeof(Num) * 2) : (base == 2 ? (sizeof(Num) * 8) : (int) (((float) sizeof(Num) * 8.0) * 0.301029995664) + 1)))
		{
			destination[i] = digits[number % base];
			number /= base;
			i++;
		}
		destination[i] = 0;
		if(bigEndian)
		{
			for(int j = 0; j < (i % 2 == 0 ? (i / 2) : ((i - 1) / 2)); j++)
			{
				char buffer = destination[j];
				destination[j] = destination[i - j - 1];
				destination[i - j - 1] = buffer;
			}
		}
	}
	
};

extern Console kout;

#endif
