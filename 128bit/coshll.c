/*							coshl.c
 *
 *	Hyperbolic cosine, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, coshl();
 *
 * y = coshl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns hyperbolic cosine of argument in the range MINLOGL to
 * MAXLOGL.
 *
 * cosh(x)  =  ( exp(x) + exp(-x) )/2.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     +-10000      26,000       2.5e-34     8.6e-35
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * cosh overflow    |x| > MAXLOGL       MAXNUML
 *
 *
 */


/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1985, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"
extern long double MAXLOGL, MAXNUML;

long double coshl(x)
long double x;
{
long double y;
long double expl(), ldexpl();

if( x < 0 )
	x = -x;
if( x > MAXLOGL )
	{
	mtherr( "coshl", OVERFLOW );
	return( MAXNUML );
	}	
y = expl(x);
y = y + 1.0L/y;
y = ldexpl( y, -1 );
return( y );
}
