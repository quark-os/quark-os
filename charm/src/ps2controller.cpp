#include "ps2controller.h"
#include "pio.h"
#include "eventcommand.h"

#define BUFFER_INPUT 0x02
#define BUFFER_OUTPUT 0x01

PS2Controller::PS2Controller()
{
	
}

void PS2Controller::initialize()
{
	setTranslatorState(false);
}

bool PS2Controller::setPortState(uint8_t port, bool enabled)
{
	switch(port)
	{
	case 1:
		if(enabled)
			return write(0x64, 0xAE);
		else
			return write(0x64, 0xAD);
	case 2:
		if(enabled)
			return write(0x64, 0xA8);
		else
			return write(0x64, 0xA7);
	default:
		return false;
	}
}

bool PS2Controller::setTranslatorState(bool enabled)
{
	if(!write(0x64, 0x20))
		return false;
	uint8_t config = read(0x60);
	config &= 0xBF;
	
	if(!write(0x64, 0x60))
		return false;
	if(!write(0x60, config))
		return false;
	return true;
}

bool PS2Controller::isTranslatorEnabled()
{
	if(!write(0x64, 0x20))
		return false;
	
	uint8_t config = read(0x60);
	return (config & 0x40) == 0 ? false : true;
}

bool PS2Controller::write(uint8_t port, uint8_t byte)
{
	if(waitForBuffer(BUFFER_INPUT, false))
		outb(port, byte);
	else
		return false;
	return true;
}

uint8_t PS2Controller::read(uint8_t port)
{
	if(waitForBuffer(BUFFER_OUTPUT, true))
		return inb(port);
	else
		return 0x00;
}

bool PS2Controller::waitForBuffer(uint8_t buffer, bool state)
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
