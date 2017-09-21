#include "Syscalls.h"
#include "Scheduler.h"
#include "math.h"

extern Scheduler scheduler;
extern PageAllocator pageAllocator;

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

void allocateRegion(void* linearAddress, size_t size)
{
	void* physicalAddress = pageAllocator.allocate(size);
	for(int i = 0; i < pow(2, size); i++)
	{
		scheduler.getCurrentProcess().addressSpace.map(
			reinterpret_cast<void*>(reinterpret_cast<uint32_t>(linearAddress) + (i * 4096)), 
			reinterpret_cast<void*>(reinterpret_cast<uint32_t>(physicalAddress) + (i * 4096)));
	}
}

void freeRegion(void* linearAddress, size_t size)
{
	for(int i = 0; i < pow(2, size); i++)
	{
		void* physicalAddress = scheduler.getCurrentProcess().addressSpace.getPhysicalAddress(
			reinterpret_cast<void*>(reinterpret_cast<uint32_t>(linearAddress) + (i * 4096)));
		if(reinterpret_cast<uint32_t>(physicalAddress) != 0xFFFFFFFF)
		{
			pageAllocator.free(0, physicalAddress);
		}
	}
}

size_t fork(void* entryPoint)
{
	return scheduler.createProcess(scheduler.getCurrentProcess().childAddressSpace, entryPoint);
}
	
}
