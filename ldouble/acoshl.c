/*							acoshl.c
 *
 *	Inverse hyperbolic cosine, long double precision
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
 *    IEEE      1,3         30000       2.0e-19     3.9e-20
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
Cephes Math Library Release 2.7:  May, 1998
Copyright 1984, 1991, 1998 by Stephen L. Moshier
*/


/* acosh(1+x) = sqrt(2x) * R(x), interval 0 < x < 0.5 */

#include "mconf.h"

#ifdef UNK
static long double P[] = {
 2.9071989653343333587238E-5L,
 3.2906030801088967279449E-3L,
 6.3034445964862182128388E-2L,
 4.1587081802731351459504E-1L,
 1.0989714347599256302467E0L,
 9.9999999999999999999715E-1L,
};
static long double Q[] = {
 1.0443462486787584738322E-4L,
 6.0085845375571145826908E-3L,
 8.7750439986662958343370E-2L,
 4.9564621536841869854584E-1L,
 1.1823047680932589605190E0L,
 1.0000000000000000000028E0L,
};
#endif


#ifdef IBMPC
static unsigned short P[] = {
0x4536,0x4dba,0x9f55,0xf3df,0x3fef, XPD
0x23a5,0xf9aa,0x289c,0xd7a7,0x3ff6, XPD
0x7e8b,0x8645,0x341f,0x8118,0x3ffb, XPD
0x0fd5,0x937f,0x0515,0xd4ed,0x3ffd, XPD
0x2364,0xc41b,0x1891,0x8cab,0x3fff, XPD
0x0000,0x0000,0x0000,0x8000,0x3fff, XPD
};
static short Q[] = {
0x1e7c,0x4f16,0xe98c,0xdb03,0x3ff1, XPD
0xc319,0xc272,0xa90a,0xc4e3,0x3ff7, XPD
0x2f83,0x9e5e,0x80af,0xb3b6,0x3ffb, XPD
0xe1e0,0xc97c,0x573a,0xfdc5,0x3ffd, XPD
0xcdf2,0x6ec5,0xc33c,0x9755,0x3fff, XPD
0x0000,0x0000,0x0000,0x8000,0x3fff, XPD
};
#endif

#ifdef MIEEE
static long P[] = {
0x3fef0000,0xf3df9f55,0x4dba4536,
0x3ff60000,0xd7a7289c,0xf9aa23a5,
0x3ffb0000,0x8118341f,0x86457e8b,
0x3ffd0000,0xd4ed0515,0x937f0fd5,
0x3fff0000,0x8cab1891,0xc41b2364,
0x3fff0000,0x80000000,0x00000000,
};
static long Q[] = {
0x3ff10000,0xdb03e98c,0x4f161e7c,
0x3ff70000,0xc4e3a90a,0xc272c319,
0x3ffb0000,0xb3b680af,0x9e5e2f83,
0x3ffd0000,0xfdc5573a,0xc97ce1e0,
0x3fff0000,0x9755c33c,0x6ec5cdf2,
0x3fff0000,0x80000000,0x00000000,
};
#endif

extern long double LOGE2L;
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif
#ifdef ANSIPROT
extern long double logl ( long double );
extern long double sqrtl ( long double );
extern long double polevll ( long double, void *, int );
extern int isnanl ( long double );
#else
long double logl(), sqrtl(), polevll(), isnanl();
#endif

long double acoshl(x)
long double x;
{
long double a, z;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
if( x < 1.0L )
	{
	mtherr( "acoshl", DOMAIN );
#ifdef NANS
	return(NANL);
#else
	return(0.0L);
#endif
	}

if( x > 1.0e10 )
	{
#ifdef INFINITIES
	if( x == INFINITYL )
		return( INFINITYL );
#endif
	return( logl(x) + LOGE2L );
	}

z = x - 1.0L;

if( z < 0.5L )
	{
	a = sqrtl(2.0L*z) * (polevll(z, P, 5) / polevll(z, Q, 5) );
	return( a );
	}

a = sqrtl( z*(x+1.0L) );
return( logl(x + a) );
}
