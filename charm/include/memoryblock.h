#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <stdint.h>

class MemoryBlock
{
public:

	MemoryBlock();
	
	MemoryBlock(void* location);
	
	bool exists();
	
	bool isFree();
	
	uint32_t getSize();
	
	uint32_t getType();
	
	void* getLocation();
	
	MemoryBlock getLeftBlockIfFree();
	
	MemoryBlock getRightBlockIfFree();
	
	MemoryBlock getNext();
	
	MemoryBlock getLast();
	
	void writeTags(uint32_t tag);
	
	void resize(uint32_t size);
	
	void linkNext(MemoryBlock next);
	
	void linkLast(MemoryBlock last);
			
private:

	void* location;
	
};

#endif
