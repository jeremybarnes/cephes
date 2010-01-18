# 68000 version of qfltb.c
	file "qf68k.s"
	data 1
	text 1
# Set NQ = size of number in words.
# Also adjust rndbit[] array to have a 1 bit in the rounding
# position (see at end of the file).
	set	nq,24

	global shdn1
shdn1:	link	%a6,&0
#	movm.l	%d7/%a5,-(%a7)
	movm.l	&0x104,-(%a7)
	mov.l	8(%a6),%a5
	add.l	&4,%a5
	mov.l	&nq-2,%d7
	mov.w	&0,%cc
sdnl:	roxr.w	&1,(%a5)+
	dbra	%d7,sdnl
#	movm.l	(%a7)+,%a5/%d7
	movm.l	(%a7)+,&0x2080
	unlk	%a6
	rts

	global shup1
shup1:	link	%a6,&0
#	movm.l	%d7/%a5,-(%a7)
	movm.l	&0x104,-(%a7)
	mov.l	8(%a6),%a5
	add.w	&nq+nq+2,%a5
	mov.l	&nq-2,%d7	# do nq-1 words
	mov.w	&0,%cc
sup1l:	roxl.w	&1,-(%a5)
	dbra	%d7,sup1l
#	movm.l	(%a7)+,%a5/%d7
	movm.l	(%a7)+,&0x2080
	unlk	%a6
	rts

	global shdn8
shdn8:	link	%a6,&0
#	movm.l	%d7/%a5/%a4,-(%a7)
	movm.l	&0x10c,-(%a7)
	mov.l	8(%a6),%a5
	add.w	&nq+nq+1,%a5
	mov.l	%a5,%a4
	add.l	&1,%a4
	mov.l	&nq+nq-4,%d7	# (nq-1) * 2 - 1 - 1
sd8l:	mov.b	-(%a5),-(%a4)
	dbra	%d7,sd8l
	mov.b	&0,-(%a4)
#	movm.l	(%a7)+,%a4/%a5/%d7
	movm.l	(%a7)+,&0x3080
	unlk	%a6
	rts

	global shup8
shup8:	link	%a6,&0
#	movm.l	%d7/%a5/%a4,-(%a7)
	movm.l	&0x10c,-(%a7)
	mov.l	8(%a6),%a5
	add.l	&4,%a5
	mov.l	%a5,%a4
	add.l	&1,%a4
	mov.l	&nq+nq-4,%d7	# (nq-1) * 2 - 1 - 1
su8l:	mov.b	(%a4)+,(%a5)+
	dbra	%d7,su8l
	mov.b	&0,(%a5)
#	movm.l	(%a7)+,%a4/%a5/%d7
	movm.l	(%a7)+,&0x3080
	unlk	%a6
	rts

	global shdn16
shdn16:	link	%a6,&0
#	movm.l	%d7/%a5/%a4,-(%a7)
	movm.l	&0x10c,-(%a7)
	mov.l	8(%a6),%a5
	add.w	&nq+nq,%a5
	mov.l	%a5,%a4
	add.l	&2,%a4
	mov.l	&nq-3,%d7	# do nq-2 words
sd6l:	mov.w	-(%a5),-(%a4)
	dbra	%d7,sd6l
	mov.w	&0,-(%a4)
#	movm.l	(%a7)+,%a4/%a5/%d7
	movm.l	(%a7)+,&0x3080
	unlk	%a6
	rts

	global shup16
shup16:	link	%a6,&0
#	movm.l	%d7/%a5/%a4,-(%a7)
	movm.l	&0x10c,-(%a7)
	mov.l	8(%a6),%a5
	add.l	&4,%a5
	mov.l	%a5,%a4
	add.l	&2,%a4
	mov.l	&nq-3,%d7	# do nq-2 words
su6l:	mov.w	(%a4)+,(%a5)+
	dbra	%d7,su6l
	mov.w	&0,(%a5)
