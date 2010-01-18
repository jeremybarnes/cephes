/*		qflta.c
 * Utilities for extended precision arithmetic, called by qflt.c.
 * These should all be written in machine language for speed.
 *
 * addm( x, y )		add significand of x to that of y
 * shdn1( x )		shift significand of x down 1 bit
 * shdn8( x )		shift significand of x down 8 bits
 * shdn16( x )		shift significand of x down 16 bits
 * shup1( x )		shift significand of x up 1 bit
 * shup8( x )		shift significand of x up 8 bits
 * shup16( x )		shift significand of x up 16 bits
 * divm( a, b )		divide significand of a into b
 * mulm( a, b )		multiply significands, result in b
 * mdnorm( x )		normalize and round off
 *
 * Copyright (c) 1984 - 1988 by Stephen L. Moshier.  All rights reserved.
 */

#include "qhead.h"
#if WORDSIZE != 16
 Error qflta.c works only with WORDSIZE 16.
#endif

#ifdef ANSIPROT
int addm( QELT *, QELT * );
int subm( QELT *, QELT * );
int mulm( QELT *, QELT * );
int divm( QELT *, QELT * );
int cmpm( QELT *, QELT * );
int qmovz( QELT *, QELT * );
int shup1( QELT * ), shdn1( QELT * );
int shup8( QELT * ), shdn8( QELT * );
int shup16( QELT * ), shdn16( QELT * );
int mdnorm( QELT *, int );
#else
int qmovz(), cmpm(), mdnorm();
#endif

/*
;	Shift mantissa down by 1 bit
*/

int shdn1(x)
register QELT *x;
{
register short bits;
int i;

x += 2;	/* point to mantissa area */

bits = 0;
for( i=0; i<NQ-1; i++ )
	{
	if( *x & 1 )
		bits |= 1;
	*x >>= 1;
	if( bits & 2 )
		*x |= 0x8000;
	bits <<= 1;
	++x;
	}	
return 0;
}

/*
;	Shift mantissa up by 1 bit
*/
int shup1(x)
register QELT *x;
{
register short bits;
int i;

x += NQ;
bits = 0;

for( i=0; i<NQ-1; i++ )
	{
	if( *x & 0x8000 )
		bits |= 1;
	*x <<= 1;
	if( bits & 2 )
		*x |= 1;
	bits <<= 1;
	--x;
	}
return 0;
}



/*
;	Shift mantissa down by 8 bits
*/

int shdn8(x)
register QELT *x;
{
register QELT newbyt, oldbyt;
int i;

x += 2;
oldbyt = 0;
for( i=0; i<NQ-1; i++ )
	{
	newbyt = *x << 8;
	*x >>= 8;
	*x |= oldbyt;
	oldbyt = newbyt;
	++x;
	}
return 0;
}

/*
;	Shift mantissa up by 8 bits
*/

int shup8(x)
register QELT *x;
{
int i;
register QELT newbyt, oldbyt;

x += NQ;
oldbyt = 0;

for( i=0; i<NQ-1; i++ )
	{
	newbyt = *x >> 8;
	*x <<= 8;
	*x |= oldbyt;
	oldbyt = newbyt;
	--x;
	}
return 0;
}



/*
;	Shift mantissa up by 16 bits
*/

int shup16(x)
register QELT *x;
{
int i;
register QELT *p;

p = x+2;
x += 3;

for( i=0; i<NQ-2; i++ )
	*p++ = *x++;

*p = 0;
return 0;
}

/*
;	Shift mantissa down by 16 bits
*/

int shdn16(x)
register QELT *x;
{
int i;
register QELT *p;

x += NQ;
p = x+1;

for( i=0; i<NQ-2; i++ )
	*(--p) = *(--x);

*(--p) = 0;
return 0;
}



/*
;	add mantissas
;	x + y replaces y
*/

int addm( x, y )
QELT *x, *y;
{
register unsigned long a;
int i;
unsigned int carry;

x += NQ;
y += NQ;
carry = 0;
for( i=0; i<NQ-1; i++ )
	{
	a = (unsigned long )(*x) + (unsigned long )(*y) + carry;
	if( a & 0x10000 )
		carry = 1;
	else
		carry = 0;
	*y = a;
	--x;
	--y;
	}
return 0;
}

/*
;	subtract mantissas
;	y - x replaces y
*/

int subm( x, y )
QELT *x, *y;
{
register unsigned long a;
int i;
unsigned int carry;

x += NQ;
y += NQ;
carry = 0;
for( i=0; i<NQ-1; i++ )
	{
	a = (unsigned long )(*y) - (unsigned long )(*x) - carry;
	if( a & 0x10000 )
		carry = 1;
	else
		carry = 0;
	*y = a;
	--x;
	--y;
	}
return 0;
}


