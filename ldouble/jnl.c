/*							jnl.c
 *
 *	Bessel function of integer order
 *
 *
 *
 * SYNOPSIS:
 *
 * int n;
 * long double x, y, jnl();
 *
 * y = jnl( n, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of order n, where n is a
 * (possibly negative) integer.
 *
 * The ratio of jn(x) to j0(x) is computed by backward
 * recurrence.  First the ratio jn/jn-1 is found by a
 * continued fraction expansion.  Then the recurrence
 * relating successive orders is applied until j0 or j1 is
 * reached.
 *
 * If n = 0 or 1 the routine for j0 or j1 is called
 * directly.
 *
 *
 *
 * ACCURACY:
 *
 *                      Absolute error:
 * arithmetic   domain      # trials      peak         rms
 *    IEEE     -30, 30        5000       3.3e-19     4.7e-20
 *
 *
 * Not suitable for large n or x.
 *
 */

/*							jn.c
Cephes Math Library Release 2.0:  April, 1987
Copyright 1984, 1987 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/
#include "mconf.h"

extern long double MACHEPL;
#ifdef ANSIPROT
extern long double fabsl ( long double );
extern long double j0l ( long double );
extern long double j1l ( long double );
#else
long double fabsl(), j0l(), j1l();
#endif

long double jnl( n, x )
int n;
long double x;
{
long double pkm2, pkm1, pk, xk, r, ans;
int k, sign;

if( n < 0 )
	{
	n = -n;
	if( (n & 1) == 0 )	/* -1**n */
		sign = 1;
	else
		sign = -1;
	}
else
	sign = 1;

if( x < 0.0L )
	{
	if( n & 1 )
		sign = -sign;
	x = -x;
	}


if( n == 0 )
	return( sign * j0l(x) );
if( n == 1 )
	return( sign * j1l(x) );
if( n == 2 )
	return( sign * (2.0L * j1l(x) / x  -  j0l(x)) );

if( x < MACHEPL )
	return( 0.0L );

/* continued fraction */
k = 53;
pk = 2 * (n + k);
ans = pk;
xk = x * x;

do
	{
	pk -= 2.0L;
	ans = pk - (xk/ans);
	}
while( --k > 0 );
ans = x/ans;

/* backward recurrence */

pk = 1.0L;
pkm1 = 1.0L/ans;
k = n-1;
r = 2 * k;

do
	{
	pkm2 = (pkm1 * r  -  pk * x) / x;
	pk = pkm1;
	pkm1 = pkm2;
	r -= 2.0L;
	}
while( --k > 0 );

if( fabsl(pk) > fabsl(pkm1) )
	ans = j1l(x)/pk;
else
	ans = j0l(x)/pkm1;
return( sign * ans );
}
