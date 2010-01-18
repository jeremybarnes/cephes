;	Static Name Aliases
;
	TITLE   subm

include	qhead.asm

;_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
;_TEXT	ENDS
;CONST	SEGMENT  WORD PUBLIC 'CONST'
;CONST	ENDS
;_BSS	SEGMENT  WORD PUBLIC 'BSS'
;_BSS	ENDS
;_DATA	SEGMENT  WORD PUBLIC 'DATA'
;_DATA	ENDS
;DGROUP	GROUP	CONST,	_BSS,	_DATA
;	ASSUME  CS: _TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP

PUBLIC  _subm, _addm, _shdn1, _shup1, _shdn8, _shup8, _shdn16, _shup16	

;_DATA	SEGMENT
;EXTRN	__chkstk:NEAR
;_DATA	ENDS

_TEXT      SEGMENT

; number of 16 bit words in mantissa area:
;OMG = 10
; offset to last word of mantissa area = 2*OMG+2:
;OFFS = 22

; Subtract mantissa of x from mantissa of y, result to y
; subm( x, y )
	PUBLIC	_subm
_subm	PROC NEAR
	push	bp
	mov	bp,sp
	push	si

	mov	si,WORD PTR [bp+4]
	mov	bx,WORD PTR [bp+6]
	clc

i=NQ+NQ
REPT OMG+1
	mov	dx,i[bx]
	sbb	dx,i[si]
	mov	i[bx],dx
i=i-2
ENDM
	pop	si
	pop	bp
	ret	
_subm	ENDP



; add mantissa of x to mantissa of y, result to y
; addm( x, y );
	PUBLIC	_addm
_addm	PROC NEAR
	push	bp
	mov	bp,sp
	push	si

	mov	si,[bp+4]
	mov	bx,[bp+6]
	clc

i=NQ+NQ
REPT OMG+1
	mov	dx,i[bx]
	adc	dx,i[si]
	mov	i[bx],dx
i=i-2
ENDM
	pop	si
	pop	bp
	ret	
_addm	ENDP


; shift mantissa of x down 1 bit
; shdn1(x);
	PUBLIC	_shdn1
_shdn1	PROC NEAR
	push	bp
	mov	bp,sp

	mov	bx,[bp+4]
	clc

i=4
REPT OMG+1
	rcr	WORD PTR i[bx],1
i=i+2
ENDM
	pop	bp
	ret	
_shdn1	ENDP



; shift mantissa of x up 1 bit
; shup1(x);
	PUBLIC	_shup1
_shup1	PROC NEAR
	push	bp
	mov	bp,sp

	mov	bx,[bp+4]
	clc

i=NQ+NQ
REPT OMG+1
	rcl	WORD PTR i[bx],1
i=i-2
ENDM
	pop	bp
	ret	
_shup1	ENDP



; shift mantissa of x down 8 bits
; shdn8(x);
	PUBLIC	_shdn8
_shdn8	PROC NEAR
	push	bp
	mov	bp,sp

	mov	bx,[bp+4]
	sub	dx,dx
i=4
REPT OMG+1
	mov	ax,WORD PTR i[bx]
	xchg	al,ah
	xchg	ah,dl
	mov	WORD PTR i[bx],ax
i=i+2
ENDM
	pop	bp
	ret	
_shdn8	ENDP



; shift mantissa of x up 8 bits
; shup8(x);
	PUBLIC	_shup8
_shup8	PROC NEAR
	push	bp
	mov	bp,sp

	mov	bx,[bp+4]
	sub	dx,dx
i=NQ+NQ
REPT OMG+1
	mov	ax,WORD PTR i[bx]
	xchg	al,ah
	xchg	al,dl
	mov	WORD PTR i[bx],ax
i=i-2
ENDM
	pop	bp
	ret	
_shup8	ENDP



; shift mantissa of x down 16 bits
; shdn16(x);
	PUBLIC	_shdn16
_shdn16	PROC NEAR
	push	bp
	mov	bp,sp

	mov	bx,[bp+4]
	sub	dx,dx
i=4
REPT OMG+1
	mov	ax,WORD PTR i[bx]
	xchg	ax,dx
	mov	WORD PTR i[bx],ax
i=i+2
ENDM
	pop	bp
	ret	
_shdn16	ENDP


; shift mantissa of x up 16 bits
; shup16(x);
	PUBLIC	_shup16
_shup16	PROC NEAR
	push	bp
	mov	bp,sp

	mov	bx,[bp+4]
	sub	dx,dx
i=NQ+NQ
REPT OMG+1
	mov	ax,WORD PTR i[bx]
	xchg	ax,dx
	mov	WORD PTR i[bx],ax
i=i-2
ENDM
	pop	bp
	ret	
_shup16	ENDP




; qmov(a,b)
; copy q type number from a to b

	PUBLIC _qmov
_qmov	PROC NEAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	cx
	push	es

	push	ds
	pop	es
	mov	si, word ptr [bp]+4
	mov	di, word ptr [bp]+6
	mov	cx, NQ
	rep	movsw

	pop	es
	pop	cx
	pop	di
	pop	si
	pop	bp
	ret
_qmov	ENDP




; qmovz(a,b)
; copy q type number from a to b
; and clear the bottom guard word of b

	PUBLIC _qmovz
_qmovz	PROC NEAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	cx
	push	es

	push	ds
	pop	es
	mov	si, word ptr [bp]+4
	mov	di, word ptr [bp]+6
	mov	cx, NQ
	rep	movsw

	mov	word ptr [di],0

	pop	es
	pop	cx
	pop	di
	pop	si
	pop	bp
	ret
_qmovz	ENDP

; qclear(a)
; set a to zero

	PUBLIC _qclear
_qclear	PROC NEAR
	push	bp
	mov	bp,sp
	push	di
	push	cx
	push	es

	push	ds
	pop	es
	xor	ax,ax
	mov	di, word ptr [bp]+4
	mov	cx, NQ
	rep	stosw

	pop	es
	pop	cx
	pop	di
	pop	bp
	ret
_qclear	ENDP


_TEXT	ENDS
END
