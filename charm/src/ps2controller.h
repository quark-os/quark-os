#ifndef PS2CONTROLLER_H
#define PS2CONTROLLER_H

#include <stdint.h>

#include "eventlistener.h"
#include "event.h"

class PS2Controller
{
public:

	PS2Controller();
	
	void initialize();
	
	void setKeyboardState(bool enabled);
	
	void setMouseState(bool enabled);
				
private:

	bool setPortState(uint8_t port, bool enabled);
	
	bool setTranslatorState(bool enabled);
	
	bool isTranslatorEnabled();

	bool write(uint8_t port, uint8_t byte);
	
	uint8_t read(uint8_t port);

	bool waitForBuffer(uint8_t buffer, bool state);
	
};

#endif
