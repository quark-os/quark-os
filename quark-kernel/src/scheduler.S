.intel_syntax noprefix

.align 16
p0_stack_bottom:
.skip 4096
p0_stack_top:

.align 16
p1_stack_bottom:
.skip 4096
p1_stack_top:

.global process_table
process_table:
num_p:	.word 0x0000, 0x0000
curr:	.word 0xFFFF, 0xFFFF
p0:		.word 0x0000, 0x0000
p1:		.word 0x0000, 0x0000

index:
	.word 0, 0
	
index2:
	.word 0x0000, 0x0000

.global task1
task1:
.att_syntax
	mov $p0_stack_top, %esp
.intel_syntax noprefix
	mov eax, [num_p]
	inc eax
	mov [num_p], eax
	mov eax, 0
	mov [curr], eax
	pushfd
	call 0x08:task2
	_loop1:
		mov eax, [index]
		movb [0xB8000 + eax * 2], '1'
		inc eax
		cmp eax, 2000
		jl _save_index
		mov eax, 0
	_save_index:
		mov [index], eax
		
		#pushfd
		#call 0x08:switch_context
		mov ecx, 0
		_wait0:
		cmp ecx, 0x200000
		jge _waitdone0
		inc ecx
		jmp _wait0
		_waitdone0:
		
		jmp _loop1

.global task2
task2:
	pusha
	mov [p0], esp
.att_syntax
	mov $p1_stack_top, %esp
.intel_syntax noprefix
	mov eax, [num_p]
	inc eax
	mov [num_p], eax
	mov eax, 1
	mov [curr], eax
	pushfd
	call 0x08:switch_context
	_loop2:
		mov eax, [index2]
		movb [0xB8001 + eax * 2], 0x1F
		inc eax
		cmp eax, 2000
		jl _save_index2
		mov eax, 0
	_save_index2:
		mov [index2], eax
		
		#pushfd
		#call 0x08:switch_context
		mov ecx, 0
		_wait1:
		cmp ecx, 0x200000
		jge _waitdone1
		inc ecx
		jmp _wait1
		_waitdone1:
		
		jmp _loop2

.global switch_context
switch_context:
	pusha
	mov eax, [curr]
	mov [p0 + eax * 4], esp
	mov ebx, [num_p]
	inc eax
	cmp eax, ebx
	jne _nextp
	mov eax, 0
	mov esp, [p0]
	mov [curr], eax
	jmp _done
_nextp:
	mov esp, [p0 + eax * 4]
	mov [curr], eax
_done:
	mov eax, 0x20
	out 0x20, eax
	popa
	iret
