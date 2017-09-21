#include "PagingStructure.h"

PagingStructure::PagingStructure()
{
	this->available = 0;
	this->rw = 0;
	this->usermode = 0;
	this->writeThrough = 0;
	this->cacheDisable = 0;
	this->accessed = 0;
	this->dirty = 0;
	this->zero = 0;
	this->ignored = 0;
	this->physicalAddress = 0;
}

PagingStructure::PagingStructure(void* physicalAddress)
{
	this->available = 1;
	this->rw = 0;
	this->usermode = 0;
	this->writeThrough = 0;
	this->cacheDisable = 0;
	this->accessed = 0;
	this->dirty = 0;
	this->zero = 0;
	this->ignored = 0;
	this->physicalAddress = reinterpret_cast<uint32_t>(physicalAddress) >> 12;
}

