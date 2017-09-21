#include <stddef.h>
#include <stdint.h>

#include "interruptcontroller.h"
#include "gdt.h"
#include "pio.h"
#include "Heap.h"
#include "Console.h"
#include "List.h"
#include "Interrupts.h"
#include "Scheduler.h"
#include "ATAPIODriver.h"
#include "Filesystem.h"
#include "ELF.h"
#include "PageAllocator.h"
#include "math.h"
#include "AddressSpace.h"
#include "paging.h"

/*
 * Memory Map:
 * 	0x100000	Kernel
 * 	0x180000	Heap
 * 	0x200000	IDT
 * 	0x280000	GDT
 * 
 * TODO: Allocate the IDT and GDT on the kernel's heap
 * 
 * */
 
//Console kout;

PageAllocator pageAllocator;

extern "C"
{
		
	void initializeHeap(void* heapLocation, uint32_t size);
	
	struct Block
	{
		void* location;
		uint32_t size;
	};
	
	uint32_t state = 192837368;
	
	uint32_t rand()
	{
		uint32_t x = state;
		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;
		state = x;
		return x;
	}
		
	void main()
	{
		asm("cli");
		kout = Console();
		heap = Heap((void*) 0x180000, 0x280000);
		pageAllocator = PageAllocator(0x400000, 256 * 100); // Allocate pages from a pool of 100MiB, starting at 4MiB
		kout.clear();
		void* gdtLocation = heap.allocate(GDT_SIZE * GDT_DESC_SIZE);
		void* idtLocation = heap.allocate(IDT_SIZE * IDT_DESC_SIZE);
		
		GDT gdt = GDT(gdtLocation);
		gdt.writeDescriptor(1, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_CODE_SEL);
		gdt.writeDescriptor(2, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_DATA_SEL);
		gdt.update();
		InterruptController::init(idtLocation);
		for(int i = 0; i < 256; i++)
		{
			InterruptController::addInterrupt((uint8_t) i, (void*) _preempt);
		}
		InterruptController::addInterrupt((uint8_t) 0x80, (void*) _syscall);
		InterruptController::addInterrupt((uint8_t) 0x0D, (void*) gp_fault);
		InterruptController::addInterrupt((uint8_t) 0x0E, (void*) page_fault);
		
		AddressSpace kernelAddressSpace = AddressSpace();
		for(int i = 0; i < 24; i++)
		{
			for(int j = 0; j < 1024; j++)
			{
				void* address = (void*) (j*4096*4096 + i*4096);
				kernelAddressSpace.map(address, address);
			}
		}
		
		kout << sizeof(PagingStructure) << "\n";
		
		kernelAddressSpace.load();
		start_paging();
		
		kernelAddressSpace.map((void*) 0x800000, (void*)0xF00000);
		kernelAddressSpace.invalidate((void*) 0x800000);
		int* array1 = (int*) 0x800000;
		int* array2 = (int*) 0xF00000;
		kout << "0xF00000 before write: ";
		for(int i = 0; i < 4; i++)
		{
			kout << array2[i] << " ";
		}
		kout << "\n";
		for(int i = 0; i < 4; i++)
		{
			array1[i] = rand();
		}
		kout << "0xF00000 after write: ";
		for(int i = 0; i < 4; i++)
		{
			kout << array2[i] << " ";
		}
		kout << "\n";
		
		ATAPIODriver::initialize();
		Filesystem fs;
		fs.mount();
		
		const char* filename = "quark-test";
		File* file = fs.getFileInfo(filename);
		if(file != 0)
		{
			kout << "File '" << filename << "' contains " << (uint32_t) file->fileSectors << " sectors\n";
			void* fileDest = heap.allocate(file->fileSectors * 512);
			fs.loadFile(fileDest, filename);
			ELF prog(fileDest);
			prog.load();
			scheduler.createProcess(prog.getEntryPoint(), (void*) 0x410000);
		}
		else
		{
			kout << "File not found\n";
		}
		
		asm("sti");
		void* a = inb;
		while(true) asm("hlt");
	}
}
