/*							qflt.c
 *			QFLOAT
 *
 *	Extended precision floating point routines
 *
 *	asctoq( string, q )	ascii string to q type
 *	dtoq( &d, q )		DEC double precision to q type
 *	etoq( &d, q )		IEEE double precision to q type
 *	e24toq( &d, q )		IEEE single precision to q type
 *	e113toq( &d, q )	128-bit long double precision to q type
 *	ltoq( &l, q )		long integer to q type
 *	qabs(q)			absolute value
 *	qadd( a, b, c )		c = b + a
 *	qclear(q)		q = 0
 *	qcmp( a, b )		compare a to b
 *	qdiv( a, b, c )		c = b / a
 *	qifrac( x, &l, frac )   x to integer part l and q type fraction
 *	qfrexp( x, l, y )	find exponent l and fraction y between .5 and 1
 *	qldexp( x, l, y )	multiply x by 2^l
 *	qinfin( x )		set x to infinity, leaving its sign alone
 *	qmov( a, b )		b = a
 *	qmul( a, b, c )		c = b * a
 *	qmuli( a, b, c )	c = b * a, a has only 16 significant bits
 *	qisneg(q)		returns sign of q
 *	qneg(q)			q = -q
 *	qnrmlz(q)		adjust exponent and mantissa
 *	qsub( a, b, c )		c = b - a
 *	qtoasc( a, s, n )	q to ASCII string, n digits after decimal
 *	qtod( q, &d )		convert q type to DEC double precision
 *	qtoe( q, &d )		convert q type to IEEE double precision
 *	qtoe24( q, &d )		convert q type to IEEE single precision
 *	qtoe113( q, &d )	convert q type to 128-bit long double precision
 *
 * Data structure of the number (a "word" is 16 bits)
 *
 *	sign word		(0 for positive, -1 for negative)
 *	exponent		(EXPONE for 1.0)
 *	high guard word		(always zero after normalization)
 *	N-1 mantissa words	(most significant word first,
 *				 most significant bit is set)
 *
 * Numbers are stored in C language as arrays.  All routines
 * use pointers to the arrays as arguments.
 *
 * The result is always normalized after each arithmetic operation.
 * All arithmetic results are chopped. No rounding is performed except
 * on conversion to double precision.
 */

/*
 * Revision history:
 *
 * SLM,  5 Jan 84	PDP-11 assembly language version
 * SLM,  2 Mar 86	fixed bug in asctoq()
 * SLM,  6 Dec 86	C language version
 *
 */

#include <stdio.h>
#include "mconf.h"
#include "qhead.h"

#ifdef UNK
#if BIGENDIAN
#define MIEEE
#else
#define IBMPC
#endif
#undef UNK
#endif

/* array index of the sign (a whole int wasted) */
#define SIGNWORD 0
/* array index of the exponent */
#define E 1
/* array index of first (high order) mantissa word */
#define M 2
/* least significant word */
#define LSWORD NQ-1
/* least significant guard word */
#define LSGUARD NQ
/* Define 1 for sticky bit rounding */
#ifndef STICKY
#define STICKY 0
#endif

	/* accumulators */
static QELT ac1[NQ+1];
static QELT ac2[NQ+1];
static QELT ac3[NQ+1];
static QELT ac4[NQ+1];
	/* shift count register */
static int SC;
extern QELT qzero[], qone[];

#ifdef ANSIPROT
int shdn1( QELT * ), shup1( QELT * ), shdn8( QELT * ), shup8( QELT * );
int shdn16( QELT * ), shup16( QELT * );
int mulm( QELT *, QELT * ), mulin( QELT *, QELT * ), divn( QELT *, QELT * );
int qmovz( QELT *, QELT * ), normlz( QELT * ), cmpm( QELT *, QELT * );
int addm( QELT *, QELT * ), subm( QELT *, QELT * ), shift( QELT * );
int qadd1( QELT *, QELT *, QELT * );
#else
int shdn1(), shup1(), shdn8(), shup8(), shdn16(), shup16();
int mulm(), mulin(), divm(), qmovz(), normlz();
int cmpm(), addm(), subm(), qadd1(), shift();
#endif

/* Absolute value  */

int qabs (x)
QELT x[];
{
  x[SIGNWORD] = 0;
  return  0;
}


/* Test if negative  */

int qisneg(x)
QELT x[];
{
return (x[SIGNWORD] != 0);
}

/*
;	Negate
*/

int qneg(x)
QELT x[];
{

#if WORDSIZE == 32
x[SIGNWORD] ^= -1; /* complement the sign */
#else
x[SIGNWORD] ^= 0xffff;
#endif
return 0;
}

/*
; convert long integer to q type
;
;	long l;
;	QELT q[NQ];
;	ltoq( &l, q );
; note &l is the memory address of l
*/

int ltoq( lp, y )
long *lp;
QELT y[];
{
long ll;

qclear( ac1 );
ll = *lp;		/* local copy of lp */
if( ll < 0 )
	{
	ll = -ll;	/* make it positive */
	ac1[SIGNWORD] = -1; /* put correct sign in the q type number */
	}

/* move the long integer to ac1 mantissa area */
#if WORDSIZE == 32
ac1[M] = ll;
ac1[E] = EXPONE - 1;	/* exponent if normalize shift count were 0 */
#else
ac1[M] = ll >> 16;
ac1[M+1] = ll;
ac1[E] = EXPONE+15;
#endif

ac1[LSGUARD] = 0;
if( normlz( ac1 ) )	/* normalize the mantissa */
	qclear( ac1 );	/* it was zero */
else
	ac1[E] -= SC;	/* else subtract shift count from exponent */
qmov( ac1, y );		/* output the answer */
return 0;
}

/*
;	convert DEC double precision to q type
;	double d;
;	QELT q[NQ];
;	dtoq( &d, q );
*/
int dtoq( d, y )
unsigned short *d;
QELT *y;
{
register QELT r, *p;

qclear(y);		/* start with a zero */
p = y;			/* point to our number */
r = *d;			/* get DEC exponent word */
if( r & 0x8000 )
	*p = -1;	/* fill in our sign */
++p;			/* bump pointer to our exponent word */
r &= 0x7fff;		/* strip the sign bit */
if( r == 0 )		/* answer = 0 if high order DEC word = 0 */
	return 0;


r >>= 7;	/* shift exponent word down 7 bits */
r -= 0200;	/* subtract DEC exponent offset */
r += EXPONE - 1;	/* add our q type exponent offset */
*p++ = r;	/* to form our exponent */

r = *d++;	/* now do the high order mantissa */
r &= 0177;	/* strip off the DEC exponent and sign bits */
r |= 0200;	/* the DEC understood high order mantissa bit */
	/* put result in our high guard word, combined with more mantissa */
#if WORDSIZE == 32
r = (r << 16) | *d++;
*p++ = r;
r = *d++;
r = (r << 16) | *d++;
*p = r;
#else
*p++ = r;	/* put result in our high guard word */
*p++ = *d++;	/* fill in the rest of our mantissa */
*p++ = *d++;
*p = *d;
#endif
shdn8(y);	/* shift our mantissa down 8 bits */
return 0;
}

