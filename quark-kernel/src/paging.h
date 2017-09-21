#ifndef __PAGING_H
#define __PAGING_H

extern "C"
{

/**
 * Loads the desired page directory into the control register
 */
void load_cr3(void* pageDirectory);

/**
 * Invalidates translation info in the CPU's cache regarding the page
 * the provided linear address maps to. Use when updating paging
 * information in memory, as the cache can potentially be out-of-date.
 */
void invalidate_page(void* linearAddress);

/**
 * Tells the processor to start using the provided paging structures to
 * map linear addresses
 */
void start_paging();

}

#endif
