/*							sinhl.c
 *
 *	Hyperbolic sine, 128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, sinhl();
 *
 * y = sinhl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns hyperbolic sine of argument in the range MINLOGL to
 * MAXLOGL.
 *
 * The range is partitioned into two segments.  If |x| <= 1, a
 * rational function of the form x + x**3 P(x)/Q(x) is employed.
 * Otherwise the calculation is sinh(x) = ( exp(x) - exp(-x) )/2.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       -2,2      100,000      4.1e-34     7.9e-35
 *
 */

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"

/* sinh(x) = x + x^3 R(x^2)
 * Theoretical peak relative error = 8.9e-37
 * relative peak error spread = 1.1e-15
 */
static long double P[6] = {
 1.622194395724068297909052717437740288268E3L,
 1.124862584587770079742188354390171794549E6L,
 3.047548980769660162696832999871894196102E8L,
 3.966215348072348368191433063260384329745E10L,
 2.375869584584371194838551715348965605295E12L,
 6.482835792103233269752264509192030816323E13L
};
static long double Q[6] = {
/* 1.000000000000000000000000000000000000000E0L, */
-9.101683853129357776079049616394849086007E2L,
 4.486400519836461218634448973793765123186E5L,
-1.492531313030440305095318968983514314656E8L,
 3.457771488856930054902696708717192082887E10L,
-5.193289868803472640225483235513427062460E12L,
 3.889701475261939961851358705515223019890E14L
};


extern long double MAXNUML, MAXLOGL;

long double sinhl(x)
long double x;
{
long double a;
long double fabsl(), expl(), polevll(), p1evll();

a = fabsl(x);
if( a > MAXLOGL )
	{
	mtherr( "sinhl", DOMAIN );
	if( x > 0 )
		return( MAXNUML );
	else
		return( -MAXNUML );
	}
if( a > 1.0L )
	{
	a = expl(a);
	a = 0.5L*a - (0.5L/a);
	if( x < 0 )
		a = -a;
	return(a);
	}

a *= a;
return( x + x * a * (polevll(a,P,5)/p1evll(a,Q,6)) );
}
