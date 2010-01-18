/*							ynl.c
 *
 *	Bessel function of second kind of integer order
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, ynl();
 * int n;
 *
 * y = ynl( n, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of order n, where n is a
 * (possibly negative) integer.
 *
 * The function is evaluated by forward recurrence on
 * n, starting with values computed by the routines
 * y0l() and y1l().
 *
 * If n = 0 or 1 the routine for y0l or y1l is called
 * directly.
 *
 *
 *
 * ACCURACY:
 *
 *
 *       Absolute error, except relative error when y > 1.
 *       x >= 0,  -30 <= n <= +30.
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -30, 30       10000       1.3e-18     1.8e-19
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * ynl singularity   x = 0              MAXNUML
 * ynl overflow                         MAXNUML
 *
 * Spot checked against tables for x, n between 0 and 100.
 *
 */

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"
extern long double MAXNUML;
#ifdef ANSIPROT
extern long double y0l ( long double );
extern long double y1l ( long double );
#else
long double y0l(), y1l();
#endif

long double ynl( n, x )
int n;
long double x;
{
long double an, anm1, anm2, r;
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


if( n == 0 )
	return( sign * y0l(x) );
if( n == 1 )
	return( sign * y1l(x) );

/* test for overflow */
if( x <= 0.0L )
	{
	mtherr( "ynl", SING );
	return( -MAXNUML );
	}

/* forward recurrence on n */

anm2 = y0l(x);
anm1 = y1l(x);
k = 1;
r = 2 * k;
do
	{
	an = r * anm1 / x  -  anm2;
	anm2 = anm1;
	anm1 = an;
	r += 2.0L;
	++k;
	}
while( k < n );


return( sign * an );
}
