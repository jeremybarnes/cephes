/*							sqrt.c
 *
 *	Square root
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, sqrt();
 *
 * y = sqrt( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the square root of x.
 *
 * Range reduction involves isolating the power of two of the
 * argument and using a polynomial approximation to obtain
 * a rough value for the square root.  Then Heron's iteration
 * is used three times to converge to an accurate value.
 *
 *
 *
 * ACCURACY:
 *
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    DEC       0, 30        2000       2.1e-17     5.2e-18
 *    IEEE      0,1.7e308   30000       1.7e-16     6.3e-17
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * sqrt domain        x < 0            0.0
 *
 */

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1988 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/


#include "mconf.h"

extern double SQRT2;  /*  SQRT2 = 1.41421356237309504880 */
double frexp(), ldexp();

double sqrt(x)
double x;
{
int e;
double z, w;

if( x <= 0.0 )
	{
	if( x < 0.0 )
		mtherr( "sqrt", DOMAIN );
	return( 0.0 );
	}
w = x;

/* Separate exponent and significand
 * frexp and ldexp are used in order to
 * handle denormal numbers properly.
 */
z = frexp( x, &e );

/* Approximate square root of number between 0.5 and 1.
 * Relative error of approximation = 7.47e-3.
 */
x = 4.173075996388649989089E-1 + 5.9016206709064458299663E-1 * z;

/* adjust for odd powers of 2 */
if( (e & 1) != 0 )
	x *= SQRT2;

/* re-insert exponent */
x = ldexp( x, (e >> 1) );

/* Newton iterations: */
x = 0.5*(x + w/x);
x = 0.5*(x + w/x);
x = 0.5*(x + w/x);

return(x);
}
