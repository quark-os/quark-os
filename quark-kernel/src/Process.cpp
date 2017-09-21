#include "Process.h"

Process::Process()
{
	
}

Process::Process(void* stack, void* entry, size_t pid)
{
	this->stack = stack;
	this->entry = entry;
	this->pid = pid;
	this->addressSpace = AddressSpace();
}

Process::Process(void* stack, void* entry, size_t pid, AddressSpace addressSpace)
{
	this->stack = stack;
	this->entry = entry;
	this->pid = pid;
	this->addressSpace = addressSpace;
}

void Process::initialize()
{
	uint32_t* stackData = (uint32_t*) stack;
	stackData -= 3;
	stackData[0] = entry;
	stackData[1] = 0x08;
	stackData[2] = 0x202;
	stackData -= 8;
	stackData[0] = 0;
	stackData[1] = 0;
	stackData[2] = 0;
	stackData[3] = stack;
	stackData[4] = 0;
	stackData[5] = 0;
	stackData[6] = 0;
	stackData[7] = 0;
	stack = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(stack) - (11 * 4));
}
