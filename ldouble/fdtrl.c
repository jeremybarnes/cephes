/*							fdtrl.c
 *
 *	F distribution, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * int df1, df2;
 * long double x, y, fdtrl();
 *
 * y = fdtrl( df1, df2, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the area from zero to x under the F density
 * function (also known as Snedcor's density or the
 * variance ratio density).  This is the density
 * of x = (u1/df1)/(u2/df2), where u1 and u2 are random
 * variables having Chi square distributions with df1
 * and df2 degrees of freedom, respectively.
 *
 * The incomplete beta integral is used, according to the
 * formula
 *
 *	P(x) = incbetl( df1/2, df2/2, (df1*x/(df2 + df1*x) ).
 *
 *
 * The arguments a and b are greater than zero, and x
 * x is nonnegative.
 *
 * ACCURACY:
 *
 * Tested at random points (a,b,x) in the indicated intervals.
 *                x     a,b                     Relative error:
 * arithmetic  domain  domain     # trials      peak         rms
 *    IEEE      0,1    1,100       10000       9.3e-18     2.9e-19
 *    IEEE      0,1    1,10000     10000       1.9e-14     2.9e-15
 *    IEEE      1,5    1,10000     10000       5.8e-15     1.4e-16
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * fdtrl domain     a<0, b<0, x<0         0.0
 *
 */
/*							fdtrcl()
 *
 *	Complemented F distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int df1, df2;
 * long double x, y, fdtrcl();
 *
 * y = fdtrcl( df1, df2, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the area from x to infinity under the F density
 * function (also known as Snedcor's density or the
 * variance ratio density).
 *
 *
 *                      inf.
 *                       -
 *              1       | |  a-1      b-1
 * 1-P(x)  =  ------    |   t    (1-t)    dt
 *            B(a,b)  | |
 *                     -
 *                      x
 *
 * (See fdtr.c.)
 *
 * The incomplete beta integral is used, according to the
 * formula
 *
 *	P(x) = incbet( df2/2, df1/2, (df2/(df2 + df1*x) ).
 *
 *
 * ACCURACY:
 *
 * See incbet.c.
 * Tested at random points (a,b,x).
 *
 *                x     a,b                     Relative error:
 * arithmetic  domain  domain     # trials      peak         rms
 *    IEEE      0,1    0,100       10000       4.2e-18     3.3e-19
 *    IEEE      0,1    1,10000     10000       7.2e-15     2.6e-16
 *    IEEE      1,5    1,10000     10000       1.7e-14     3.0e-15
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * fdtrcl domain    a<0, b<0, x<0         0.0
 *
 */
/*							fdtril()
 *
 *	Inverse of complemented F distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int df1, df2;
 * long double x, p, fdtril();
 *
 * x = fdtril( df1, df2, p );
 *
 * DESCRIPTION:
 *
 * Finds the F density argument x such that the integral
 * from x to infinity of the F density is equal to the
 * given probability p.
 *
 * This is accomplished using the inverse beta integral
 * function and the relations
 *
 *      z = incbi( df2/2, df1/2, p )
 *      x = df2 (1-z) / (df1 z).
 *
 * Note: the following relations hold for the inverse of
 * the uncomplemented F distribution:
 *
 *      z = incbi( df1/2, df2/2, p )
 *      x = df2 z / (df1 (1-z)).
 *
 * ACCURACY:
 *
 * See incbi.c.
 * Tested at random points (a,b,p).
 *
 *              a,b                     Relative error:
 * arithmetic  domain     # trials      peak         rms
 *  For p between .001 and 1:
 *    IEEE     1,100       40000       4.6e-18     2.7e-19
 *    IEEE     1,10000     30000       1.7e-14     1.4e-16
 *  For p between 10^-6 and .001:
 *    IEEE     1,100       20000       1.9e-15     3.9e-17
 *    IEEE     1,10000     30000       2.7e-15     4.0e-17
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * fdtril domain   p <= 0 or p > 1       0.0
 *                     v < 1
 */


/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/


#include "mconf.h"
#ifdef ANSIPROT
extern long double incbetl ( long double, long double, long double );
extern long double incbil ( long double, long double, long double );
#else
long double incbetl(), incbil();
#endif

long double fdtrcl( ia, ib, x )
int ia, ib;
long double x;
{
long double a, b, w;

if( (ia < 1) || (ib < 1) || (x < 0.0L) )
	{
	mtherr( "fdtrcl", DOMAIN );
	return( 0.0L );
	}
a = ia;
b = ib;
w = b / (b + a * x);
return( incbetl( 0.5L*b, 0.5L*a, w ) );
}



long double fdtrl( ia, ib, x )
int ia, ib;
long double x;
{
long double a, b, w;

if( (ia < 1) || (ib < 1) || (x < 0.0L) )
	{
	mtherr( "fdtrl", DOMAIN );
	return( 0.0L );
	}
a = ia;
b = ib;
w = a * x;
w = w / (b + w);
return( incbetl(0.5L*a, 0.5L*b, w) );
}


long double fdtril( ia, ib, y )
int ia, ib;
long double y;
{
long double a, b, w, x;

if( (ia < 1) || (ib < 1) || (y <= 0.0L) || (y > 1.0L) )
	{
	mtherr( "fdtril", DOMAIN );
	return( 0.0L );
	}
a = ia;
b = ib;
/* Compute probability for x = 0.5.  */
w = incbetl( 0.5L*b, 0.5L*a, 0.5L );
/* If that is greater than y, then the solution w < .5.
   Otherwise, solve at 1-y to remove cancellation in (b - b*w).  */
if( w > y || y < 0.001L)
	{
	w = incbil( 0.5L*b, 0.5L*a, y );
	x = (b - b*w)/(a*w);
	}
else
	{
	w = incbil( 0.5L*a, 0.5L*b, 1.0L - y );
	x = b*w/(a*(1.0L-w));
	}
return(x);
}
