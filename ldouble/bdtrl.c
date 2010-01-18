/*							bdtrl.c
 *
 *	Binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int k, n;
 * long double p, y, bdtrl();
 *
 * y = bdtrl( k, n, p );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms 0 through k of the Binomial
 * probability density:
 *
 *   k
 *   --  ( n )   j      n-j
 *   >   (   )  p  (1-p)
 *   --  ( j )
 *  j=0
 *
 * The terms are not summed directly; instead the incomplete
 * beta integral is employed, according to the formula
 *
 * y = bdtr( k, n, p ) = incbet( n-k, k+1, 1-p ).
 *
 * The arguments must be positive, with p ranging from 0 to 1.
 *
 *
 *
 * ACCURACY:
 *
 * Tested at random points (k,n,p) with a and b between 0
 * and 10000 and p between 0 and 1.
 *    Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,10000      3000       1.6e-14     2.2e-15
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * bdtrl domain        k < 0            0.0
 *                     n < k
 *                     x < 0, x > 1
 *
 */
/*							bdtrcl()
 *
 *	Complemented binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int k, n;
 * long double p, y, bdtrcl();
 *
 * y = bdtrcl( k, n, p );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms k+1 through n of the Binomial
 * probability density:
 *
 *   n
 *   --  ( n )   j      n-j
 *   >   (   )  p  (1-p)
 *   --  ( j )
 *  j=k+1
 *
 * The terms are not summed directly; instead the incomplete
 * beta integral is employed, according to the formula
 *
 * y = bdtrc( k, n, p ) = incbet( k+1, n-k, p ).
 *
 * The arguments must be positive, with p ranging from 0 to 1.
 *
 *
 *
 * ACCURACY:
 *
 * See incbet.c.
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * bdtrcl domain     x<0, x>1, n<k       0.0
 */
/*							bdtril()
 *
 *	Inverse binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int k, n;
 * long double p, y, bdtril();
 *
 * p = bdtril( k, n, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Finds the event probability p such that the sum of the
 * terms 0 through k of the Binomial probability density
 * is equal to the given cumulative probability y.
 *
 * This is accomplished using the inverse beta integral
 * function and the relation
 *
 * 1 - p = incbi( n-k, k+1, y ).
 *
 * ACCURACY:
 *
 * See incbi.c.
 * Tested at random k, n between 1 and 10000.  The "domain" refers to p:
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       0,1        3500       2.0e-15     8.2e-17
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * bdtril domain     k < 0, n <= k         0.0
 *                  x < 0, x > 1
 */

/*								bdtr() */


/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double incbetl ( long double, long double, long double );
extern long double incbil ( long double, long double, long double );
extern long double powl ( long double, long double );
extern long double expm1l ( long double );
extern long double log1pl ( long double );
#else
long double incbetl(), incbil(), powl(), expm1l(), log1pl();
#endif

long double bdtrcl( k, n, p )
int k, n;
long double p;
{
long double dk, dn;

if( (p < 0.0L) || (p > 1.0L) )
	goto domerr;
if( k < 0 )
	return( 1.0L );

if( n < k )
	{
domerr:
	mtherr( "bdtrcl", DOMAIN );
	return( 0.0L );
	}

if( k == n )
	return( 0.0L );
dn = n - k;
if( k == 0 )
	{
	if( p < .01L )
		dk = -expm1l( dn * log1pl(-p) );
	else
		dk = 1.0L - powl( 1.0L-p, dn );
	}
else
	{
	dk = k + 1;
	dk = incbetl( dk, dn, p );
	}
return( dk );
}



long double bdtrl( k, n, p )
int k, n;
long double p;
{
long double dk, dn, q;

if( (p < 0.0L) || (p > 1.0L) )
	goto domerr;
if( (k < 0) || (n < k) )
	{
domerr:
	mtherr( "bdtrl", DOMAIN );
	return( 0.0L );
	}

if( k == n )
	return( 1.0L );

q = 1.0L - p;
dn = n - k;
if( k == 0 )
	{
	dk = powl( q, dn );
	}
else
	{
	dk = k + 1;
	dk = incbetl( dn, dk, q );
	}
return( dk );
}


long double bdtril( k, n, y )
int k, n;
long double y;
{
long double dk, dn, p;

if( (y < 0.0L) || (y > 1.0L) )
	goto domerr;
if( (k < 0) || (n <= k) )
	{
domerr:
	mtherr( "bdtril", DOMAIN );
	return( 0.0L );
	}

dn = n - k;
if( k == 0 )
	{
	if( y > 0.8L )
		p = -expm1l( log1pl(y-1.0L) / dn );
	else
		p = 1.0L - powl( y, 1.0L/dn );
	}
else
	{
	dk = k + 1;
	p = incbetl( dn, dk, y );
	if( p > 0.5 )
		p = incbil( dk, dn, 1.0L-y );
	else
		p = 1.0 - incbil( dn, dk, y );
	}
return( p );
}
