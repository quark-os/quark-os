#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdint.h>
#include <stddef.h>

class Process
{
public:

	Process();
	
	Process(void* stack, void* entry, size_t pid);
	
	void initialize();
	
	void* stack;
	
	void* entry;
	
	size_t pid;

};

#endif
