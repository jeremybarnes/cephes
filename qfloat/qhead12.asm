
_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
_TEXT	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
DGROUP	GROUP	CONST,	_BSS,	_DATA
	ASSUME  CS: _TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP

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
	mov	ax, word ptr [di]+ao
	mul	word ptr [si]+bo
	add	word ptr [bx]+co,ax
	adc	word ptr [bx]+do,dx
	adc	word ptr [bx]+eo,cx
	ENDM