/*
;	convert q type to DEC double precision
;	double d;
;	QELT q[NQ];
;	qtod( q, &d );
*/

int qtod( x, d )
QELT *x;
unsigned short *d;
{
register int r;
int i, j;

*d = 0;
if( *x != 0 )
	*d = 0100000;
qmovz( x, ac1 );
r = ac1[E];
if( r < (EXPONE - 1 - 0200) )
	goto zout;
#if WORDSIZE == 32
i = ac1[M+2];
#else
i = ac1[M+4];
#endif
if( (i & 0200) != 0 )
	{
	if( (i & 0377) == 0200 )
		{
		if( (i & 0400) != 0 )
			{
		/* check all less significant bits */
#if WORDSIZE == 32
			for( j=M+3; j<=NQ; j++ )
#else
			for( j=M+5; j<=NQ; j++ )
#endif
				{
				if( ac1[j] != 0 )
					goto yesrnd;
				}
			}
		goto nornd;
		}
yesrnd:
	qclear( ac2 );
#if WORDSIZE == 32
	ac2[ M+2 ] = 0200;
#else
	ac2[ M+4 ] = 0200;
#endif
	ac2[NQ] = 0;
	addm( ac2, ac1 );
	normlz(ac1);
	r -= SC;
	}

nornd:

r -= EXPONE - 1;
r += 0200;
if( (int) r < 0 )
	{
zout:
	*d++ = 0;
	*d++ = 0;
	*d++ = 0;
	*d++ = 0;
	return 0;
	}
if( r >= 0377 )
	{
	*d++ = 077777;
	*d++ = -1;
	*d++ = -1;
	*d++ = -1;
	return 0;
	}
r &= 0377;
r <<= 7;
shup8( ac1 );
ac1[M] &= 0177;
r |= ac1[M];
*d++ |= r;
#if WORDSIZE == 32
*d++ = ac1[M+1] >> 16;
*d++ = ac1[M+1];
*d++ = ac1[M+2] >> 16;
#else
*d++ = ac1[M+1];
*d++ = ac1[M+2];
*d++ = ac1[M+3];
#endif
return 0;
}

/*
;	Find integer and fractional parts

;	long i;
;	QELT x[NQ], frac[NQ];
;	qifrac( x, &i, frac );
*/

int qifrac( x, i, frac )
QELT x[];
long *i;
QELT frac[];
{

qmovz( x, ac1 );
SC = ac1[E] - (EXPONE - 1);
if( SC <= 0 )
	{
/* if exponent <= 0, integer = 0 and argument is fraction */
	*i = 0L;
	qmov( ac1, frac );
	return 0;
	}
if( SC > 31 )
	{
/*
;	long integer overflow: output large integer
;	and correct fraction
*/
	*i = 0x7fffffff;
	shift( ac1 );
	goto lab10;
	}
#if WORDSIZE == 16
if( SC > 16 )
	{
/*
; shift more than 16 bits: shift up SC-16, output the integer,
; then complete the shift to get the fraction.
*/
	SC -= 16;
	shift( ac1 );
	*i = ((unsigned long )ac1[M] << 16) | (unsigned short )ac1[M+1];
/*
 * 	p = (short *)i;
 * #ifdef DEC
 * 	*p++ = ac1[M];
 * 	*p++ = ac1[M+1];
 * #else
 * 	*p++ = ac1[M+1];
 * 	*p++ = ac1[M];
 * #endif
 */
	shup16( ac1 );
	goto lab10;
	}
#endif

shift( ac1 );
#if WORDSIZE == 32
*i = ac1[M];
#else
*i = ac1[M] & 0xffff;
#endif

lab10:
if( x[SIGNWORD] )
	*i = -(*i);
ac1[SIGNWORD] = 0;
ac1[E] = EXPONE - 1;
ac1[M] = 0;
if( normlz(ac1) )
	qclear( ac1 );
else
	ac1[E] -= SC;

qmov( ac1, frac );
return 0;
}

int qldexp( x, n, y)
QELT *x;
long n;
QELT *y;
{
long k;

k = (long) x[E]  +  n;
qmov( x, y );
y[E] = k;
if( (k > MAXEXP) || (n > (2 * (long)MAXEXP)) )
        qinfin(y);
if( (k <= 0) || (n < (-2 * (long)MAXEXP)) )
        qclear(y);
return(0);
}


int qfrexp (x, e, y)
QELT *x;
long *e;
QELT *y;
{

if( x[E] == 0 )
	{
	*e = 0;
	qclear(y);
	}
else
	{
	*e = (long) x[E] - (long) EXPONE + 1;
	qmov( x, y );
	y[1] = EXPONE - 1;
	}
return 0;
}

/*
;	subtract
;
;	QELT a[NQ], b[NQ], c[NQ];
;	qsub( a, b, c );	 c = b - a
*/

static short subflg = 0;

int qsub( a, b, c )
QELT *a, *b, *c;
{

subflg = 1;
qadd1( a, b, c );
return 0;
}


/*
;	add
;
;	QELT a[NQ], b[NQ], c[NQ];
;	qadd( a, b, c );	 c = b + a
*/

int qadd( a, b, c )
QELT *a, *b, *c;
{

subflg = 0;
qadd1( a, b, c );
return 0;
}

int qadd1( a, b, c )
QELT *a, *b, *c;
{
long lt;
int i;
#if STICKY
int lost;
#endif

qmovz( a, ac1 );
qmovz( b, ac2 );
if( subflg )
	ac1[SIGNWORD] = ~ac1[SIGNWORD];

/* compare exponents */
lt = (long) ac1[E] - (long) ac2[E]; 
if( lt > 0 )
	{	/* put the larger number in ac2 */
	qmovz( ac2, ac3 );
	qmov( ac1, ac2 );
	qmov( ac3, ac1 );
	lt = -lt;
	}
SC = lt;
#if STICKY
lost = 0;
#endif
if( lt != 0 )
	{
	if( lt < -NBITS-1 )
		goto done;	/* answer same as larger addend */

#if STICKY
	lost = shift( ac1 ); /* shift the smaller number down */
#else
	shift( ac1 ); /* shift the smaller number down */
#endif
	}
else
	{
/* exponents were the same, so must compare mantissae */
	i = cmpm( ac1, ac2 );
	if( i == 0 )
		{	/* the numbers are identical */
		/* if different signs, result is zero */
		if( ac1[SIGNWORD] != ac2[SIGNWORD] )
			goto underf;
		/* if exponents zero, result is zero */
		if( ac1[E] == 0 )
			goto underf;
		/* if same sign, result is double */
		if( ac1[E] >= MAXEXP )
			{
			qclear(c);
			if( ac1[SIGNWORD] != 0 )
				qneg(c);
			goto overf;
			}
		ac2[E] += 1;
		goto done;
		}
	if( i > 0 )
		{	/* put the larger number in ac2 */
		qmovz( ac2, ac3 );
		qmov( ac1, ac2 );
		qmov( ac3, ac1 );
		}
	}

if( ac1[SIGNWORD] == ac2[SIGNWORD] )
	{
	addm( ac1, ac2 );
	subflg = 0;
	}
else
	{
	subm( ac1, ac2 );
	subflg = 1;
	}
if( normlz(ac2) )
	goto underf;

lt = (long )ac2[E] - SC;
if( lt > (long)MAXEXP )
	goto overf;
if( lt < 0 )
	{
/*	mtherr( "qadd", UNDERFLOW );*/
	goto underf;	
	}
ac2[E] = lt;

/* round off */
#if WORDSIZE == 32
i = ac2[LSGUARD];
#else
i = ac2[NQ] & 0xffff;
#endif
if( i & SIGNBIT )
	{
#if STICKY
	if( i == SIGNBIT )
		{
		if( lost == 0 )
			{
/* Critical case, round to even */
			if( (ac2[LSWORD] & 1) == 0 )
				goto done;
			}
		else
			{
			if( subflg != 0 )
				goto done;
			}
		}
#else
	if( subflg != 0 )
		goto done;
#endif
	
	qclear( ac1 );
	ac1[LSGUARD] = 0;
	ac1[LSWORD] = 1;
	addm( ac1, ac2 );
	normlz(ac2);
	if( SC )
		{
		lt = (long )ac2[E] - SC;
		if( lt > (long)MAXEXP )
			goto overf;
		ac2[E] = lt;
		}
	}
done:
qmov( ac2, c );
return 0;

underf:
qclear(c);
return 0;

overf:
mtherr( "qadd", OVERFLOW );
qinfin(c);
return 0;
}

