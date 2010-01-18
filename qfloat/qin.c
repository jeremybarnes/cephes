/*							qin.c
 *
 *	Modified Bessel function I of noninteger order
 *
 *
 * SYNOPSIS:
 *
 * int qin( v, x, y );
 * QELT *v, *x, *y;
 *
 * qin( v, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns modified Bessel function of order v of the
 * argument.
 *
 * The power series is
 *
 *                inf      2   k
 *              v  -     (z /4)
 * I (z) = (z/2)   >  --------------
 *  v              -       -
 *                k=0  k! | (v+k+1)
 *
 *
 * For large x,
 *                                    2          2       2
 *             exp(z)            u - 1     (u - 1 )(u - 3 )
 * I (z)  =  ------------ { 1 - -------- + ---------------- + ...}
 *  v        sqrt(2 pi z)              1              2
 *                              1! (8z)        2! (8z)
 *
 * asymptotically, where
 * 
 *            2
 *     u = 4 v .
 * 
 *
 * x <= 0 is not supported.
 *
 * Series expansion is truncated at less than full working precision.
 *
 */

/*
Cephes Math Library Release 2.1:  November, 1988
Copyright 1984, 1987, 1988 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

/*							qin.c	*/
/*
 * Modified Bessel function of first kind of order n
 */

#include <stdio.h>
#include "qhead.h"
extern QELT qone[], qtwo[], qpi[];
static QELT qspi[NQ];
static QELT nkf[NQ];
static QELT nk1f[NQ];
static QELT pk[NQ];
static QELT fn[NQ];
static QELT s[NQ];
static QELT pn[NQ];
static QELT z0[NQ];
static QELT t1[NQ];
static int qsflg = 0;
int qgamma();

int qin( n, x, y )
QELT n[], x[], y[];
{
QELT t[NQ], u[NQ], z[NQ], k[NQ], ans[NQ];
long i, ln;
union
  {
    unsigned short s[4];
    double d;
  } dx;

if( qsflg == 0 )
	{ /* compute 1/sqrt(2 pi) */
	qsflg = 1;
	qmov( qpi, qspi );
	qspi[1] += 1;
	qdiv( qspi, qone, qspi );
	qsqrt( qspi, qspi );
	}

qtoe( x, dx.s );
if( dx.d > 40.0 )
	goto asymp;

qmul( x, x, z );	/*z = -x * x / 4.0;		*/

if( z[1] < 3 )		/* x = 0, n = 0 is special case	*/
	{
	if( n[1] == 0 )
		qmov( qone, y );
/*
	else
		qclear( y );
*/
	return(0);
	}

if( z[1] > 1 )
	z[1] -= 2;

if( n[1] < 3 )
	qmov( qone, u );
else
	{
	qmov( n, ans );
	qadd( qone, ans, ans );
	qgamma( ans, u );
	qdiv( u, qone, u );		/*u = 1.0/gamma(n+1);*/
	}
qmov( u, ans );		/*ans = u;*/
qmov( qone, k );	/*k = 1.0;*/

while( ((int) ans[1] - (int) u[1]) < NBITS/2 )	/* 70 */
	{
	qadd( n, k, t );
	qmul( t, k, t );	/*u *= z / (k * (n+k));*/
	qdiv( t, z, t );
	qmul( t, u, u );
	if( (int) u[1] <= 0 )
		printf( "qin overflow\n" );
	qadd( u, ans, ans );	/*ans += u;*/
	qadd( qone, k, k );	/*k += 1.0;*/
	}

/* ans *= exp( n * log( x/2.0 ) );*/

if( n[1] < 3 )
	{
	qmov( ans, y );
	goto done;
	}
qmov( x, t );
t[1] -= 1;
qlog( t, u );
qmul( u, n, u );
qexp( u, t );
qmul( ans, t, y );

done:
/*
qexp( x, t );
qdiv( t, y, y );
qsqrt( x, t );
qmul( x, y, y );
*/
return 0;


/* Asymptotic expansion for In(x) */

asymp:

qtoe( n, dx.s );		/* ln = n */
ln = dx.d;
dx.d = 4.0 * dx.d * dx.d;		/* pn = 4.0 * n * n; */
etoq( dx.s, pn );
qmov( qone, pk );		/* pk = 1.0; */
qmov( x, z0 );			/* z0 = -8.0 * x; */
z0[1] += 3;
qneg( z0 );
qmov( qone, fn );		/* fn = 1.0; */
qmov( qone, t );		/* t = 1.0; */
qmov( t, s );			/* s = t; */
qmov( qone, nkf );		/* nkf = MAXNUM; */
nkf[1] += 16000;
i = 0;
do
	{
	qmul( pk, pk, t1 );	/* z = pn - pk * pk; */
	qsub( t1, pn, z );
	qmul( fn, z0, t1 );	/* t = t * z /(fn * z0); */
	qdiv( t1, z, t1 );
	qmul( t, t1, t );
	qmov( t, nk1f );	/* nk1f = fabs(t); */
	nk1f[0] = 0;
	qsub( nkf, nk1f, t1 );
	if( (i >= ln) && (t1[0] == 0) ) /* nk1f > nkf */
		{
		printf( "qin: i=%ld, %d\n", i, t[1]-s[1] );
		goto adone;
		}
	qmov( nk1f, nkf );	/* nkf = nk1f; */
	qadd( s, t, s );	/* s += t; */
	qadd( qone, fn, fn );	/* fn += 1.0; */
	qadd( qtwo, pk, pk );	/* pk += 2.0; */
	i += 1;
	}
while( ((int) s[1] - (int) t[1]) < NBITS/2 );	/* fabs(t/s) > MACHEP ); */

adone:

qmul( qspi, s, y );	/* ans = s exp(x) / sqrt( 2 pi x ) */

qexp( x, t );
qsqrt( x, z );
qdiv( z, t, t );
qmul( t, y, y );
return 0;
}
