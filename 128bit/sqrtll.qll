/* This invokes Sun's _Q_ arithmetic square root. */
/* For Solaris, delete the leading underscore character.  */
gcc2_compiled.:
___gnu_compiled_c:
.text
	.align 4
	.global sqrtl
	.proc	07
sqrtl:
	!#PROLOGUE# 0
	save %sp,-160,%sp
	!#PROLOGUE# 1
	ldd [%i0],%o0;ldd [%i0+8],%o2
	std %o0,[%fp-48];std %o2,[%fp-40]
	add %fp,-48,%o0
	ld [%fp+64],%i0
	add %fp,-32,%o2
	st %o2,[%sp+64]
	call _Q_sqrt,0
	nop
	unimp 16
	ldd [%fp-32],%o0;ldd [%fp-24],%o2
	std %o0,[%i0];std %o2,[%i0+8]
	jmp %i7+12
	restore
