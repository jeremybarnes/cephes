; 68020 version of qfltb.c
; Uses 32*32=64 bit multiply and divide instructions.
; Copyright 1989 by Stephen L. Moshier

	IDENT qfltd
; Set NQ = size of number in words.
; Also adjust rndbit[] array to have a 1 bit in the rounding
; position (see at end of the file).
NQ	equ	24

	GLOBAL shdn1
shdn1	LINK	A6,#0
	MOVEM.L	D7/A5,-(A7)
	MOVE.L	8(A6),A5
	ADDQ.L	#4,A5
	MOVEQ	#NQ-2,D7
	MOVE.W	#0,CCR
sdnl	ROXR	(A5)+
	DBRA	D7,sdnl
	MOVEM.L	(A7)+,A5/D7
	UNLK	A6
	RTS

	GLOBAL shup1
shup1	LINK	A6,#0
	MOVEM.L	D7/A5,-(A7)
	MOVE.L	8(A6),A5
	ADDA.W	#NQ+NQ+2,A5
	MOVEQ	#NQ-2,D7	; do NQ-1 words
	MOVE	#0,CCR
sup1l	ROXL	-(A5)
	DBRA	D7,sup1l
	MOVEM.L	(A7)+,A5/D7
	UNLK	A6
	RTS

	GLOBAL shdn8
shdn8	LINK	A6,#0
	MOVEM.L	D7/A5/A4,-(A7)
	MOVE.L	8(A6),A5
	ADDA.W	#NQ+NQ+1,A5
	MOVEA.L	A5,A4
	ADDQ.L	#1,A4
	MOVEQ	#NQ+NQ-4,D7	; (NQ-1) * 2 - 1 - 1
sd8l	MOVE.B	-(A5),-(A4)
	DBRA	D7,sd8l
	MOVE.B	#0,-(A4)
	MOVEM.L	(A7)+,A4/A5/D7
	UNLK	A6
	RTS

	GLOBAL shup8
shup8	LINK	A6,#0
	MOVEM.L	D7/A5/A4,-(A7)
	MOVE.L	8(A6),A5
	ADDQ	#4,A5
	MOVEA.L	A5,A4
	ADDQ.L	#1,A4
	MOVEQ	#NQ+NQ-4,D7	; (NQ-1) * 2 - 1 - 1
su8l	MOVE.B	(A4)+,(A5)+
	DBRA	D7,su8l
	MOVE.B	#0,(A5)
	MOVEM.L	(A7)+,A4/A5/D7
	UNLK	A6
	RTS

	GLOBAL shdn16
shdn16	LINK	A6,#0
	MOVEM.L	D7/A5/A4,-(A7)
	MOVE.L	8(A6),A5
	ADDA.W	#NQ+NQ,A5
	MOVEA.L	A5,A4
	ADDQ.L	#2,A4
	MOVEQ	#NQ-3,D7	; do NQ-2 words
sd6l	MOVE.W	-(A5),-(A4)
	DBRA	D7,sd6l
	MOVE.W	#0,-(A4)
	MOVEM.L	(A7)+,A4/A5/D7
	UNLK	A6
	RTS

	GLOBAL shup16
shup16	LINK	A6,#0
	MOVEM.L	D7/A5/A4,-(A7)
	MOVE.L	8(A6),A5
	ADDQ	#4,A5
	MOVEA.L	A5,A4
	ADDQ.L	#2,A4
	MOVEQ	#NQ-3,D7	; do NQ-2 words
su6l	MOVE.W	(A4)+,(A5)+
	DBRA	D7,su6l
	MOVE.W	#0,(A5)
	MOVEM.L	(A7)+,A4/A5/D7
	UNLK	A6
	RTS

	GLOBAL addm