int divm( a, ac3 )
QELT a[], ac3[];
{
QELT act[NQ+1], ac1[NQ+1];
int i, ctr, lost;
QELT d, *p, *q, *r;

qmovz( a, ac1 );
qclear( act );
act[0] = ac3[0];
act[1] = ac3[1];
act[NQ] = 0;
ac3[NQ] = 0;

/* test for word-precision denominator */
for( i=4; i<NQ; i++ )
	{
	if( ac1[i] )
		goto longdiv;
	}
/* Do divide with faster compare and subtract */
d = ac1[3];
p = &ac3[3];
q = &ac3[2];
r = &act[NQ];
for( i=0; i<NBITS+2; i++ )
	{
	if( (*q != 0) || (*p >= d) )
		{
		*p -= d;
		*q = 0;
		*r = 0x8000;
		}
	shup1( ac3 );
	shup1( act );
	}
goto divdon;

/* Slower compare and subtract required */
longdiv:
for( i=0; i<NBITS+2; i++ )
	{
	if( cmpm( ac3, ac1 ) >= 0 )
		{
		subm( ac1, ac3 );
		ctr = 1;
		}
	else
		ctr = 0;
	shup1( ac3 );
	shup1( act );
	act[NQ-1] |= ctr;
	}

divdon:

p = &ac3[2];
lost = 0;
for( i=2; i<NQ; i++ )
	{
	if( *p++ != 0 )
		{
		lost = 1;
		break;
		}
	}
shdn1( act );
shdn1( act );
if( act[1] >= 1 )
	act[1] -= 1;
mdnorm( act, lost );
qmov( act, ac3 );
return 0;
}




int mulm( b, ac3 )
QELT b[], ac3[];
{
QELT act[NQ+1], ac2[NQ+1];
QELT *p, *q;
int ctr, nct, lost;

qmov( b, ac2 );
qclear( act );
act[0] = ac3[0];
act[1] = ac3[1];
act[NQ] = 0;
/* strip trailing zero bits */
nct = NBITS;
p = &ac2[NQ-1];
while( *p == 0 )
	{
	shdn16( ac2 );
	nct -= 16;
	}
if( (*p & 0xff) == 0 )
	{
	shdn8( ac2 );
	nct -= 8;
	}
lost = 0;
q = &act[NQ];
for( ctr=0; ctr<nct; ctr++ )
	{
	if( *p & 1 )
		addm(ac3, act);
	shdn1(ac2);
	lost |= *q & 1;
	shdn1(act);
	}
mdnorm( act, lost );
qmov( act, ac3 );
return 0;
}




int mulin( a, b )
QELT a[], b[];
{
mulm(a,b);
return 0;
}


QELT rndbit[NQ+1];
short rndset = 0;

int mdnorm( x, lost )
QELT x[];
int lost;
{
int i;
register QELT *p;

if( rndset == 0 )
	{
	qclear( rndbit );
	rndbit[NQ-1] = 1;
	rndbit[NQ] = 0;
	rndset = 1;
	}
p = (QELT *)&x[1];
for( i=0; i<3; i++ )
	{
	if( x[2] == 0 )
		break;
	shdn1(x);
	if( *p < MAXEXP )
		*p += 1;
	else
		*p = MAXEXP; 
	}
for( i=0; i<3; i++ )
	{
	if( x[3] & 0x8000 )
		break;
	shup1(x);
	if( *p >= 1 )
		*p -= 1;
	}
i = x[NQ] & 0xffff;
if( i & 0x8000 )
	{
	if( (i == 0x8000) && (lost == 0) )
		{
		if( (x[NQ-1] & 1) == 0 )
			goto nornd;
		}
	addm( rndbit, x );
	}
if( x[2] )
	{
	shdn1( x );
	if( *p < MAXEXP )
		*p += 1;
	else
		*p = MAXEXP; 
	}
nornd:
x[NQ] = 0;
return 0;
}

/*
;	move a to b
;
;	QELT a[NQ], b[NQ];
;	qmov( a, b );
*/

int qmov( a, b )
register QELT *a, *b;
{
int i;

for( i=0; i<NQ; i++ )
	*b++ = *a++;
return 0;
}


int qmovz( a, b )
register QELT *a, *b;
{
int i;

for( i=0; i<NQ; i++ )
	*b++ = *a++;
*b++ = 0;
return 0;
}


/*
; Clear out entire number, including sign and exponent, pointed
; to by x
;
; QELT x[];
; qclear( x );
*/

int qclear( x )
register QELT *x;
{
register int i;

for( i=0; i<NQ; i++ )
	*x++ = 0;
return 0;
}
