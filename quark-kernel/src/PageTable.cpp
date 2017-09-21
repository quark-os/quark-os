#include "PageTable.h"
#include <stddef.h>
#include <stdint.h>

PageTable::PageTable()
{
	this->entries = (PagingStructure*) 0;
}

PageTable::PageTable(PagingStructure& parent)
{
	this->entries = (PagingStructure*) (reinterpret_cast<uint32_t>(parent.physicalAddress) << 12);
}

PageTable::PageTable(PagingStructure* entries)
{
	this->entries = entries;
}

PagingStructure& PageTable::operator[](int index)
{
	return entries[index];
}

void* PageTable::getTableAddress()
{
	return (void*) entries;
}
