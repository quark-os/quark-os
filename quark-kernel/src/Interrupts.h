#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

extern "C" 
{
	void _preempt();
	void _syscall();
	void syscall_test();
	void page_fault();
	void gp_fault();
}

#endif
