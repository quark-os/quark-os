#ifndef __PAGE_ALLOCATOR_H
#define __PAGE_ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

class PageAllocator
{
public:

	static const int minimumPages = 128;

	static const int maxBlockSize = 8;
	
	/**
	 * Leaves data members undefined. Using a PageAllocator constructed with this method is dangerous.
	 */
	PageAllocator();
	
	/**
	 * Constructs a new PageAllocator that will allocate pages from a region
	 * in physical memory starting from 'base' with a size of 'numPages'.
	 * 
	 * @param base A pointer to the beginning of the region from which to allocate
	 * @param numPages The length of the region, starting from 'base', in pages
	 */
	PageAllocator(void* base, size_t numPages);
	
	/**
	 * Allocates a contiguous block of 2^blockSize pages.
	 * 
	 * @param blockSize 2^blockSize pages will be allocated
	 * @returns A pointer to the beginning of the allocated block
	 * 
	 */
	void* allocate(size_t blockSize);
	
	/**
	 * Frees 2^blockSize pages starting at location
	 * 
	 * @param blockSize The size of the block to free, in 2^blockSize pages
	 * @param location The starting location of the block to free
	 */
	void free(size_t blockSize, void* location);

private:

	void* base;
	
	size_t numPages;

	unsigned char* blocks[maxBlockSize];
	
	void merge(size_t blockSize, int index);
	
	size_t split(size_t blockSize);
	
	size_t split(size_t blockSize, size_t index);
	
	bool getBit(size_t blockSize, size_t index);
	
	void setBit(size_t blockSize, size_t index, bool value);
	
	void* calculatePageAddress(size_t blockSize, size_t offset);
	
	size_t calculateBlockIndex(size_t blockSize, void* location);

};

extern PageAllocator pageAllocator;

#endif
