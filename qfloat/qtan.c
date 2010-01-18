/*							qtan.c
 *      Circular tangent check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qtan( x, y );
 * QELT *x, *y;
 *
 * qtan( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Domain of approximation is reduced by the transformation
 *
 * x -> x - pi floor((x + pi/2)/pi)
 *
 *
 * then tan(x) is the continued fraction
 *
 *                  2   2   2
 *             x   x   x   x
 * tan(x)  =  --- --- --- --- ...
 *            1 - 3 - 5 - 7 -
 *
 */
/*							qcot
 *
 *	Circular cotangent check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qcot( x, y );
 * QELT *x, *y;
 *
 * qcot( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * cot (x) = 1 / tan (x).
 *
 */

#include "qhead.h"
extern QELT qone[], qtwo[], qpi[];

int qtan( x, y )
QELT *x, *y;
{
QELT xxx[NQ], e[NQ], r[NQ], j[NQ], xx[NQ], m2[NQ];
int i, sign, n;
long li;

sign = x[0];
qmov( x, xxx );
xxx[0] = 0;
/*	range reduction to +-pi/2		*/
qmov( qpi, e );		/* make pi/2	*/
e[1] -= 1;
qadd( xxx, e, e );	/*  x - pi * int( (x + pi/2)/pi  )	*/
qdiv( qpi, e, e );
qfloor( e, e );
qmul( qpi, e, e );
qsub( e, xxx, xxx );

qmov( qtwo, m2 );
qneg( m2 );

/* loop count = 17 for convergence to 9*16 bit mantissa if x < 1 */
/* accuracy better than double precision for x beyond 2 */
n = NBITS/8;	/* 17 */
li = 2*n + 1;
ltoq( &li, j );
qmov( j, e );

qmul( xxx, xxx, xx );
qneg( xx );

/* continued fraction expansion */
for( i=0; i<n; i++)
	{
	qdiv( e, xx, r );
	qadd( m2, j, j );
	qadd( r, j, e );
	}

qdiv( e, xxx, y );
if( sign != 0 )
	y[0] = ~y[0];
return 0;
}



int qcot( x, y )
QELT x[], y[];
{
QELT z[NQ];

qtan( x, z );
qdiv( z, qone, y );
return 0;
}
