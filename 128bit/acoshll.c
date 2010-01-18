/*							acoshl.c
 *
 *	Inverse hyperbolic cosine, 128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, acoshl();
 *
 * y = acoshl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns inverse hyperbolic cosine of argument.
 *
 * If 1 <= x < 1.5, a rational approximation
 *
 *	sqrt(2z) * P(z)/Q(z)
 *
 * where z = x-1, is used.  Otherwise,
 *
 * acosh(x)  =  log( x + sqrt( (x-1)(x+1) ).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      1,3        100,000      4.1e-34     7.3e-35
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * acoshl domain      |x| < 1            0.0
 *
 */

/*							acosh.c	*/

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/


#include "mconf.h"

/* acosh(1+x) = sqrt(2x) * R(x), interval 0 < x < 0.5
 * Theoretical peak relative error = 8.2e-36
 * relative peak error spread = 9.9e-8
 */
static long double P[10] = {
 1.895467874386341763387398084072833727168E-1L,
 6.443902084393244878979969557171256604767E1L,
 3.914593556594721458616408528941154205393E3L,
 9.164040999602964494412169748897754668733E4L,
 1.065909694792026382660307834723001543839E6L,
 6.899169896709615182428217047370629406305E6L,
 2.599781868717579447900896150777162652518E7L,
 5.663733059389964024656501196827345337766E7L,
 6.606302846870644033621560858582696134512E7L,
 3.190482951215438078279772140481195200593E7L
};
static long double Q[9] = {
/* 1.000000000000000000000000000000000000000E0L, */
 1.635418024331924674147953764918262009321E2L,
 7.290983678312632723073455563799692165828E3L,
 1.418207894088607063257675159183397062114E5L,
 1.453154285419072886840913424715826321357E6L,
 8.566841438576725234955968880501739464425E6L,
 3.003448667795089562511136059766833630017E7L,
 6.176592872899557661256383958395266919654E7L,
 6.872176426138597206811541870289420510034E7L,
 3.190482951215438078279772140481195226621E7L
};


extern long double LOGE2L;

long double acoshl(x)
long double x;
{
long double a, z;
long double logl(), sqrtl(), polevll(), p1evll();

if( x < 1.0L )
	{
	mtherr( "acoshl", DOMAIN );
	return(0.0L);
	}

if( x > 1.0e17L )
	return( logl(x) + LOGE2L );

z = x - 1.0L;

if( z < 0.5L )
	{
	a = sqrtl(2.0L*z) * (polevll(z, P, 9) / p1evll(z, Q, 9) );
	return( a );
	}

a = sqrtl( z*(x+1.0L) );
return( logl(x + a) );
}
