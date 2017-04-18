.intel_syntax noprefix

.global test_handler
test_handler:
	pusha
	movb [0xB8004], 0x24
	popa
	iret
	
.global pic_routine
pic_routine:
	pusha
	movb [0xB8008], 0x24
	call picEOI
	popa
	iret

.global keyboard_isr
keyboard_isr:
	pusha
	call handleKeyboardEvent
	call picEOI
	popa
	iret
	
.global double_fault
double_fault: # Never returns, just hangs the system
	cli
	movb [0xB8000], '#'
	hng:
		hlt
		jmp hng

.global unhandled_exception
unhandled_exception:
	sub esp, 4
	iret



