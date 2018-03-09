#include "Kernel.h"
#include "Interrupts.h"
#include "interruptcontroller.h"
#include "ATAPIODriver.h"
#include "paging.h"
#include "PagingStructure.h"
#include "PageTable.h"

Heap& Kernel::kernelHeap = heap;
		
PageAllocator Kernel::pageAllocator;
		
Console& Kernel::console = kout;
		
Filesystem Kernel::fs;
		
GDT Kernel::gdt;
		
Scheduler Kernel::scheduler;

AddressSpace Kernel::addressSpace;

void Kernel::initialize()
{
	kout = Console();
	heap = Heap((void*) (0xC0180000), 0x180000);
	//Kernel::heap = heap;
	//console = kout;
	console.clear();
	pageAllocator = PageAllocator(0x400000, 256 * 100);
	void* gdtLocation = heap.allocate(GDT_SIZE * GDT_DESC_SIZE);
	void* idtLocation = heap.allocate(IDT_SIZE * IDT_DESC_SIZE);
	gdt = GDT(gdtLocation);
	gdt.writeDescriptor(1, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_CODE_SEL);
	gdt.writeDescriptor(2, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_DATA_SEL);
	gdt.update();
	InterruptController::init(idtLocation);
	for(int i = 0; i < 256; i++)
	{
		InterruptController::addInterrupt((uint8_t) i, (void*) _preempt);
	}
	InterruptController::addInterrupt((uint8_t) 0x80, (void*) _syscall);
	InterruptController::addInterrupt((uint8_t) 0x00, (void*) handleDivisionByZero);
	InterruptController::addInterrupt((uint8_t) 0x06, (void*) handleInvalidOpcode);
	InterruptController::addInterrupt((uint8_t) 0x08, (void*) handleDoubleFault);
	InterruptController::addInterrupt((uint8_t) 0x0D, (void*) handleGeneralProtectionFault);
	InterruptController::addInterrupt((uint8_t) 0x0E, (void*) handlePageFault);
			
	addressSpace = AddressSpace();
	addressSpace.initialize();
	for(uint32_t i = 0; i < 1024*4; i++)
	{
		addressSpace.map(reinterpret_cast<void*>(i * 4096 + 0xC0000000), reinterpret_cast<void*>(i * 4096));
	}
	addressSpace.load();
	start_paging();
	
	ATAPIODriver::initialize();
	fs.mount();
}

void Kernel::panic()
{
	console << "oh no\n";
	asm("cli");
	asm("hlt");
}

void Kernel::initializePaging()
{
	//PageTable pageDirectory((PagingStructure*) INITIAL_PAGE_DIRECTORY_ADDRESS);
}
