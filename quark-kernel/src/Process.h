#ifndef __PROCESS_H
#define __PROCESS_H

#include "AddressSpace.h"

#include <stdint.h>
#include <stddef.h>

class Process
{
public:

	Process();
	
	Process(void* stack, void* entry, size_t pid);
	
	Process(void* stack, void* entry, size_t pid, AddressSpace addressSpace);
	
	void initialize();
	
	void* stack;
	
	void* entry;
	
	size_t pid;
	
	AddressSpace addressSpace;
	
	AddressSpace childAddressSpace;

};

#endif
