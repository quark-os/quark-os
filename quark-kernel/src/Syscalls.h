#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include <stdint.h>
#include <stddef.h>

extern "C"
{
	
size_t createProcess(void* entry);
		
void* preempt(void* prevStack);
	
}

#endif
