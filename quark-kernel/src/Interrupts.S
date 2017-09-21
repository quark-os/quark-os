.intel_syntax noprefix

.global gp_fault
gp_fault:
	pusha
	call handleGeneralProtectionFault
	popa
	iret

.global page_fault
page_fault:
	pusha
	call handlePageFault
	popa
	iret

.global syscall_test
syscall_test:
	pusha
	mov eax, 0x10
	mov ch, 'z'
	int 0x80
	popa
	ret

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

#; Syscalls:
#;	0x10	Print char	BH=char
#;	0x20	Preempt		
.global _syscall
_syscall:
	push ebx
	mov ebx, esp		#; Save the stack pointer of the previous task
	mov esp, 0x180000	#; Switch to the kernel's stack
	push ebx			#; Save task's stck pointer on kernel's stack
	
	cmp eax, 0x10
	je _printchar
	
	cmp eax, 0x20
	je _allocateregion
	
	cmp eax, 0x21
	je _freeregion
	
	cmp eax, 0x30
	je _fork
	
	jmp _syscallret
	
	_printchar:
		mov [0xB8008], ch
		jmp _syscallret
	
	_allocateregion:
		push edx
		push ecx
		call allocateRegion
		jmp _syscallret
	
	_freeregion:
		push edx
		push ecx
		call allocateRegion
		jmp _syscallret
	
	_fork:
		push ecx
		call fork
		jmp _syscallret
		
	#; Other syscalls...
	
	_syscallret:
	pop ebx
	mov esp, edx		#; Restore the caller's stack
	pop ebx				#; Restore ebx to its value before syscall was entered
	iret				#; Return control to caller (eax will have return value if present)
