/*							qellik.c
 *
 *	Incomplete elliptic integral of the first kind
 *
 *
 *
 * SYNOPSIS:
 *
 * int qellik( phi, m, y );
 * QELT *phi, *m, *y;
 *
 * qellik( phi, m, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *
 *
 *                phi
 *                 -
 *                | |
 *                |           dt
 * F(phi_\m)  =    |    ------------------
 *                |                   2
 *              | |    sqrt( 1 - m sin t )
 *               -
 *                0
 *
 * of amplitude phi and modulus m, using the arithmetic -
 * geometric mean algorithm.
 *
 *
 *
 *
 * ACCURACY:
 *
 * Sequence terminates at NBITS/2.
 *
 */

/*
Cephes Math Library Release 2.0:  April, 1987
Copyright 1984, 1987 by Stephen L. Moshier
*/

/*	qellik.c		*/
/*	Incomplete elliptic integral of first kind	*/
/*	Arguments are phi and m		*/

#include "qhead.h"
extern QELT qone[], qpi[];
int qtan();

int qellik( phi, m, y )
QELT phi[], m[], y[];
{
QELT a[NQ], b[NQ], c[NQ], d[NQ], t[NQ], temp[NQ];
QELT lphi[NQ], temp2[NQ];
long mod;
int sign;
union
  {
    unsigned short s[4];
    double d;
  } dmod;

if( qcmp( m, qone ) > 0 || m[0] != 0 )
	{
	mtherr( "qellik", DOMAIN );
	return(0);
	}
qmov( phi, lphi );
if( lphi[0] )
  sign = -1;
else
  sign = 0;
lphi[0] = 0;
qmov( qone, a );	/* a = 1		*/
qsub( m, qone, b );	/* b = sqrt( 1 - m )	*/
qmov( qpi, temp );	/* temp = pi/2 + phi	*/
temp[1] -= 1;
qadd( lphi, temp, temp );

if( b[1] < (QELT) (EXPONE-129) )
	{
	temp[1] -= 1;
	qtan( temp, temp );		/* log(tan( (pi/2 + phi)/2 )) */
	qlog( temp, y );
	return(0);
	}
qsqrt( b, b );
qsqrt( m, c );			/* c = sqrt( m )	*/
qmov( qone, d );		/* d = 1		*/
qtan( lphi, t );			/* t = tan( phi )	*/
qdiv( qpi, temp, temp );	/* mod = (phi + pi/2)/pi	*/
qtoe( temp, dmod.s );
mod = dmod.d;

while( ((int) a[1] - (int) c[1]) < (NBITS/2) )
	{
	qdiv( a, b, temp );	/* temp = b/a		*/
	qmul( t, temp, temp2 );	/* phi += arctan(t*temp) + mod*pi	*/
	qatn( temp2, temp2 );
	qadd( lphi, temp2, lphi );
	ltoq( &mod, temp2 );
	qmul( temp2, qpi, temp2 );
	qadd( lphi, temp2, lphi );
	qmov( qpi, temp2 );	/* mod = (phi + pi/2)/pi	*/
	temp2[1] -= 1;
	qadd( lphi, temp2, temp2 );
	qdiv( qpi, temp2, temp2 );
	qtoe( temp2, dmod.s );
	mod = dmod.d;
	qmul( t, t, temp2 );	/* t *= (1+temp)/(1-temp*t*t)	*/
	qmul( temp, temp2, temp2 );
	qsub( temp2, qone, temp2 );
	qadd( qone, temp, temp );
	qmul( t, temp, temp );
	qdiv( temp2, temp, t );
	qsub( b, a, c );	/* c = (a - b)/2.0	*/
	c[1] -= 1;
	qmul( a, b, temp );	/* temp = sqrt( a * b )	*/
	qsqrt( temp, temp );
	qadd( a, b, a );	/* a = (a + b)/2.0	*/
	a[1] -= 1;
	qmov( temp, b );	/* b = temp		*/
	qadd( d, d, d );	/* d += d 		*/
	}

ltoq( &mod, temp);	/* (arctan(t) + mod*pi)/(d*a)	*/
qmul( temp, qpi, temp );
qatn( t, t );
qadd( t, temp, t );
qmul( d, a, temp );
qdiv( temp, t, y );
y[0] = sign;
return(0);
}
