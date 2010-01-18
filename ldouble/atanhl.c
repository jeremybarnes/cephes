/*							atanhl.c
 *
 *	Inverse hyperbolic tangent, long double precision
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
 *    IEEE      -1,1        30000       1.1e-19     3.3e-20
 *
 */



/*
Cephes Math Library Release 2.7:  May, 1998
Copyright (C) 1987, 1991, 1998 by Stephen L. Moshier
*/

#include "mconf.h"

#ifdef UNK
static long double P[] = {
 2.9647757819596835680719E-3L,
-8.0026596513099094380633E-1L,
 7.7920941408493040219831E0L,
-2.4330686602187898836837E1L,
 3.0204265014595622991082E1L,
-1.2961142942114056581210E1L,
};
static long double Q[] = {
/* 1.0000000000000000000000E0L,*/
-1.3729634163247557081869E1L,
 6.2320841104088512332185E1L,
-1.2469344457045341444078E2L,
 1.1394285233959210574352E2L,
-3.8883428826342169425890E1L,
};
#endif

#ifdef IBMPC
static short P[] = {
0x3aa2,0x036b,0xaf06,0xc24c,0x3ff6, XPD
0x528e,0x56e8,0x3af4,0xccde,0xbffe, XPD
0x9d89,0xc9a1,0xd5cf,0xf958,0x4001, XPD
0xa653,0x6cfa,0x3f04,0xc2a5,0xc003, XPD
0xc651,0x2b3d,0x55b2,0xf1a2,0x4003, XPD
0xd76d,0xf293,0xd76b,0xcf60,0xc002, XPD
};
static short Q[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0xd1b9,0x5314,0x94df,0xdbac,0xc002, XPD
0x3caa,0x0517,0x8a92,0xf948,0x4004, XPD
0x535e,0xaf5f,0x0b2a,0xf963,0xc005, XPD
0xa6f9,0xb702,0xbd8a,0xe3e2,0x4005, XPD
0xe136,0xf5ee,0xa190,0x9b88,0xc004, XPD
};
#endif

#ifdef MIEEE
static long P[] = {
0x3ff60000,0xc24caf06,0x036b3aa2,
0xbffe0000,0xccde3af4,0x56e8528e,
0x40010000,0xf958d5cf,0xc9a19d89,
0xc0030000,0xc2a53f04,0x6cfaa653,
0x40030000,0xf1a255b2,0x2b3dc651,
0xc0020000,0xcf60d76b,0xf293d76d,
};
static long Q[] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0xc0020000,0xdbac94df,0x5314d1b9,
0x40040000,0xf9488a92,0x05173caa,
0xc0050000,0xf9630b2a,0xaf5f535e,
0x40050000,0xe3e2bd8a,0xb702a6f9,
0xc0040000,0x9b88a190,0xf5eee136,
};
#endif

extern long double MAXNUML;
#ifdef ANSIPROT
extern long double fabsl ( long double );
extern long double logl ( long double );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
#else
long double fabsl(), logl(), polevll(), p1evll();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif

long double atanhl(x)
long double x;
{
long double s, z;

#ifdef MINUSZERO
if( x == 0.0L )
	return(x);
#endif
z = fabsl(x);
if( z >= 1.0L )
	{
	if( x == 1.0L )
		{
#ifdef INFINITIES
		return( INFINITYL );
#else
		return( MAXNUML );
#endif
		}
	if( x == -1.0L )
		{
#ifdef INFINITIES
		return( -INFINITYL );
#else
		return( -MAXNUML );
#endif
		}
	mtherr( "atanhl", DOMAIN );
#ifdef NANS
	return( NANL );
#else
	return( MAXNUML );
#endif
	}

if( z < 1.0e-8L )
	return(x);

if( z < 0.5L )
	{
	z = x * x;
	s = x   +  x * z * (polevll(z, P, 5) / p1evll(z, Q, 5));
	return(s);
	}

return( 0.5L * logl((1.0L+x)/(1.0L-x)) );
}
