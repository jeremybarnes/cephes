/*							qtanh.c
 *      Hyperbolic tangent check routine
 *
 *
 *
 *
 * SYNOPSIS:
 *
 * int qtanh( x, y );
 * QELT *x, *y;
 *
 * qtanh( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * For x >= 1 the program uses the definition
 *
 *             exp(x) - exp(-x)
 * tanh(x)  =  ----------------
 *             exp(x) + exp(-x)
 *
 *
 * For x < 1 the method is a continued fraction
 *
 *                   2   2   2
 *              x   x   x   x
 * tanh(x)  =  --- --- --- --- ...
 *              1+  3+  5+  7+
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995, 1996 by Stephen L. Moshier
*/


#include "qhead.h"

extern QELT qone[], qtwo[];

int qtanh( x, y )
QELT *x, *y;
{
QELT e[NQ], r[NQ], j[NQ], xx[NQ], m2[NQ];
int i, n, sign;
long lj;

sign = x[0];
qmov( x, r );
r[0] = 0;
if( qcmp(r, qone) >= 0 )
	{
/* This subroutine is used by the exponential function routine.
 * tanh(x) = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
 * Note qexp() calls qtanh, but with an argument less than (1 + log 2)/2.
 */
	qexp( r, e );
	qdiv( e, qone, r );
	qsub( r, e, xx );
	qadd( r, e, j );
	qdiv( j, xx, y );
	goto done;
	}

qmov( qtwo, m2 );
qneg( m2 );

/* Adjust loop count for convergence to working precision.  */
n = NBITS/9 + 1; /*10;*/
lj = 2 * n + 1;
ltoq( &lj, j );

qmov( j, e );
qmul( x, x, xx );

/* continued fraction */

for( i=0; i<n; i++)
	{
	qdiv( e, xx, r );
	qadd( m2, j, j );
	qadd( r, j, e );
	}

qdiv( e, x, y );

done:
if( sign != 0 )
	y[0] = -1;
return 0;
}
