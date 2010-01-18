/*							qndtr.c
 *
 *	Normal distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qndtr( x, y );
 * QELT *x, *y;
 *
 * qndtr( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the area under the Gaussian probability density
 * function, integrated from minus infinity to x:
 *
 *                            x
 *                             -
 *                   1        | |          2
 *    ndtr(x)  = ---------    |    exp( - t /2 ) dt
 *               sqrt(2pi)  | |
 *                           -
 *                          -inf.
 *
 *             =  ( 1 + erf(z) ) / 2
 *             =  erfc(z) / 2
 *
 * where z = x/sqrt(2).
 *
 */

#include "qhead.h"

/* loop counts adjusted for full convergence to 9 word mantissa */

extern QELT oneopi[], qsqrt2[], qone[];

int qndtr( x, y )
QELT *x, *y;
{
QELT xx[NQ], z[NQ], a[NQ], b[NQ];
long i, j;
int sign;

qdiv( qsqrt2, x, xx );
qmul( xx, xx, z );	/* square of y				*/

sign = xx[0];
xx[0] = 0;

if( xx[1] < (QELT) (EXPONE + 2) ) /* xx < 4 */
{

i = NBITS * 3 / 8;  /*54;*/

/*
 * if( xx[1] < EXPONE )
 *	i = NBITS/6;
 */

j = 4 * i + 1;
ltoq( &j, a);		/*  2 * i  +  1			*/
qmov( a, y );

/* continued fraction expansion
 * Hart et al., p. 137; AMS55 #26.2.15
 *                    1                  x    x**2   2 x**2   3 x**2
 * P(x) - 0.5  =  --------- exp(-x**2)  ---  ------  ------   ------ ...
 *                sqrt(2pi)             1 -    3 +     5 -      7 +
 */
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
/* *(y+1) += 1; */
qneg(z);		/* exp( -xx**2 )	*/
qexp( z, a );
qmul( a, y, y );
qmul( oneopi, y, y );

if( sign != 0 )
	qneg(y);

qadd( y, qone, y );
*(y+1) -= 1;

}


/* alternate continued fraction expansion for large x
 * AMS55 #7.1.14
 *
 *              inf.
 *                -
 *               | |                1   1/2  2/2  3/2  4/2
 * 2 exp(x**2)   |   exp(-t**2) =  ---  ---  ---  ---  ---  ...
 *             | |                 x +  x +  x +  x +  x +
 *              -
 *               x
 */
else
{

j = NBITS * 6 / 5;  /* 172 */

/*
 *if( xx[1] > (EXPONE + 1) )
 *	j = NBITS;
 */

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

*(y+1) -= 2;
if( sign == 0 )
	qsub( y, qone, y );
}

return 0;
}
