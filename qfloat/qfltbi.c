/* NOTE, this version of qfltb.c uses the 64-bit `long long int'
   type available in the GNU C compiler.  It can be used only
   with WORDSIZE = 32 bits.  */

#ifdef _MSC_VER
/* Microsoft Visual C */
#define INT64 __int64
#else
/* GNU C */
#define INT64 long long
#endif

/* Define nonzero for processors that can shift by 31 bits quickly. */
#define FASTSHIFT 1

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
 * Copyright (c) 1984 - 1988 by Stephen L. Moshier.  All rights reserved.
 */

#include "qhead.h"
/* #define N (NQ-2) */
#if WORDSIZE != 32
 Error qfltbi.c works only with WORDSIZE 32.
#endif

static int mulv(), squarev();
int mdnorm();

/*
;	Shift mantissa down by 1 bit
*/

int shdn1(x)
register QELT *x;
{
#if FASTSHIFT
register QELT newbits;
#endif
register QELT bits, u;
int i;

x += 2;	/* point to mantissa area */

bits = 0;
for( i=0; i<NQ-1; i++ )
	{
	u = *x;
#if FASTSHIFT
	newbits = u << (WORDSIZE - 1);
	u >>= 1;
	u |= bits;
	bits = newbits;
#else
	bits |= u & 0x1;
	u >>= 1;
	if( bits & 2 )
		u |= SIGNBIT;
	bits <<= 1;
#endif
	*x++ = u;
	}	
return 0;
}