/*
;	divide
;
;	QELT a[NQ], b[NQ], c[NQ];
;	qdiv( a, b, c );	c = b / a
*/
/* for Newton iteration version:
 * extern short qtwo[];
 * static short qt[NQ] = {0};
 * static short qu[NQ] = {0};
 */
int qdiv( a, b, c )
QELT *a, *b, *c;
{
long lt;

if( b[E] == 0 )
	{
divunderf:
	qclear(c);	/* numerator is zero */
	return 0;
	}

if( a[E] == 0 )
	{	/* divide by zero */
	qinfin(c);
	mtherr( "qdiv", SING );
	return 0;
	}
qmovz( b, ac3 );

/* Avoid exponent underflow in mdnorm.  */
lt = (long) ac3[E];
ac3[E] = 4;

divm( a, ac3 );

if( a[SIGNWORD] == b[SIGNWORD] )
	ac3[SIGNWORD] = 0;
else
	ac3[SIGNWORD] = -1;

/* calculate exponent */
lt = lt + (long )ac3[E] -4L - (long )a[E];
ac3[E] = lt;
ac3[LSGUARD] = 0;
normlz(ac3);

lt = lt - SC + EXPONE + 1;
if( lt > (long)MAXEXP )
	{
	qinfin(ac3);
	mtherr( "qdiv", OVERFLOW );
	}
else if( lt <= 0 )
	goto divunderf;	
else
	ac3[E] = lt;

qmov( ac3, c );
return 0;
}

/*
;	multiply
;
;	QELT a[NQ], b[NQ], c[NQ];
;	qmul( a, b, c );	c = b * a
*/
int qmul( a, b, c )
QELT *a, *b, *c;
{
QELT *p;
register int ctr;
long lt;

if( (a[E] == 0) || (b[E] == 0) )
	{
	qclear(c);
	return 0;
	}
/* detect multiplication by small integer a */
if( a[M+2] == 0 )
	{
	p = &a[M+3];
	for( ctr=M+3; ctr<NQ; ctr++ )
		{
		if( *p++ != 0 )
			goto nota;
		}
	qmov( b, ac3 );
	mulin( a, ac3 );
	lt = ((long)a[E] - (EXPONE-1)) + ((long )ac3[E] - (EXPONE - 1));
	goto mulcon;
	}

nota:
/* detect multiplication by small integer b */
if( b[M+2] == 0 )
	{
	p = &b[M+3];
	for( ctr=M+3; ctr<NQ; ctr++ )
		{
		if( *p++ != 0 )
			goto notb;
		}
	qmov( a, ac3 );
	mulin( b, ac3 );
	lt = ((long)b[E] - (EXPONE-1)) + ((long )ac3[E] - (EXPONE - 1));
	goto mulcon;
	}

notb:

qmov( a, ac3 );
mulm( b, ac3 );
lt = ((long)b[E] - (EXPONE-1)) + ((long )ac3[E] - (EXPONE - 1));

mulcon:
/* calculate sign of product */
if( b[SIGNWORD] == a[SIGNWORD] )
	ac3[SIGNWORD] = 0;
else
	ac3[SIGNWORD] = -1;

if( lt > (long)MAXEXP )
	goto overf;
ac3[E] = lt;
ac3[NQ] = 0;
if( normlz(ac3) )
	goto underf;
lt = lt - SC + EXPONE -1;
if( lt > (long)MAXEXP )
	goto overf;
if( lt <= 0 )
	goto underf;
ac3[E] = lt;
qmov( ac3, c );
return 0;

underf:
qclear(c);
return 0;

overf:
qinfin(c);
mtherr( "qmul", OVERFLOW );
return 0;
}




/* Multiply, a has at most WORDSIZE significant bits */

int qmuli( a, b, c )
QELT *a, *b, *c;
{
long lt;

if( (a[E] == 0) || (b[E] == 0) )
	{
	qclear(c);
	return 0;
	}

qmov( b, ac3 );
mulin( a, ac3 );

/* calculate sign of product */
if( b[SIGNWORD] == a[SIGNWORD] )
	ac3[SIGNWORD] = 0;
else
	ac3[SIGNWORD] = -1;

/* calculate exponent */
lt = ((long)ac3[E] - (EXPONE-1)) + ((long )a[E] - (EXPONE - 1));
if( lt > (long)MAXEXP )
	goto overf;
ac3[E] = lt;
ac3[NQ] = 0;
if( normlz(ac3) )
	goto underf;
lt = lt - SC + EXPONE - 1;
if( lt > (long)MAXEXP )
	goto overf;
if( lt < 0 )
	goto underf;
ac3[E] = lt;
qmov( ac3, c );
return 0;

underf:
qclear(c);
return 0;

overf:
qinfin(c);
mtherr( "qmuli", OVERFLOW );
return 0;
}




/*
;	Compare mantissas
;
;	QELT a[NQ], b[NQ];
;	cmpm( a, b );
;
;	for the mantissas:
;	returns	+1 if a > b
;		 0 if a == b
;		-1 if a < b
*/

int cmpm( a, b )
register QELT *a, *b;
{
int i;

a += M; /* skip up to mantissa area */
b += M;
for( i=0; i<OMG; i++ )
	{
	if( *a++ != *b++ )
		goto difrnt;
	}
return(0);

difrnt:
if( (unsigned int) *(--a) > (unsigned int) *(--b) )
	return(1);
else
	return(-1);
}

/*
;	shift mantissa
;
;	Shifts mantissa area up or down by the number of bits
;	given by the variable SC.
*/

