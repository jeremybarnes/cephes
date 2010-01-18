/*							asinhl.c
 *
 *	Inverse hyperbolic sine, 128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, asinhl();
 *
 * y = asinhl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns inverse hyperbolic sine of argument.
 *
 * If |x| < 0.5, the function is approximated by a rational
 * form  x + x**3 P(x)/Q(x).  Otherwise,
 *
 *     asinh(x) = log( x + sqrt(1 + x*x) ).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -2,2         100,000     2.8e-34     6.7e-35
 *
 */


/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/


#include "mconf.h"

/* arcsinh(x) = x + x^3 R(x^2)
 * Theoretical peak relative error = 8.4e-36
 * relative peak error spread = 1.4e-7
 */
static long double P[9] = {
-8.104404283317298189545629468767571317688E-1L,
-4.954206127425209147110732546633675599008E1L,
-8.438175619831548439550086251740438689853E2L,
-6.269710069245210459536983820505214648057E3L,
-2.418935474493501382372711518024193326434E4L,
-5.208121780431312783866941311277024486498E4L,
-6.302755086521614763280617114866439227971E4L,
-4.003566436224198252093684987323233921339E4L,
-1.037690841528359305134494613113086980551E4L
};
static long double Q[9] = {
/* 1.000000000000000000000000000000000000000E0L, */
 8.175806439951395194771977809279448392548E1L,
 1.822215299975696008284027212745010251320E3L,
 1.772040003462901790853111853838978236828E4L,
 9.077625379864046240143413577745818879353E4L,
 2.675554475070211205153169988669677418808E5L,
 4.689758557916492969463473819426544383586E5L,
 4.821923684550711724710891114802924039911E5L,
 2.682316388947175963642524537892687560973E5L,
 6.226145049170155830806967678679167550122E4L
};


extern long double LOGE2L;

long double asinhl(x)
long double x;
{
long double a, z;
int sign;
long double logl(), sqrtl(), polevll(), p1evll();

if( x < 0 )
	{
	sign = -1;
	x = -x;
	}
else
	sign = 1;

if( x > 1.0e10L )
	return( sign * (logl(x) + LOGE2L) );

z = x * x;
if( x < 0.5 )
	{
	a = ( polevll(z, P, 8)/p1evll(z, Q, 9) ) * z;
	a = a * x  +  x;
	if( sign < 0 )
		a = -a;
	return(a);
	}	

a = sqrtl( z + 1.0L );
return( sign * logl(x + a) );
}
