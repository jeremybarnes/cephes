/*							qlog1.c
 *
 *	Relative error logarithm
 *
 *
 *
 * SYNOPSIS:
 *
 * int qlog1( x, y );
 * QELT *x, *y;
 *
 * qlog1( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the base e (2.718...) logarithm of 1 + x.
 *
 * For small x, this continued fraction is used:
 * 
 *     1+z
 * w = ---
 *     1-z
 *
 *               2   2   2
 *         2z   z  4z  9z
 * ln(w) = --- --- --- --- ...
 *         1 - 3 - 5 - 7 -
 *
 * after setting z = x/(x+2).
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

/*						qlog1.c	*/
/* natural logarithm of 1 + x */

#include "qhead.h"

extern QELT qone[], qtwo[], qlog2[], qsqrt2[];
static QELT xx[NQ] = {0};
static QELT z[NQ] = {0};
static QELT a[NQ] = {0};
static QELT b[NQ] = {0};
static QELT qj[NQ] = {0};

int qlog1( x, y )
QELT *x, *y;
{
long i, j, nsq;

qmov(x, xx );
qmov(qone, z);
qadd(xx, z, z);  /* 1 + x */
qmov(qsqrt2, b);
if(qcmp(z, b) > 0)
  goto uselog;
b[1] -= 1;
if(qcmp(z,b) >= 0)
  goto nouselog;
uselog:
qlog(z, y);
return(0);

nouselog:
qmov( qone, b );
b[1] += 1;
qadd( b, xx, b );
qdiv( b, xx, y );	/* store x/(x+2) in y */

qmul( y, y, z );

i = NBITS*5/18;		/* 40 for 144 bits */
j = 2 * i + 1;
ltoq( &j, b );		/* 2 * i + 1 */
qmov( b, qj );

while( j > 1 )
	{
	nsq = i * i;
	ltoq( &nsq, a );	/* i**2			*/
	qmuli( a, z, a );	/* i**2 * x**2		*/
	qdiv( b, a, b );	/* i**2 * x**2 / (2*i+1) */
	j -= 2;
	i -= 1;
/*	ltoq( &j, a );	*/
	qsub( qtwo, qj, qj );	/* 2 * i + 1		*/
	qsub( b, qj, b );
	}

qdiv( b, y, y );
if (y[1] != 0)
	y[1] += 1;
return 0;
}
