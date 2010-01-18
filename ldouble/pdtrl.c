/*							pdtrl.c
 *
 *	Poisson distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int k;
 * long double m, y, pdtrl();
 *
 * y = pdtrl( k, m );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the sum of the first k terms of the Poisson
 * distribution:
 *
 *   k         j
 *   --   -m  m
 *   >   e    --
 *   --       j!
 *  j=0
 *
 * The terms are not summed directly; instead the incomplete
 * gamma integral is employed, according to the relation
 *
 * y = pdtr( k, m ) = igamc( k+1, m ).
 *
 * The arguments must both be positive.
 *
 *
 *
 * ACCURACY:
 *
 * See igamc().
 *
 */
/*							pdtrcl()
 *
 *	Complemented poisson distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int k;
 * long double m, y, pdtrcl();
 *
 * y = pdtrcl( k, m );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms k+1 to infinity of the Poisson
 * distribution:
 *
 *  inf.       j
 *   --   -m  m
 *   >   e    --
 *   --       j!
 *  j=k+1
 *
 * The terms are not summed directly; instead the incomplete
 * gamma integral is employed, according to the formula
 *
 * y = pdtrc( k, m ) = igam( k+1, m ).
 *
 * The arguments must both be positive.
 *
 *
 *
 * ACCURACY:
 *
 * See igam.c.
 *
 */
/*							pdtril()
 *
 *	Inverse Poisson distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int k;
 * long double m, y, pdtrl();
 *
 * m = pdtril( k, y );
 *
 *
 *
 *
 * DESCRIPTION:
 *
 * Finds the Poisson variable x such that the integral
 * from 0 to x of the Poisson density is equal to the
 * given probability y.
 *
 * This is accomplished using the inverse gamma integral
 * function and the relation
 *
 *    m = igami( k+1, y ).
 *
 *
 *
 *
 * ACCURACY:
 *
 * See igami.c.
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * pdtri domain    y < 0 or y >= 1       0.0
 *                     k < 0
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double igaml ( long double, long double );
extern long double igamcl ( long double, long double );
extern long double igamil ( long double, long double );
#else
long double igaml(), igamcl(), igamil();
#endif

long double pdtrcl( k, m )
int k;
long double m;
{
long double v;

if( (k < 0) || (m <= 0.0L) )
	{
	mtherr( "pdtrcl", DOMAIN );
	return( 0.0L );
	}
v = k+1;
return( igaml( v, m ) );
}



long double pdtrl( k, m )
int k;
long double m;
{
long double v;

if( (k < 0) || (m <= 0.0L) )
	{
	mtherr( "pdtrl", DOMAIN );
	return( 0.0L );
	}
v = k+1;
return( igamcl( v, m ) );
}


long double pdtril( k, y )
int k;
long double y;
{
long double v;

if( (k < 0) || (y < 0.0L) || (y >= 1.0L) )
	{
	mtherr( "pdtril", DOMAIN );
	return( 0.0L );
	}
v = k+1;
v = igamil( v, y );
return( v );
}
