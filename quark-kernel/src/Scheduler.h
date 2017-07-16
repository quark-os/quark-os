#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include <stdint.h>
#include "Process.h"
#include "List.h"

class Scheduler
{
public:

	Scheduler();
	
	size_t createProcess(void* entry);
	
	void* nextProcess(void* prevStack);
	
	void killProcess(size_t pid);

private:

	List<Process> processTable;
	
	Iterator currentProcess;
	
	size_t pidCounter;

};

extern Scheduler scheduler;

#endif
