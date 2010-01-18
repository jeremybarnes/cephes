/*							stdtrl.c
 *
 *	Student's t distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * long double p, t, stdtrl();
 * int k;
 *
 * p = stdtrl( k, t );
 *
 *
 * DESCRIPTION:
 *
 * Computes the integral from minus infinity to t of the Student
 * t distribution with integer k > 0 degrees of freedom:
 *
 *                                      t
 *                                      -
 *                                     | |
 *              -                      |         2   -(k+1)/2
 *             | ( (k+1)/2 )           |  (     x   )
 *       ----------------------        |  ( 1 + --- )        dx
 *                     -               |  (      k  )
 *       sqrt( k pi ) | ( k/2 )        |
 *                                   | |
 *                                    -
 *                                   -inf.
 * 
 * Relation to incomplete beta integral:
 *
 *        1 - stdtr(k,t) = 0.5 * incbet( k/2, 1/2, z )
 * where
 *        z = k/(k + t**2).
 *
 * For t < -1.6, this is the method of computation.  For higher t,
 * a direct method is derived from integration by parts.
 * Since the function is symmetric about t=0, the area under the
 * right tail of the density is found by calling the function
 * with -t instead of t.
 * 
 * ACCURACY:
 *
 * Tested at random 1 <= k <= 100.  The "domain" refers to t.
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -100,-1.6    10000       5.7e-18     9.8e-19
 *    IEEE     -1.6,100     10000       3.8e-18     1.0e-19
 */

/*							stdtril.c
 *
 *	Functional inverse of Student's t distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * long double p, t, stdtril();
 * int k;
 *
 * t = stdtril( k, p );
 *
 *
 * DESCRIPTION:
 *
 * Given probability p, finds the argument t such that stdtrl(k,t)
 * is equal to p.
 * 
 * ACCURACY:
 *
 * Tested at random 1 <= k <= 100.  The "domain" refers to p:
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       0,1        3500       4.2e-17     4.1e-18
 */


/*
Cephes Math Library Release 2.3:  January, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"

extern long double PIL, MACHEPL, MAXNUML;
#ifdef ANSIPROT
extern long double sqrtl ( long double );
extern long double atanl ( long double );
extern long double incbetl ( long double, long double, long double );
extern long double incbil ( long double, long double, long double );
extern long double fabsl ( long double );
#else
long double sqrtl(), atanl(), incbetl(), incbil(), fabsl();
#endif

long double stdtrl( k, t )
int k;
long double t;
{
long double x, rk, z, f, tz, p, xsqk;
int j;

if( k <= 0 )
	{
	mtherr( "stdtrl", DOMAIN );
	return(0.0L);
	}

if( t == 0.0L )
	return( 0.5L );

if( t < -1.6L )
	{
	rk = k;
	z = rk / (rk + t * t);
	p = 0.5L * incbetl( 0.5L*rk, 0.5L, z );
	return( p );
	}

/*	compute integral from -t to + t */

if( t < 0.0L )
	x = -t;
else
	x = t;

rk = k;	/* degrees of freedom */
z = 1.0L + ( x * x )/rk;

/* test if k is odd or even */
if( (k & 1) != 0)
	{

	/*	computation for odd k	*/

	xsqk = x/sqrtl(rk);
	p = atanl( xsqk );
	if( k > 1 )
		{
		f = 1.0L;
		tz = 1.0L;
		j = 3;
		while(  (j<=(k-2)) && ( (tz/f) > MACHEPL )  )
			{
			tz *= (j-1)/( z * j );
			f += tz;
			j += 2;
			}
		p += f * xsqk/z;
		}
	p *= 2.0L/PIL;
	}


else
	{

	/*	computation for even k	*/

	f = 1.0L;
	tz = 1.0L;
	j = 2;

	while(  ( j <= (k-2) ) && ( (tz/f) > MACHEPL )  )
		{
		tz *= (j - 1)/( z * j );
		f += tz;
		j += 2;
		}
	p = f * x/sqrtl(z*rk);
	}

/*	common exit	*/


if( t < 0.0L )
	p = -p;	/* note destruction of relative accuracy */

	p = 0.5L + 0.5L * p;
return(p);
}


long double stdtril( k, p )
int k;
long double p;
{
long double t, rk, z;
int rflg;

if( k <= 0 || p <= 0.0L || p >= 1.0L )
	{
	mtherr( "stdtril", DOMAIN );
	return(0.0L);
	}

rk = k;

if( p > 0.25L && p < 0.75L )
	{
	if( p == 0.5L )
		return( 0.0L );
	z = 1.0L - 2.0L * p;
	z = incbil( 0.5L, 0.5L*rk, fabsl(z) );
	t = sqrtl( rk*z/(1.0L-z) );
	if( p < 0.5L )
		t = -t;
	return( t );
	}
rflg = -1;
if( p >= 0.5L)
	{
	p = 1.0L - p;
	rflg = 1;
	}
z = incbil( 0.5L*rk, 0.5L, 2.0L*p );

if( MAXNUML * z < rk )
	return(rflg* MAXNUML);
t = sqrtl( rk/z - rk );
return( rflg * t );
}
