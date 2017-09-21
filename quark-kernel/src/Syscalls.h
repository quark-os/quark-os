#ifndef __SYSCALLS_H
#define __SYSCALLS_H

#include <stdint.h>
#include <stddef.h>

extern "C"
{
	
size_t createProcess(void* entry);
		
void* preempt(void* prevStack);

void allocateRegion(void* linearAddress, size_t size);

void freeRegion(void* linearAddress, size_t size);

size_t fork(void* entryPoint);
	
}

#endif