#	movm.l	(%a7)+,%a4/%a5/%d7
	movm.l	(%a7)+,&0x3080
	unlk	%a6
	rts

	global addm
addm:	link	%a6,&0
#	movm.l	%d7/%a5/%a4,-(%a7)
	movm.l	&0x10c,-(%a7)
	mov.l	8(%a6),%a4
	add.w	&nq+nq+2,%a4 # 2 * (2 + (nq-1))
	mov.l	12(%a6),%a5
	add.w	&nq+nq+2,%a5
	mov.l	&nq-2,%d7
	mov.w	&0,%cc
add1l:	addx.w	-(%a4),-(%a5)
	dbra	%d7,add1l
#	movm.l	(%a7)+,%a4/%a5/%d7
	movm.l	(%a7)+,&0x3080
	unlk	%a6
	rts

	global subm
subm:	link	%a6,&0
#	movm.l	%d7/%a5/%a4,-(%a7)
	movm.l	&0x10c,-(%a7)
	mov.l	8(%a6),%a4
	add.w	&nq+nq+2,%a4 # 2 * (2 + (nq-1))
	mov.l	12(%a6),%a5
	add.w	&nq+nq+2,%a5
	mov.l	&nq-2,%d7
	mov.w	&0,%cc
sub1l:	subx.w	-(%a4),-(%a5)
	dbra	%d7,sub1l
#	movm.l	(%a7)+,%a4/%a5/%d7
	movm.l	(%a7)+,&0x3080
	unlk	%a6
	rts

# variable precision multiply of significands.
# c must not be in the same location as either a or b.
#
# static int mulv( a, b, c, prec )
# unsigned short a[], b[], c[]
# int prec

	global mulv
mulv:	link	%a6,&0
#	movm.l	%d7/%d6/%d5/%d2/%d1/%a5/%a4/%a3/%a2/%a1,-(%a7)
	movm.l	&0x677c,-(%a7)
	mov.l	8(%a6),%a4
	mov.l	12(%a6),%a5
	mov.l	16(%a6),%a3
	mov.l	20(%a6),%d6	# precision, in words

# clear the output array of prec+2 words
	mov.l	%d6,%d7
	add.l	&1,%d7
	mov.l	%a3,%a0
	add.l	&4,%a0
	clr.l	%d1
mv2l:
	mov.w	%d1,(%a0)+
	dbra	%d7,mv2l


# for( k=prec+2; k>=3; k-- )
	mov.l	%d6,%d7	# k = prec+2
	asl.l	&1,%d7
	add.l	%d7,%a3
	add.l	&6,%a3	# r = &c[prec+3];
	add.l	&4,%d7
# {
mv0l:	mov.l %a5,%a1	# q = &b[3];
	add.l	&6,%a1
	mov.l	%a4,%a0
	add.l	%d7,%a0	# p = &a[k];

# for( i=k; i>=3; i-- )
#	{
	mov.l	%d7,%d5

#	if( (*p == 0) || (*q == 0) )
#		{
#		--p;
#		++q;
#		continue;
#		}
mv1l:	mov.w	(%a0),%d0	# *p--
	sub.l	&2,%a0
	mulu.w	(%a1)+,%d0 # *q++
	sub.l	&2,%a3	# accumulate in *r
	add.l	%d0,(%a3)
	mov.w	-(%a3),%d2
	addx.w	%d1,%d2
	mov.w	%d2,(%a3)+
	add.l	&2,%a3
	sub.l	&2,%d5
	cmp.w	%d5,&4
	bne	mv1l

	sub.l	&2,%a3
	sub.l	&2,%d7
	cmp.w	%d7,&4
	bne	mv0l

#	movm.l	(%a7)+,%a1/%a2/%a3/%a4/%a5/%d7/%d6/%d5/%d2/%d1
	movm.l	(%a7)+,&0x3ee6
	unlk	%a6
	rts

