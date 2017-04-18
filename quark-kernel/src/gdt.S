.intel_syntax noprefix

.global lgdt
lgdt:	# void lgdt(void* gdt)
	mov ax, 23
	mov [esp - 6], ax
	mov eax, [esp + 4]
	mov [esp - 4], eax
	lgdt [esp - 6]
	
	jmp 0x08:reload_CS
reload_CS:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
