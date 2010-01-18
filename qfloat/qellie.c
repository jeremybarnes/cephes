/*							qellie.c
 *
 *	Incomplete elliptic integral of the second kind
 *
 *
 *
 * SYNOPSIS:
 *
 * int qellie( phi, m, y );
 * QELT *phi, *m, *y;
 *
 * qellie( phi, m, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *
 *                phi
 *                 -
 *                | |
 *                |                   2
 * E(phi_\m)  =    |    sqrt( 1 - m sin t ) dt
 *                |
 *              | |    
 *               -
 *                0
 *
 * of amplitude phi and modulus m, using the arithmetic -
 * geometric mean algorithm.
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
Copyright 1984, 1987, 1993 by Stephen L. Moshier
*/

/*	qellie.c		*/
/*	Incomplete elliptic integral of second kind	*/
/*	Arguments are phi and m		*/

#include "qhead.h"
extern QELT qone[], qpi[];
static QELT a[NQ], b[NQ], c[NQ], e[NQ], t[NQ], temp[NQ];
static QELT lphi[NQ], temp2[NQ];
int qellpe(), qellpk(), qtan();

int qellie( phi, m, y )
QELT phi[], m[], y[];
{
int sign;
long d, mod;
union
  {
    unsigned short s[4];
    double d;
  } dmod;

if( m[1] < (QELT) (EXPONE - 129) )
	{
	qmov( phi, y );
	return(0);
	}

if( qcmp( m, qone ) > 0 || m[0] != 0 )
	{
	mtherr( "qellie", DOMAIN );
	return(0);
	}
qsub( m, qone, b );	/* b = sqrt( 1 - m )	*/
if( b[1] < (QELT) (EXPONE - 129) )
	{
	qsin( phi, y );
	return(0);
	}
qmov( phi, lphi );
if( lphi[0] )
  sign = -1;
else
  sign = 0;
lphi[0] = 0;		/* make positive */
qmov( qone, a );	/* a = 1		*/
qsqrt( b, b );
qsqrt( m, c );		/* c = sqrt( m )	*/
d = 1L;			/* d = 1		*/
qmov( qone, e );	/* e = 0		*/
e[1] = 0;
e[2] = 0;
qtan( lphi, t );			/* t = tan( phi )	*/
qmov( qpi, temp );	/* temp = pi/2 + phi	*/
temp[1] -= 1;
qadd( lphi, temp, temp );
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
	d += d;			/* d += d 		*/
	qsin( lphi, temp );	/* e += c * sin(lphi)	*/
	qmul( temp, c, temp );
	qadd( e, temp, e );
	}

qsub( m, qone, b );	/* b = 1 - m			*/
qellpe( b, temp );	/* ellpe(b)/ellpk(b)		*/
qellpk( b, temp2 );
qdiv( temp2, temp, c );
ltoq( &mod, temp);	/* (arctan(t) + mod*pi)/(d*a)	*/
qmul( temp, qpi, temp );
qatn( t, t );
qadd( t, temp, t );
ltoq( &d, temp2 );
qmul( temp2, a, temp );
qdiv( temp, t, t );
qmul( c, t, c );
qadd( c, e, y );
y[0] = sign;
return(0);
}
