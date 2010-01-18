/*							powl.c
 *
 *	Power function, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, z, powl();
 *
 * z = powl( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Computes x raised to the yth power.  For noninteger y,
 *
 *      x^y  =  exp2( y log2(x) ).
 *
 * using the base 2 logarithm and exponential functions.  If y
 * is an integer, |y| < 32768, the function is computed by powil.
 *
 *
 *
 * ACCURACY:
 *
 * The relative error of pow(x,y) can be estimated
 * by   y dl ln(2),   where dl is the absolute error of
 * the internally computed base 2 logarithm.
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *
 *    IEEE     +-1000      100,000     1.0e-30      1.4e-31
 * .001 < x < 1000, with log(x) uniformly distributed.
 * -1000 < y < 1000, y uniformly distributed.
 *
 *    IEEE     0,8700      100,000     1.4e-30      3.1e-31
 * 0.99 < x < 1.01, 0 < y < 8700, uniformly distributed.
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * pow overflow     x^y > MAXNUM      MAXNUM
 * pow underflow   x^y < 1/MAXNUM       0.0
 * pow domain      x<0 and y noninteger  0.0
 *
 */

#include "mconf.h"
long double exp2l(), log2l();
long double expl(), logl(), powil(), floorl(), fabsl(), ldexpl();

long double powl(x, y)
long double x, y;
{
long double w, z;
int nflg;

nflg = 0;	/* flag = 1 if x<0 raised to integer power */
w = floorl(y);
if( (w == y) && (fabsl(w) < 32768.0L) )
	{
	w = powil( x, (int )w );
	return( w );
	}


if( x <= 0.0L )
	{
	if( x == 0.0L )
		{
		if( y == 0.0L )
			return( 1.0L );  /*   0**0   */
		else  
			return( 0.0L );  /*   0**y   */
		}
	else
		{
		if( w != y )
			{ /* noninteger power of negative number */
			mtherr( "powl", DOMAIN );
			return(0.0L);
			}
/* For negative x,
 * find out if the integer exponent
 * is odd or even.
 */
		w = ldexpl( y, -1 );
		w = floorl(w);
		w = ldexpl( w, 1 );
		if( w != y )
		  nflg = 1;
		x = fabsl(x);
		}
	}

z = log2l(x);
z = exp2l( y*z);
if( nflg )
	z = -z; /* odd exponent */
return(z);
}
