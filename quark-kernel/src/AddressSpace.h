#ifndef __ADDRESS_SPACE_H
#define __ADDRESS_SPACE_H

#include "PageTable.h"

class AddressSpace
{
public:
	
	AddressSpace();
	
	void map(void* linearAddress, void* physicalAddress);
	
	void unmap(void* linearAddress);
	
	void invalidate(void* linearAddress);
	
	void load();
	
	void* getPageDirectoryAddress();
	
private:

	PageTable pageDirectory;
	
};

#endif
