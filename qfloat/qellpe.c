/*							qellpe.c
 *
 *	Complete elliptic integral of the second kind
 *
 *
 *
 * SYNOPSIS:
 *
 * int qellpe(x, y);
 * QELT *x, *y;
 *
 * qellpe(x, y);
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *
 *            pi/2
 *             -
 *            | |                 2
 * E(m)  =    |    sqrt( 1 - m sin t ) dt
 *          | |    
 *           -
 *            0
 *
 * Where m = 1 - m1, using the arithmetic-geometric mean method.
 *
 *
 * ACCURACY:
 *
 * Method terminates at NBITS/2.
 *
 */

/*
Cephes Math Library, Release 2.1:  February, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
*/

/*	qellpe.c		*/
/*	Complete elliptic integral of second kind	*/
/*	Argument is m	*/

#include "qhead.h"
extern QELT qone[], qpi[];

int qellpe(x,y)
QELT *x, *y;
{
QELT a[NQ], b[NQ], c[NQ], d[NQ], e[NQ], temp[NQ];

if( qcmp( x, qone ) > 0 || x[0] != 0 )
	{
	mtherr( "qellpe", DOMAIN );
	return(0);
	}
if( x[1] < (QELT) (EXPONE - 2*NBITS) )
	{
	qmov( qone, y );
	return(0);
	}
qsub( x, qone, temp );
if( temp[1] < (QELT) (EXPONE - 129) )
	{
	qmov( qpi, y );
	y[1] -= 1;
	return(0);
	}
qmov( temp, e );
qsqrt( temp, c );		/* c = sqrt(x)		*/
/*qsub( temp, qone, b );*/	/* b = sqrt( 1 - x )	*/
qsqrt( x, b );
qmov( qone, a );	/* a = 1	*/
qmov( qone, d );

while( ((int) a[1] - (int) c[1]) < (NBITS/2) )
	{
	qsub( b, a, c );	/* c = (a - b)/2.0	*/
	c[1] -= 1;
	qmul( a, b, temp );	/* temp = sqrt( a * b )	*/
	qsqrt( temp, temp );
	qadd( a, b, a );	/* a = (a + b)/2.0	*/
	a[1] -= 1;
	qmov( temp, b );	/* b = temp		*/
	qadd( d, d, d );	/* d += d 		*/
	qmul( c, c, temp );	/* e += c * c * d	*/
	qmul( temp, d, temp );
	qadd( e, temp, e );
	}

qmov( qpi, temp );	/* get pi/2	*/
temp[1] -= 1;
qdiv( a, temp, temp );	/* Integral of first kind */
qmov( qone, a );	/* make a 2	*/
a[1] += 1;
qsub( e, a, b );	/* ( 2 - e )/2	*/
b[1] -= 1;
qmul( temp, b, y );
return(0);
}
