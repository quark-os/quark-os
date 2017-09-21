#include "AddressSpace.h"
#include "PageAllocator.h"
#include "paging.h"
#include <stddef.h>

extern PageAllocator pageAllocator;

AddressSpace::AddressSpace()
{
	void* pageDirectoryAddress = pageAllocator.allocate(0);
	pageDirectory = PageTable(pageDirectoryAddress);
	for(int i = 0; i < PageTable::tableSize; i++)
	{
		pageDirectory[i] = PagingStructure();
	}
}

void AddressSpace::map(void* linearAddress, void* physicalAddress)
{
	uint32_t pageDirectoryIndex = (reinterpret_cast<uint32_t>(linearAddress) & 0xFFC00000) >> 22;
	uint32_t pageTableIndex = (reinterpret_cast<uint32_t>(linearAddress) & 0x003FF000) >> 12;
	if(pageDirectory[pageDirectoryIndex].available == 0)
	{
		void* pageTableAddress = pageAllocator.allocate(0);
		pageDirectory[pageDirectoryIndex].physicalAddress = reinterpret_cast<uint32_t>(pageTableAddress) >> 12;
		pageDirectory[pageDirectoryIndex].available = 1;
		PageTable pageTable(pageDirectory[pageDirectoryIndex]);
		for(int i = 0; i < PageTable::tableSize; i++)
		{
			pageTable[i] = PagingStructure();
		}
	}
	PageTable pageTable(pageDirectory[pageDirectoryIndex]);
	pageTable[pageTableIndex].physicalAddress = reinterpret_cast<uint32_t>(physicalAddress) >> 12;
	pageTable[pageTableIndex].available = 1;
}

void AddressSpace::unmap(void* linearAddress)
{
	uint32_t pageDirectoryIndex = (reinterpret_cast<uint32_t>(linearAddress) & 0xFFC00000) >> 22;
	uint32_t pageTableIndex = (reinterpret_cast<uint32_t>(linearAddress) & 0x003FF000) >> 12;
	if(pageDirectory[pageDirectoryIndex].available == 1)
	{
		PageTable pageTable = pageDirectory[pageDirectoryIndex];
		pageTable[pageTableIndex].available = 0;
		pageTable[pageTableIndex].physicalAddress = 0;
	}
}

void AddressSpace::invalidate(void* linearAddress)
{
	invalidate_page(linearAddress);
}

void AddressSpace::load()
{
	load_cr3(pageDirectory.getTableAddress());
}

void* AddressSpace::getPageDirectoryAddress()
{
	return pageDirectory.getTableAddress();
}
