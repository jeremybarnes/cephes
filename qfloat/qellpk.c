/*							qellpk.c
 *
 *	Complete elliptic integral of the first kind
 *
 *
 *
 * SYNOPSIS:
 *
 * int qellpk(x, y);
 * QELT *x, *y;
 *
 * qellpk(x, y);
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *
 *
 *            pi/2
 *             -
 *            | |
 *            |           dt
 * K(m)  =    |    ------------------
 *            |                   2
 *          | |    sqrt( 1 - m sin t )
 *           -
 *            0
 *
 * where m = 1 - m1, using the arithmetic-geometric mean method.
 *
 * The argument m1 is used rather than m so that the logarithmic
 * singularity at m = 1 will be shifted to the origin; this
 * preserves maximum accuracy.
 *
 * K(0) = pi/2.
 *
 * ACCURACY:
 *
 * Truncated at NBITS/2.
 *
 */

/*
Cephes Math Library, Release 2.0:  April, 1987
Copyright 1984, 1987 by Stephen L. Moshier
*/


/*	qellpk.c		*/

#include "qhead.h"
extern QELT qone[], qzero[], qpi[];

int qellpk(x,y)
QELT *x, *y;
{
QELT a[NQ], b[NQ], c[NQ], temp[NQ];

if( qcmp( x, qone ) > 0 || qcmp( x, qzero ) <= 0 )
	{
	mtherr( "qellpk", DOMAIN );
	return(0);
	}
qsub( x, qone, temp );
qsqrt( temp, c );		/* c = sqrt(x)		*/
/*qsub( temp, qone, b );*/	/* b = sqrt( 1 - x )	*/
qsqrt( x, b );
qmov( qone, a );	/* a = 1	*/

while( ((int) a[1] - (int) c[1]) < (NBITS/2) )
	{
	qsub( b, a, c );	/* c = (a - b)/2.0	*/
	c[1] -= 1;
	qmul( a, b, temp );	/* temp = sqrt( a * b )	*/
	qsqrt( temp, temp );
	qadd( a, b, a );	/* a = (a + b)/2.0	*/
	a[1] -= 1;
	qmov( temp, b );	/* b = temp		*/
	}

qmov( qpi, temp );	/* get pi/2	*/
temp[1] -= 1;
qdiv( a, temp, y );
return(0);
}
