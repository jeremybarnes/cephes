/*							qerf.c
 *
 *	Error function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qerf( x, y );
 * QELT *x, *y;
 *
 * qerf( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * The integral is
 *
 *                           x 
 *                            -
 *                 2         | |          2
 *   erf(x)  =  --------     |    exp( - t  ) dt.
 *              sqrt(pi)   | |
 *                          -
 *                           0
 *
 *
 */

/*
Cephes Math Library Release 2.2:  June, 1992
Copyright 1984, 1987, 1988, 1992 by Stephen L. Moshier
*/

/*							qerf.c	*/
/* error function check routine */

/* loop counts adjusted for full convergence to 9 word mantissa */

#include "qhead.h"
extern QELT oneopi[], qsqrt2[], qone[];


int qerf( x, y )
QELT *x, *y;
{
QELT xx[NQ], z[NQ], a[NQ], b[NQ];
long i, j;
int sign;

qmov( x, xx );
qmul( xx, xx, z );	/* square of y				*/

sign = xx[0];

if( xx[1]< (QELT) (EXPONE + 2) )
{
i = 54; /* 27;*/
if( xx[1] < EXPONE )
        i = 20; /*10;*/
j = 4 * i + 1;
ltoq( &j, a);		/*  2 * i  +  1			*/
qmov( a, y );

/* continued fraction expansion */
while( j > 1 )
	{
	qsub( qone, a, a );
	qmul( a, z, b );
	qdiv( y, b, y );
	qsub( qone, a, a );
	qadd( a, y, y );

	qsub( qone, a, a );
	qmul( a, z, b );
	qdiv( y, b, y );
	qsub( qone, a, a );
	qsub( y, a, y );
	j -= 4;
	}
qdiv( y, xx, y );

qneg(z);		/* exp( -xx**2 )	*/
qexp( z, a );
qmul( a, y, y );
qmul( oneopi, y, y );
}

/*							qerf.c 2	*/

/* alternate continued fraction expansion for large x */
else
{
j = 172;
if( xx[1] > (QELT) (EXPONE + 1) )
        j = 120; /*76;*/
ltoq( &j, a );
if( sign != 0 )
	xx[0] = 0;
qmov( xx, y );
while( j > 0 )
	{
	qdiv( y, a, y );
	*(y+1) -= 1;
	qadd( xx, y, y );
	qsub( qone, a, a );
	j -= 1;
	}
qdiv( y, qone, y );

qneg(z);		/* exp( -xx**2 )	*/
qexp( z, a );
qmul( a, y, y );
qmul( oneopi, y, y );
y[1] -= 1;
qsub( y, qone, y );
if( sign != 0 )
	qneg(y);
}
return 0;
}
