/*							qatanh.c
 *
 *	Inverse hyperbolic tangent
 *
 *
 *
 * SYNOPSIS:
 *
 * int qatanh( x, y );
 * QELT x[], y[];
 *
 * qatanh( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns inverse hyperbolic tangent of argument.
 *
 *        atanh(x) = 0.5 * log( (1+x)/(1-x) ).
 *
 * For very small x, the first few terms of the Taylor series
 * are summed.
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright (C) 1987, 1995 by Stephen L. Moshier
*/

#include "qhead.h"
extern QELT qone[], qtwo[], qthree[];

int qatanh( x, y )
QELT x[], y[];
{
QELT a[NQ], b[NQ], c[NQ];
QELT sign;

qmov( x, a );
sign = a[0];
a[0] = 0;
if( qcmp( a, qone ) >= 0 )
	{
	mtherr( "qatanh", DOMAIN );
	qinfin(y);
	y[0] = sign;
	return 0;
	}
if( ((long)EXPONE - (long)a[1]) >= NBITS/4 )
	{
	/* x + x^3 / 3 + x^5 / 5  */
	qmul( a, a, b );
	qmul( a, b, y ); /* x^3 */

	qmul( b, y, b ); /* x^5 */
	qdiv( qthree, y, y );

	qadd( qthree, qtwo, c );
	qdiv( c, b, b );
	qadd( b, y, y );
	qadd( a, y, y );
	y[0] = sign;
	return 0;
	}
/* 0.5 * log((1+x)/(1-x)) */
qmov( qone, a );
qadd( x, a, a );
qmov( qone, b );
qsub( x, b, b );
qdiv( b, a, a );
qlog( a, y );
y[1] -= 1;
return 0;
}
