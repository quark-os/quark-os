#include <stddef.h>
#include <stdint.h>

#include "interruptcontroller.h"
#include "gdt.h"
#include "pio.h"

/*
 * Memory Map:
 * 	0x100000	Kernel
 * 	0x180000	TSSes
 * 	0x200000	IDT
 * 	0x280000	GDT
 * 
 * */

extern "C"
{
	
	typedef struct FreeBlockHeader
	{
		uint32_t flag;
		uint32_t size;
		uint32_t link;
	};
	
	char* vga;
		
	int cursorX, cursorY;
	
	void switch_context();
	
	void task_init(void* entry, void* stack, uint32_t pid);
	
	void task1();
	
	void task2();
	
	void setup_paging();
	
	void initializeHeap(void* heapLocation, uint32_t size);
	
	void* allocate(FreeBlockHeader* avail, uint32_t size);
	
	void itoa(uint32_t src, char* dest, int base = 10, bool trailingZeros = false, bool bigEndian = true)
	{
		char* digits = "0123456789ABCDEF";
		int i = 0;
		while((src > 0 || trailingZeros) && i < (base == 16 ? 8 : (base == 2 ? 32 : 10)))
		{
			dest[i] = digits[src % base];
			src /= base;
			i++;
		}
		dest[i] = 0;
		if(bigEndian)
		{
			for(int j = 0; j < (i % 2 == 0 ? (i / 2) : ((i - 1) / 2)); j++)
			{
				char buffer = dest[j];
				dest[j] = dest[i - j - 1];
				dest[i - j - 1] = buffer;
			}
		}
	}
	
	void printString(char* str)
	{
		for(int i = 0; str[i] != 0; i++)
		{
			if(str[i] != '\n')
			{
				vga[(cursorY * 80 + cursorX) * 2] = str[i];
			}
			else
			{
				cursorX = 0;
				cursorY++;
			}
			
			if(cursorX == 79)
			{
				cursorX = 0;
				cursorY++;
			}
			else
			{
				cursorX++;
			}
		}
	}
	
	void main()
	{
		vga = (char*) 0xB8000;
		cursorX = 0;
		cursorY = 0;
		
		for(int i = 0; i < 80; i++)
		{
			for(int j = 0; j < 25; j++)
			{
				vga[(j * 80 + i) * 2] = ' ';
			}
		}
		
		FreeBlockHeader heap;
		heap.link = 0x180000;
		initializeHeap((void*) heap.link, 0x80000);
		int* num = allocate(&heap, sizeof(int));
		char s[64];
		itoa((uint32_t) num, s, 16, true, true);
		printString(s);
		
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
