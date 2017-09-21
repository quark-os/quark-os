#include "exceptions.h"
#include "Console.h"

void handleGeneralProtectionFault(uint32_t errorCode)
{
	kout << "General protection fault with error code " << errorCode;
	asm("cli");
	asm("hlt");
}

void handlePageFault(uint32_t errorCode)
{
	kout << "Page fault with error code " << errorCode;
	asm("cli");
	asm("hlt");
}
