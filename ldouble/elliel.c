/*							elliel.c
 *
 *	Incomplete elliptic integral of the second kind
 *
 *
 *
 * SYNOPSIS:
 *
 * long double phi, m, y, elliel();
 *
 * y = elliel( phi, m );
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
 * Tested at random arguments with phi in [-10, 10] and m in
 * [0, 1].
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -10,10       50000       2.7e-18     2.3e-19
 *
 *
 */


/*
Cephes Math Library Release 2.3:  November, 1995
Copyright 1984, 1987, 1993, 1995 by Stephen L. Moshier
*/

/*	Incomplete elliptic integral of second kind	*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double sqrtl ( long double );
extern long double fabsl ( long double );
extern long double logl ( long double );
extern long double sinl ( long double );
extern long double tanl ( long double );
extern long double atanl ( long double );
extern long double floorl ( long double );
extern long double ellpel ( long double );
extern long double ellpkl ( long double );
long double elliel ( long double, long double );
#else
long double sqrtl(), fabsl(), logl(), sinl(), tanl(), atanl(), floorl();
long double ellpel(), ellpkl(), elliel();
#endif
extern long double PIL, PIO2L, MACHEPL;


long double elliel( phi, m )
long double phi, m;
{
long double a, b, c, e, temp, lphi, t, E;
int d, mod, npio2, sign;

if( m == 0.0L )
	return( phi );
lphi = phi;
npio2 = floorl( lphi/PIO2L );
if( npio2 & 1 )
	npio2 += 1;
lphi = lphi - npio2 * PIO2L;
if( lphi < 0.0L )
	{
	lphi = -lphi;
	sign = -1;
	}
else
	{
	sign = 1;
	}
a = 1.0L - m;
E = ellpel( a );
if( a == 0.0L )
	{
	temp = sinl( lphi );
	goto done;
	}
t = tanl( lphi );
b = sqrtl(a);
if( fabsl(t) > 10.0L )
	{
	/* Transform the amplitude */
	e = 1.0L/(b*t);
	/* ... but avoid multiple recursions.  */
	if( fabsl(e) < 10.0L )
		{
		e = atanl(e);
		temp = E + m * sinl( lphi ) * sinl( e ) - elliel( e, m );
		goto done;
		}
	}
c = sqrtl(m);
a = 1.0L;
d = 1;
e = 0.0L;
mod = 0;

while( fabsl(c/a) > MACHEPL )
	{
	temp = b/a;
	lphi = lphi + atanl(t*temp) + mod * PIL;
	mod = (lphi + PIO2L)/PIL;
	t = t * ( 1.0L + temp )/( 1.0L - temp * t * t );
	c = 0.5L*( a - b );
	temp = sqrtl( a * b );
	a = 0.5L*( a + b );
	b = temp;
	d += d;
	e += c * sinl(lphi);
	}

temp = E / ellpkl( 1.0L - m );
temp *= (atanl(t) + mod * PIL)/(d * a);
temp += e;

done:

if( sign < 0 )
	temp = -temp;
temp += npio2 * E;
return( temp );
}