# variable precision square.
# b must be in a different location from a.
#
# static squarev( a, b, prec )
# unsigned short a[], b[]#
# int prec;
# {

	global squarev
squarev:	link	%a6,&0
#	movm.l	%d7/%d6/%d5/%d2/%d1/%a5/%a4/%a3/%a2/%a1,-(%a7)
	movm.l	&0x677c,-(%a7)
	mov.l	8(%a6),%a4	# a
	mov.l	12(%a6),%a3	# b
	mov.l	16(%a6),%d6	# precision, in words

# clear the output array of prec+2 words
	mov.l	%d6,%d7
	add.l	&1,%d7
	mov.l	%a3,%a0
	add.l	&4,%a0
	clr.l	%d1
sq5l:
	mov.w	%d1,(%a0)+
	dbra	%d7,sq5l

# r = &b[prec+3];
# for( k=prec+2; k>=3; k-- )
# {
	mov.l	%d6,%d7	# k = prec+2
	asl.l	&1,%d7
	add.l	%d7,%a3
	add.l	&6,%a3	# r = &b[prec+3];
	add.l	&4,%d7

sq0l:	mov.l	%a4,%a1	# q = &a[3];
	add.l	&6,%a1
	mov.l	%a4,%a0
	add.l	%d7,%a0	# p = &a[k];

#while( p >= q )	
#	{
sq1l:	cmp.l	%a1,%a0
	bhi.b	sq3l
#	if( (*p == 0) || (*q == 0) )
#		{
#		--p;
#		++q;
#		continue;
#		}
	mov.w	(%a0),%d0
	mulu.w	(%a1),%d0
	cmp.l	%a1,%a0
	beq.b	sq2l

	clr.l	%d1
	lsl.l	&1,%d0		# 2ab term of square
	roxl.l	&1,%d1
	add.w	%d1,-4(%a3)

sq2l:	sub.l	&2,%a0
	add.l	&2,%a1
	sub.l	&2,%a3	# accumulate in *r
	clr.l	%d1
	add.l	%d0,(%a3)
	mov.w	-(%a3),%d2
	addx.w	%d1,%d2
	mov.w	%d2,(%a3)+
	add.l	&2,%a3
	bra	sq1l

#	}
# --r;
# }
sq3l:	sub.l	&2,%a3
	sub.l	&2,%d7
	cmp.w	%d7,&4
	bne	sq0l
# shup1(b);
	mov.l	12(%a6),-(%a7)
	bsr	shup1
	add.l	&4,%a7
#	movm.l	(%a7)+,%a1/%a2/%a3/%a4/%a5/%d7/%d6/%d5/%d2/%d1
	movm.l	(%a7)+,&0x3ee6
	unlk	%a6
	rts


# mulm( b, ac3 )
# unsigned short b[], ac3[];
# {
	global mulm
mulm:	link	%a6,&0
#	movm.l	%d7/%d6/%d5/%d2/%d1/%a5/%a4/%a3/%a2/%a1,-(%a7)
	movm.l	&0x677c,-(%a7)
	mov.l	8(%a6),%a4	# b
	mov.l	12(%a6),%a2	# ac3
	sub.l	&nq+nq+4,%a7
	mov.l	%a7,%a5		# act
# qclear( act );
	mov.l	&nq+1,%d0
	mov.l	%a5,%a1
	clr.l	%d1
mm0l:
	mov.w	%d1,(%a1)+
	dbra	%d0,mm0l

#act[0] = ac3[0];
#act[1] = ac3[1];
	mov.l	%a2,%a0	# ac3
	mov.l	%a5,%a1	# act
	mov.w	(%a0)+,(%a1)+
	mov.w	(%a0)+,(%a1)+
#r = &act[nq+1];
	mov.l	%a5,%a3
	add.l	&nq+nq+2,%a3
#for( k=nq; k>=3; k-- )
#{
	mov.l	&nq,%d7	# k
mm1l:	cmp.b	%d7,&2
	beq.b	mm7l
