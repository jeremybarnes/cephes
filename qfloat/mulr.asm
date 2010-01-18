;	Static Name Aliases
;
	TITLE   mulm

include qhead.asm

_DATA	SEGMENT
temp	DW	NQ+2 DUP (?)
_DATA	ENDS

_TEXT      SEGMENT

; Multiply mantissa of x by mantissa of y, result to y
; subm( x, y )
	PUBLIC	_mulm
_mulm	PROC NEAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ax
	push	bx
	push	cx
	push	dx
	push	es

	push	ds
	pop	es

; temp area for the product
	lea	bx,temp
; clear the temp area
	xor	ax,ax
	mov	di,bx
	mov	cx,NQ+2
	rep	stosw

	mov	si,WORD PTR [bp+4]	;source
	mov	di,WORD PTR [bp+6]	;dest

	xor	cx,cx	; for adc 0

hh = OMG-1

rept OMG-1

g = 0
h = hh
rept (hh+1)/2
if g LE OMG-2
if h LE OMG-2
	zmul	g,h
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

mulnorm:

	call	mdnorm

	mov	si,bx
	add	si,4
	add	di,4
	mov	cx,OMG
	rep	movsw


	pop	es
	pop	dx
	pop	cx
	pop	bx
	pop	ax
	pop	di
	pop	si
	pop	bp
	ret	
_mulm	ENDP



; Source has at most 16 significant bits

	PUBLIC	_mulin
_mulin	PROC NEAR
	push	bp
	mov	bp,sp
	push	si
	push	di
	push	ax
	push	bx
	push	cx
	push	dx
	push	es

	push	ds
	pop	es

; temp area
	lea	bx,temp
; clear the temp area
	xor	ax,ax
	mov	di,bx
	mov	cx,NQ+2
	rep	stosw

	mov	si,WORD PTR [bp+4]	;source
	mov	di,WORD PTR [bp+6]	;dest

	xor	cx,cx	; for adc 0


h = OMG-2

rept OMG-1
	zmul	h,0
h = h-1
endm

	jmp	mulnorm

_mulin	ENDP



; normalize and round off
; enter with source = bx, dest = di

	PUBLIC mdnorm
mdnorm	PROC NEAR

	mov	cx,3
$normlp:
	test	word ptr [bx]+6,08000H
	jnz	$round
; Prevent exponent underflow.
; Rounding may be incorrect when this happens.
	test	word ptr [di]+2,0FFFFH
	jz	$nlup
	sub	word ptr [di]+2,1
; shift up 1 bit and decrement exponent
g = NQ+NQ
	clc
rept OMG
	rcl	word ptr [bx]+g,1
g = g-2
endm
$nlup:
	loop	$normlp

$round:


; round off
g = NQ+NQ
	test	word ptr [bx]+g,08000H
	jnz	$doround
	jmp	$done

$doround:
	xor	cx,cx
g = NQ+NQ-2
	add	word ptr [bx]+g,1
rept OMG-1
g = g-2
	adc	word ptr [bx]+g,cx
endm

; check for overflow on rounding up
	test	word ptr [bx]+4,0FFFFH
	jz	$done

; shift down 1 bit and increment exponent
g = 4
	clc
rept OMG
	rcr	word ptr [bx]+g,1
g = g+2
endm
	add	word ptr [di]+2,1


$done:
	ret
mdnorm	ENDP

_TEXT	ENDS
END