int shift( x )
QELT *x;
{
QELT *p;
#if STICKY
int lost;
#endif

if( SC == 0 )
	return(0);

#if STICKY
lost = 0;
#endif
if( SC < 0 )
	{
	p = x + LSGUARD;
	SC = -SC;
	while( SC >= 16 )
		{
#if STICKY
		lost |= *p;
#endif
		shdn16(x);
		SC -= 16;
		}

	while( SC >= 8 )
		{
#if STICKY
		lost |= *p & 0xff;
#endif
		shdn8(x);
		SC -= 8;
		}

	while( SC > 0 )
		{
#if STICKY
		lost |= *p & 1;
#endif
		shdn1(x);
		SC -= 1;
		}
	}
else
	{
	while( SC >= 16 )
		{
		shup16(x);
		SC -= 16;
		}

	while( SC >= 8 )
		{
		shup8(x);
		SC -= 8;
		}

	while( SC > 0 )
		{
		shup1(x);
		SC -= 1;
		}
	}
#if STICKY
return( lost );
#else
return(0);
#endif
}

/*
;	normalize
;
; shift normalizes the mantissa area pointed to by R1
; shift count (up = positive) returned in SC
*/

int normlz(x)
QELT x[];
{
register QELT *p;

SC = 0;
p = &x[M];
if( *p != 0 )
	goto normdn;
++p;
if( *p & SIGNBIT )
	return(0);	/* already normalized */

while( *p == 0 )
	{
	shup16(x);
	SC += 16;
/* With guard word, there are NBITS+WORDSIZE bits available.
 * return true if all are zero.
 */
	if( SC > NBITS )
		return(1);
	}

/* see if high byte is zero */
#if WORDSIZE == 32
while( (*p & 0xff000000) == 0 )
#else
while( (*p & 0xff00) == 0 )
#endif
	{
	shup8(x);
	SC += 8;
	}

/* now shift 1 bit at a time */
while( (*p & SIGNBIT) == 0)
	{
	shup1(x);
	SC += 1;
/*
	if( SC > NBITS )
		{
		printf( "normlz error\n");
		return(0);
		}
*/
	}
return(0);

/* normalize by shifting down out of the high guard word
   of the mantissa */

normdn:

#if WORDSIZE == 32
while( *p & 0xffffff00 )
#else
while( *p & 0xff00 )
#endif
	{
	shdn8(x);
	SC -= 8;
	}
while( *p != 0 )
	{
	shdn1(x);
	SC -= 1;
/*
	if( SC < -NBITS )
		{
		printf( "low normlz error\n");
		return(0);
		}
*/
	}
return(0);
}

/*
; Clear out entire number, including sign and exponent, pointed
; to by x
;
; QELT x[];
; qclear( x );
*/
/* Moved to qfltb.c */
#if 0
int qclear( x )
register QELT *x;
{
register int i;

for( i=0; i<NQ; i++ )
	*x++ = 0;
return 0;
}
#endif

/*
; Fill entire number, including exponent and mantissa, with
; largest possible number.
*/

int qinfin(x)
QELT *x;
{
register int i;

++x;	/* skip over the sign */
*x++ = MAXEXP;
*x++ = 0;
for( i=0; i<OMG-1; i++ )
	*x++ = -1;
return 0;
}

/* normalization program */
int qnrmlz(x)
QELT *x;
{

qmovz( x, ac1 );
normlz( ac1 );	/* shift normalize the mantissa */
ac1[E] -= SC;	/* subtract the shift count from the exponent */
qmov( ac1, x );
return 0;
}


/*
; Convert IEEE single precision to e type
;	float d;
;	unsigned short x[N+2];
;	dtox( &d, x );
*/
int e24toq( pe, y )
unsigned short *pe;
QELT *y;
{
register unsigned short r;
register unsigned short *e;
QELT *p;
QELT yy[NQ+1];
int denorm;
unsigned long m;

e = pe;
denorm = 0;	/* flag if denormalized number */
qclear(yy);
yy[NQ] = 0;
#ifdef IBMPC
e += 1;
#endif
#ifdef DEC
e += 1;
#endif
r = *e;
if( r & 0x8000 )
	yy[SIGNWORD] = -1;
yy[M] = (r & 0x7f) | 0200;
r &= ~0x807f;	/* strip sign and 7 significand bits */
if( r == 0x7f80 )
	{
	qclear( y );
	qinfin( y );
	if( yy[SIGNWORD] )
		qneg(y);
	return 0;
	}
r >>= 7;
/* If zero exponent, then the significand is denormalized.
 * So take back the understood high significand bit. */ 
if( r == 0 )
	{
	denorm = 1;
	yy[M] &= ~0200;
	}
r += EXPONE - 0177;
yy[E] = r;
p = &yy[M+1];

#ifdef IBMPC
m = *(--e);
#endif
#ifdef DEC
m = *(--e);
#endif
#ifdef MIEEE
++e;
m = *e++;
#endif
#if WORDSIZE == 32
m <<= 16;
#endif
*p++ = m;
shdn8( yy );
if( denorm )
	{ /* if zero exponent, then normalize the significand */
	  normlz(yy);
	if( SC > NBITS )
		qclear(yy);
	else
		yy[E] -= (QELT) (SC-1);
	}
qmov( yy, y );
return 0;
}

/*
; e type to IEEE single precision
;	float d;
;	QELT x[NQ];
;	xtod( x, &d );
*/
int qtoe24( x, e )
QELT *x;
unsigned short *e;
{
long exp;
QELT xi[NQ+1];
QELT *p;
QELT k;
unsigned long f;
int j, sign;

qmov( x, xi );
xi[NQ] = 0;
p = &xi[0];
f = 0L;
if( *p++ )
	sign = 0x8000;	/* output sign bit */
else
	sign = 0;

exp = (long )*p++ - (EXPONE - 0177); /* adjust exponent for offsets */

if( exp <= 0 )
	{
	if( exp > -24 )
		{
		SC = exp - 1;
		shift( xi );
		exp = 0;
		}
	else
		{
		f = 0;
		goto fout;
		}
	}
/* round off to nearest or even */
#if WORDSIZE == 32
k = xi[M+1];
#else
k = xi[M+2];
#endif
if( (k & 0x80) != 0 )
	{
	if( (k & 0x0ff) == 0x80 )
		{
		if( (k & 0x100) != 0 )
			{
		/* check all less significant bits */
#if WORDSIZE == 32
			for( j=M+2; j<=NQ; j++ )
#else
			for( j=M+3; j<=NQ; j++ )
#endif
				{
				if( xi[j] != 0 )
					goto yesrnd;
				}
			}
		goto nornd;
		}
yesrnd:
	qclear( ac2 );
	ac2[NQ] = 0;
#if WORDSIZE == 32
	ac2[M+1] = 0x80;
#else
	ac2[M+2] = 0x80;
#endif
	addm( ac2, xi );
	if( xi[2] )
		{
		shdn1(xi);
		exp += 1;
		}
	if( (exp == 0) && (xi[M+1] & SIGNBIT) )
		exp += 1;
	}

nornd:

if( exp >= 255 )
	{	/* Saturate at largest number less than infinity. */
#ifdef INFINITY
	f = (unsigned long )0x7f800000;
#else
	f = (unsigned long )0x7f7fffff;
#endif
	goto fout;
	}

f = *p++; /* Skip over high guard word.  */
f = *p++;
#if WORDSIZE == 16
f = (f << 16) | *p;
#endif

f &= 0x7fffffff;
/* f += 0x80; */  /* Retire old DEC rounding.  */
f >>= 8;
f |= (exp << 23) & 0x7f800000;
fout:
if(sign)
  f |= 0x80000000L;
#ifdef IBMPC
*e++ = f;
*e = f >> 16;
#endif
#ifdef DEC
*e++ = f;
*e = f >> 16;
#endif
#ifdef MIEEE
*e++ = f >> 16;
*e = f;
#endif
return 0;
}


