/* 
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
 * Copyright (c) 1984 - 1996 by Stephen L. Moshier.  All rights reserved.
 */

#include "qhead.h"

#define N (NQ-2)
#if WORDSIZE != 16
 Error qfltb.c works only with WORDSIZE 16.
#endif

#ifdef ANSIPROT
int mdnorm ( QELT *);
static int mulv(QELT *, QELT *, QELT *, int);
static int squarev(QELT *, QELT *, int);
#else
static int mulv(), squarev();
int mdnorm();
#endif

/*
;	Shift mantissa down by 1 bit
*/

int shdn1(x)
register unsigned short *x;
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
register unsigned short *x;
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
register unsigned short *x;
{
register unsigned short newbyt, oldbyt;
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
register unsigned short *x;
{
int i;
register unsigned short newbyt, oldbyt;

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
register short *x;
{
int i;
register short *p;

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
register unsigned short *x;
{
int i;
register unsigned short *p;

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
unsigned short *x, *y;
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
unsigned short *x, *y;
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


int divm( a, b )
unsigned short a[], b[];
{
unsigned short sqr[NQ+2], prod[NQ+2], quot[NQ+2];
int i, prec, k;
unsigned short d, qu, *p;
unsigned long u;

/* Test if denominator has only 16 bits of significance. */
p = &a[4];
i = NQ-4;
do
	{
	if( *p++ != 0 )
		goto longdiv;
	}
while( --i );

/* Do single precision divides if so. */
qmov( b, prod );
prod[NQ] = 0;
prod[NQ+1] = 0;
shdn1( prod );
shdn1( prod );
d = a[3];
u = ((unsigned long )prod[3] << 16) | prod[4];
for( i=3; i<NQ; i++ )
	{
	qu = u / d;
	prod[i] = qu;
	u = ((u - (unsigned long )d * qu) << 16) | prod[i+2];
	}
prod[NQ] = u / d;
goto divdon;


longdiv:

/* Slower procedure is required */
qclear(quot);
quot[NQ] = 0;
qclear(prod);
qclear(sqr);
quot[3] = ((unsigned long )0x40000000) / (unsigned long )a[3];
prec = 2;
k = 1;
while( prec < NQ-2 )
	{
	k = 2 * k;
	if( k > NQ-2 )
		prec = NQ - 2;
	else
		prec = k;
	squarev( quot, sqr, prec );
	mulv( a, sqr, prod, prec );
	subm( prod, quot );
	shup1( quot );
	}
mulv( quot, b, prod, NQ-2 );
prod[0] = b[0];
prod[1] = b[1];

divdon:

mdnorm( prod );
qmov( prod, b );
return 0;
}

/*
prtemp( s, z )
char *s;
unsigned short z[];
{
int i;

printf( "%s ", s );
for( i=0; i<8; i++ )
	printf( "%04x ", z[i+2] );
printf( "\n" );
}
*/


/* Variable precision multiply of significands.
 * c must not be in the same location as either a or b.
 */
static int mulv( a, b, c, prec )
unsigned short a[], b[], c[];
int prec;
{
register unsigned short *p, *q, *r;
register unsigned long u, lp;
unsigned short carry;
int k, i;

k = prec+2;
p = &c[2];
do
	*p++ = 0;
while( --k );

r = &c[prec+3];
for( k=prec+2; k>=3; k-- )
{
q = &b[3];
p = &a[k];
for( i=k; i>=3; i-- )
	{
	if( (*p == 0) || (*q == 0) )
		{
		--p;
		++q;
		continue;
		}
	lp = (unsigned long )(*p--) * (unsigned long )(*q++);
	u = (unsigned long )(*r) + (lp & 0xffff);
	if( u & 0x10000 )
		carry = 1;
	else
		carry = 0;
	*r-- = u;
	u = (unsigned long )(*r) + ((lp >> 16) & 0xffff) + carry;
	*r-- = u;
	if( u & 0x10000 )
		*r += 1;
	r += 2;
	}
--r;
}
return 0;
}



/* Variable precision square.
 * b must be in a different location from a.
 */
static int squarev( a, b, prec )
unsigned short a[], b[];
int prec;
{
register unsigned short *p, *q, *r;
register unsigned long u, lp;
unsigned short carry;
int k;

k = prec+2;
p = &b[2];
do
	*p++ = 0;
while( --k );

r = &b[prec+3];
for( k=prec+2; k>=3; k-- )
{
q = &a[3];
p = &a[k];
while( p >= q )	
	{
	if( (*p == 0) || (*q == 0) )
		{
		--p;
		++q;
		continue;
		}
/*	printf( "%d %d %d\n", p - &a[3], q - &a[3], r - &b[3] );*/
	lp = (unsigned long )(*p) * (unsigned long )(*q);
	if( p != q )
		{
		if( lp & 0x80000000 )
			*(r-2) += 1;
		lp <<= 1;
		}
	--p;
	++q;
	u = (unsigned long )(*r) + (lp & 0xffff);
	if( u & 0x10000 )
		carry = 1;
	else
		carry = 0;
	*r-- = u;
	u = (unsigned long )(*r) + ((lp >> 16) & 0xffff) + carry;
	*r-- = u;
	if( u & 0x10000 )
		*r += 1;
	r += 2;
	}
--r;
}
shup1(b);
return 0;
}





int mulm( b, ac3 )
unsigned short b[], ac3[];
{
register unsigned short *p, *q;
unsigned short act[NQ+2];
unsigned short carry, *r;
unsigned long lp, a;
int i, k, m, o;

qclear( act );
act[0] = ac3[0];
act[1] = ac3[1];
act[NQ] = 0;
act[NQ+1] = 0;
r = &act[NQ+1];
for( k=NQ; k>=3; k-- )
{
if( k == NQ )
	{
	m = NQ-1;
	o = 4;
	}
else
	{
	m = k;
	o = 3;
	}
q = &ac3[o];
p = &b[m];

for( i=m; i>=o; i-- )
	{
	if( (*p == 0) || (*q == 0) )
		{
		--p;
		++q;
		continue;
		}
	lp = (unsigned long )(*p--) * (unsigned long )(*q++);
	a = (unsigned long )(*r) + (lp & 0xffff);
	if( a & 0x10000 )
		carry = 1;
	else
		carry = 0;
	*r-- = a;
	a = (unsigned long )(*r) + ((lp >> 16) & 0xffff) + carry;
	*r-- = a;
	if( a & 0x10000 )
		*r += 1;
	r += 2;
	}
--r;
}
mdnorm( act );
qmov( act, ac3 );
return 0;
}




int mulin( b, ac3 )
unsigned short b[], ac3[];
{
register unsigned short *p, *r;
unsigned short act[NQ+1];
unsigned short carry, y;
unsigned long lp, a;
int i;

qclear( act );
act[0] = ac3[0];
act[1] = ac3[1];
act[NQ] = 0;
r = &act[NQ];
y = b[3];
p = &ac3[NQ-1];
for( i=NQ-1; i>=3; i-- )
	{
	if( *p == 0 )
		{
		--p;
		--r;
		continue;
		}
	lp = (unsigned long )(*p--) * y;
	a = (unsigned long )(*r) + (lp & 0xffff);
	if( a & 0x10000 )
		carry = 1;
	else
		carry = 0;
	*r-- = a;
	a = (unsigned long )(*r) + ((lp >> 16) & 0xffff) + carry;
	*r-- = a;
	if( a & 0x10000 )
		*r += 1;
	++r;
	}
mdnorm( act );
qmov( act, ac3 );
return 0;
}


unsigned short rndbit[NQ+1];
short rndset = 0;

int mdnorm( x )
unsigned short x[];
{
int i;
register unsigned short *p;

if( rndset == 0 )
	{
	qclear( rndbit );
	rndbit[NQ-1] = 1;
	rndbit[NQ] = 0;
	rndset = 1;
	}
p = &x[1];
for( i=0; i<3; i++ )
	{
	if( x[2] == 0 )
		break;
	shdn1(x);
	if( *p < MAXEXP )
		*p += 1;
	else
		{
		*p = MAXEXP; 
		mtherr("mdnorm", OVERFLOW);
		}
	}
for( i=0; i<3; i++ )
	{
	if( x[3] & 0x8000 )
		break;
	/* Prevent exponent underflow.
	   Rounding may be incorrect when this happens.  */
	if( *p >= 1 )
		{
		shup1(x);
		*p -= 1;
		}
	else
		mtherr("mdnorm", UNDERFLOW);
	}

if( x[NQ] & 0x8000 )
	{
/*
	if(  ((x[NQ] & 0x8000) == 0x8000) && ((x[NQ-1] & 1) == 0)  )
		goto nornd;
*/
	addm( rndbit, x );
	}
if( x[2] )
	{
	shdn1( x );
	if( *p < MAXEXP )
		*p += 1;
	else
		{
		*p = MAXEXP; 
		mtherr("mdnorm", OVERFLOW);
		}
	}
/* nornd: */
x[NQ] = 0;
return 0;
}

/*
;	move a to b
;
;	short a[NQ], b[NQ];
;	qmov( a, b );
*/

int qmov( a, b )
register unsigned short *a, *b;
{
register int i;

i = NQ;
do
	{
	*b++ = *a++;
	}
while( --i );
return 0;
}


int qmovz( a, b )
register unsigned short *a, *b;
{
register int i;

i = NQ;
do
	{
	*b++ = *a++;
	}
while( --i );
*b++ = 0;
return 0;
}


/*
; Clear out entire number, including sign and exponent, pointed
; to by x
;
; short x[];
; qclear( x );
*/

int qclear( x )
register unsigned short *x;
{
register int i;

for( i=0; i<NQ; i++ )
	*x++ = 0;
return 0;
}
