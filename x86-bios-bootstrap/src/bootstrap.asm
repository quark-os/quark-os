org 0x7C00

%macro ALIVE 0
	MOV BX, 0x1234
	CALL NEAR print_bx
%endmacro

; Pointer macros; these point to the location to store some working data
%define da_packet	0x500
%define blkcnt		0x502
%define da_off		0x504
%define da_seg		0x506
%define da_lba		0x508
%define outstr16	0x510

%define data_bottom 0x520

%define status		0x521
%define tlba		0x530
%define fslba		0x538
%define charm_entry 0x540
%define gdt			0x550
%define gdtr		0x570

%define data_top	0x580

; Data macros; these store important constants
%define stack_top 0x7C00
%define gpt_header_location 0x0600
%define gpt_data_location 0x0800
%define gpt_sector_count 32
%define fs_location 0x8000
%define charm_dest_seg 0x4000
%define file_header_buffer 0x7E00
%define gdt_code_access 0x9A
%define gdt_data_access 0x92
%define gdt_size 23

; Program status codes to debugging purposes
%define status_gpt 0x10
%define status_gptcheck_magic 0x20
%define status_gptcheck_parts 0x21
%define status_loadpart 0x30
%define status_loadelf 0x40
%define status_parseelf 0x50

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Prepare runtime environment											;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
XOR AX, AX
MOV SP, stack_top
MOV SS, AX ; Prepare stack

MOV BYTE [da_packet], 0x10
MOV [da_packet + 1], AL ; Initialize data access packet
MOV [outstr16 + 4], AL ; Initialize hex string buffer

MOV CX, data_top - data_bottom
MOV DI, data_bottom
CALL blkzero ; Zero data block
MOV BYTE [tlba], 1 ; Initialize first LBA in GPT


OR WORD [gdt + 8], 0xFFFF
MOV BYTE [gdt + 8 + 5], gdt_code_access
MOV BYTE [gdt + 8 + 6], 0xCF ; Initialize GDT entry for code

OR WORD [gdt + 16], 0xFFFF
MOV BYTE [gdt + 16 + 5], gdt_data_access
MOV BYTE [gdt + 16 + 6], 0xCF ; Initialize GDT entry for code

MOV WORD [gdtr], gdt_size
MOV WORD [gdtr + 2], gdt

MOV BYTE [status], status_gpt
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read the GPT from the disk											;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV SI, tlba
MOV DI, gpt_header_location
MOV CX, gpt_sector_count
XOR BX, BX
CALL read_block

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Verify that a valid GPT had been loaded								;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV BYTE [status], status_gptcheck_magic
MOV SI, gpt_header_location
MOV DI, gpt_magic_num
MOV CX, 8
REPE CMPSB
JNE NEAR error

MOV BYTE [status], status_gptcheck_parts
MOV AX, [SI + 0x50 - 8]
OR AX, AX
JZ error

MOV BYTE [status], status_loadpart
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Load first bootable partition to be found								;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV SI, gpt_data_location
		
MOV DI, efs_magic_num
MOV CX, 16
MOV DX, [SI - 0x200 + 0x50]
_checkboot:
	REPE CMPSB
	JE SHORT _isboot
	DEC DX
	JZ error
	ADD SI, 128 - 16
	JMP SHORT _checkboot
	
_isboot:
ADD SI, 0x10
MOV DI, fslba
MOV CX, 2
REP MOVSD
MOV SI, fslba
MOV DI, fs_location
MOV CX, 1
XOR BX, BX
XOR AX, AX ; AX is considered the segment to read to by the disk input routine. Zero it.
CALL NEAR read_block

MOV BYTE [status], status_loadelf
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Load Charm ELF file													;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MOV BX, fslba
MOV SI, fs_location + 0x18
_load_next_fh: ; Iterate through file headers until one is found which is marked bootable
	MOV CX, 1
	MOV DI, file_header_buffer
	CALL NEAR read_block

	MOV SI, boot_marker 
	MOV DI, 0x7E1C
	MOV CX, 4
	REPE CMPSB
	JE SHORT _bootable
	MOV SI, file_header_buffer
JMP _load_next_fh	; BUG: If no file is marked bootable, the loop will continue forever

_bootable:
MOV SI, file_header_buffer + 0x20
XOR DI, DI
MOV AX, charm_dest_seg
ADD BYTE [BX], 1
MOV CX, [file_header_buffer + 0x10]
CALL read_block

MOV BYTE [status], status_parseelf
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; TODO Build program image from ELF										;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
XOR SI, SI
;MOV AX, charm_dest_seg (Logically this must happen, but was already done to read the file above)
MOV DS, AX
MOV DI, elf_magic_num
MOV CX, 4
REPE CMPSB
JNE error

ADD SI, 0x14
MOV DI, charm_entry
MOV CX, 4
REPE MOVSB

XOR SI, SI
	
MOV CX, [DS:SI + 0x2C]
XOR DX, DX
ADD SI, [DS:SI + 0x1C]
	
_sectionloop:
	;MOV AX, [DS:SI]
	;OR AX, AX
	;JZ _sectionloop
	
	;TEST CX, CX
	;JZ endsectionloop
	; BUG: Loader cannot handle blank sections in the middle of the table
		
	PUSH CX ; Save array counter
		
	; Zero memory block
	MOV CX, [DS:SI + 20]
	MOV DI, [DS:SI + 8]
	MOV AX, [DS:SI + 10]
	AND AX, 0x000F
	ROR AX, 4	; Extract segment from upper part of pointer
	MOV ES, AX
	
	CALL NEAR blkzero
		
	PUSH SI ; Save location in program header array
		
	; Copy data from file, use dest parameters from before
	MOV CX, [DS:SI + 16]
	MOV AX, [DS:SI + 4]
	XOR SI, SI ; Restore SI to start of ELF

	ADD SI, AX
	REP MOVSB
		
	POP SI ; Restore location in program header array
		
	ADD SI, 32 ; Move to next element in array
	POP CX ; Restore array counter
	LOOP _sectionloop
endsectionloop:

; ELF is parsed i guess
XOR AX, AX
MOV DS, AX
MOV ES, AX

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Load GDT																;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

LGDT [gdtr]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Switch to protected mode												;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CLI
MOV EAX, CR0 
OR AL, 1     ; set PE (Protection Enable) bit in CR0 (Control Register 0)
MOV CR0, EAX
JMP 0x08:load_cs

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Protected mode code; load GDT and enter next stage					;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

BITS 32

	load_cs:
	MOV AX, 0x10
	MOV DS, AX
	MOV ES, AX
	MOV FS, AX
	MOV GS, AX
	MOV SS, AX

;halt32:
;	HLT
;	JMP halt32

	MOV EAX, [charm_entry]
	JMP EAX

BITS 16

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This should never be reached, it just halts if an error happens		;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
halt:
	HLT
	JMP halt
error:
	;MOV BX, [status - 1]
	;CALL print_bx
	;MOV SI, error_msg
	;CALL print_string
	JMP halt

%include "src/print.asm"
%include "src/stdlib.asm"
%include "src/disk.asm"

elf_magic_num:
		DB	0x7F, "ELF"
gpt_magic_num:
		DB	"EFI PART"
efs_magic_num:
		DB	"HI THIS IS "
boot_marker:			; boot_marker is just a label in the middle of efs_magic_num,
		DB	"BOOT!"		; in order to save a few bytes. it really reads to 'BOOT'

times 510-($-$$) db 0
db 0x55
db 0xAA
