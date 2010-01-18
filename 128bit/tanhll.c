/*							tanhl.c
 *
 *	Hyperbolic tangent, 128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, tanhl();
 *
 * y = tanhl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns hyperbolic tangent of argument in the range MINLOGL to
 * MAXLOGL.
 *
 * A rational function is used for |x| < 0.625.  The form
 * x + x**3 P(x)/Q(x) of Cody _& Waite is employed.
 * Otherwise,
 *    tanh(x) = sinh(x)/cosh(x) = 1  -  2/(exp(2x) + 1).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -2,2       100,000      2.1e-34     4.5e-35
 *
 */

/*
Cephes Math Library Release 2.1:  February, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"

/* tanh(x) = x + x^3 R(x^2)
 * 0 <= x <= 0.625
 * Theoretical peak relative error = 7.7e-37,
 * relative peak error spread = 3.9e-11
 */
static long double P[6] = {
-6.505693197948351084912624750702492767503E-6L,
-9.804083860188429726356968570322356183383E-1L,
-5.055287638900473250703725789725376004355E2L,
-7.307477148073823966594990496301416814519E4L,
-3.531606586182691280701462523692471322688E6L,
-4.551377146142783468144190926206842300707E7L
};
static long double Q[5] = {
/* 1.000000000000000000000000000000000000000E0L, */
 5.334865598460027935735737253027154828002E2L,
 8.058475607422391042912151298751537172870E4L,
 4.197073523796142343374222405869721575491E6L,
 6.521134551226147545983467868553677881771E7L,
 1.365413143842835040443257277862054198329E8L
};

extern long double MAXLOGL;

long double tanhl(x)
long double x;
{
long double s, z;
long double fabsl(), expl(), polevll(), p1evll();


z = fabsl(x);
if( z > 0.5L * MAXLOGL )
	{
	if( x > 0 )
		return( 1.0L );
	else
		return( -1.0L );
	}
if( z >= 0.625L )
	{
	s = expl(2.0L * z);
	z =  1.0L  - 2.0L/(s + 1.0L);
	if( x < 0 )
		z = -z;
	}
else
	{
	s = x * x;
	z = polevll( s, P, 5 )/p1evll(s, Q, 5);
	z = x * s * z;
	z = x + z;
	}
return( z );
}
