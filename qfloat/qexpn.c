/*							qexpn.c
 *
 *		Exponential integral En
 *
 *
 *
 * SYNOPSIS:
 *
 * int qexpn( n, x, y );
 * int n;
 * QELT *x, *y;
 *
 * qexpn( n, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Evaluates the exponential integral
 *
 *                 inf.
 *                   -
 *                  | |   -xt
 *                  |    e
 *      E (x)  =    |    ----  dt.
 *       n          |      n
 *                | |     t
 *                 -
 *                  1
 *
 *
 * Both n and x must be nonnegative.
 *
 *
 * ACCURACY:
 * 
 * Series expansions are truncated at less than full working precision.
 *
 */

/*							expn.c	*/

/* Cephes Math Library Release 1.1:  March, 1985
 * Copyright 1985 by Stephen L. Moshier */

/* Exponential integral	*/
#include "qhead.h"
extern QELT qone[];
extern QELT qeul[];

extern double MAXNUM;
extern int qgamma();

int qexpn( n, x, yy )
int n;
QELT x[], yy[];
{
static QELT ans[NQ], r[NQ], t[NQ], yk[NQ], xk[NQ], qn[NQ];
static QELT pk[NQ], pkm1[NQ], pkm2[NQ], qk[NQ], qkm1[NQ], qkm2[NQ];
static QELT psi[NQ], z[NQ];
int i, k;
long ln;
union
  {
    unsigned short s[4];
    double d;
  } temp;


if( n < 0 )
	{
	mtherr("qexpn", DOMAIN );
	goto overf;
	}

if( x[0] != 0 )
	{
	mtherr("qexpn", DOMAIN );
	goto overf;
	}

if( x[1] < 3 )
	{
	if( n < 2 )
		goto overf;
	else
		{
		ln = n - 1;
		ltoq( &ln, ans );
		qdiv( ans, qone, yy );
		return 0;
		}
	}

if( n == 0 )
	{
	qexp( x, ans );		/* exp(-x)/x */
	qmul( ans, x, ans );
	qdiv( ans, qone, yy );
	return 0;
	}

/*							expn.c	*/
/*		Expansion for large n		*/
/*
if( n > 5000 )
	{
	xk = x + n;
	yk = 1.0 / (xk * xk);
	t = n;
	ans = yk * t * (6.0 * x * x  -  8.0 * t * x  +  t * t);
	ans = yk * (ans + t * (t  -  2.0 * x));
	ans = yk * (ans + t);
	ans = (ans + 1.0) * exp( -x ) / xk;
	goto done;
	}
*/

if( x[1] > (QELT) (EXPONE+1) )
	goto cfrac;

/*							expn.c	*/

/*		Power series expansion		*/


qlog( x, psi );		/* psi = -EUL - log(x) */
qneg( psi );
qsub( qeul, psi, psi );

for( i=1; i<n; i++ )
	{
	ln = i;
	ltoq( &ln, qn );
	qdiv( qn, qone, qn );	/* psi = psi + 1.0/i */
	qadd( qn, psi, psi );
	}

qmov( x, z );		/* z = -x */
qneg( z );
qclear( xk );		/* xk = 0.0 */
qmov( qone, yk );	/* yk = 1.0 */
ln = n;
ltoq( &ln, qn );
qsub( qn, qone, pk );	/* pk = 1.0 - n */
if( n == 1 )
	qclear( ans );	/* ans = 0.0 */
else
	qdiv( pk, qone, ans );	/* ans = 1.0/pk */

do
	{
	qadd( qone, xk, xk );	/* xk += 1.0 */
	qdiv( xk, z, qn );	/* yk *= z/xk */
	qmul( qn, yk, yk );
	qadd( qone, pk, pk );	/* pk += 1.0 */
	if( pk[1] > 10 )
		{
		qdiv( pk, yk, t );	/* ans += yk/pk */
		qadd( t, ans, ans );
		}
	else
		qmov( qone, t );	/* t = 1.0 */
	}
while( ((int) t[1] - (int) ans[1]) > -70 );

qtoe( xk, temp.s );
k = temp.d;	/* k = xk */
ln = n;
ltoq( &ln, t );	/* t = n */
/* ans = (powi(z, n-1) * psi / gamma(t)) - ans */
qgamma( t, t );
qlog( x, qn );
ln = n - 1;
ltoq( &ln, yk );
qmul( yk, qn, qn );
qexp( qn, qn );
if( ((n-1) & 1) != 0 )
	qneg( qn );
qmul( psi, qn, qn );
qdiv( t, qn, qn );
qsub( ans, qn, ans );
goto done;


/*							expn.c	*/
/*		continued fraction		*/
cfrac:
k = 1;
qmov( qone, pkm2 );	/* pkm2 = 1.0 */
qmov( x, qkm2 );	/* qkm2 = x   */
qmov( qone, pkm1 );	/* pkm1 = 1.0 */
ln = n;
ltoq( &ln, qn );
qadd( qn, x, qkm1 );	/* qkm1 = x + n */
qdiv( qkm1, pkm1, ans );	/* ans = pkm1/qkm1 */

do
	{
	k += 1;
	if( k & 1 )
		{
		qmov( qone, yk );	/* yk = 1.0 */

		ln = n + (k-1)/2;	/* xk = n + (k-1)/2 */
		ltoq( &ln, xk );
		}
	else
		{
		qmov( x, yk );		/* yk = x */
		ln = k/2;		/* xk = k/2 */
		ltoq( &ln, xk );
		}
	qmul( yk, pkm1, qn );	/* pk = pkm1 * yk  +  pkm2 * xk */
	qmul( xk, pkm2, pk );
	qadd( qn, pk, pk );	
	qmul( yk, qkm1, qn );	/* qk = qkm1 * yk  +  qkm2 * xk */
	qmul( xk, qkm2, qk );
	qadd( qn, qk, qk );
	if( qk[1] > 2 )
		{
		qdiv( qk, pk, r );	/* r = pk/qk */
		qsub( r, ans, t );	/* t = abs( (ans - r)/r ) */
		qmov( r, ans );		/* ans = r */
		}
	else
		qmov( qone, t );	/* t = 1.0 */
	qmov( pkm1, pkm2 );		/* pkm2 = pkm1 */
	qmov( pk, pkm1 );		/* pkm1 = pk   */
	qmov( qkm1, qkm2 );		/* qkm2 = qkm1 */
	qmov( qk, qkm1 );		/* qkm1 = qk   */
if( pk[1] > (QELT) (EXPONE + 64) )
	{
	pkm2[1] -= 64;
	pkm1[1] -= 64;
	qkm2[1] -= 64;
	qkm1[1] -= 64;
	}
	}
while( ((int) t[1] - (int) r[1]) > -70 );

qexp( x, qn );
qdiv( qn, ans, ans );	/* ans *= exp( -x ) */

done:
qmov( ans, yy );
return 0;

overf:
mtherr( "qexpn", OVERFLOW );
return 0;
}
