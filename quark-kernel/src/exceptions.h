#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

#include <stddef.h>
#include <stdint.h>

extern "C"
{

void handleGeneralProtectionFault(uint32_t errorCode);
void handlePageFault(uint32_t errorCode);
	
}

#endif
