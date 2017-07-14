#include <stddef.h>
#include <stdint.h>


#include "interruptcontroller.h"
#include "gdt.h"
#include "pio.h"
#include "Heap.h"
#include "Console.h"

/*
 * Memory Map:
 * 	0x100000	Kernel
 * 	0x180000	TSSes
 * 	0x200000	IDT
 * 	0x280000	GDT
 * 
 * */
 
Console kout;

extern "C"
{
	
	void switch_context();
	
	void task_init(void* entry, void* stack, uint32_t pid);
	
	void task1();
	
	void task2();
	
	void setup_paging();
	
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
		kout.clear();
		
		Heap heap(0x180000, 0x1000000);		
		
		/*
		asm("cli");
		GDT gdt = GDT(0x280000);
		gdt.writeDescriptor(1, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_CODE_SEL);
		gdt.writeDescriptor(2, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_DATA_SEL);
		gdt.writeDescriptor(3, (void*) 0x180000, (void*) 104, TSS_DESC);
		gdt.update();
		InterruptController::init(0x200000);
		InterruptController::addInterrupt((uint8_t) 0x20, (void*) switch_context);
		setup_paging();
		asm("sti");
		
		asm("jmp task1");*/
		
		asm("cli");
		void* a = inb;
		
		while(true) asm("hlt");
	}
}
