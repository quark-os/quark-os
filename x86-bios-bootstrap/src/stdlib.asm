;	DI	Location to start zeroing
;	CX	Length of block
blkzero:
	PUSHA
	XOR AL, AL
	REP STOSB
	POPA
	RET
