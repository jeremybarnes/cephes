; Floating point divide
; Large memory model
;
	TITLE   divm

include qheadl.asm
extrn	_mulm:far
extrn	mdnorm:far

QFLT_DATA	SEGMENT
;extrn	SC:word
B	DW	0
count	DW	0
quot	DW	NQ+2 DUP (?)
_square	DW	NQ+2 DUP (?)
_ans	DW	NQ+2 DUP (?)
_denom	DW	NQ+2 DUP (?)
_numer	DW	NQ+2 DUP (?)
QFLT_DATA	ENDS

QFLT_TEXT      SEGMENT
	ASSUME	CS: QFLT_TEXT


; Macro to accumulate twice the partial product
z2mul	MACRO	a,b
ao = 6 + 2*a
bo = 6 + 2*b
co = 8 + 2*a + 2*b
do = 6 + 2*a + 2*b
eo = 4 + 2*a + 2*b
	mov	ax, word ptr [di]+ao
	mul	word ptr [si]+bo
	xor	cx,cx
	sal	ax,1
	rcl	dx,1
	rcl	cx,1
	add	word ptr [bx]+co,ax
	adc	word ptr [bx]+do,dx
	adc	word ptr [bx]+eo,cx
	ENDM




extrn	_shdn1:far
extrn	_shup1:far
extrn	_normlz:far
;
; Floating point divide
; @stack into @stack+1
;
;
; The program makes use of 80286 16-bit unsigned
; MUL and DIV instructions.
; -Copyright 1988 by Stephen L. Moshier


; zdiv( source, dest )
; dest = numerator


	PUBLIC _divm
_divm	PROC	FAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	bx
	push	cx
	push	es
	push	ds

	push	ds
	pop	es

; copy in the denominator
	lea	di, _denom
	push	ds
	mov	cx,NQ+1
	lds	si, dword ptr [bp+6]	;source, denominator
	ASSUME DS: NOTHING
	rep	movsw
	pop	ds
	ASSUME DS: DGROUP

; copy the numerator
	lea	di, _numer
	push	ds
	mov	cx,NQ+1
	lds	si, dword ptr [bp+10]
	ASSUME DS: NOTHING
	rep	movsw
	pop	ds
	ASSUME DS: DGROUP

	lea	si,_denom
	mov	di,si
	add	si,8

; Test if the low order denominator is zero.
	lodsw
	or	ax,ax
	je	tiszer
	jmp	nzero

tiszer:
	mov	cx,OMG-2
$iszer:
	lodsw
	or	ax,ax
	jne	nnzero
	loop	$iszer

	jmp	short yzer


nnzero:
	jmp	nzero


yzer:
; Denominator only has one word of significant bits.
; Do a single precision divide.

	xor	bx,bx
	mov	dx, word ptr [di]+6	; denominator value
; get numerator and shift down 1
	lea	si, _numer+4	;numerator
	lea	di, quot+4
	mov	cx, OMG
	clc
$gnum:
	lodsw
	rcr	ax,1
	stosw
	loop	$gnum

	mov	ax,bx
	rcr	ax,1
	stosw

	xor	ax,ax
	stosw

	lea	si, quot+6		; numerator
	lea	di,_ans+6
	mov	cx,dx			;denominator

	lodsw
	mov	dx,ax
	lodsw
	div	cx
	stosw

REPT OMG-1
	lodsw
	div	cx
	stosw
ENDM

	lea	di, _numer
	sub	word ptr [di]+2,1
; This is the exact quotient since the low order denominator is zero.
	jmp	divfin


nzero:


	mov	si,di	;get denominator pointer

; clear temp area
	lea	di,quot
	mov	cx,NQ+2
	xor	ax,ax
	rep	stosw

	mov	cx, word ptr [si]+6	; denominator value

; divide numerator = 1 by most significant word of denominator
; compute quotient 1/B to 2 word precision
	lea	di,quot+6	; significand of quotient
	mov	dx,04000H	; 1.0

REPT 2
	xor	ax,ax
	div	cx
	stosw
ENDM

; Calculate double precision quotient using Taylor series

; clear out temporary storage
	xor	ax,ax
	lea	di,_square
	mov	cx,2*NQ+4
	rep	stosw

; square the quotient
	lea	di,quot
	mov	si,di
	lea	bx,_square
	z2mul	0,1
	zmul	1,1
	zmul	0,0

