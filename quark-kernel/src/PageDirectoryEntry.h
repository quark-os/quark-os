#ifndef __PAGE_DIRECTORY_ENTRY_H
#define __PAGE_DIRECTORY_ENTRY_H

#include <stddef.h>
#include <stdint.h>

class PageDirectoryEntry
{
public:
	
	
	
	
private:

	uint32_t available : 1;
	uint32_t rw : 1;
	uint32_t usermode : 1;
	uint32_t writeThrough : 1;
	uint32_t cacheDisable : 1;
	
	
};

#endif
