/*							expl.c
 *
 *	Exponential function, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, expl();
 *
 * y = expl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns e (2.71828...) raised to the x power.
 *
 * Range reduction is accomplished by separating the argument
 * into an integer k and fraction f such that
 *
 *     x    k  f
 *    e  = 2  e.
 *
 * A Pade' form of degree 2/3 is used to approximate exp(f) - 1
 * in the basic range [-0.5 ln 2, 0.5 ln 2].
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      +-10000     50000       1.12e-19    2.81e-20
 *
 *
 * Error amplification in the exponential function can be
 * a serious matter.  The error propagation involves
 * exp( X(1+delta) ) = exp(X) ( 1 + X*delta + ... ),
 * which shows that a 1 lsb error in representing X produces
 * a relative error of X times 1 lsb in the function.
 * While the routine gives an accurate result for arguments
 * that are exactly represented by a long double precision
 * computer number, the result contains amplified roundoff
 * error for large arguments not exactly represented.
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * exp underflow    x < MINLOG         0.0
 * exp overflow     x > MAXLOG         MAXNUM
 *
 */

/*
Cephes Math Library Release 2.7:  May, 1998
Copyright 1984, 1990, 1998 by Stephen L. Moshier
*/


/*	Exponential function	*/

#include "mconf.h"

#ifdef UNK
static long double P[3] = {
 1.2617719307481059087798E-4L,
 3.0299440770744196129956E-2L,
 9.9999999999999999991025E-1L,
};
static long double Q[4] = {
 3.0019850513866445504159E-6L,
 2.5244834034968410419224E-3L,
 2.2726554820815502876593E-1L,
 2.0000000000000000000897E0L,
};
static long double C1 = 6.9314575195312500000000E-1L;
static long double C2 = 1.4286068203094172321215E-6L;
#endif

#ifdef DEC
not supported in long double precision
#endif

#ifdef IBMPC
static short P[] = {
0x424e,0x225f,0x6eaf,0x844e,0x3ff2, XPD
0xf39e,0x5163,0x8866,0xf836,0x3ff9, XPD
0xfffe,0xffff,0xffff,0xffff,0x3ffe, XPD
};
static short Q[] = {
0xff1e,0xb2fc,0xb5e1,0xc975,0x3fec, XPD
0xff3e,0x45b5,0xcda8,0xa571,0x3ff6, XPD
0x9ee1,0x3f03,0x4cc4,0xe8b8,0x3ffc, XPD
0x0000,0x0000,0x0000,0x8000,0x4000, XPD
};
static short sc1[] = {0x0000,0x0000,0x0000,0xb172,0x3ffe, XPD};
#define C1 (*(long double *)sc1)
static short sc2[] = {0x4f1e,0xcd5e,0x8e7b,0xbfbe,0x3feb, XPD};
#define C2 (*(long double *)sc2)
#endif

#ifdef MIEEE
static long P[9] = {
0x3ff20000,0x844e6eaf,0x225f424e,
0x3ff90000,0xf8368866,0x5163f39e,
0x3ffe0000,0xffffffff,0xfffffffe,
};
static long Q[12] = {
0x3fec0000,0xc975b5e1,0xb2fcff1e,
0x3ff60000,0xa571cda8,0x45b5ff3e,
0x3ffc0000,0xe8b84cc4,0x3f039ee1,
0x40000000,0x80000000,0x00000000,
};
static long sc1[] = {0x3ffe0000,0xb1720000,0x00000000};
#define C1 (*(long double *)sc1)
static long sc2[] = {0x3feb0000,0xbfbe8e7b,0xcd5e4f1e};
#define C2 (*(long double *)sc2)
#endif

extern long double LOG2EL, MAXLOGL, MINLOGL, MAXNUML;
#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double floorl ( long double );
extern long double ldexpl ( long double, int );
extern int isnanl ( long double );
#else
long double polevll(), floorl(), ldexpl(), isnanl();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif

long double expl(x)
long double x;
{
long double px, xx;
int n;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
if( x > MAXLOGL)
	{
#ifdef INFINITIES
	return( INFINITYL );
#else
	mtherr( "expl", OVERFLOW );
	return( MAXNUML );
#endif
	}

if( x < MINLOGL )
	{
#ifndef INFINITIES
	mtherr( "expl", UNDERFLOW );
#endif
	return(0.0L);
	}

/* Express e**x = e**g 2**n
 *   = e**g e**( n loge(2) )
 *   = e**( g + n loge(2) )
 */
px = floorl( LOG2EL * x + 0.5L ); /* floor() truncates toward -infinity. */
n = px;
x -= px * C1;
x -= px * C2;


/* rational approximation for exponential
 * of the fractional part:
 * e**x =  1 + 2x P(x**2)/( Q(x**2) - P(x**2) )
 */
xx = x * x;
px = x * polevll( xx, P, 2 );
x =  px/( polevll( xx, Q, 3 ) - px );
x = 1.0L + ldexpl( x, 1 );

x = ldexpl( x, n );
return(x);
}
