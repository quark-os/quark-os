#ifndef __HEAP_H
#define __HEAP_H

#include <stddef.h>
#include <stdint.h>

typedef struct BlockHeader
{
	uint32_t size;
	BlockHeader* link;
};

#define REDUCED_BLOCK_HEADER_SIZE sizeof(BlockHeader) - sizeof(BlockHeader::link)

class Heap
{
public:

	Heap();

	Heap(void* location, uint32_t size);

	void* allocate(uint32_t size);

	void* free(void* location);
	
	size_t freeMemory();
	
	void dumpState();
	
	bool checkListOrder();

private:

	BlockHeader location;
	
	void* heapStart;
	
	uint32_t heapSize;
	
	uint32_t numFreeBlocks;

};

extern Heap heap;

#endif
