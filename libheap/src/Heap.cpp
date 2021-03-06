#include "Heap.h"

Heap heap;

Heap::Heap()
{
	
}

Heap::Heap(void* location, uint32_t size)
{
	this->location.size = 0;
	this->location.link = (BlockHeader*) location;
	this->location.link->size = size - sizeof(BlockHeader);
	this->location.link->link = 0;
	this->heapStart = location;
	this->heapSize = size;
	this->numFreeBlocks = 1;	
}

void* Heap::allocate(uint32_t size)
{
	if(size % 4 != 0) size = size + (4 - (size % 4));
	else if(size < 16) size = 16;
	else if(size == 0) return (void*) 0;
	
	BlockHeader* q = &location;
	BlockHeader* p;
	while(true)
	{
		p = q->link;
		if(p == 0)
		{
			return 0;
		}
		else if(p->size + sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE >= size)
		{
			break;
		}
		else
		{
			q = p;
		}
	}
	
	void* allocatedLocation = 0;
	if(p->size - 16 > size + REDUCED_BLOCK_HEADER_SIZE)
	{
		BlockHeader* newBlock = reinterpret_cast<BlockHeader*>(reinterpret_cast<uint32_t>(p) + size + REDUCED_BLOCK_HEADER_SIZE);
		newBlock->link = p->link;
		newBlock->size = p->size - (size + REDUCED_BLOCK_HEADER_SIZE);
		q->link = newBlock;
		p->size = size;
		allocatedLocation = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(p) + REDUCED_BLOCK_HEADER_SIZE);
	}
	else
	{
		q->link = p->link;
		p->size += sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE;
		allocatedLocation = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(p) + REDUCED_BLOCK_HEADER_SIZE);
		numFreeBlocks--;
	}
	
	if(reinterpret_cast<uint32_t>(allocatedLocation) < reinterpret_cast<uint32_t>(heapStart))
	{
		return 0;
	}
	else if(reinterpret_cast<uint32_t>(allocatedLocation) > reinterpret_cast<uint32_t>(heapStart) + this->heapSize)
	{
		return 0;
	}
	else
	{
		return allocatedLocation;
	}
}

void* Heap::free(void* location)
{
	BlockHeader* freedBlock = (BlockHeader*) (reinterpret_cast<uint32_t>(location) - static_cast<uint32_t>(REDUCED_BLOCK_HEADER_SIZE));
	BlockHeader* prev = &(this->location);
	BlockHeader* next;
	while(true)
	{
		next = prev->link;
		if(((uint32_t) next == 0) || ((uint32_t) next > (uint32_t) location))
		{
			break;
		}
		prev = next;
	}

	bool mergedRight = false;
	if(reinterpret_cast<uint32_t>(location) + freedBlock->size == reinterpret_cast<uint32_t>(next))
	{
		freedBlock->size += next->size + sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE;
		freedBlock->link = next->link;
		mergedRight = true;
	}
	else
	{
		freedBlock->size -= sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE;
		freedBlock->link = next;
	}
	
	if(reinterpret_cast<uint32_t>(freedBlock) == (reinterpret_cast<uint32_t>(prev) + prev->size + sizeof(BlockHeader)))
	{
		prev->size += freedBlock->size + sizeof(BlockHeader);
		prev->link = freedBlock->link;
		if(mergedRight)
			numFreeBlocks--;
	}
	else
	{
		if(!mergedRight)
			numFreeBlocks++;
		prev->link = freedBlock;
	}
	return reinterpret_cast<void*>(0);
}

size_t Heap::freeMemory()
{
	size_t freeMemory = 0;
	BlockHeader* prev = &location;
	BlockHeader* next;
	while(true)
	{
		next = prev->link;
		if((uint32_t) next == 0)
		{
			break;
		}
		else
		{
			freeMemory += next->size + sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE;
		}
		prev = next;
	}
	return freeMemory;
}

void Heap::dumpState()
{
	size_t freeMem = freeMemory();
	
	size_t listSize = 0;
	size_t largestBlock = 0;
	BlockHeader* prev = &location;
	BlockHeader* next;
	while(true)
	{
		next = prev->link;
		if((uint32_t) next == 0)
		{
			break;
		}
		else
		{
			//kout << "    " << next->size << " byte block at " << (void*) next << "\n";
			if((next->size + sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE) > largestBlock)
			{
				largestBlock = next->size + sizeof(BlockHeader) - REDUCED_BLOCK_HEADER_SIZE;
			}
			listSize++;
		}
		prev = next;
	}
}

bool Heap::checkListOrder()
{
	BlockHeader* prev = &location;
	BlockHeader* next;
	while(true)
	{
		next = prev->link;
		if((uint32_t) next == 0)
		{
			break;
		}
		else
		{
			if(reinterpret_cast<uint32_t>(next) <= reinterpret_cast<uint32_t>(prev))
			{
				return false;
			}
		}
		prev = next;
	}
	return true;
}

void *operator new(size_t size)
{
    return heap.allocate(size);
}
 
void *operator new[](size_t size)
{
    return heap.allocate(size);
}
 
void operator delete(void *p)
{
    heap.free(p);
}
 
void operator delete[](void *p)
{
    heap.free(p);
}
