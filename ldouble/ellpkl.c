/*							ellpkl.c
 *
 *	Complete elliptic integral of the first kind
 *
 *
 *
 * SYNOPSIS:
 *
 * long double m1, y, ellpkl();
 *
 * y = ellpkl( m1 );
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *
 *
 *            pi/2
 *             -
 *            | |
 *            |           dt
 * K(m)  =    |    ------------------
 *            |                   2
 *          | |    sqrt( 1 - m sin t )
 *           -
 *            0
 *
 * where m = 1 - m1, using the approximation
 *
 *     P(x)  -  log x Q(x).
 *
 * The argument m1 is used rather than m so that the logarithmic
 * singularity at m = 1 will be shifted to the origin; this
 * preserves maximum accuracy.
 *
 * K(0) = pi/2.
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       0,1        10000       1.1e-19     3.3e-20
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * ellpkl domain      x<0, x>1           0.0
 *
 */

/*							ellpkl.c */


/*
Cephes Math Library, Release 2.3:  October, 1995
Copyright 1984, 1987, 1995 by Stephen L. Moshier
*/

#include "mconf.h"

#if UNK
static long double P[13] = {
 1.247539729154838838628E-6L,
 2.149421654232011240659E-4L,
 2.265267575136470585139E-3L,
 6.723088676584254248821E-3L,
 8.092066790639263075808E-3L,
 5.664069509748147028621E-3L,
 4.579865994050801042865E-3L,
 5.797368411662027645234E-3L,
 8.767698209432225911803E-3L,
 1.493761594388688915057E-2L,
 3.088514457872042326871E-2L,
 9.657359027999314232753E-2L,
 1.386294361119890618992E0L,
};
static long double Q[12] = {
 5.568631677757315398993E-5L,
 1.036110372590318802997E-3L,
 5.500459122138244213579E-3L,
 1.337330436245904844528E-2L,
 2.033103735656990487115E-2L,
 2.522868345512332304268E-2L,
 3.026786461242788135379E-2L,
 3.738370118296930305919E-2L,
 4.882812208418620146046E-2L,
 7.031249999330222751046E-2L,
 1.249999999999978263154E-1L,
 4.999999999999999999924E-1L,
};
static long double C1 = 1.386294361119890618834L; /* log(4) */
#endif
#if IBMPC
static short P[] = {
0xf098,0xad01,0x2381,0xa771,0x3feb, XPD
0xd6ed,0xea22,0x1922,0xe162,0x3ff2, XPD
0x3733,0xe2f1,0xe226,0x9474,0x3ff6, XPD
0x3031,0x3c9d,0x5aff,0xdc4d,0x3ff7, XPD
0x9a46,0x4310,0x968e,0x8494,0x3ff8, XPD
0xbe4c,0x3ff2,0xa8a7,0xb999,0x3ff7, XPD
0xf35c,0x0eaf,0xb355,0x9612,0x3ff7, XPD
0xbc56,0x8fd4,0xd9dd,0xbdf7,0x3ff7, XPD
0xc01e,0x867f,0x6444,0x8fa6,0x3ff8, XPD
0x4ba3,0x6392,0xe6fd,0xf4bc,0x3ff8, XPD
0x62c3,0xbb12,0xd7bc,0xfd02,0x3ff9, XPD
0x08fe,0x476c,0x5fdf,0xc5c8,0x3ffb, XPD
0x79ad,0xd1cf,0x17f7,0xb172,0x3fff, XPD
};
static short Q[] = {
0x96a4,0x8474,0xba33,0xe990,0x3ff0, XPD
0xe5a7,0xa50e,0x1854,0x87ce,0x3ff5, XPD
0x8999,0x72e3,0x3205,0xb43d,0x3ff7, XPD
0x3255,0x13eb,0xb438,0xdb1b,0x3ff8, XPD
0xb717,0x497f,0x4691,0xa68d,0x3ff9, XPD
0x30be,0x8c6b,0x624b,0xceac,0x3ff9, XPD
0xa858,0x2a0d,0x5014,0xf7f4,0x3ff9, XPD
0x8615,0xbfa6,0xa6df,0x991f,0x3ffa, XPD
0x103c,0xa076,0xff37,0xc7ff,0x3ffa, XPD
0xf508,0xc515,0xffff,0x8fff,0x3ffb, XPD
0x1af5,0xfffb,0xffff,0xffff,0x3ffb, XPD
0x0000,0x0000,0x0000,0x8000,0x3ffe, XPD
};
static unsigned short ac1[] = {
0x79ac,0xd1cf,0x17f7,0xb172,0x3fff, XPD
};
#define C1 (*(long double *)ac1)
#endif

#ifdef MIEEE
static long P[39] = {
0x3feb0000,0xa7712381,0xad01f098,
0x3ff20000,0xe1621922,0xea22d6ed,
0x3ff60000,0x9474e226,0xe2f13733,
0x3ff70000,0xdc4d5aff,0x3c9d3031,
0x3ff80000,0x8494968e,0x43109a46,
0x3ff70000,0xb999a8a7,0x3ff2be4c,
0x3ff70000,0x9612b355,0x0eaff35c,
0x3ff70000,0xbdf7d9dd,0x8fd4bc56,
0x3ff80000,0x8fa66444,0x867fc01e,
0x3ff80000,0xf4bce6fd,0x63924ba3,
0x3ff90000,0xfd02d7bc,0xbb1262c3,
0x3ffb0000,0xc5c85fdf,0x476c08fe,
0x3fff0000,0xb17217f7,0xd1cf79ad,
};
static long Q[36] = {
0x3ff00000,0xe990ba33,0x847496a4,
0x3ff50000,0x87ce1854,0xa50ee5a7,
0x3ff70000,0xb43d3205,0x72e38999,
0x3ff80000,0xdb1bb438,0x13eb3255,
0x3ff90000,0xa68d4691,0x497fb717,
0x3ff90000,0xceac624b,0x8c6b30be,
0x3ff90000,0xf7f45014,0x2a0da858,
0x3ffa0000,0x991fa6df,0xbfa68615,
0x3ffa0000,0xc7ffff37,0xa076103c,
0x3ffb0000,0x8fffffff,0xc515f508,
0x3ffb0000,0xffffffff,0xfffb1af5,
0x3ffe0000,0x80000000,0x00000000,
};
static unsigned long ac1[] = {
0x3fff0000,0xb17217f7,0xd1cf79ac
};
#define C1 (*(long double *)ac1)
#endif


#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double logl ( long double );
#else
long double polevll(), logl();
#endif
extern long double MACHEPL, MAXNUML;

long double ellpkl(x)
long double x;
{

if( (x < 0.0L) || (x > 1.0L) )
	{
	mtherr( "ellpkl", DOMAIN );
	return( 0.0L );
	}

if( x > MACHEPL )
	{
	return( polevll(x,P,12) - logl(x) * polevll(x,Q,11) );
	}
else
	{
	if( x == 0.0L )
		{
		mtherr( "ellpkl", SING );
		return( MAXNUML );
		}
	else
		{
		return( C1 - 0.5L * logl(x) );
		}
	}
}
