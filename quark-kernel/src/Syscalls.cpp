#include "Syscalls.h"
#include "math.h"
#include "Kernel.h"

extern Scheduler scheduler;
extern PageAllocator pageAllocator;

extern "C"
{
	
size_t createProcess(void* entry)
{
	return Kernel::scheduler.createProcess(entry);
}

void* preempt(void* prevStack)
{
	return Kernel::scheduler.nextProcess(prevStack);
}

void allocateRegion(void* linearAddress, size_t size)
{
	void* physicalAddress = Kernel::pageAllocator.allocate(size);
	for(int i = 0; i < pow(2, size); i++)
	{
		Kernel::scheduler.getCurrentProcess().addressSpace.map(
			reinterpret_cast<void*>(reinterpret_cast<uint32_t>(linearAddress) + (i * 4096)), 
			reinterpret_cast<void*>(reinterpret_cast<uint32_t>(physicalAddress) + (i * 4096)));
	}
}

void freeRegion(void* linearAddress, size_t size)
{
	for(int i = 0; i < pow(2, size); i++)
	{
		void* physicalAddress = Kernel::scheduler.getCurrentProcess().addressSpace.getPhysicalAddress(
			reinterpret_cast<void*>(reinterpret_cast<uint32_t>(linearAddress) + (i * 4096)));
		if(reinterpret_cast<uint32_t>(physicalAddress) != 0xFFFFFFFF)
		{
			Kernel::pageAllocator.free(0, physicalAddress);
		}
	}
}

size_t fork(void* entryPoint)
{
	return Kernel::scheduler.createProcess(Kernel::scheduler.getCurrentProcess().childAddressSpace, entryPoint);
}
	
}
