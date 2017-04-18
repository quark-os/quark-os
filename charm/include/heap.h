#ifndef HEAP_H
#define HEAP_H

#define BLOCK_FREE 0x01234567
#define BLOCK_RESERVED 0xFEDCBA98

#include "memoryblock.h"
#include <stdint.h>

class Heap
{
public:

	Heap();
	
	Heap(void* location, uint32_t size);
	
	void* allocate(uint32_t size);
	
	void free(void* block);
	
	uint32_t getFreeSpace();
			
private:

	void* location;
	
	void* availableList;
	
	uint32_t size;
	
};

#endif