/*
; Convert IEEE double precision to Q type
;	double d;
;	QELT q[NQ];
;	etoq( &d, q );
*/

int etoq( e, y )
unsigned short *e;
QELT *y;
{
#ifdef DEC
dtoq(e,y);
#else
register int r;
register QELT *p;
QELT yy[NQ+1];
int denorm;


denorm = 0;	/* flag if denormalized number */
qclear(yy);
yy[NQ] = 0;

#ifdef MIEEE

#endif
#ifdef IBMPC
e += M+1;
#endif
#ifdef DEC
e += M+1;
#endif

/*
r = *e & 0x7fff;
if( r == 0 )
	return 0;
*/

r = *e;
yy[SIGNWORD] = 0;
if( r & 0x8000 )
	yy[SIGNWORD] = -1;

yy[M] = (r & 0x0f) | 0x10;
r &= ~0x800f;	/* strip sign and 4 mantissa bits */
r >>= 4;
/* If zero exponent, then the mantissa is denormalized.
 * So take back the understood high mantissa bit. */ 
if( r == 0 )
	{
	denorm = 1;
	yy[M] &= ~0x10;
	}
r += EXPONE - 01777;
yy[E] = r;
p = &yy[M+1];
#if WORDSIZE == 32
#ifdef MIEEE
++e;
*p = ((unsigned int) *e++) << 16;
*p++ |= *e++;
*p = ((unsigned int) *e++) << 16;
#endif
#ifdef IBMPC
*p = ((unsigned int) *(--e)) << 16;
*p++ |= *(--e);
*p = ((unsigned int) *(--e)) << 16;
#endif
#else /* 16-bit word size */
#ifdef MIEEE
++e;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
#endif
#ifdef IBMPC
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
#endif
#ifdef DEC
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
#endif
#endif /* 16-bit word size */
SC = -5;
shift(yy);
if( denorm )
	{ /* if zero exponent, then normalize the mantissa */
	if( normlz( yy ) )
		qclear(yy);
	else
		yy[E] -= SC-1;
	}
qmov( yy, y );
/* not DEC */
#endif
return 0;
}

/*
; Q type to IEEE double precision
;	double d;
;	QELT q[NQ];
;	qtoe( q, &d );
*/
int qtoe( x, e )
QELT *x;
unsigned short *e;
{
#ifdef DEC
qtod(x,e);
#else
int j, k;
long i;
register QELT *p;


#ifdef MIEEE

#endif
#ifdef IBMPC
e += M+1;
#endif
#ifdef DEC
e += M+1;
#endif


*e = 0;	/* output high order */
p = &ac1[SIGNWORD];
qmovz( x, ac1 );
if( *p++ != 0 )
	*e = 0x8000;	/* output sign bit */

if( normlz(ac1) )
	goto ozero;
*p -= SC;

i = (long) *p++ - (EXPONE - 1023);	/* adjust exponent for offsets */

/* Handle denormalized small numbers.  */
if( i <= 0 )
	{
	if( i > -53 )
		{
		SC = i - 1;
		shift( ac1 );
		i = 0;
		}
	else
		{
ozero:
#ifdef MIEEE
		++e;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
#endif
#ifdef IBMPC
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
#endif
#ifdef DEC
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
#endif
		return 0;
		}
	}

/* round off to nearest or even */
#if WORDSIZE == 32
k = ac1[M+2];
#else
k = ac1[M+4];
#endif
if( (k & 0x400) != 0 )
	{
	if( (k & 0x07ff) == 0x400 )
		{
		if( (k & 0x800) != 0 )
			{
		/* check all less significant bits */
#if WORDSIZE == 32
			for( j=M+3; j<=NQ; j++ )
#else
			for( j=M+5; j<=NQ; j++ )
#endif
				{
				if( ac1[j] != 0 )
					goto yesrnd;
				}
			}
		goto nornd;
		}
yesrnd:
	qclear( ac2 );
	ac2[NQ] = 0;
#if WORDSIZE == 32
	ac2[M+2] = 0x800;
#else
	ac2[M+4] = 0x800;
#endif
	addm( ac2, ac1 );
	if( ac1[2] )
		{
		shdn1(ac1);
		i += 1;
		}
	if( (i == 0) && (ac1[M+1] & SIGNBIT) )
		i += 1;
	}

nornd:

if( i > 2047 )
	{	/* Saturate at largest number less than infinity. */
	mtherr( "qtoe", OVERFLOW );
	*e |= 0x7fef;
#ifdef MIEEE
	++e;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
#endif
#ifdef IBMPC
	*(--e) = 0xffff;
	*(--e) = 0xffff;
	*(--e) = 0xffff;
#endif
#ifdef DEC
	*(--e) = 0xffff;
	*(--e) = 0xffff;
	*(--e) = 0xffff;
#endif
	return 0;
	}


i <<= 4;
SC = 5;
shift( ac1 );
i |= *p++ & 0x0f;	/* *p = ac1[M] */
*e |= i;	/* high order output already has sign bit set */
#if WORDSIZE == 32
#ifdef MIEEE
++e;
*e++ = *p >> 16;
*e++ = *p++;
*e++ = *p >> 16;
#endif
#ifdef IBMPC
*(--e) = *p >> 16;
*(--e) = *p++;
*(--e) = *p >> 16;
#endif
#else /* 16 bit words */
#ifdef MIEEE
++e;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
#endif
#ifdef IBMPC
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p;
#endif
#ifdef DEC
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p;
#endif
#endif /* 16 bit words */
/* not DEC */
#endif
return 0;
}


/*
; Convert 80-bit IEEE long double precision to Q type
;	long double d;
;	QELT q[N+2];
;	etoq( &d, q );
*/

