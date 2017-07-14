#include "Heap.h"
#include "Console.h"

extern Console kout;

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
	
	kout << "[heap] Initializing heap at " << location << " with size " << size << "\n";
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
			kout << "[heap] ERROR: Out of memory!!\n";
			while(true);
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
		kout << "[heap] BUG: Attempt to allocate below bounds of heap\n";
		while(true);
		return 0;
	}
	else if(reinterpret_cast<uint32_t>(allocatedLocation) > reinterpret_cast<uint32_t>(heapStart) + this->heapSize)
	{
		kout << "[heap] BUG: Attempt to allocate above bounds of heap\n";
		while(true);
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
	//kout << "[heap] Freeing block w/ allocated space at " << location << " with size " << freedBlock->size << "\n";
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
	kout << "Heap info:" << "\n";
	kout << "  " << freeMem / 1024 << " KiB free\n";
	
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
	kout << "  " << largestBlock / 1024 << " KiB in largest free block\n";
	kout << "  " << listSize << " separate free blocks\n";
	kout << "  " << (freeMem / listSize) / 1024 << " KiB is mean block size (" << ((freeMem - largestBlock) / listSize) << "B excluding largest block)\n";
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
