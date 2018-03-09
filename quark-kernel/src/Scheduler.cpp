#include "Scheduler.h"
#include "Kernel.h"

// See note above Scheduler::createProcess()
#define PROCESS_STACK_SIZE 65536

Scheduler::Scheduler()
{
	//processTable = List<Process>();
	currentProcess = processTable.end;
	pidCounter = 1;
}

size_t Scheduler::createProcess(AddressSpace addressSpace, void* entry)
{
	void* stack = reinterpret_cast<void*>(0x4100000); // MUST be changed after kernel is mapped to high memory
	Process p(stack, entry, pidCounter, addressSpace);
	p.initialize();
	pidCounter++;
	processTable.insertAtEnd(p);
	return pidCounter - 1;
}

/**
 * Adds a new process to the process table with a given entry point.
 * The scheduler will allocate space for the program's stack.
 * 
 * IMPORTANT:
 * 
 * For testing purposes, the kernel allocates the process' stack in its
 * a heap shared with the kernel itself. There is no currently no
 * page allocator, so software is strictly limited to a small stack.
 * 
 * Also note that there is no guard to ensure each process terminates
 * itself correctly. This must be corrected soon. Preferably, each
 * process shares a common entry point that initializes the process,
 * calls the real entry point of the program, and terminates the process
 * throughthe kernel.
 * 
 */
size_t Scheduler::createProcess(void* entry)
{
	void* stack = reinterpret_cast<void*>(reinterpret_cast<uint32_t>(Kernel::kernelHeap.allocate(PROCESS_STACK_SIZE)) + PROCESS_STACK_SIZE);
	Process p(stack, entry, pidCounter);
	p.initialize();
	pidCounter++;
	processTable.insertAtEnd(p);
	return pidCounter - 1;
}

size_t Scheduler::createProcess(void* entry, void* stack)
{
	Process p(stack, entry, pidCounter);
	p.initialize();
	pidCounter++;
	processTable.insertAtEnd(p);
	return pidCounter - 1;
}

void* Scheduler::nextProcess(void* prevStack)
{
	if(currentProcess != processTable.end)
	{
		processTable.get(currentProcess).stack = prevStack;
		currentProcess = processTable.next(currentProcess);
	}
	
	if(currentProcess == processTable.end)
	{
		currentProcess = processTable.getFirst();
	}
	return processTable.get(currentProcess).stack;

}

Process& Scheduler::getCurrentProcess()
{
	return processTable.get(currentProcess);
}
