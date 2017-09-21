#ifndef __PAGING_STRUCTURE_H
#define __PAGING_STRUCTURE_H

#include <stddef.h>
#include <stdint.h>

/**
 * Structure containing information used in translating linear addresses
 * to physical addresses. The pointer field points to a page-aligned 
 * physical address. Paging structures are hierarchical; higher level 
 * structures point to tables of lower level structures. The pointers 
 * from the lowest level paging structures become the upper 20 bits in a 
 * translated physical address (the bottom 12 coming from the bottom 12 
 * bits of the original linear address). In its lower 12 bits, this 
 * structure contains various fields describing permissions and settings 
 * for pages mapped under this structure. Pages can be restricted to
 * kernel-mode code, put in read-only mode, etc.
 * 
 */
class PagingStructure
{
public:
	/**
	 * Set if this structure actually maps to a physical address.
	 * If this structure is reached during address translation and this
	 * bit is not set, a page fault will occur.
	 */
	uint32_t available : 1;
	
	/**
	 * Set if code is allowed to write to this memory. Should generally 
	 * be cleared for pages containing code (especially shared libraries).
	 */
	uint32_t rw : 1;
	
	/**
	 * Set if access to addresses mapped by this structure are available
	 * to usermode programs. Should be clear for all pages used by the
	 * kernel.
	 */
	uint32_t usermode : 1;
	
	uint32_t writeThrough : 1;
	
	uint32_t cacheDisable : 1;
	
	uint32_t accessed : 1;
	
	uint32_t dirty : 1;
	
	/**
	 * Should always be cleared.
	 */
	uint32_t zero : 1;
	
	/**
	 * Not used by the processor. Just leave these bits cleared. 
	 */
	uint32_t ignored : 4;
	
	/**
	 * A pointer to a page-aligned address. If this is a page table 
	 * entry, it will become the upper 20 bits of a translated physical 
	 * address. Otherwise, it stores the physical address of the next 
	 * level of paging structures.
	 */
	uint32_t physicalAddress : 20;
	
	/**
	 * Initializes all data to zero
	 */
	PagingStructure();
	
	/**
	 * Sets the available bit and stores the upper 20 bits of the 
	 * provided physical address; initializes everything else to zero
	 */
	PagingStructure(void* physicalAddress);
	
};

#endif
