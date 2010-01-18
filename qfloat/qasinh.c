/*							qasinh.c
 *
 *	Inverse hyperbolic sine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qasinh( x, y );
 * QELT *x, *y;
 *
 * qasinh( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns inverse hyperbolic sine of argument.
 *
 *     asinh(x) = log( x + sqrt(1 + x*x) ).
 *
 * For very large x, asinh(x) = log x  +  log 2.
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/


#include "qhead.h"

extern QELT qone[], qlog2[];

int qasinh( xxx, y )
QELT xxx[], y[];
{
QELT x[NQ], a[NQ], b[NQ], s[NQ], xx[NQ], z[NQ];
long j, n;
int i, sign;

sign = xxx[0];
qmov( xxx, x );
x[0] = 0;
if( x[1] < (QELT) (EXPONE - 4) )
	goto cfrac;
	
if( ((long)x[1] - (long)EXPONE) >= ((long)MAXEXP - (long)EXPONE)/2 )
	{
	qlog( x, y );
	qadd( qlog2, y, y );
	if( sign )
		qneg( y );
	return 0;
	}
qmul( x, x, a );	/* sqrt( x**2 + 1 )	*/
qadd( qone, a, a );
qsqrt( a, a );
qadd( x, a, a );
qlog( a, y );		/* log( x + sqrt(...)	*/
if( sign )
	qneg( y );
return 0;


cfrac:


qmul( x, x, z );	/* z = x * x */
qadd( qone, z, a );	/* a = sqrt( z + 1.0 ) */
qsqrt( a, a );

i = NBITS/6;     /* 8  */
n = 2*i+1;     /* 17 for x < .05, convergence to 1e-17 */
qclear( s );

 do
	{
	j = i * (i-1);
	ltoq( &j, b );	/* b = j * z */
	qmul( b, z, b );
	ltoq( &n, xx );     /* s = b/(s+n) */
	qadd( s, xx, xx );
	qdiv( xx, b, s );
	n -= 2;
	ltoq( &n, xx );     /* s = b/(s+n) */
	qadd( s, xx, xx );
	qdiv( xx, b, s );
	n -= 2;
	i -= 2;
	}
while( n > 1 );

/* a * x / (1+s) */
qadd( qone, s, s );
qdiv( s, a, a );
qmul( a, x, y );
if( sign )
	qneg( y );
return 0;
}
