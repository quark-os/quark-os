print_bx:
	PUSHA
	MOV CX, 4   ;four places
	hexloop:
		ROL BX, 4   ;leftmost will
		MOV AX, BX   ; become
		AND AX, 0x0f   ; rightmost index into hexstr
		ADD AL, '0'
					;CMP BL, '9'
					;JLE noshift
					;ADD BL, 7
					;noshift:
					; The code above maps digits A-F properly. Until it can
					; fit in the finished binary, developers will have to
					; interpret the characters between '9' and 'A' themselves:
						; A = ':'
						; B = ';'
						; C = '<'
						; D = '='
						; E = '>'
						; F = '?'
		MOV AH, 0x0E
		INT 0x10
		LOOP hexloop
	POPA
	RET
