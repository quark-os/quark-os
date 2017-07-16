#include "Syscalls.h"
#include "Scheduler.h"

extern Scheduler scheduler;

extern "C"
{
	
size_t createProcess(void* entry)
{
	return scheduler.createProcess(entry);
}

void* preempt(void* prevStack)
{
	return scheduler.nextProcess(prevStack);
}
	
}
