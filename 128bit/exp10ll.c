/*							exp10l.c
 *
 *	Base 10 exponential function, long double precision
 *      (Common antilogarithm)
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, exp10l()
 *
 * y = exp10l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns 10 raised to the x power.
 *
 * Range reduction is accomplished by expressing the argument
 * as 10**x = 2**n 10**f, with |f| < 0.5 log10(2).
 * The Pade' form
 *
 *     1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 *
 * is used to approximate 10**f.
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      +-4900     100,000      2.1e-34     4.7e-35
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * exp10l underflow    x < -MAXL10        0.0
 * exp10l overflow     x > MAXL10       MAXNUM
 *
 * IEEE arithmetic: MAXL10 = 4932.0754489586679023819
 *
 */

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/


#include "mconf.h"

static long double P[6] = {
 6.781965388610215141646963666801877147888E1L,
 4.930988843306627886355612005613845141123E4L,
 9.112966716416345527154611203937593471620E6L,
 5.880306836049276068401249115246879608067E8L,
 1.294143447497151402129871056524193102276E10L,
 6.737236378815985929063482575381049393067E10L
};
static long double Q[6] = {
/* 1.000000000000000000000000000000000000000E0L, */
 2.269602544366008200564158516293459788943E3L,
 7.712352920905011963059413773034169405418E5L,
 8.312829542416079818945631366865677745737E7L,
 3.192530874297321568824835872165913128965E9L,
 3.709588725051672862074295071447979432510E10L,
 5.851889165195258152098281616369230806944E10L
};
/*static long double LOG102 = 3.0102999566398119521373889e-1L;*/
static long double LOG210 = 3.321928094887362347870319429489390175864831L;
static long double LG102A = 3.01025390625e-1L;
static long double LG102B = 4.6050389811952137388947244930267681898814621E-6L;
static long double MAXL10 =  4.932075448958667902381898051166093750570023E3L;
extern long double MAXNUML;



long double exp10l(x)
long double x;
{
long double px, xx;
short n;
long double floorl(), ldexpl(), polevll(), p1evll();

if( x > MAXL10 )
	{
	mtherr( "exp10l", OVERFLOW );
	return( MAXNUML );
	}

if( x < -MAXL10 )	/* Would like to use MINLOG but can't */
	{
	mtherr( "exp10l", UNDERFLOW );
	return(0.0L);
	}

/* Express 10**x = 10**g 2**n
 *   = 10**g 10**( n log10(2) )
 *   = 10**( g + n log10(2) )
 */
px = floorl( LOG210 * x + 0.5L );
n = px;
x -= px * LG102A;
x -= px * LG102B;

/* rational approximation for exponential
 * of the fractional part:
 * 10**x = 1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 */
xx = x * x;
px = x * polevll( xx, P, 5 );
x =  px/( p1evll( xx, Q, 6 ) - px );
x = 1.0L + ldexpl( x, 1 );

/* multiply by power of 2 */
x = ldexpl( x, n );
return(x);
}
