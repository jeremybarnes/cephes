/*							qsin.c
 *      Circular sine check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qsin( x, y );
 * QELT *x, *y;
 *
 * qsin( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Range reduction is into intervals of pi/2.
 * Then
 *
 *               3    5    7
 *              z    z    z
 * sin(z) = z - -- + -- - -- + ...
 *              3!   5!   7!
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1985, 1995, 1996 by Stephen L. Moshier
*/

#include "qhead.h"
extern QELT qone[], qpi[];

int qsin( x, y )
QELT *x, *y;
{
QELT a[NQ], b[NQ], z[NQ], xx[NQ];
int sign;
long mod;

if( x[0] != 0 )
	sign = -1;
else
	sign = 1;
qmov( x, xx );
xx[0] = 0;
/* range reduction to [0, pi/2]	*/
qmov( qpi, z );
z[1] -= 1;
qdiv( z, xx, a );
qfloor( a, xx );
/* b = xx - 8 * floor(xx/8) */
if( xx[1] >= 3)
	{
	xx[1] -= 3;
	qfloor( xx, b );
	b[1] += 3;
	xx[1] += 3;
	}
else
	qclear(b);

qsub( b, xx, b );
qifrac( b, &mod, b );

qsub( xx, a, b );
qmul( b, z, xx );

mod &= 3;
if( mod > 1 )
	sign = -sign;
if( mod & 1 )
	qsub( xx, z, xx );	/* xx = 1 - xx */

qmul( xx, xx, z );
qneg( z );

qmov( qone, a );
qmov( qone, b );
qmov( qone, y );

/* power series */
do
	{
	qadd( qone, a, a );	/* a += 1	*/
	qdiv( a, b, b );	/* b /= a	*/
	qadd( qone, a, a );	/* a += 1	*/
	qdiv( a, b, b );	/* b /= a	*/
	qmul( z, b, b );	/* b *= z	*/
	qadd( b, y, y );	/* y += b	*/
	}
while( (int)(y[1] - b[1]) < NBITS );

qmul( xx, y, y );
if( sign < 0 )
	y[0] = -1;
return 0;
}


/* sin(x) - x */

int qsinmx3( x, y )
QELT *x, *y;
{
QELT z[NQ], b[NQ], n[NQ];

qmul( x, x, z );
qneg( z );

qmov( qone, n );
qclear( y );

/* compute the cube term x^3/3! */
qmov( x, b );
qadd( qone, n, n );
qdiv( n, b, b );     /* x / 2 */
qadd( qone, n, n );
qdiv( n, b, b );     /* x / 6 */
qmul( z, b, b );     /* x^3 / 6 */

/* power series */
do
	{
	qadd( qone, n, n );	/* n += 1	*/
	qdiv( n, b, b );	/* b /= n	*/
	qadd( qone, n, n );	/* n += 1	*/
	qdiv( n, b, b );	/* b /= n	*/
	qmul( z, b, b );	/* b *= z	*/
	qadd( b, y, y );	/* y += b	*/
	}
while( (int)(y[1] - b[1]) < NBITS );
return 0;
}
