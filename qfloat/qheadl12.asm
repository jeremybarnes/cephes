	.8087
INCLUDELIB	LLIBCE
QFLT_TEXT	SEGMENT  WORD PUBLIC 'CODE'
QFLT_TEXT	ENDS
QFLT_DATA	SEGMENT WORD PUBLIC 'DATA'
QFLT_DATA	ENDS
CONST	SEGMENT WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT WORD PUBLIC 'BSS'
_BSS	ENDS

DGROUP	GROUP	CONST, _BSS, QFLT_DATA
	ASSUME DS:DGROUP, SS:DGROUP

; number of 16 bit words in the entire number format
NQ = 12		; 24, 12
; number of iterations for a divide: 3 -> 77 decimal accuracy
NQDIV = 3	;  4,  3

; number of 16 bit words in mantissa area:
OMG = NQ-2
; byte offset to last word of mantissa area = 2*OMG+2:
OFFS = OMG+OMG+2


zmul	MACRO	a,b
ao = 6 + 2*a
bo = 6 + 2*b
co = 8 + 2*a + 2*b
do = 6 + 2*a + 2*b
eo = 4 + 2*a + 2*b
	mov	ax, word ptr es:[di]+ao
	mul	word ptr [si]+bo
	add	word ptr [bx]+co,ax
	adc	word ptr [bx]+do,dx
	adc	word ptr [bx]+eo,cx
	ENDM

