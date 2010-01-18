/*							qacosh.c
 *
 *	Inverse hyperbolic cosine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qacosh( x, y )
 * QELT *x, *y;
 *
 * qacosh( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * acosh(x)  =  log( x + sqrt( (x-1)(x+1) ).
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "qhead.h"

extern QELT qone[], qlog2[];

int qacosh( x, y )
QELT x[], y[];
{
QELT a[NQ];

if( qcmp( x, qone ) < 0 )
	{
	mtherr( "qacosh", DOMAIN );
	qclear(y);
	return 0;
	}
if( x[1] > (QELT) (EXPONE + NBITS))
	{
	qlog( x, y );
	qadd( qlog2, y, y );
	return 0;
	}
qmul( x, x, a );	/* sqrt( x**2 - 1 )	*/
qsub( qone, a, a );
qsqrt( a, a );
qadd( x, a, a );
qlog( a, y );		/* log( x + sqrt(...)	*/
return 0;
}