; multiply squared quotient by low order denominator
	lea	si,_denom
	lea	di,_square
	lea	bx,_ans
;		di si
	zmul	2,1
	zmul	1,1
	zmul	0,1
; shift up 2
	sal	word ptr [bx]+10,1
	rcl	word ptr [bx]+8,1
	rcl	word ptr [bx]+6,1
	sal	word ptr [bx]+10,1
	rcl	word ptr [bx]+8,1
	rcl	word ptr [bx]+6,1

; subtract from quotient
	lea	di,quot
	lea	si,_ans
	mov	ax, word ptr [si]+8
	sub	word ptr [di]+8,ax
	mov	ax, word ptr [si]+6
	sbb	word ptr [di]+6,ax

; do first Newton-Raphson iteration to four word precision
prec = 4
;	mov	count,3
;newtlup:

; Loop is done by in-line code with increasing
; arithmetic precision each iteration.

; Start of Newton-Raphson iterations.
rept NQDIV

; limit precision to maximum available
if prec gt OMG-1
prec = OMG-1
endif

; clear out accumulators
	lea	di,_square
	mov	cx,2*NQ+4
	xor	ax,ax
	rep	stosw

; Form the square of the approximate quotient

	lea	si,quot
	mov	di,si
	lea	bx,_square


hh = prec

rept prec
g = 0
h = hh
rept (hh+1)/2
if g LE OMG-1
if h LE OMG-1
	z2mul	g,h
endif
endif
g = g+1
h = h-1
endm

if g EQ h
	xor	cx,cx
	zmul	g,g
endif

hh = hh-1

endm
	xor	cx,cx
	zmul	0,0


; multiply the square of the quotient by the denominator

	lea	si, _denom	;source, denominator
	lea	di,_square
	lea	bx,_ans

hh = prec

rept prec
g = 0
h = hh
rept (hh+1)/2
if g LE OMG-1
if h LE OMG-2
	zmul	g,h
endif
endif
if h LE OMG-1
if g LE OMG-2
	zmul	h,g
endif
endif
g = g+1
h = h-1
endm

if g EQ h
	zmul	g,g
endif

hh = hh-1

endm
	zmul	0,0


; shift the product up 1 bit

	lea	bx,_ans

h = 2*prec+8
	sal	word ptr [bx]+h,1

rept prec+2
h = h-2
	rcl	word ptr [bx]+h,1
endm

; subtract quot - ans

i = 2*prec+8
	std
	lea	si,_ans+i
	lea	di,quot

	lodsw
	sub	word ptr [di]+i,ax

rept prec+2
i=i-2
	lodsw
	sbb	word ptr [di]+i,ax
endm
	cld

; shift result up 1 bit

	lea	bx,quot

h = 2*prec+8
	sal	word ptr [bx]+h,1

rept prec+2
h = h-2
	rcl	word ptr [bx]+h,1
endm

; increase the arithmetic precision for next loop
prec = prec+prec

; end of Newton-Raphson iteration loop
endm

;	sub	count,1
;	je	divdon
;	jmp	newtlup

divdon:
	cld

; multiply 1/x by the numerator

; temp area for the product
	lea	bx, _ans
; clear the temp area
	xor	ax,ax
	mov	di,bx
	mov	cx,NQ+2
	rep	stosw

	lea	si,quot
;	mov	si,WORD PTR [bp+4]	;source
	lea	di,_numer		;dest

	xor	cx,cx	; for adc 0

hh = OMG-1

rept OMG-1

g = 0
h = hh
rept (hh+1)/2
if g LE OMG-2
if h LE OMG-1
	zmul	g,h
endif
endif
if h LE OMG-2
if g LE OMG-1
	zmul	h,g
endif
endif
g = g+1
h = h-1
endm

if g EQ h
	zmul	g,g
endif

hh = hh-1

endm

	zmul	0,0



divfin:


; normalize
	lea	di,_numer
	lea	bx, _ans
	push ds
	pop dx
	call	far ptr mdnorm

; move out the answer
	les	di, dword ptr [bp+10]
	lea	si, _ans+4
	mov	ax, word ptr _numer
	stosw
	mov	ax, word ptr _numer+2
	stosw
	mov	cx,OMG
	rep	movsw

	pop	ds
	pop	es
	pop	cx
	pop	bx
	pop	di
	pop	si
	pop	bp
	ret	
_divm	ENDP


QFLT_TEXT	ENDS
END
