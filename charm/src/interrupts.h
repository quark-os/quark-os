#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

extern "C"
{
	void test_handler();
	
	void pic_routine();
	
	void keyboard_isr();
	
	void double_fault();
	
	void unhandled_exception();
	
	void picEOI();
}

#endif