#if( k == nq )
#	{
	cmp.b	%d7,&nq
	bne.b	mm3al
#	m = nq-1;
#	o = 4;
	mov.l	&nq-1,%d6 # m
	mov.l	&4,%d5	# o
	bra.b	mm3bl
#	}
#else
#	{
#	m = k;
#	o = 3;
mm3al:	mov.l	%d7,%d6	# m
	mov.l	&3,%d5
#	}
# p = &b[m];
mm3bl:	asl.l	&1,%d6	# m
	mov.l	%a4,%a0
	add.l	%d6,%a0	# p
# q = &ac3[o];
	asl.l	&1,%d5	# o
	mov.l	%a2,%a1
	add.l	%d5,%a1	# q
# for( i=m; i>=o; i-- )
#	{
mm2l:	cmp.l %d6,%d5
	blt.b	mm6l
#	if( (*p == 0) || (*q == 0) )
#		{
#		--p;
#		++q;
#		continue;
#		}
	mov.w	(%a0),%d0	# p
	beq.b	mm4l
	tst.w	(%a1)
	beq.b	mm4l
	mulu.w	(%a1),%d0
	sub.l	&2,%a3	# accumulate in *r
	add.l	%d0,(%a3)
	mov.w	-(%a3),%d2
	addx.w	%d1,%d2
	mov.w	%d2,(%a3)+
	add.l	&2,%a3
mm4l:	sub.l	&2,%a0
	add.l	&2,%a1
	sub.l	&2,%d6
	bra.b	mm2l
#	}
#--r;
mm6l:	sub.l	&2,%a3
	sub.l	&1,%d7
	bra.b	mm1l
#}
#mdnorm( act );
mm7l:	mov.l	%a5,-(%a7)
	bsr	mdnorm
	add.l	&4,%a7
#qmov( act, ac3 );
	mov.l	%a5,%a0	# act
	mov.l	%a2,%a1	# ac3
	mov.l	&nq-1,%d0
mm8l:	mov.w	(%a0)+,(%a1)+
	dbra	%d0,mm8l
#}
	add.l	&nq+nq+4,%a7
#	movm.l	(%a7)+,%a1/%a2/%a3/%a4/%a5/%d7/%d6/%d5/%d2/%d1
	movm.l	(%a7)+,&0x3ee6
	unlk	%a6
	rts


#mulin( b, ac3 )
#unsigned short b[], ac3[];
#{
	global mulin
mulin:	link	%a6,&0
#	movm.l	%d7/%d6/%d5/%d2/%d1/%a5/%a4/%a3/%a2/%a1,-(%a7)
	movm.l	&0x677c,-(%a7)
	mov.l	8(%a6),%a4	# b
	mov.l	12(%a6),%a2	# ac3
	sub.l	&nq+nq+4,%a7
	mov.l	%a7,%a5		# act
# qclear( act );
	mov.l	&nq+1,%d0
	mov.l	%a5,%a1
	clr.l	%d1
mn0l:
	mov.w	%d1,(%a1)+
	dbra	%d0,mn0l

#act[0] = ac3[0];
#act[1] = ac3[1];
	mov.l	%a2,%a0	# ac3
	mov.l	%a5,%a1	# act
	mov.w	(%a0)+,(%a1)+
	mov.w	(%a0)+,(%a1)+
#r = &act[nq];
	mov.l	%a5,%a3
	add.l	&nq+nq,%a3
#y = b[3];
	clr.l	%d6
	mov.w	6(%a4),%d6
#p = &ac3[nq-1];
	mov.l	%a2,%a1
	add.l	&nq+nq-2,%a1
#for( i=nq-1; i>=3; i-- )
#	{
	mov.l	&nq-1,%d7
mn1l:	cmp.b	%d7,&2
	beq.b	mn7l
