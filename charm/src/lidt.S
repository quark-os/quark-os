.intel_syntax noprefix

.global loadIDT
loadIDT: # loadIDT(size, location)
	mov ax, [esp + 4]
	mov [esp - 6], ax
	mov eax, [esp + 8]
	mov [esp - 4], eax
	lidt [esp - 6]
	ret
