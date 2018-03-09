#include <stddef.h>
#include <stdint.h>

#include "pio.h"
#include "Scheduler.h"
#include "ELF.h"
#include "Kernel.h"
#include "Heap.h"
 
extern "C"
{
		
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
		
		Kernel::initialize();
		
		/*const char* filename = "quark-test";
		File* file = Kernel::fs.getFileInfo(filename);
		if(file != 0)
		{
			kout << "File '" << filename << "' contains " << (uint32_t) file->fileSectors << " sectors\n";
			void* fileDest = Kernel::kernelHeap.allocate(file->fileSectors * 512);
			Kernel::fs.loadFile(fileDest, filename);
			ELF prog(fileDest);
			prog.load();
			Kernel::scheduler.createProcess(prog.getEntryPoint(), (void*) 0x410000);
		}
		else
		{
			kout << "File not found\n";
		}*/
		
		Kernel::console << "Higher-half kernel might actually work!\n";	
		
		asm("sti");
		void* a = inb;
		while(true) asm("hlt");
	}
}