#	if( *p == 0 )
#		{
#		--p;
#		--r;
#		continue;
#		}
	mov.l	%d6,%d0
	mulu.w	(%a1),%d0
	sub.l	&2,%a1
	sub.l	&2,%a3
	add.l	%d0,(%a3)
	mov.w	-(%a3),%d2
	addx.w	%d1,%d2
	mov.w	%d2,(%a3)+
#	add.l	&2,%a3
#	}
	sub.l	&1,%d7
	bra.b	mn1l
#mdnorm( act );
mn7l:	mov.l	%a5,-(%a7)
	bsr	mdnorm
	add.l	&4,%a7
#qmov( act, ac3 );
	mov.l	%a5,%a0	# act
	mov.l	%a2,%a1	# ac3
	mov.l	&nq-1,%d0
mn8l:	mov.w	(%a0)+,(%a1)+
	dbra	%d0,mn8l
#}
	add.l	&nq+nq+4,%a7
#	movm.l	(%a7)+,%a1/%a2/%a3/%a4/%a5/%d7/%d6/%d5/%d2/%d1
	movm.l	(%a7)+,&0x3ee6
	unlk	%a6
	rts

#divsh( a, prod )
	global divsh
divsh:	link	%a6,&0
#	movm.l	%d7/%d6/%d5/%d2/%d1/%a5/%a4/%a3/%a2/%a1,-(%a7)
	movm.l	&0x677c,-(%a7)
	mov.l	8(%a6),%a4	# a
	mov.l	12(%a6),%a5	# prod

#prod[nq] = 0;
	clr.w	nq+nq(%a5)
#prod[nq+1] = 0;
	clr.w	nq+nq+2(%a5)
#shdn1( prod );
	mov.l	%a5,-(%a7)
	bsr	shdn1
	add.l	&4,%a7
#shdn1( prod );
	mov.l	%a5,-(%a7)
	bsr	shdn1
	add.l	&4,%a7
#d = a[3];
	clr.l	%d6
	mov.w	6(%a4),%d6
#u = ((unsigned long )prod[3] << 16) | prod[4];
	mov.l	%a5,%a0
	add.l	&6,%a0	# &prod[3]
	mov.l	%a0,%a1
	mov.l	&-65536,%d5	# 0xffff0000
	mov.w	(%a0)+,%d0
	swap	%d0
	and.l	%d5,%d0
	or.w	(%a0)+,%d0

#for( i=3; i<nq; i++ )
#	{
	mov.l	&3,%d7
ds1l:	cmp.b	%d7,&nq
	beq.b	ds2l
#	qu = u / d;
	divu.w	%d6,%d0
#	prod[i] = qu;
	mov.w	%d0,(%a1)+
#	u = ((u - (unsigned long )d * qu) << 16) | prod[i+2];
	and.l	%d5,%d0
	or.w	(%a0)+,%d0
#	}
	add.l	&1,%d7
	bra.b	ds1l
ds2l:
# prod[nq] = u / d;
	divu.w	%d6,%d0
	mov.w	%d0,(%a1)+
#	movm.l	(%a7)+,%a1/%a2/%a3/%a4/%a5/%d7/%d6/%d5/%d2/%d1
	movm.l	(%a7)+,&0x3ee6
	unlk	%a6
	rts


#	move a to b
#
#	short a[nq], b[nq];
#	qmov( a, b );

	global qmov
qmov:	link	%a6,&0
	mov.l	%a1,-(%a7)
	mov.l	8(%a6),%a0
	mov.l	12(%a6),%a1
	mov.l	&nq/2-1,%d0	# 2*(11+1) = nq words
qm1l:	mov.l	(%a0)+,(%a1)+
	dbra	%d0,qm1l
	mov.l	(%a7)+,%a1
	unlk	%a6
	rts



#qmovz( a, b )
# same as qmov but clears 1 low guard word at 25th array position
	global qmovz
qmovz:	link	%a6,&0
	mov.l	%a1,-(%a7)
	mov.l	8(%a6),%a0
	mov.l	12(%a6),%a1
	mov.l	&nq/2-1,%d0	# 2*(11+1) = nq words
