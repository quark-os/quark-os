#ifndef __KERNEL_H
#define __KERNEL_H

#include "Heap.h"
#include "PageAllocator.h"
#include "Console.h"
#include "Filesystem.h"
#include "gdt.h"
#include "Scheduler.h"
#include "AddressSpace.h"

#include <stddef.h>

class Kernel
{
public:

	//static const void* KERNEL_BASE = 0xC0000000;

	//static const void* HEAP_LOCATION = KERNEL_BASE + 0x180000;
		
	static const size_t HEAP_SIZE = 0x180000;
	
	//static const void* INITIAL_PAGE_DIRECTORY_ADDRESS = 0xC0300000;

	static Heap& kernelHeap;
		
	static PageAllocator pageAllocator;
		
	static Console& console;
		
	static Filesystem fs;
		
	static GDT gdt;
		
	static Scheduler scheduler;
		
	static AddressSpace addressSpace;
		
	static void initialize();
		
	static void panic();

private:

	static void initializePaging();
	
};

#endif
