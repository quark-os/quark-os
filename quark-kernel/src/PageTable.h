#ifndef __PAGE_TABLE_H
#define __PAGE_TABLE_H

#include "PagingStructure.h"

/**
 * A table of PagingStructures. The table must be pre-allocated; this 
 * class will not allocate memory. Do not confuse this with Intel's name
 * for type of structure used to make 4k pages; this handles each type
 * of table Intel defines.
 */
class PageTable
{
public:

	static const size_t tableSize = 1024;

	/**
	 * Initializes the 
	 */
	PageTable();
	
	PageTable(PagingStructure& parent);
	
	PageTable(PagingStructure* entries);
	
	PagingStructure& operator[](int index);
	
	void* getTableAddress();
	
private:

	PagingStructure* entries;
	
};

#endif
