/*							atanhl.c
 *
 *	Inverse hyperbolic tangent, 128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, atanhl();
 *
 * y = atanhl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns inverse hyperbolic tangent of argument in the range
 * MINLOGL to MAXLOGL.
 *
 * If |x| < 0.5, the rational form x + x**3 P(x)/Q(x) is
 * employed.  Otherwise,
 *        atanh(x) = 0.5 * log( (1+x)/(1-x) ).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      -1,1       100,000      2.0e-34     4.6e-35
 *
 */



/*
Cephes Math Library Release 2.2:  January, 1991
Copyright (C) 1987, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"

/* atanh(x) = x + x^3 R(x^2)
 * Theoretical peak relative error = 7.0e-37
 * relative peak error spread = 2.7e-6
 */
static long double P[10] = {
-9.217569843805850417698565442251656375681E-1L,
 5.321929116410615470118183794063211260728E1L,
-9.139522976807685333981548145417830690552E2L,
 7.204314536952949779101646454146682033772E3L,
-3.097809640165146436529075324081668598891E4L,
 7.865376554210973897486215630898496100534E4L,
-1.211716814094785128366087489224821937203E5L,
 1.112669508789123834670923967462068457013E5L,
-5.600242872292477863751728708249167956542E4L,
 1.188901082233997739779618679364295772810E4L
};
static long double Q[] = {
/* 1.000000000000000000000000000000000000000E0L, */
-6.807348436010016270202879229504392062418E1L,
 1.386763299649315831625106608182196351693E3L,
-1.310805752656879543134785263832907269320E4L,
 6.872174720355764193772953852564737816928E4L,
-2.181008360536226513009076189881617939380E5L,
 4.362736119602298592874941767284979857248E5L,
-5.535251007539393347687001489396152923502E5L,
 4.321594849688346708841188057241308805551E5L,
-1.894075056489862952285849974761239845873E5L,
 3.566703246701993219338856038092901974725E4L
};


extern double MAXNUML;

long double atanhl(x)
long double x;
{
long double s, z;
long double fabsl(), logl(), polevll(), p1evll();

z = fabsl(x);
if( z >= 1.0L )
	{
	if( x == 1.0L )
		return( MAXNUML );
	if( x == -1.0L )
		return( -MAXNUML );
	mtherr( "atanhl", DOMAIN );
	return( MAXNUML );
	}

if( z < 1.0e-12L )
	return(x);

if( z < 0.5L )
	{
	z = x * x;
	s = x   +  x * z * (polevll(z, P, 9) / p1evll(z, Q, 10));
	return(s);
	}

return( 0.5L * logl((1.0L+x)/(1.0L-x)) );
}
