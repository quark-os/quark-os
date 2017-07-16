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
 
Console kout;

extern "C"
{
	
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
	
	void task1_main()
	{
		asm("sti");
		while(true)
		{
			kout << "a";
			for(int i = 0; i < 10000000; i++) kout << "";
		}
	}
	
	void task2_main()
	{
		asm("sti");
		while(true)
		{
			kout << "b";
			for(int i = 0; i < 10000000; i++) kout << "";
		}
	}
	
	void task3_main()
	{
		asm("sti");
		while(true)
		{
			kout << "c";
			for(int i = 0; i < 10000000; i++) kout << "";
		}
	}
	
	void task4_main()
	{
		asm("sti");
		while(true)
		{
			kout << "d";
			for(int i = 0; i < 10000000; i++) kout << "";
		}
	}
		
	void main()
	{
		kout.clear();
		
		heap = Heap((void*) 0x180000, 0x80000);
		
		asm("cli");
		GDT gdt = GDT(0x280000);
		gdt.writeDescriptor(1, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_CODE_SEL);
		gdt.writeDescriptor(2, (void*) 0, (void*) 0xFFFFFFFF, KERNEL_DATA_SEL);
		gdt.writeDescriptor(3, (void*) 0x180000, (void*) 104, TSS_DESC);
		gdt.update();
		InterruptController::init(0x200000);
		InterruptController::addInterrupt((uint8_t) 0x20, (void*) _preempt);
		scheduler.createProcess((void*) task1_main);
		scheduler.createProcess((void*) task2_main);
		scheduler.createProcess((void*) task3_main);
		scheduler.createProcess((void*) task4_main);
		setup_paging();
		asm("sti");
		
		//asm("cli");
		void* a = inb;
		
		while(true) asm("hlt");
	}
}
