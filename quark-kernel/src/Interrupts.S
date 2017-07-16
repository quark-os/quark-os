.intel_syntax noprefix

.global _preempt
_preempt:
	pushad
	mov eax, esp
	mov esp, 0x180000
	push eax
	call preempt
	mov esp, eax
	mov al, 0x20
	out 0xA0, al
	out 0x20, al
	popad
	iret