qmz1l:	mov.l	(%a0)+,(%a1)+
	dbra	%d0,qmz1l
	clr.w	(%a1)+
	mov.l	(%a7)+,%a1
	unlk	%a6
	rts

	global qclear
qclear:	link	%a6,&0
	mov.l	%a1,-(%a7)
	mov.l	%d1,-(%a7)
	mov.l	8(%a6),%a0
	mov.l	&0,%d1
	mov.l	&nq/2-1,%d0	# 2*(11+1) = nq words
qm1l:	mov.l	d1,(%a0)+
	dbra	%d0,qm1l
	mov.l	(%a7)+,%d1
	mov.l	(%a7)+,%a1
	unlk	%a6
	rts



	global	qclear
	set	sqr,-nq-nq-4	# -52
	set	prod,-4*nq-8		# -104
	set	quot,-6*nq-12	# -156
# divm( a, b )
# unsigned short a[], b[];
	global divm
divm:	link	%a6,&-188
#	movm.l	%d7/%d6/%d5/%a5/%a4/%a3/%a2,-(%a7)
	movm.l	&0x703c,-(%a7)
	mov.l	8(%a6),%a3
	mov.l	12(%a6),%a5
	lea	prod(%a6),%a4
	lea	8(%a3),%a2
	mov.l	&nq-4,%d5
dvm1al:	mov.l	%a2,%a0
	add.l	&2,%a2
	mov.l	 &0,%d0
	mov.w	(%a0),%d0
	beq.b	dvm26l
	bra.b	dvm48l
dvm26l:	sub.l	&1,%d5
	tst.l	%d5
	bne.b	dvm1al
	mov.l	%a4,-(%a7)	# prod
	mov.l	%a5,-(%a7)	# b
	bsr	qmov
	add.w	&8,%a7
	mov.l	%a4,-(%a7)	# prod
	mov.l	%a3,-(%a7)	# a
	bsr	divsh
	add.w	&8,%a7
	bra	dvm100l
dvm48l:	pea	quot(%a6)
	jsr	qclear
	add.w	&4,%a7
	clr.w	quot+nq+nq(%a6)
	mov.l	%a4,-(%a7)
	jsr	qclear
	add.w	&4,%a7
	pea	sqr(%a6)
	jsr	qclear
	add.w	&4,%a7
	mov.l	&0,%d0
	mov.w	6(%a3),%d0	# a[3]
	mov.l	&1073741824,%d1
	divu.w	%d0,%d1
	mov.w	%d1,quot+6(%a6)	# quot[3]
	mov.l	&2,%d7	# prec
	mov.l	&1,%d6	# k
dvm86l:	mov.l	&nq-2,%d0
	cmp.l	%d0,%d7
	ble.b	dvme2l
	mov.l	%d6,%d0
	lsl.l	&1,%d0
	mov.l	%d0,%d6
	mov.l	&nq-2,%d0
	cmp.l	%d0,%d6
	bge.b	dvm9cl
	mov.l	&nq-2,%d7	# prec = nq - 2
	bra.b	dvm9el
dvm9cl:	mov.l	%d6,%d7
dvm9el:	mov.l	%d7,-(%a7)
	pea	sqr(%a6)
	pea	quot(%a6)
	bsr	squarev
	add.w	&12,%a7
	mov.l	%d7,-(%a7)
	mov.l	%a4,-(%a7)
	pea	sqr(%a6)
	mov.l	%a3,-(%a7)
	bsr	mulv
	add.w	&16,%a7
	pea	quot(%a6)
	mov.l	%a4,-(%a7)
	bsr	subm
	add.w	&8,%a7
	pea	quot(%a6)
	bsr	shup1
	add.w	&4,%a7
	bra.b	dvm86l