addm	LINK	A6,#0
	MOVEM.L	D7/A5/A4,-(A7)
	MOVE.L	8(A6),A4
	ADDA.W	#NQ+NQ+2,A4 ; 2 * (2 + (NQ-1))
	MOVE.L	12(A6),A5
	ADDA.W	#NQ+NQ+2,A5
	MOVEQ	#NQ-2,D7
	MOVE	#0,CCR
add1l	ADDX.W	-(A4),-(A5)
	DBRA	D7,add1l
	MOVEM.L	(A7)+,A4/A5/D7
	UNLK	A6
	RTS

	GLOBAL subm
subm	LINK	A6,#0
	MOVEM.L	D7/A5/A4,-(A7)
	MOVE.L	8(A6),A4
	ADDA.W	#NQ+NQ+2,A4 ; 2 * (2 + (NQ-1))
	MOVE.L	12(A6),A5
	ADDA.W	#NQ+NQ+2,A5
	MOVEQ	#NQ-2,D7
	MOVE	#0,CCR
sub1l	SUBX.W	-(A4),-(A5)
	DBRA	D7,sub1l
	MOVEM.L	(A7)+,A4/A5/D7
	UNLK	A6
	RTS

; Variable precision multiply of significands.
; c must not be in the same location as either a or b.
;
; static int mulv( a, b, c, prec )
; unsigned short a[], b[], c[];
; int prec;

	GLOBAL mulv
mulv	LINK	A6,#0
	MOVEM.L	D7/D6/D5/D4/D3/D2/D1/A5/A4/A3/A2/A1,-(A7)
	MOVE.L	8(A6),A4
	MOVE.L	12(A6),A5
	MOVE.L	16(A6),A3
	MOVE.L	20(A6),D6	; precision, in words

; clear the output array of prec+3 words
	MOVE.L	D6,D7
	ADDQ.L	#2,D7
;	MOVE.L	A3,A0
;	ADDQ	#4,A0
	LEA	4(A3),A0
	CLR.L	D1
mv2l
	MOVE.W	D1,(A0)+
	DBRA	D7,mv2l


; for( k=prec+1; k>=3; k -= 2 )
	MOVE.L	D6,D7	; prec
	ASL.L	#1,D7
	ADDA.L	D7,A3
	ADDQ	#6,A3	; r = &c[prec+3];
	ADDQ.L	#2,D7	; D7 = 2*k, k = prec+1
