#include "system.h"
#include "keyboard.h"
#include "keyboardtranslator.h"
#include "interruptcontroller.h"

Display System::display;

Heap System::heap;

EventSystem System::eventSystem;

PS2Controller System::ps2;

void System::initialize()
{	
	heap = Heap(0x200000, 0x100000);
	eventSystem = EventSystem();
	display = Display();		
	ps2 = PS2Controller();
	KeyboardTranslator::initialize();
	Keyboard::initialize();
	display.initialize();
	InterruptController::init(0x100000); // This is a bad place to allocate the IDT	
}

void System::run()
{
	asm("sti");
	while(true)
	{
		asm("hlt");
		eventSystem.handleEvents();
	}
}

void System::panic()
{
	display.printError("PANIC: Halting system");
	asm("cli");
	while(true)
		asm("hlt");
}