/*
;	Shift mantissa up by 1 bit
*/
int shup1(x)
register QELT *x;
{
#if FASTSHIFT
register QELT newbits;
#endif
register QELT bits, u;
int i;

x += NQ;
bits = 0;

for( i=0; i<NQ-1; i++ )
	{
	u = *x;
#if FASTSHIFT
	newbits = u >> (WORDSIZE - 1);
	u <<= 1;
	u |= bits;
	bits = newbits;
#else
	bits |= u & SIGNBIT;
	u <<= 1;
	if( bits & (SIGNBIT >> 1) )
		u |= 1;
	bits >>= 1;
#endif
	*x-- = u;
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
	newbyt = *x << (WORDSIZE - 8);
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
	newbyt = *x >> (WORDSIZE - 8);
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
#if WORDSIZE == 16
int i;
register short *p;

p = x+2;
x += 3;

for( i=0; i<NQ-2; i++ )
	*p++ = *x++;

*p = 0;
return 0;
#else
int i;
register QELT newbyt, oldbyt;

x += NQ;
oldbyt = 0;

for( i=0; i<NQ-1; i++ )
	{
	newbyt = *x >> 16;
	*x <<= 16;
	*x |= oldbyt;
	oldbyt = newbyt;
	--x;
        }
return 0;
#endif
}

/*
;	Shift mantissa down by 16 bits
*/

int shdn16(x)
register QELT *x;
{
#if WORDSIZE == 16
int i;
register unsigned short *p;

x += NQ;
p = x+1;

for( i=0; i<NQ-2; i++ )
	*(--p) = *(--x);

*(--p) = 0;
return 0;
#else
register QELT newbyt, oldbyt;
int i;

x += 2;
oldbyt = 0;
for( i=0; i<NQ-1; i++ )
	{
	newbyt = *x << 16;
	*x >>= 16;
	*x |= oldbyt;
	oldbyt = newbyt;
	++x;
	}
return 0;
#endif
}



/*
;	add mantissas
;	x + y replaces y
*/

int addm( x, y )
QELT *x, *y;
{
register unsigned INT64 a;
int i;
QELT carry;

x += NQ;
y += NQ;
carry = 0;
for( i=0; i<NQ-1; i++ )
	{
	a = (unsigned INT64 )(*x) + (unsigned INT64 )(*y) + carry;
	if( ((unsigned int) (a >> 32)) & 0x1 )
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
register unsigned INT64 a;
int i;
QELT carry;

x += NQ;
y += NQ;
carry = 0;
for( i=0; i<NQ-1; i++ )
	{
	a = (unsigned INT64 )(*y) - (unsigned INT64 )(*x) - carry;
	if( ((unsigned int) (a >> 32)) & 0x1 )
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
QELT a[], b[];
{
QELT sqr[NQ+2], prod[NQ+2], quot[NQ+2];
int i, prec, k;
QELT d, qu, *p;
unsigned INT64 u;

/* Test if denominator has only 32 bits of significance. */
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
u = ((unsigned INT64)prod[3] << 32) | prod[4];
for( i=3; i<NQ; i++ )
	{
	qu = u / d;
	prod[i] = qu;
	u = ((u - (unsigned INT64)d * qu) << 32) | prod[i+2];
	}
prod[NQ] = u / d;
goto divdon;


longdiv:

/* Slower procedure is required */
qclear(quot);
quot[NQ] = 0;
qclear(prod);
qclear(sqr);
#ifdef _MSC_VER
quot[3] = (((unsigned INT64)0x40000000) << 32) / a[3];
#else
quot[3] = ((unsigned INT64)0x4000000000000000ULL) / a[3];
#endif
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
QELT a[], b[], c[];
int prec;
{
register QELT *p, *q, *r;
register unsigned INT64 u, lp;
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
	lp = (unsigned INT64)(*p--) * (*q++);
	u = (unsigned INT64)(*r) + ((unsigned int) lp);
	*r = u;
	u = (unsigned INT64)(*(r-1)) + (lp >> 32) + (u >> 32);
	*(r-1) = u;
	*(r-2) += u >> 32;
	}
--r;
}
return 0;
}



/* Variable precision square.
 * b must be in a different location from a.
 */
static int squarev( a, b, prec )
QELT a[], b[];
int prec;
{
QELT *p, *q, *r;
register unsigned INT64 u, lp;
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
	lp = (unsigned INT64)(*p) * (*q);
	if( p != q )
		{
		if( (lp >> 32) & 0x80000000 )
			*(r-2) += 1;
		lp <<= 1;
		}
	--p;
	++q;
	u = (unsigned INT64)(*r) + ((unsigned int) lp);
	*r = u;
	u = (unsigned INT64)(*(r-1)) + (lp >> 32) + (u >> 32);
	*(r-1) = u;
	*(r-2) += u >> 32;
	}
--r;
}
shup1(b);
return 0;
}





int mulm( b, ac3 )
QELT b[], ac3[];
{
register QELT *p, *q;
QELT act[NQ+2];
QELT *r;
unsigned INT64 lp, a;
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
	lp = (unsigned INT64)(*p--) * (*q++);
	a = (unsigned INT64)(*r) + ((unsigned int) lp);
	*r = a;
	a = (unsigned INT64)(*(r-1)) + (lp >> 32) + (a >> 32);
	*(r-1) = a;
	*(r-2) += a >> 32;
	}
--r;
}
mdnorm( act );
qmov( act, ac3 );
return 0;
}




int mulin( b, ac3 )
QELT b[], ac3[];
{
register QELT *p, *r;
QELT act[NQ+1];
QELT y;
unsigned INT64 lp, a;
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
	lp = (unsigned INT64)(*p--) * y;
	a = (unsigned INT64)(*r) + ((unsigned int) lp);
	*r = a;
	a = (unsigned INT64)(*(r-1)) + (lp >> 32) + (a >> 32);
	*(r-1) = a;
	*(r-2) += a >> 32;
	--r;
	}
mdnorm( act );
qmov( act, ac3 );
return 0;
}


QELT rndbit[NQ+1];
static int rndset = 0;

int mdnorm( x )
QELT x[];
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
		{
		*p = MAXEXP; 
		mtherr("mdnorm", OVERFLOW);
		}
	}
for( i=0; i<3; i++ )
	{
	if( x[3] & SIGNBIT )
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

if( x[NQ] & SIGNBIT )
	{
/*
	if(  ((x[NQ] & SIGNBIT) == SIGNBIT) && ((x[NQ-1] & 1) == 0)  )
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
x[NQ] = 0;
return 0;
}

/*
;	move a to b
*/

int qmov( a, b )
QELT *a, *b;
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
QELT *a, *b;
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
*/

int qclear( x )
register QELT *x;
{
register int i;

for( i=0; i<NQ; i++ )
	*x++ = 0;
return 0;
}
