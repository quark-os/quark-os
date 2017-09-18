#include "PageAllocator.h"
#include "Console.h"
#include "math.h"
#include "Heap.h"

#define PAGE_SIZE 4096

PageAllocator::PageAllocator()
{
	
}

PageAllocator::PageAllocator(void* base, size_t numPages)
{
	this->base = base;
	this->numPages = numPages;
	for(int i = 0; i < maxBlockSize; i++)
	{
		blocks[i] = heap.allocate((numPages / pow(2, i)) / 8);
		for(int j = 0; j * 8 < numPages / pow(2, i); j++)
		{
			blocks[i][j] = i == maxBlockSize - 1 ? 0xFF : 0;
		}
	}
}

void* PageAllocator::allocate(size_t blockSize)
{
	if(blockSize < maxBlockSize)
	{
		for(int i = 0; i < numPages / pow(2, blockSize); i++)
		{
			if(getBit(blockSize, i))
			{
				setBit(blockSize, i, false);
				return calculatePageAddress(blockSize, i);
			}
		}
		
		int splitBlock = split(blockSize + 1);
		if(splitBlock >= 0)
		{
			setBit(blockSize, splitBlock, false);
			return calculatePageAddress(blockSize, splitBlock);
		}
		return (void*) 0;
	}
	else
	{
		return (void*) 0;
	}
}

void PageAllocator::free(size_t blockSize, void* location)
{
	if(reinterpret_cast<size_t>(location) % pow(2, 12) != 0)
	{
		kout << "[pageallocator] WARN: free() called with location not aligned to page. Silently failing.\n";
	}
	
	for(int i = 0; i < pow(2, blockSize); i++)
	{
		size_t blockIndex = calculateBlockIndex(0, (void*) (reinterpret_cast<size_t>(location) + (4096 * i)));
		if(getBit(0, blockIndex))
		{
			kout << "[pageallocator] WARN: Attempt to free block that was already free.\n";
			kout << "    This is either a bug in the page allocator or the software using it\n";
		}
		setBit(0, blockIndex, true);
		merge(0, blockIndex);
	}
}

void PageAllocator::merge(size_t blockSize, int index)
{
	if(blockSize < maxBlockSize - 1)
	{
		int buddy = (((index + 1) % 2) * 2) - 1 + index;
		if(getBit(blockSize, index) && getBit(blockSize, buddy))
		{
			setBit(blockSize, index, false);
			setBit(blockSize, buddy, false);
			if(getBit(blockSize + 1, index / 2))
			{
				kout << "[pageallocator] BUG: When merging buddies, higher block was already available\n";
			}
			else
			{
				setBit(blockSize + 1, index / 2, true);
				if(blockSize + 1 < maxBlockSize - 1)
				{
					merge(blockSize + 1, index / 2);
				}
			}
		}
	}
	else
	{
		kout << "[pageallocator] WARN: Merge called on maximum block size\n";
	}
}

size_t PageAllocator::split(size_t blockSize)
{
	for(int i = 0; i < numPages / pow(2, blockSize); i++)
	{
		if(getBit(blockSize, i))
		{
			split(blockSize, i);
			return i * 2;
		}
	}
	
	if(blockSize + 1 < maxBlockSize)
	{
		return split(blockSize, split(blockSize + 1));
	}
	else
	{
		return -1;
	}
}

size_t PageAllocator::split(size_t blockSize, size_t index)
{
	setBit(blockSize, index, false);
	setBit(blockSize - 1, 2 * index, true);
	setBit(blockSize - 1, 2 * index + 1, true);
	return index * 2;
}

bool PageAllocator::getBit(size_t blockSize, size_t index)
{
	if(blockSize < maxBlockSize)
	{
		if(index < numPages / pow(2, blockSize))
		{
			int bit = (blocks[blockSize][index / 8] >> (index % 8)) & 1;
			if(bit != 0 && bit != 1)
			{
				kout << "[pageallocator] BUG: Read bit with nonbinary value\n";  
			}
			return bit == 1 ? true : false;
		}
		else
		{
			kout << "[pageallocator] BUG: Index in bitmap out of bounds\n";
			return false;
		}
	}
	else
	{
		kout << "[pageallocator] BUG: Tried to read bit from invalid block size.\n";  
		return false;
	}
}

void PageAllocator::setBit(size_t blockSize, size_t index, bool value)
{
	if(value == getBit(blockSize, index))
	{
		return;
	}
	
	if(blockSize < maxBlockSize)
	{
		if(index < numPages / pow(2, blockSize))
		{
			//blocks[blockSize][index / 8] += value ? -pow(2, blockSize) : pow(2, blockSize);
			if(value) 
				blocks[blockSize][index / 8] = blocks[blockSize][index / 8] | pow(2, index % 8);
			else
				blocks[blockSize][index / 8] = blocks[blockSize][index / 8] & (255 - pow(2, index % 8));
		}
		else
		{
			kout << "[pageallocator] BUG: Index in bitmap out of bounds\n";
		}
	}
	else
	{
		kout << "[pageallocator] BUG: Tried to write bit from invalid block size.\n";  
	}
}

void* PageAllocator::calculatePageAddress(size_t blockSize, size_t offset)
{
	return reinterpret_cast<void*>(reinterpret_cast<int>(base) + (offset * (PAGE_SIZE * pow(2, blockSize))));
}

size_t PageAllocator::calculateBlockIndex(size_t blockSize, void* location)
{
	return (reinterpret_cast<size_t>(location) - reinterpret_cast<size_t>(base)) / (pow(2, blockSize) * 4096);
}
