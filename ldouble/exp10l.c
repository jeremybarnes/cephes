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
 *    IEEE      +-4900      30000       1.0e-19     2.7e-20
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

#ifdef UNK
static long double P[] = {
 3.1341179396892496811523E1L,
 4.5618283154904699073999E3L,
 1.3433113468542797218610E5L,
 7.6025447914440301593592E5L,
};
static long double Q[] = {
/* 1.0000000000000000000000E0,*/
 4.7705440288425157637739E2L,
 2.9732606548049614870598E4L,
 4.0843697951001026189583E5L,
 6.6034865026929015925608E5L,
};
/*static long double LOG102 = 3.0102999566398119521373889e-1L;*/
static long double LOG210 = 3.3219280948873623478703L;
static long double LG102A = 3.01025390625e-1L;
static long double LG102B = 4.6050389811952137388947e-6L;
#endif


#ifdef IBMPC
static short P[] = {
0x399a,0x7dc7,0xbc43,0xfaba,0x4003, XPD
0xb526,0xdf32,0xa063,0x8e8e,0x400b, XPD
0x18da,0xafa1,0xc89e,0x832e,0x4010, XPD
0x503d,0x9352,0xe7aa,0xb99b,0x4012, XPD
};
static short Q[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0x947d,0x7855,0xf6ac,0xee86,0x4007, XPD
0x18cf,0x7749,0x368d,0xe849,0x400d, XPD
0x85be,0x2560,0x9f58,0xc76e,0x4011, XPD
0x6d3c,0x80c5,0xca67,0xa137,0x4012, XPD
};
/*
static short L102[] = {0xf799,0xfbcf,0x9a84,0x9a20,0x3ffd, XPD};
#define LOG102 *(long double *)L102
*/
static short L210[] = {0x8afe,0xcd1b,0x784b,0xd49a,0x4000, XPD};
#define LOG210 *(long double *)L210
static short L102A[] = {0x0000,0x0000,0x0000,0x9a20,0x3ffd, XPD};
#define LG102A *(long double *)L102A
static short L102B[] = {0x8f89,0xf798,0xfbcf,0x9a84,0x3fed, XPD};
#define LG102B *(long double *)L102B
#endif

#ifdef MIEEE
static long P[] = {
0x40030000,0xfababc43,0x7dc7399a,
0x400b0000,0x8e8ea063,0xdf32b526,
0x40100000,0x832ec89e,0xafa118da,
0x40120000,0xb99be7aa,0x9352503d,
};
static long Q[] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x40070000,0xee86f6ac,0x7855947d,
0x400d0000,0xe849368d,0x774918cf,
0x40110000,0xc76e9f58,0x256085be,
0x40120000,0xa137ca67,0x80c56d3c,
};
/*
static long L102[] = {0x3ffd0000,0x9a209a84,0xfbcff799};
#define LOG102 *(long double *)L102
*/
static long L210[] = {0x40000000,0xd49a784b,0xcd1b8afe};
#define LOG210 *(long double *)L210
static long L102A[] = {0x3ffd0000,0x9a200000,0x00000000};
#define LG102A *(long double *)L102A
static long L102B[] = {0x3fed0000,0x9a84fbcf,0xf7988f89};
#define LG102B *(long double *)L102B
#endif

static long double MAXL10 = 4.9320754489586679023819e3L;
extern long double MAXNUML;
#ifdef ANSIPROT
extern long double floorl ( long double );
extern long double ldexpl ( long double, int );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern int isnanl ( long double );
#else
long double floorl(), ldexpl(), polevll(), p1evll(), isnanl();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif

long double exp10l(x)
long double x;
{
long double px, xx;
short n;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
if( x > MAXL10 )
	{
#ifdef INFINITIES
	return( INFINITYL );
#else
	mtherr( "exp10l", OVERFLOW );
	return( MAXNUML );
#endif
	}

if( x < -MAXL10 )	/* Would like to use MINLOG but can't */
	{
#ifndef INFINITIES
	mtherr( "exp10l", UNDERFLOW );
#endif
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
px = x * polevll( xx, P, 3 );
x =  px/( p1evll( xx, Q, 4 ) - px );
x = 1.0L + ldexpl( x, 1 );

/* multiply by power of 2 */
x = ldexpl( x, n );
return(x);
}
