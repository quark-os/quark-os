.intel_syntax noprefix

.global syscall
syscall:
	pusha
	cmp eax, 1
	je create_process
	popa
	iret

create_process:
	jmp next_process

next_process:
	jmp end_syscall

end_syscall
	popa
	iret
