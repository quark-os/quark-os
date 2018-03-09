#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include <stddef.h>
#include <stdint.h>

typedef struct interrupt_frame;

extern "C" 
{
	void handleGeneralProtectionFault(interrupt_frame* frame, uint32_t errorCode);
	void handlePageFault(interrupt_frame* frame, uint32_t errorCode);
	void handleDivisionByZero(interrupt_frame* frame);
	void handleDoubleFault(interrupt_frame* frame);
	void handleInvalidOpcode(interrupt_frame* frame);
	
	void _preempt();
	void _syscall();
	void syscall_test();
}

#endif