; {
mv0l	MOVE.L	A5,A1	; q = &b[3];
	ADDQ	#6,A1
	MOVEA.L	A4,A0
	ADDA.L	D7,A0	; p = &a[k];

; for( i=k; i>=3; i-- )
;	{
	MOVE.L	D7,D5	; 2*k

;	if( (*p == 0) || (*q == 0) )
;		{
;		--p;
;		++q;
;		continue;
;		}
	ADDQ.L	#4,A0
	MOVE.L	(A3),D6
	MOVE.L	-(A3),D4
	MOVE.W	-(A3),D2
mv1l	MOVE.L	-(A0),D0	; *p--
	DATA.W	$4C11, $0403	;MULU.L	(A1),D3:D0
	ADDQ	#4,A1
	ADD.L	D0,D6
	ADDX.L	D3,D4
	ADDX.W	D1,D2
	SUBQ	#4,D5
	CMPI.W	#6,D5
	BGE.S	mv1l

	MOVE.W	D2,(A3)+
	MOVE.L	D4,(A3)+
	MOVE.L	D6,(A3)
	SUBQ	#4,A3
	SUBQ	#4,D7
	CMPI.W	#6,D7
	BGE.S	mv0l

	MOVEM.L	(A7)+,A1/A2/A3/A4/A5/D7/D6/D5/D4/D3/D2/D1
	UNLK	A6
	RTS

; Variable precision square.
; b must be in a different location from a.
;
; static squarev( a, b, prec )
; unsigned short a[], b[];
; int prec;
; {

	GLOBAL squarev
squarev	LINK	A6,#0
	MOVEM.L	D7/D6/D5/D3/D2/D1/A5/A4/A3/A2/A1,-(A7)
	MOVE.L	8(A6),A4	; a
	MOVE.L	12(A6),A3	; b
	MOVE.L	16(A6),D6	; precision, in words

; clear the output array of prec+3 words
	MOVE.L	D6,D7	; prec
	ADDQ.L	#2,D7
	MOVE.L	A3,A0	; b
	ADDQ	#4,A0
	CLR.L	D1
sq5l	MOVE.W	D1,(A0)+
	DBRA	D7,sq5l

; r = &b[prec+3];
; for( k=prec+1; k>=3; k-- )
; {
	MOVE.L	D6,D7	; prec
	ASL.L	#1,D7
	ADDA.L	D7,A3
	ADDQ	#6,A3	; r = &b[prec+3];
	ADDQ.L	#2,D7	; k = prec + 1

sq0l	MOVE.L	A4,A1	; q = &a[3];
	ADDQ	#6,A1
	MOVEA.L	A4,A0
	ADDA.L	D7,A0	; p = &a[k];

;while( p >= q )	
;	{
sq1l	CMPA.L	A0,A1
	BHI.S	sq3l
;	if( (*p == 0) || (*q == 0) )
;		{
;		--p;
;		++q;
;		continue;
;		}
	MOVE.L	(A0),D0
	DATA.W	$4C11, $0403	;MULU.L	(A1),D3:D0
	CMPA.L	A0,A1
	BEQ.S	sq2l

	CLR.L	D1
	LSL.L	#1,D0		; 2ab term of square
	ROXL.L	#1,D3
	ROXL.L	#1,D1
	ADD.W	D1,-6(A3)

sq2l	SUBQ	#4,A0
	ADDQ	#4,A1
;	SUBQ	#4,A3	; accumulate in *r
	CLR.L	D1

	ADD.L	D0,(A3)
	MOVE.L	-(A3),D2
	ADDX.L	D3,D2
	MOVE.L	D2,(A3)
	MOVE.W	-(A3),D2
	ADDX.W	D1,D2
	MOVE.W	D2,(A3)+
	ADDQ	#4,A3
	BRA	sq1l

;	}
; --r;
; }
sq3l	SUBQ	#4,A3
	SUBQ	#4,D7
	CMPI.W	#6,D7
	BGE.S	sq0l
; shup1(b);
	MOVE.L	12(A6),-(A7)
	JSR	shup1
	ADDQ	#4,A7
	MOVEM.L	(A7)+,A1/A2/A3/A4/A5/D7/D6/D5/D3/D2/D1
	UNLK	A6
	RTS


; mulm( b, ac3 )
; unsigned short b[], ac3[];
; {
	GLOBAL mulm
mulm	LINK	A6,#0
	MOVEM.L	D7/D6/D5/D4/D3/D2/D1/A5/A4/A3/A2/A1,-(A7)
	MOVE.L	8(A6),A4	; b
	MOVE.L	12(A6),A2	; ac3
	SUBA.L	#NQ+NQ+6,A7
	MOVE.L	A7,A5		; act
; qclear( act );
	MOVE.L	#NQ+2,D0
	MOVE.L	A5,A1
	CLR.L	D1
mm0l
	MOVE.W	D1,(A1)+
	DBRA	D0,mm0l

;act[0] = ac3[0];
;act[1] = ac3[1];
	MOVE.L	A2,A0	; ac3
	MOVE.L	A5,A1	; act
	MOVE.W	(A0)+,(A1)+
	MOVE.W	(A0)+,(A1)+
;r = &act[NQ+1];
	MOVE.L	A5,A3
	ADDA.L	#NQ+NQ+2,A3
;for( k=NQ-1; k>=3; k -= 2 )
;{
	MOVE.L	#NQ-1,D7	; k
mm1l	CMP.B	#3,D7
	BLT.S	mm7l
;if( k == NQ-1 )
;	{
;;	CMP.B	#NQ-1,D7
;;	BNE.S	mm3al
;	m = NQ-3;
;	o = 5;
;;	MOVE.L	#NQ-3,D6 ; m
;;	MOVE.L	#5,D5	; o
;;	BRA.S	mm3bl
;	}
;else
;	{
;	m = k;
;	o = 3;
mm3al	MOVE.L	D7,D6	; m
	MOVEQ.L	#3,D5
;	}
; p = &b[m];
mm3bl	ASL.L	#1,D6	; m
	MOVE.L	A4,A0
	ADDA.L	D6,A0	; p
; q = &ac3[o];
	ASL.L	#1,D5	; o
	MOVE.L	A2,A1
	ADDA.L	D5,A1	; q
	SUB.W	D5,D6
	ASR.W	#2,D6
; for( i=m; i>=o; i-- )
;	{
	ADDQ	#4,A0
	MOVE.L	(A3),D5
	MOVE.L	-(A3),D4
	MOVE.W	-(A3),D2
mm2l
	MOVE.L	-(A0),D0
	DATA.W	$4C11, $0403	;MULU.L	(A1),D3:D0
	ADD.L	D0,D5
	ADDX.L	D3,D4
	ADDX.W	D1,D2
mm4l
	ADDQ.L	#4,A1
	DBRA	D6,mm2l
;	}
;--r;
mm6l
	MOVE.W	D2,(A3)+
	MOVE.L	D4,(A3)+
	MOVE.L	D5,(A3)
	SUBQ.L	#4,A3
	SUBQ.L	#2,D7
	BRA.S	mm1l
;}
;mdnorm( act );
mm7l	MOVE.L	A5,-(A7)
	JSR	mdnorm
	ADDQ.L	#4,A7
;qmov( act, ac3 );
	MOVE.L	A5,A0	; act
	MOVE.L	A2,A1	; ac3
	MOVE.L	#NQ-1,D0
mm8l	MOVE.W	(A0)+,(A1)+
	DBRA	D0,mm8l
;}
	ADDA.L	#NQ+NQ+6,A7
	MOVEM.L	(A7)+,A1/A2/A3/A4/A5/D7/D6/D5/D4/D3/D2/D1
	UNLK	A6
	RTS


;mulin( b, ac3 )
;unsigned short b[], ac3[];
;{
	GLOBAL mulin
mulin	LINK	A6,#0
	MOVEM.L	D7/D6/D5/D2/D1/A5/A4/A3/A2/A1,-(A7)
	MOVE.L	8(A6),A4	; b
	MOVE.L	12(A6),A2	; ac3
	SUBA.L	#NQ+NQ+4,A7
	MOVE.L	A7,A5		; act
; qclear( act );
	MOVE.L	#NQ+1,D0
	MOVE.L	A5,A1
	CLR.L	D1
mn0l
	MOVE.W	D1,(A1)+
	DBRA	D0,mn0l

;act[0] = ac3[0];
;act[1] = ac3[1];
	MOVE.L	A2,A0	; ac3
	MOVE.L	A5,A1	; act
	MOVE.W	(A0)+,(A1)+
	MOVE.W	(A0)+,(A1)+
;r = &act[NQ];
	MOVE.L	A5,A3
	ADDA.L	#NQ+NQ,A3
;y = b[3];
	CLR.L	D6
	MOVE.W	6(A4),D6
;p = &ac3[NQ-1];
	MOVE.L	A2,A1
	ADDA.L	#NQ+NQ-2,A1
;for( i=NQ-1; i>=3; i-- )
;	{
	MOVE.L	#NQ-1,D7
mn1l	CMP.B	#2,D7
	BEQ.S	mn7l
;	if( *p == 0 )
;		{
;		--p;
;		--r;
;		continue;
;		}
	MOVE.L	D6,D0
	MULU	(A1),D0
	SUBQ	#2,A1
	SUBQ	#2,A3
	ADD.L	D0,(A3)
	MOVE.W	-(A3),D2
	ADDX.W	D1,D2
	MOVE.W	D2,(A3)+
;	ADDQ	#2,A3
;	}
	SUBQ	#1,D7
	BRA.S	mn1l
;mdnorm( act );
mn7l	MOVE.L	A5,-(A7)
	JSR	mdnorm
	ADDQ	#4,SP
;qmov( act, ac3 );
	MOVE.L	A5,A0	; act
	MOVE.L	A2,A1	; ac3
	MOVE.L	#NQ-1,D0
mn8l	MOVE.W	(A0)+,(A1)+
	DBRA	D0,mn8l
;}
	ADDA.L	#NQ+NQ+4,A7
	MOVEM.L	(A7)+,A1/A2/A3/A4/A5/D7/D6/D5/D2/D1
	UNLK	A6
	RTS

;divsh( a, prod )
	GLOBAL divsh
divsh	LINK	A6,#0
	MOVEM.L	D7/D6/D5/D2/D1/A5/A4/A3/A2/A1,-(A7)
	MOVE.L	8(A6),A4	; a
	MOVE.L	12(A6),A5	; prod

;prod[NQ] = 0;
	CLR.W	NQ+NQ(A5)
;prod[NQ+1] = 0;
	CLR.W	NQ+NQ+2(A5)
;prod[NQ+2] = 0;
	CLR.W	NQ+NQ+4(A5)
;shdn1( prod );
	MOVE.L	A5,-(A7)
	JSR	shdn1
	ADDQ	#4,A7
;shdn1( prod );
	MOVE.L	A5,-(A7)
	JSR	shdn1
	ADDQ	#4,A7
;d = a[3];
	MOVE.L	#-65536,D5	; 0xffff0000
	MOVE.W	6(A4),D6
	SWAP	D6
	AND.L	D5,D6
;u = ((unsigned long )prod[3] << 16) | prod[4];
	MOVE.L	A5,A0
	ADDA.L	#6,A0	; &prod[3]
	MOVE.L	A0,A1
	MOVE.L	(A0)+,D0
	MOVE.L	(A0)+,D1

;for( i=3; i<NQ; i++ )
;	{
	MOVE.L	#3,D7
ds1l	CMP.B	#NQ,D7
	BGT.S	ds2l
;	qu = u / d;
	DATA.W	$4C46, $1400	; DIVU.L D6,D0:D1
;	prod[i] = qu;
	MOVE.L	D1,(A1)+
;	u = ((u - (unsigned long )d * qu) << 16) | prod[i+2];
	MOVE.L	(A0)+,D1
;	}
	ADDQ.L	#2,D7
	BRA.S	ds1l
ds2l
; prod[NQ] = u / d;
;	DIVU.W	D6,D0
;	MOVE.W	D0,(A1)+
	MOVEM.L	(A7)+,A1/A2/A3/A4/A5/D7/D6/D5/D2/D1
	UNLK	A6
	RTS


;	move a to b
;
;	short a[NQ], b[NQ];
;	qmov( a, b );

	GLOBAL qmov
qmov	LINK	A6,#0
	MOVE.L	A1,-(A7)
	MOVE.L	8(A6),A0
	MOVE.L	12(A6),A1
	MOVE.L	#NQ/2-1,D0	; 2*(11+1) = NQ words
qm1l	MOVE.L	(A0)+,(A1)+
	DBRA	D0,qm1l
	MOVE.L	(A7)+,A1
	UNLK	A6
	RTS



;qmovz( a, b )
; Same as qmov but clears 1 low guard word at 25th array position
	GLOBAL qmovz
qmovz	LINK	A6,#0
	MOVE.L	A1,-(A7)
	MOVE.L	8(A6),A0
	MOVE.L	12(A6),A1
	MOVE.L	#NQ/2-1,D0	; 2*(11+1) = NQ words
qmz1l	MOVE.L	(A0)+,(A1)+
	DBRA	D0,qmz1l
	CLR.W	(A1)+
	MOVE.L	(A7)+,A1
	UNLK	A6
	RTS

	GLOBAL qclear
qclear	LINK	A6,#0
	MOVE.L	D1,-(A7)
	MOVEQ	#0,D1
	MOVE.L	8(A6),A0
	MOVE.L	#NQ/2-1,D0	; 2*(11+1) = NQ words
qm1l	MOVE.L	D1,(A0)+
	DBRA	D0,qm1l
	MOVE.L	(A7)+,D1
	UNLK	A6
	RTS


sqr	equ	-NQ-NQ-6	; -54
prod	equ	-4*NQ-12	; -108
quot	equ	-6*NQ-18	; -162
; divm( a, b )
; unsigned short a[], b[];
	GLOBAL divm
divm     LINK       A6,#-188
         MOVEM.L    D7/D6/D5/D2/D1/A5/A4/A3/A2,-(A7)
         MOVE.L     8(A6),A3
         MOVE.L     12(A6),A5
         LEA        prod(A6),A4
	LEA	8(A3),A2	; &a[4]
	MOVEQ	#NQ-5,D5
dvm1al	TST.W	(A2)+
	BNE.S	dvm48l
	DBRA	D5,dvm1al
         MOVE.L     A4,-(A7)	; prod
         MOVE.L     A5,-(A7)	; b
         JSR        qmov
         ADDQ.W     #8,A7
         MOVE.L     A4,-(A7)	; prod
         MOVE.L     A3,-(A7)	; a
         JSR        divsh
         ADDQ.W     #8,A7
         BRA        dvm100l
dvm48l
	LEA	quot(A6),A0
	MOVE.L	#NQ+2,D0
	CLR.L	D1
dvm49l	MOVE.W	D1,(A0)+
	DBRA	D0,dvm49l
         MOVE.L     A4,-(A7)
         JSR        qclear
         ADDQ.W     #4,A7
         PEA        sqr(A6)
         JSR        qclear
         ADDQ.W     #4,A7
	MOVEQ      #0,D0
	MOVE.W     6(A3),D0	; a[3]
	MOVE.L     #1073741824,D1
         DIVU	D0,D1
         MOVE.W     D1,quot+6(A6)	; quot[3]
         MOVEQ      #2,D7	; prec
         MOVEQ      #1,D6	; k
dvm86l         MOVEQ      #NQ-2,D0
         CMP.L      D7,D0
         BLE.S      dvme2l
         MOVE.L     D6,D0
         LSL.L      #1,D0
         MOVE.L     D0,D6
         MOVEQ      #NQ-2,D0
         CMP.L      D6,D0
         BGE.S      dvm9cl
         MOVEQ      #NQ-2,D7	; prec = NQ - 2
         BRA.S      dvm9el
dvm9cl         MOVE.L     D6,D7
dvm9el         MOVE.L     D7,-(A7)
         PEA        sqr(A6)
         PEA        quot(A6)
         JSR        squarev
         ADDA.W     #12,A7
         MOVE.L     D7,-(A7)
         MOVE.L     A4,-(A7)
         PEA        sqr(A6)
         MOVE.L     A3,-(A7)
         JSR        mulv
         ADDA.W     #16,A7
         PEA        quot(A6)
         MOVE.L     A4,-(A7)
         JSR        subm
         ADDQ.W     #8,A7
         PEA        quot(A6)
         JSR        shup1
         ADDQ.W     #4,A7
         BRA.S      dvm86l
dvme2l	MOVE.L	#NQ-2,-(A7)
         MOVE.L     A4,-(A7)
         MOVE.L     A5,-(A7)
         PEA        quot(A6)
         JSR        mulv
         ADDA.W     #16,A7
         MOVE.W     (A5),(A4)
         MOVE.W     2(A5),2(A4)
dvm100l         MOVE.L     A4,-(A7)
         JSR        mdnorm
         MOVE.L     A5,(A7)
         MOVE.L     A4,-(A7)
         JSR        qmov
         ADDQ.W     #8,A7
         MOVEM.L    (A7)+,A2/A3/A4/A5/D1/D2/D5/D6/D7
         UNLK       A6
         RTS        


	GLOBAL mdnorm
mdnorm   LINK       A6,#0
         MOVEM.L    D7/A5/A4/A3/A2,-(A7)
         MOVE.L     8(A6),A4
;	LEA	rndset,A3
;	MOVE.W     (A3),D0
;        EXT.L      D0
;         BNE.S      mdn38l
;         PEA        rndbit
;	MOVE.L	(A7),A2
;         JSR        qclear
;         ADDQ.W     #4,A7
;         MOVEQ      #1,D0
;         MOVE.W     D0,NQ+NQ-2(A2)
;         CLR.W      NQ+NQ(A2)
;         MOVE.W     D0,(A3)	;rndset
;mdn38l
	LEA        2(A4),A5
         CLR.L      D7
         BRA.S      mdn62l
mdn40l	MOVEQ      #0,D0
         MOVE.W     4(A4),D0
         BNE.S      mdn4al
         BRA.S      mdn68l
mdn4al	MOVE.L     A4,-(A7)
         JSR        shdn1
         ADDQ.W     #4,A7
         ADDQ.W     #1,(A5)
;         MOVE.W     (A5),D0
;         EXT.L      D0
;         BGE.S      mdn60l
;         MOVE.W     #32767,(A5)
mdn60l         ADDQ.L     #1,D7
mdn62l         MOVEQ      #3,D0
         CMP.L      D7,D0
         BGT.S      mdn40l
mdn68l         CLR.L      D7
         BRA.S      mdn92l
mdn6cl         MOVEQ      #0,D0
         MOVE.W     6(A4),D0
         AND.L      #32768,D0
         BEQ.S      mdn7cl
         BRA.S      mdn98l
mdn7cl         MOVE.L     A4,-(A7)
         JSR        shup1
         ADDQ.W     #4,A7
         MOVE.W     (A5),D0
         EXT.L      D0
         BEQ.S      mdn90l
         SUBQ.W     #1,(A5)
mdn90l         ADDQ.L     #1,D7
mdn92l         MOVEQ      #3,D0
         CMP.L      D7,D0
         BGT.S      mdn6cl
mdn98l         MOVEQ      #0,D0
         MOVE.W     NQ+NQ(A4),D0
         AND.L      #32768,D0
         BEQ.S      mdnb6l
         MOVE.L     A4,-(A7)
         PEA        rndbit
         JSR        addm
         ADDQ.W     #8,A7
mdnb6l         TST.W      4(A4)
         BEQ.S      mdnd2l
         MOVE.L     A4,-(A7)
         JSR        shdn1
         ADDQ.W     #4,A7
         ADDQ.W     #1,(A5)
;         MOVE.W     (A5),D0
;         EXT.L      D0
;         BGE.S      mdnd2l
;         MOVE.W     #32767,(A5)
mdnd2l         CLR.W      NQ+NQ(A4)
         MOVEM.L    (A7)+,A2/A3/A4/A5/D7
         UNLK       A6
         RTS        



qfltc	LINK       A6,#0
         UNLK       A6
         RTS        

; for 24 word format
rndbit	DATA.W 0,0,0,0,0,0,0,0
	DATA.W 0,0,0,0,0,0,0,0
	DATA.W 0,0,0,0,0,0,0,1,0

; for 12 word format
;rndbit	DATA.W 0,0,0,0,0,0,0,0
;	DATA.W 0,0,0,1,0

;rndset	DATA.W  0

	END