int e64toq( e, y )
unsigned short *e;
QELT *y;
{
register int r;
register QELT *p;
QELT yy[NQ+1];
int denorm;


denorm = 0;	/* flag if denormalized number */
qclear(yy);
yy[NQ] = 0;

#ifdef MIEEE

#endif
#ifdef IBMPC
e += 4;
#endif
#ifdef DEC
e += 4;
#endif

r = *e;
yy[SIGNWORD] = 0;
if( r & 0x8000 )
	yy[SIGNWORD] = -1;

r &= 0x7fff;
/* If zero exponent, then the mantissa is denormalized. */
if( r == 0 )
	{
	denorm = 1;
	}
r += (EXPONE-0x3fff);
yy[E] = r;
p = &yy[M+1];
#if WORDSIZE== 32
#ifdef MIEEE
++e;
*p = ((unsigned int) *e++) << 16;
*p++ |= *e++;
*p = ((unsigned int) *e++) << 16;
*p++ |= *e++;
#endif
#ifdef IBMPC
*p = ((unsigned int) *(--e)) << 16;
*p++ |= *(--e);
*p = ((unsigned int) *(--e)) << 16;
*p++ |= *(--e);
#endif
#else
/* 16-bit wordsize */
#ifdef MIEEE
++e;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
#endif
#ifdef IBMPC
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
#endif
#ifdef DEC
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
#endif
#endif
if( denorm )
	{ /* if zero exponent, then normalize the mantissa */
#ifdef IBMPC
/* For Intel long double, shift denormal significand up 1
   -- but only if the top significand bit is zero.  */
	if( (yy[M+1] & SIGNBIT) == 0 )
		shup1( yy );
#endif
#ifdef DEC
/* For Intel long double, shift denormal significand up 1
   -- but only if the top significand bit is zero.  */
	if( (yy[M+1] & SIGNBIT) == 0 )
		shup1( yy );
#endif
	if( normlz( yy ) )
		qclear(yy);
	else
		yy[E] -= SC;
	}
qmov( yy, y );
return 0;
}

/*
; Q type to 80-bit IEEE long double precision
;	long double d;
;	QELT q[N+2];
;	qtoe( q, &d );
*/
int qtoe64( x, e )
QELT *x;
unsigned short *e;
{
QELT k;
int j;
long i;
register QELT *p;


#ifdef MIEEE

#endif

#ifdef IBMPC
e += 4;
#if 0
/* NOTE: if data type is 96 bits wide, clear the last word here. */
*(e+1)= 0;
#endif
#endif

#ifdef DEC
e += 4;
#endif


*e = 0;	/* output high order */
p = &ac1[0];
qmovz( x, p );
if( ac1[SIGNWORD] )
	*e = 0x8000;	/* output sign bit */
++p;
if( normlz(ac1) )
	goto o64zero;
*p -= SC;

i = (long) *p++ - (EXPONE-0x3fff);	/* adjust exponent for offsets */
if( i >= 0x7fff)
	{
#ifdef MIEEE
	*e++ |= 0x7ffe;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
#endif
#ifdef IBMPC
	*e-- |= 0x7ffe;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
#endif
#ifdef DEC
	*e-- |= 0x7ffe;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
#endif
	return 0;
	}
/* We can't handle denormal numbers if the q-type exponenent is 0. */
if( i <= 0 )
	{
	if( i > -65 )
		{
#if BIGENDIAN
		SC = i;
#else
		/* Intel 80x87 loses a bit.  */
		SC = i - 1;
#endif
		shift( ac1 );
		i = 0;
		}
	else
		{
o64zero:
#ifdef MIEEE
		++e;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
#endif
#ifdef IBMPC
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
#endif
#ifdef DEC
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
#endif
		return 0;
		}
	}

/* round off to nearest or even */
#if WORDSIZE == 32
k = ac1[M+3];
#else
k = ac1[M+5];
#endif
if( (k & SIGNBIT) != 0 )
	{
	if( (k & ((QELT) SIGNBIT - 1)) == 0 )
		{
		/* check all less significant bits */
#if WORDSIZE == 32
/* 0 1 2  3   4   5   6 */
/* S E M 1,2 3,4 5,6 7,8 */
		for( j=M+4; j<=NQ; j++ )
#else
/* 0 1 2 3 4 5 6 7 8 9 10 */
/* S E M 1 2 3 4 5 6 7 8 */
		for( j=M+6; j<=NQ; j++ )
#endif
			{
			if( ac1[j] != 0 )
				goto yesrnd;
			}
/* round to even */
#if WORDSIZE == 32
		if( (ac1[4] & 1) == 0 )
#else
		if( (ac1[6] & 1) == 0 )
#endif
		  goto nornd;
		}
yesrnd:
	qclear( ac2 );
	ac2[NQ] = 0;
#if WORDSIZE == 32
	ac2[M+3] = SIGNBIT;
#else
	ac2[M+5] = SIGNBIT;
#endif
	addm( ac2, ac1 );
	if( ac1[M] )
		{
		shdn1(ac1);
		i += 1;
		}
	if( (i == 0) && (ac1[3] & SIGNBIT) )
		i += 1;
	}

nornd:

*e |= i;	/* high order output already has sign bit set */
p = &ac1[M+1];
#if WORDSIZE == 32
#ifdef MIEEE
++e;
*e++ = *p >> 16;
*e++ = *p++;
*e++ = *p >> 16;
*e++ = *p++;
#endif
#ifdef IBMPC
*(--e) = *p >> 16;
*(--e) = *p++;
*(--e) = *p >> 16;
*(--e) = *p++;
#endif
#else
/* 16-bit words */
#ifdef MIEEE
++e;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
*e++ = *p;
#endif
#ifdef IBMPC
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p;
#endif
#ifdef DEC
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p;
#endif
#endif
return 0;
}

/*
; Convert 128-bit IEEE long double precision to Q type
;	long double d;
;	QELT q[NQ];
;	e113toq( &d, q );
*/

int e113toq( e, y )
unsigned short *e;
QELT *y;
{
register int r;
register QELT *p;
QELT yy[NQ+1];
int denorm;


denorm = 0;	/* flag if denormalized number */
qclear(yy);
yy[NQ] = 0;

#ifdef MIEEE

#endif
#ifdef IBMPC
e += 7;
#endif
#ifdef DEC
e += 7;
#endif

r = *e;
yy[SIGNWORD] = 0;
if( r & 0x8000 )
	yy[SIGNWORD] = -1;

r &= 0x7fff;
yy[M] = 1;
/* If zero exponent, then the mantissa is denormalized.
 * So take back the understood high mantissa bit. */ 
if( r == 0 )
	{
	denorm = 1;
	yy[M] = 0;
	}
r += (EXPONE-0x3fff);
yy[E] = r;
p = &yy[M+1];
#if WORDSIZE== 32
#ifdef MIEEE
++e;
*p = ((unsigned int) *e++) << 16;
*p++ |= *e++;
*p = ((unsigned int) *e++) << 16;
*p++ |= *e++;
*p = ((unsigned int) *e++) << 16;
*p++ |= *e++;
*p = ((unsigned int) *e++) << 16;
#endif
#ifdef IBMPC
*p = ((unsigned int) *(--e)) << 16;
*p++ |= *(--e);
*p = ((unsigned int) *(--e)) << 16;
*p++ |= *(--e);
*p = ((unsigned int) *(--e)) << 16;
*p++ |= *(--e);
*p = ((unsigned int) *(--e)) << 16;
#endif
#else /* 16-bit word size */
#ifdef MIEEE
++e;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
*p++ = *e++;
#endif
#ifdef IBMPC
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
#endif
#ifdef DEC
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
*p++ = *(--e);
#endif
#endif /* 16-bit word size */
SC = -1;
shift(yy);
if( denorm )
	{ /* if zero exponent, then normalize the mantissa */
	if( normlz( yy ) )
		qclear(yy);
	else
		yy[E] -= SC-1;
	}
qmov( yy, y );
return 0;
}

