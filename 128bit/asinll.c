/*							asinl.c
 *
 *	Inverse circular sine, 128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, asinl();
 *
 * y = asinl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns radian angle between -pi/2 and +pi/2 whose sine is x.
 *
 * A rational function of the form x + x**3 P(x**2)/Q(x**2)
 * is used for |x| in the interval [0, 0.5].  If |x| > 0.5 it is
 * transformed by the identity
 *
 *    asin(x) = pi/2 - 2 asin( sqrt( (1-x)/2 ) ).
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -1, 1       100,000      3.7e-34      6.4e-35
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * asin domain        |x| > 1           0.0
 *
 */
/*							acosl()
 *
 *	Inverse circular cosine, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, acosl();
 *
 * y = acosl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns radian angle between -pi/2 and +pi/2 whose cosine
 * is x.
 *
 * Analytically, acos(x) = pi/2 - asin(x).  However if |x| is
 * near 1, there is cancellation error in subtracting asin(x)
 * from pi/2.  Hence if x < -0.5,
 *
 *    acos(x) =	 pi - 2.0 * asin( sqrt((1+x)/2) );
 *
 * or if x > +0.5,
 *
 *    acos(x) =	 2.0 * asin(  sqrt((1-x)/2) ).
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -1, 1       100,000      2.1e-34      5.6e-35
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * asin domain        |x| > 1           0.0
 */

/*							asin.c	*/

/*
Cephes Math Library Release 2.2:  December, 1990
Copyright 1984, 1990 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"

/* arcsin(x) = x + x^3 R(x^2)
 * Theoretical peak relative error = 3.1e-37
 * relative peak error spread = 9.4e-6
 */
static long double P[10] = {
-8.067112765482705313585175280952515549833E-1L,
 4.845649797786849136525020822000172350977E1L,
-8.510195404865297879959793548843395926847E2L,
 6.815196841370292688574521445731895826485E3L,
-2.967135182120339728996157454994675519735E4L,
 7.612250656518818109652985996692466409670E4L,
-1.183360579752620455689557157684221905030E5L,
 1.095432262510413338755837156377401348063E5L,
-5.554124580991113991999636773382495788705E4L,
 1.187132626694762543537732514905488896985E4L
};
static long double Q[10] = {
/*  1.000000000000000000000000000000000000000E0L, */
-8.005471061732009595694099899234272342478E1L,
 1.817324228942812880965069608562483918025E3L,
-1.867017317425756524289537002141956583706E4L,
 1.048196619402464497478959760337779705622E5L,
-3.527040897897253459022458866536165564103E5L,
 7.426302422018858001691440351763370029242E5L,
-9.863068411558756277454631976667880674474E5L,
 8.025654653926121907774766642393757364326E5L,
-3.653000557802254281954969843055623398839E5L,
 7.122795760168575261226395089432959614179E4L
};


long double asinl(x)
long double x;
{
long double a, p, z, zz;
long double ldexpl(), sqrtl(), polevll(), p1evll();
short sign, flag;
extern long double PIO2L;

if( x > 0 )
	{
	sign = 1;
	a = x;
	}
else
	{
	sign = -1;
	a = -x;
	}

if( a > 1.0L )
	{
	mtherr( "asinl", DOMAIN );
	return( 0.0L );
	}

if( a < 1.0e-18L )
	{
	z = a;
	goto done;
	}

if( a > 0.5L )
	{
	zz = 0.5L -a;
	zz = ldexpl( zz + 0.5L, -1 );
	z = sqrtl( zz );
	flag = 1;
	}
else
	{
	z = a;
	zz = z * z;
	flag = 0;
	}

p = zz * polevll( zz, P, 9)/p1evll( zz, Q, 10);
z = z * p + z;
if( flag != 0 )
	{
	z = z + z;
	z = PIO2L - z;
	}
done:
if( sign < 0 )
	z = -z;
return(z);
}


extern long double PIO2L, PIL;

long double acosl(x)
long double x;
{
long double asinl(), sqrtl();

if( x < -1.0L )
	goto domerr;

if( x < -0.5L) 
	return( PIL - 2.0L * asinl( sqrtl(0.5L*(1.0L+x)) ) );

if( x > 1.0L )
	{
domerr:	mtherr( "acosl", DOMAIN );
	return( 0.0L );
	}

if( x > 0.5L )
	return( 2.0L * asinl(  sqrtl(0.5L*(1.0L-x) ) ) );

return( PIO2L - asinl(x) );
}
