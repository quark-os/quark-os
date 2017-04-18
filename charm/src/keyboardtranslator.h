#ifndef KEYBOARDTRANSLATOR_H
#define KEYBOARDTRANSLATOR_H

#include <stdint.h>

class KeyboardTranslator
{
public:

	static void initialize();
	
	static uint8_t lookupToKeycode(uint8_t lookupID);
	
	static uint8_t keycodeToAscii(uint8_t keycode);
	
	static uint8_t asciiToKeycode(uint8_t ascii);
	
private:

	static uint8_t keycodeLookup[256];
	
	static uint8_t asciiLookup[256];
	
};

#endif