/*
; Q type to 128-bit IEEE long double precision
;	long double d;
;	QELT q[NQ];
;	qtoe113( q, &d );
*/
int qtoe113( x, e )
QELT *x;
unsigned short *e;
{
QELT k;
short i, j;
register QELT *p;


#ifdef MIEEE

#endif
#ifdef IBMPC
e += 7;
#endif
#ifdef DEC
e += 7;
#endif


*e = 0;	/* output high order */
p = &ac1[0];
qmovz( x, p );
if( ac1[SIGNWORD] )
	*e = 0x8000;	/* output sign bit */
++p;
normlz(ac1);
*p -= SC;

if ((unsigned int) *p > (unsigned int) 0xc000)
	{
#ifdef MIEEE
	*e++ |= 0x7ffe;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
	*e++ = 0xffff;
#endif
#ifdef IBMPC
	*e-- |= 0x7ffe;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
#endif
#ifdef DEC
	*e-- |= 0x7ffe;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
	*e-- = 0xffff;
#endif
	return 0;
	}

i = (long) *p++ - (EXPONE-0x3fff);	/* adjust exponent for offsets */

/* Denormal.  */
if( i <= 0 )
	{
	if( i > -113 )
		{
		SC = i - 1;
		shift( ac1 );
		i = 0;
		}
	else
		{
/*ozero:*/
#ifdef MIEEE
		++e;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
		*e++ = 0;
#endif
#ifdef IBMPC
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
#endif
#ifdef DEC
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
		*(--e) = 0;
#endif
		return 0;
		}
	}

/* round off to nearest or even */
#if WORDSIZE == 32
k = ac1[M+4];
#else
k = ac1[M+8];
#endif
if( (k & 0x4000) != (QELT) 0 )
	{
	if( (k & 0x7fff) == (QELT) 0x4000 )
		{
		/* check all less significant bits */
#if WORDSIZE == 32
/* 0 1 2  3   4   5   6 */
/* S E M 1,2 3,4 5,6 7,8 */
		for( j=M+5; j<=NQ; j++ )
#else
/* 0 1 2 3 4 5 6 7 8 9 10 */
/* S E M 1 2 3 4 5 6 7 8 */
		for( j=M+9; j<=NQ; j++ )
#endif
			{
			if( ac1[j] != 0 )
				goto yesrnd;
			}
/* round to even */
		if( (k & 0x8000) == (QELT) 0 )
		  goto nornd;
		}
yesrnd:
	qclear( ac2 );
	ac2[NQ] = 0;
#if WORDSIZE == 32
	ac2[M+4] = 0x8000;
#else
	ac2[M+8] = 0x8000;
#endif
	addm( ac2, ac1 );
	if( ac1[2] )
		{
		shdn1(ac1);
		i += 1;
		}
	if( (i == 0) && (ac1[3] & 0x8000) )
		i += 1;
	}

nornd:

SC = 1;
shift( ac1 );
*e |= i;	/* high order output already has sign bit set */
p = &ac1[M+1];
#if WORDSIZE == 32
#ifdef MIEEE
++e;
*e++ = *p >> 16;
*e++ = *p++;
*e++ = *p >> 16;
*e++ = *p++;
*e++ = *p >> 16;
*e++ = *p++;
*e++ = *p >> 16;
#endif
#ifdef IBMPC
*(--e) = *p >> 16;
*(--e) = *p++;
*(--e) = *p >> 16;
*(--e) = *p++;
*(--e) = *p >> 16;
*(--e) = *p++;
*(--e) = *p >> 16;
#endif
#else /* 16-bit words */
#ifdef MIEEE
++e;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
*e++ = *p++;
#endif
#ifdef IBMPC
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p;
#endif
#ifdef DEC
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p++;
*(--e) = *p;
#endif
#endif /* 16-bit words */
return 0;
}




/*						qtoasc.c	*/
/* Convert q type number to ASCII string */

/* Get values for powers of ten.  */
#include "qtens.h"

int qtoasc( q, string, ndigs )
QELT q[];
char *string;
int ndigs;
{
long digit;
QELT x[NTT], xt[NTT];
QELT *p, *r, *ten, *tenth;
QELT sign;
int i, k, expon;
char *s, *ss;

qmov( q, x );
sign = x[SIGNWORD];
x[SIGNWORD] = 0;
expon = 0;
ten = &qtens[NTEN][0];

i = qcmp( qone, x );
if( i == 0 )
	goto isone;
if( x[1] == 0 )
	{
	qclear( x );
	goto isone;
	}

if( i < 0 )
	{
	k = MAXNTEN;
	p = &qtens[0][0];
	qmov( qone, ac4 );
	qmov( x, xt );
	while( qcmp( ten, x ) <= 0 )
		{
		if( qcmp( p, xt ) <= 0 )
			{
			qdiv( p, xt, xt );
			qmul( p, ac4, ac4 );
			expon += k;
			}
		k >>= 1;
		if( k == 0 )
			break;
		p += NTT;
		}
	qdiv( ac4, x, x );
	}
else
	{
	k = MINNTEN;
	p = &qmtens[0][0];
	r = &qtens[0][0];
	tenth = &qmtens[NTEN][0];
	while( qcmp( tenth, x ) > 0 )
		{
		if( qcmp( p, x ) >= 0 )
			{
			qmul( r, x, x );
			expon += k;
			}
		k /= 2;
/* Prevent infinite loop due to arithmetic error: */
		if( k == 0 )
			break;
		p += NTT;
		r += NTT;
		}
	qmuli( ten, x, x );
	expon -= 1;
	}

isone:
qifrac( x, &digit, x );
/* The following check handles numbers very close to 10**(2**n)
 * when there is a mistake due to arithmetic error.
 */
if( digit >= 10 )
	{
	qdiv( ten, x, x );
	expon += 1;
	digit = 1;
	}
s = string;
if( sign != 0 )
	*s++ = '-';
else
	*s++ = ' ';
*s++ = (char )digit + 060;
*s++ = '.';
if( ndigs < 0 )
	ndigs = 0;
if( ndigs > NDEC )
	ndigs = NDEC;
for( k=0; k<ndigs; k++ )
	{
	qmuli( ten, x, x );
	qifrac( x, &digit, x );
	*s++ = (char )digit + 060;
	}

*s = '\0';	/* mark end of string */
ss = s;

/* round off the ASCII string */

qmuli( ten, x, x );
qifrac( x, &digit, x );
if( digit > 4 )
	{
/* Check for critical rounding case */
	if( digit == 5 )
		{
		if( qcmp( x, qzero ) != 0 )
			goto roun;	/* round to nearest */
		if( (*(s-1) & 1) == 0 )
			goto doexp;	/* round to even */
		}
roun:
	--s;
	k = *s & 0x7f;
/* Carry out to most significant digit? */
	if( k == '.' )
		{
		--s;
		k = *s & 0x7f;
		k += 1;
		*s = k;
/* Most significant digit rounds to 10? */
		if( k > '9' )
			{
			*s = '1';
			expon += 1;
			}
		goto doexp;
		}
/* Round up and carry out from less significant digits. */
	k += 1;
	*s = k;
	if( k > '9' )
		{
		*s = '0';
		goto roun;
		}
	}

doexp:

sprintf( ss, "E%d", expon );
return 0;
}