dvme2l:	mov.l	&nq-2,-(%a7)
	mov.l	%a4,-(%a7)
	mov.l	%a5,-(%a7)
	pea	quot(%a6)
	bsr	mulv
	add.w	&16,%a7
	mov.w	(%a5),(%a4)
	mov.w	2(%a5),2(%a4)
dvm100l:	mov.l	%a4,-(%a7)
	bsr	mdnorm
	mov.l	%a5,(%a7)
	mov.l	%a4,-(%a7)
	bsr	qmov
	add.w	&8,%a7
#	movm.l    (%a7)+,%a2/%a3/%a4/%a5/%d5/%d6/%d7
	movm.l	(%a7)+,&0x3ce0
	unlk	  %a6
	rts


	global mdnorm
mdnorm:	link	%a6,&0
#	movm.l	%d7/%a5/%a4/%a3/%a2,-(%a7)
	movm.l	&0x13c,-(%a7)
	mov.l	8(%a6),%a4
#	lea	rndset,%a3
#	mov.w	(%a3),%d0
#	ext.l	%d0
#	bne.b	mdn38l
#	pea	rndbit
#	mov.l	(%a7),%a2
#	jsr	qclear
#	add.w	&4,%a7
#	mov.l	&1,%d0
#	mov.w	%d0,nq+nq-2(%a2)
#	clr.w	nq+nq(%a2)
#	mov.w	%d0,(%a3)	;rndset
#mdn38l
	lea	2(%a4),%a5
	clr.l	%d7
	bra.b	mdn62l
mdn40l:	mov.l	&0,%d0
	mov.w	4(%a4),%d0
	bne.b	mdn4al
	bra.b	mdn68l
mdn4al:	mov.l	%a4,-(%a7)
	bsr	shdn1
	add.w	&4,%a7
	add.w	&1,(%a5)
#	mov.w	(%a5),%d0
#	ext.l	%d0
#	bge.b	mdn60l
#	mov.w	&32767,(%a5)
mdn60l:	add.l	&1,%d7
mdn62l:	mov.l	&3,%d0
	cmp.l	%d0,%d7
	bgt.b	mdn40l
mdn68l:	clr.l	%d7
	bra.b	mdn92l
mdn6cl:	mov.l	&0,%d0
	mov.w	6(%a4),%d0
	and.l	&32768,%d0
	beq.b	mdn7cl
	bra.b	mdn98l
mdn7cl:	mov.l	%a4,-(%a7)
	bsr	shup1
	add.w	&4,%a7
	mov.w	(%a5),%d0
	ext.l	%d0
	beq.b	mdn90l
	sub.w	&1,(%a5)
mdn90l:	add.l	&1,%d7
mdn92l:	mov.l	&3,%d0
	cmp.l	%d0,%d7
	bgt.b	mdn6cl
mdn98l:	mov.l	&0,%d0
	mov.w	nq+nq(%a4),%d0
	and.l	&32768,%d0
	beq.b	mdnb6l
	mov.l	%a4,-(%a7)
	pea	rndbit
	bsr	addm
	add.w	&8,%a7
mdnb6l:	tst.w	4(%a4)
	beq.b	mdnd2l
	mov.l	%a4,-(%a7)
	bsr	shdn1
	add.w	&4,%a7
	add.w	&1,(%a5)
#	mov.w	(%a5),%d0
#	ext.l	%d0
#	bge.b	mdnd2l
#	mov.w	&32767,(%a5)
mdnd2l:	clr.w	nq+nq(%a4)
#	movm.l	(%a7)+,%a2/%a3/%a4/%a5/%d7
	movm.l	(%a7)+,&0x3c80
	unlk	%a6
	rts


	data 1
# for 24 word format
rndbit:	short 0,0,0,0,0,0,0,0
	short 0,0,0,0,0,0,0,0
	short 0,0,0,0,0,0,0,1,0

# for 12 word format
#rndbit:	short 0,0,0,0,0,0,0,0
#	short 0,0,0,1,0

#rndset:	short  0
