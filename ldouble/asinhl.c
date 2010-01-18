/*							asinhl.c
 *
 *	Inverse hyperbolic sine, long double precision
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
 *    IEEE     -3,3         30000       1.7e-19     3.5e-20
 *
 */


/*
Cephes Math Library Release 2.7:  May, 1998
Copyright 1984, 1991, 1998 by Stephen L. Moshier
*/


#include "mconf.h"

#ifdef UNK
static long double P[] = {
-7.2157234864927687427374E-1L,
-1.3005588097490352458918E1L,
-5.9112383795679709212744E1L,
-9.5372702442289028811361E1L,
-4.9802880260861844539014E1L,
};
static long double Q[] = {
/* 1.0000000000000000000000E0L,*/
 2.8754968540389640419671E1L,
 2.0990255691901160529390E2L,
 5.9265075560893800052658E2L,
 7.0670399135805956780660E2L,
 2.9881728156517107462943E2L,
};
#endif


#ifdef IBMPC
static short P[] = {
0x8f42,0x2584,0xf727,0xb8b8,0xbffe, XPD
0x9d56,0x7f7c,0xe38b,0xd016,0xc002, XPD
0xc518,0xdc2d,0x14bc,0xec73,0xc004, XPD
0x99fe,0xc18a,0xd2da,0xbebe,0xc005, XPD
0xb46c,0x3c05,0x263e,0xc736,0xc004, XPD
};
static short Q[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0xdfed,0x33db,0x2cf2,0xe60a,0x4003, XPD
0xf109,0x61ee,0x0df8,0xd1e7,0x4006, XPD
0xf21e,0xda84,0xa5fa,0x9429,0x4008, XPD
0x13fc,0xc4e2,0x0e31,0xb0ad,0x4008, XPD
0x485c,0xad04,0x9cae,0x9568,0x4007, XPD
};
#endif

#ifdef MIEEE
static long P[] = {
0xbffe0000,0xb8b8f727,0x25848f42,
0xc0020000,0xd016e38b,0x7f7c9d56,
0xc0040000,0xec7314bc,0xdc2dc518,
0xc0050000,0xbebed2da,0xc18a99fe,
0xc0040000,0xc736263e,0x3c05b46c,
};
static long Q[] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40030000,0xe60a2cf2,0x33dbdfed,
0x40060000,0xd1e70df8,0x61eef109,
0x40080000,0x9429a5fa,0xda84f21e,
0x40080000,0xb0ad0e31,0xc4e213fc,
0x40070000,0x95689cae,0xad04485c,
};
#endif

extern long double LOGE2L;
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef ANSIPROT
extern long double logl ( long double );
extern long double sqrtl ( long double );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern int isnanl ( long double );
extern int isfinitel ( long double );
#else
long double logl(), sqrtl(), polevll(), p1evll(), isnanl(), isfinitel();
#endif

long double asinhl(x)
long double x;
{
long double a, z;
int sign;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
#ifdef MINUSZERO
if( x == 0.0L )
	return(x);
#endif
#ifdef INFINITIES
	if( !isfinitel(x) )
	    return(x);
#endif
if( x < 0.0L )
	{
	sign = -1;
	x = -x;
	}
else
	sign = 1;

if( x > 1.0e10L )
	{
	return( sign * (logl(x) + LOGE2L) );
	}

z = x * x;
if( x < 0.5L )
	{
	a = ( polevll(z, P, 4)/p1evll(z, Q, 5) ) * z;
	a = a * x  +  x;
	if( sign < 0 )
		a = -a;
	return(a);
	}	

a = sqrtl( z + 1.0L );
return( sign * logl(x + a) );
}