/* QELT a[NQ], b[NQ];
 * qcmp( a, b )
 *
 *  returns +1 if a > b
 *           0 if a == b
 *          -1 if a < b
 */

int qcmp( p, q )
register QELT *p, *q;
{
QELT r[NQ];
register int i;
int msign;

if( ( p[E] <= (QELT) NBITS)  && ( q[E] <= (QELT) NBITS ) )
	{
	qsub( q, p, r );
	if( r[E] == 0 )
		return( 0 );
	if( r[SIGNWORD] == 0 )
		return( 1 );
	return( -1 );
	}

if( p[SIGNWORD] != q[SIGNWORD] )
	{ /* the signs are different */
	if( p[SIGNWORD] == 0 )
		return( 1 );
	else
		return( -1 );
	}

/* both are the same sign */
if( *p == 0 )
	msign = 1;
else
	msign = -1;
	
i = NQ;
do
	{
	if( *p++ != *q++ )
		{
		goto diff;
		}
	}
while( --i > 0 );

return(0);	/* equality */



diff:

if( (unsigned int) *(--p) > (unsigned int) *(--q) )
	return( msign );		/* p is bigger */
else
	return( -msign );	/* p is littler */
}



/*
;								ASCTOQ
;		ASCTOQ.MAC		LATEST REV: 11 JAN 84
;					SLM, 3 JAN 78
;
;	Convert ASCII string to quadruple precision floating point
;
;		Numeric input is free field decimal number
;		with max of 15 digits with or without 
;		decimal point entered as ASCII from teletype.
;	Entering E after the number followed by a second
;	number causes the second number to be interpreted
;	as a power of 10 to be multiplied by the first number
;	(i.e., "scientific" notation).
;
;	Usage:
;		asctoq( string, q );
*/

int asctoq( s, y )
char *s;
QELT *y;
{
QELT yy[NQ+1], qt[NQ];
int esign, nsign, decflg, sgnflg, nexp, exp, prec, base, k;
long lexp;
QELT *p;


nsign = 0;
esign = 1;
decflg = 0;
sgnflg = 0;
nexp = 0;
exp = 0;
lexp = 0;
prec = 0;
qclear( yy );
yy[NQ] = 0;

base = 10;
if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
	{
	base = 16;
	s += 2;
	}

nxtcom:
if( (*s >= '0') && (*s <= '9') )
	k = *s - '0';
else if (*s >= 'a')
	k = 10 + *s - 'a';
else
	k = 10 + *s - 'A';

if( (k >= 0) && (k < base) )
{
if( (prec == 0) && (decflg == 0) && (*s == '0') )
	goto donchr;
if( prec < NDEC )
	{
	if( decflg )
		nexp += 1;	/* count digits after decimal point */
	if( base == 16 )
		{
		shup1( yy );
		shup1( yy );
		shup1( yy );
		shup1( yy );
		}
	else
		{
		shup1( yy );	/* multiply current number by 10 */
		qmovz( yy, ac2 );
		shup1( ac2 );
		shup1( ac2 );
		addm( ac2, yy );
		}
	qclear( ac2 );
	ac2[OMG+1] = k;

	/* Thanks to jacob navia <jacob.navia@wanadoo.fr> for pointing out
	   the need to initialize this for base 16. */
	ac2[NQ] = 0;

	addm( ac2, yy );
	}
prec += 1;
goto donchr;
}

switch( *s )
{
case ' ':
	break;
case 'E':
case 'e':
case 'P':
case 'p':
	goto expnt;
case '.':	/* decimal point */
	if( decflg )
		goto error;
	++decflg;
	break;
case '-':
	nsign = -1;
case '+':
	if( sgnflg )
		goto error;
	++sgnflg;
	break;
case '\0':
/* For Microware OS-9 operating system: */
#ifndef OSK
case '\n':
#endif
case '\r':
	goto daldone;
default:
error:
	printf( "asctoq conversion error\n" );
	qclear(y);
	return 0;
}
donchr:
++s;
goto nxtcom;


/*		EXPONENT INTERPRETATION */
expnt:

/* 0.0eXXX is zero, regardless of XXX.  Check for the 0.0. */
for( exp = 0; exp < NQ + 1; exp++ )
	{
	if( yy[exp] != 0 )
		goto read_expnt;
	}
goto zero;
  
read_expnt:
exp = 0;
++s;
/* check for + or - */
if( *s == '-' )
	{
	esign = -1;
	++s;
	}
if( *s == '+' )
	++s;
while( (*s >= '0') && (*s <= '9') )
	{
	/* Check for oversize decimal exponent.  */
	if( exp >= 3276 || exp < 0 )
		{
		if( esign < 0 )
			goto zero;
		else
			goto infinite;
		}
	exp *= 10;
	exp += *s++ - '0';
	}
if( esign < 0 )
	exp = -exp;

daldone:
if (base == 16)
	{
	/* Base 16 hexadecimal floating constant.  */
	normlz (yy);
	if (SC > NBITS)
		{
		qclear (yy);
		goto aexit;
		}
	/* Adjust the exponent.  NEXP is the number of hex digits,
	EXP is a power of 2.  */
	lexp = (EXPONE - 1 + NBITS) - SC + yy[E] + exp - 4 * nexp;
	if (lexp > MAXEXP)
		goto infinite;
	if (lexp < 0)
		goto zero;
	yy[E] = lexp;
	yy[SIGNWORD] = nsign;
	qmov( yy, y );
	goto aexit;
	}

nexp = exp - nexp;

if( normlz(yy) )
	{
	qclear(y);
	return 0;
	}

yy[E] = EXPONE - 1 + NBITS - SC;
qmov( yy, y );
y[SIGNWORD] = nsign;

/* Escape from excessively large exponent.  */
if( nexp >= 2 * MAXNTEN )
	{
infinite:
	y[SIGNWORD] = nsign;
	qinfin(y);
	mtherr( "asctoq", OVERFLOW );
	goto aexit;
	}
if( nexp <= -2 * MAXNTEN )
	{
zero:
	qclear(y);
	mtherr( "asctoq", UNDERFLOW );
	goto aexit;
	}

/* multiply or divide by 10**NEXP */
if( nexp == 0 )
	goto aexit;
esign = 0;
if( nexp < 0 )
	{
	esign = -1;
	nexp = -nexp;
	}

p = &qtens[NTEN][0];
exp = 1;
qmov( qone, qt );

do
	{
	if( exp & nexp )
		qmul( p, qt, qt );
	exp <<= 1;
	p -= NQ;
	}
while( exp <= MAXNTEN );

if( esign < 0 )
	qdiv( qt, y, y );
else
	qmul( qt, y, y );
aexit:
return 0;
}
