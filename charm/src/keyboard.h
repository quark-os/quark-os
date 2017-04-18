#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BUFFER_INPUT 0x02
#define BUFFER_OUTPUT 0x01

#define CMD_DISABLE 0xF5
#define CMD_ENABLE 0xF4
#define CMD_SCANCODESET 0xF0
#define CMD_SETLED 0xED
#define CMD_WRITECFG 0x60

#include <stdint.h>

class Keyboard
{
public:
		
	static void initialize();
	
	static bool getKeyState(uint8_t keycode);
	
	static void scan();
			
private:

	static uint8_t readByte();
	
	static bool waitForBuffer(uint8_t buffer, bool state);
	
	static bool waitForAck();

	static bool keyflags[128];
		
};

#endif
