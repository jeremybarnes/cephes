/*							qsqrt.c
 *      Square root check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qsqrt( x, y );
 * QELT *x, *y;
 *
 * qsqrt( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the square root of x.
 *
 * Range reduction involves isolating the power of two of the
 * argument and using a polynomial approximation to obtain
 * a rough value for the square root.  Then Heron's iteration
 * is used to converge to an accurate value.
 *
 */

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1988, 1996 by Stephen L. Moshier
*/

/* full precision for 9 word mantissa */
#include "qhead.h"
extern QELT qsqrt2[];

/* constants for first approximation polynomial */
#if WORDSIZE == 32

static QELT qsq2[NQ] =
{-1,EXPONE-3,0,0xd14fc42f,0xe79ba800,0,0,0};

static QELT qsq1[NQ] =
{0,EXPONE-1,0,0xe3e3c2ae,0x4c146700,0,0,0};

static QELT qsq0[NQ] =
{0,EXPONE-2,0,0xa08bdc7d,0xd5ffe300,0,0,0};
#if NQ < 15
#define NNEWTON 8
#else
#define NNEWTON 10
#endif

#else

static short qsq2[NQ] =
{-1,EXPONE-3,0,0150517,0142057,0163633,0124000,0,0,0,0,0,};

static short qsq1[NQ] =
{0,EXPONE-1,0,0161743,0141256,046024,063400,0,0,0,0,0,};

static short qsq0[NQ] =
{0,EXPONE-2,0,0120213,0156175,0152777,0161400,0,0,0,0,0,};
#define NNEWTON 8
#endif

extern QELT qzero[];

/* ??? For some reason alpha compilers may miscompile this file
   when these variables are made local.  */
static QELT a[NQ], b[NQ];


int qsqrt( x, y )
QELT *x, *y;
{
int i, m;

if( x[0] != 0 )
	{
	if( qcmp( qzero, x ) == 0 )
		{
		qclear(y);
		qneg(y);
		}
	else
		{
		mtherr( "qsqrt", DOMAIN );
		qclear(y);
		}
	return 0;
	}
if( x[1] == 0 )
	{
	qclear(y);
	return 0;
	}

qmov( x, a );
m = a[1];	/* save the exponent		*/
a[1] = EXPONE - 1;	/* change range to 0.5, 1 */

/* b = ( a * qsq2 + qsq1) * a + qsq0		*/
qmul( a, qsq2, b );	/* b = a * qsq2		*/
qadd( qsq1, b, b );	/* b += qsq1		*/
qmul( a, b, b );	/* b *= a;		*/
qadd( qsq0, b, b );	/* b += qsq0;		*/

/* divide exponent by 2 */
m -= EXPONE - 1;
b[1] = (m / 2) + (EXPONE - 1);


/* multiply by sqrt(2) if exponent odd */
if( (m & 1) != 0 )
	qmul( b, qsqrt2, b );


/* Newton iterations */

for( i=0; i<8; i++ )
	{
	qdiv( b, x, a );
	qadd( a, b, b );
	b[1] -= 1;
	}

qmov( b, y );
return 0;
}
