#include "Interrupts.h"
#include "Console.h"
#include "Kernel.h"

__attribute__ ((interrupt))
void handleDivisionByZero(interrupt_frame* frame)
{
	kout << "Division by zero\n";
	Kernel::panic();
}

__attribute__ ((interrupt))
void handleInvalidOpcode(interrupt_frame* frame)
{
	kout << "Tried to execute invalid opcode\n";
	Kernel::panic();
}

__attribute__ ((interrupt))
void handleGeneralProtectionFault(interrupt_frame* frame, uint32_t errorCode)
{
	kout << "General protection fault with error code " << (void*) errorCode << "\n";
	Kernel::panic();
}

__attribute__ ((interrupt))
void handlePageFault(interrupt_frame* frame, uint32_t errorCode)
{
	uint32_t addr;
	asm("movl %%cr2, %0"
		: "=r" (addr));
	kout << "Page fault with error code " << (void*) errorCode << " at address " << (void*) addr << "\n";
	Kernel::panic();
}

__attribute__ ((interrupt))
void handleDoubleFault(interrupt_frame* frame)
{
	kout << "Double fault!!!";
	Kernel::panic();
}
