.intel_syntax noprefix

.global setup_paging
setup_paging:
	pusha
	mov edi, 0x2FF000
	mov ecx, 0x300000
_write_pde:
	mov eax, ecx
	or eax, 0x03
	mov [edi], eax
	add ecx, 0x1000
	add edi, 4
	cmp edi, 0x300000
	jl _write_pde
	mov ecx, 0
_write_pte:
	mov eax, ecx
	or eax, 0x03
	mov [edi], eax
	add ecx, 0x1000
	add edi, 4
	cmp edi, 0x700000
	jl _write_pte
	mov eax, 0x2FF000
	mov cr3, eax
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax
	popa
	ret
