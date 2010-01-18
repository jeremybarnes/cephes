/*							ndtrl.c
 *
 *	Normal distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, ndtrl();
 *
 * y = ndtrl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the area under the Gaussian probability density
 * function, integrated from minus infinity to x:
 *
 *                            x
 *                             -
 *                   1        | |          2
 *    ndtr(x)  = ---------    |    exp( - t /2 ) dt
 *               sqrt(2pi)  | |
 *                           -
 *                          -inf.
 *
 *             =  ( 1 + erf(z) ) / 2
 *             =  erfc(z) / 2
 *
 * where z = x/sqrt(2). Computation is via the functions
 * erf and erfc with care to avoid error amplification in computing exp(-x^2).
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     -13,0        30000       7.7e-19     1.0e-19
 *    IEEE     -106.5,-2    30000       4.2e-19     7.2e-20
 *    IEEE       0,3        30000       1.0e-19     2.4e-20
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition           value returned
 * erfcl underflow    x^2 / 2 > MAXLOGL        0.0
 *
 */
/*							erfl.c
 *
 *	Error function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, erfl();
 *
 * y = erfl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * The integral is
 *
 *                           x 
 *                            -
 *                 2         | |          2
 *   erf(x)  =  --------     |    exp( - t  ) dt.
 *              sqrt(pi)   | |
 *                          -
 *                           0
 *
 * The magnitude of x is limited to about 106.56 for IEEE
 * arithmetic; 1 or -1 is returned outside this range.
 *
 * For 0 <= |x| < 1, erf(x) = x * P6(x^2)/Q6(x^2); otherwise
 * erf(x) = 1 - erfc(x).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,1         50000       2.0e-19     5.7e-20
 *
 */
/*							erfcl.c
 *
 *	Complementary error function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, erfcl();
 *
 * y = erfcl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 *
 *  1 - erf(x) =
 *
 *                           inf. 
 *                             -
 *                  2         | |          2
 *   erfc(x)  =  --------     |    exp( - t  ) dt
 *               sqrt(pi)   | |
 *                           -
 *                            x
 *
 *
 * For small x, erfc(x) = 1 - erf(x); otherwise rational
 * approximations are computed.
 *
 * A special function expx2l.c is used to suppress error amplification
 * in computing exp(-x^2).
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,13        50000      8.4e-19      9.7e-20
 *    IEEE      6,106.56    20000      2.9e-19      7.1e-20
 *
 *
 * ERROR MESSAGES:
 *
 *   message          condition              value returned
 * erfcl underflow    x^2 > MAXLOGL              0.0
 *
 *
 */


/*
Cephes Math Library Release 2.3:  January, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/


#include "mconf.h"

extern long double MAXLOGL;
static long double SQRTHL = 7.071067811865475244008e-1L;

/* erfc(x) = exp(-x^2) P(1/x)/Q(1/x)
   1/8 <= 1/x <= 1
   Peak relative error 5.8e-21  */
#if UNK
static long double P[10] = {
 1.130609921802431462353E9L,
 2.290171954844785638925E9L,
 2.295563412811856278515E9L,
 1.448651275892911637208E9L,
 6.234814405521647580919E8L,
 1.870095071120436715930E8L,
 3.833161455208142870198E7L,
 4.964439504376477951135E6L,
 3.198859502299390825278E5L,
-9.085943037416544232472E-6L,
};
static long double Q[10] = {
/* 1.000000000000000000000E0L, */
 1.130609910594093747762E9L,
 3.565928696567031388910E9L,
 5.188672873106859049556E9L,
 4.588018188918609726890E9L,
 2.729005809811924550999E9L,
 1.138778654945478547049E9L,
 3.358653716579278063988E8L,
 6.822450775590265689648E7L,
 8.799239977351261077610E6L,
 5.669830829076399819566E5L,
};
#endif
#if IBMPC
static short P[] = {
0x4bf0,0x9ad8,0x7a03,0x86c7,0x401d, XPD
0xdf23,0xd843,0x4032,0x8881,0x401e, XPD
0xd025,0xcfd5,0x8494,0x88d3,0x401e, XPD
0xb6d0,0xc92b,0x5417,0xacb1,0x401d, XPD
0xada8,0x356a,0x4982,0x94a6,0x401c, XPD
0x4e13,0xcaee,0x9e31,0xb258,0x401a, XPD
0x5840,0x554d,0x37a3,0x9239,0x4018, XPD
0x3b58,0x3da2,0xaf02,0x9780,0x4015, XPD
0x0144,0x489e,0xbe68,0x9c31,0x4011, XPD
0x333b,0xd9e6,0xd404,0x986f,0xbfee, XPD
};
static short Q[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0x0e43,0x302d,0x79ed,0x86c7,0x401d, XPD
0xf817,0x9128,0xc0f8,0xd48b,0x401e, XPD
0x8eae,0x8dad,0x6eb4,0x9aa2,0x401f, XPD
0x00e7,0x7595,0xcd06,0x88bb,0x401f, XPD
0x4991,0xcfda,0x52f1,0xa2a9,0x401e, XPD
0xc39d,0xe415,0xc43d,0x87c0,0x401d, XPD
0xa75d,0x436f,0x30dd,0xa027,0x401b, XPD
0xc4cb,0x305a,0xbf78,0x8220,0x4019, XPD
0x3708,0x33b1,0x07fa,0x8644,0x4016, XPD
0x24fa,0x96f6,0x7153,0x8a6c,0x4012, XPD
};
#endif
#if MIEEE
static long P[30] = {
0x401d0000,0x86c77a03,0x9ad84bf0,
0x401e0000,0x88814032,0xd843df23,
0x401e0000,0x88d38494,0xcfd5d025,
0x401d0000,0xacb15417,0xc92bb6d0,
0x401c0000,0x94a64982,0x356aada8,
0x401a0000,0xb2589e31,0xcaee4e13,
0x40180000,0x923937a3,0x554d5840,
0x40150000,0x9780af02,0x3da23b58,
0x40110000,0x9c31be68,0x489e0144,
0xbfee0000,0x986fd404,0xd9e6333b,
};
static long Q[30] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x401d0000,0x86c779ed,0x302d0e43,
0x401e0000,0xd48bc0f8,0x9128f817,
0x401f0000,0x9aa26eb4,0x8dad8eae,
0x401f0000,0x88bbcd06,0x759500e7,
0x401e0000,0xa2a952f1,0xcfda4991,
0x401d0000,0x87c0c43d,0xe415c39d,
0x401b0000,0xa02730dd,0x436fa75d,
0x40190000,0x8220bf78,0x305ac4cb,
0x40160000,0x864407fa,0x33b13708,
0x40120000,0x8a6c7153,0x96f624fa,
};
#endif

/* erfc(x) = exp(-x^2) 1/x R(1/x^2) / S(1/x^2)
   1/128 <= 1/x < 1/8
   Peak relative error 1.9e-21  */
#if UNK
static long double R[5] = {
 3.621349282255624026891E0L,
 7.173690522797138522298E0L,
 3.445028155383625172464E0L,
 5.537445669807799246891E-1L,
 2.697535671015506686136E-2L,
};
static long double S[5] = {
/* 1.000000000000000000000E0L, */
 1.072884067182663823072E1L,
 1.533713447609627196926E1L,
 6.572990478128949439509E0L,
 1.005392977603322982436E0L,
 4.781257488046430019872E-2L,
};
#endif
#if IBMPC
static short R[] = {
0x260a,0xab95,0x2fc7,0xe7c4,0x4000, XPD
0x4761,0x613e,0xdf6d,0xe58e,0x4001, XPD
0x0615,0x4b00,0x575f,0xdc7b,0x4000, XPD
0x521d,0x8527,0x3435,0x8dc2,0x3ffe, XPD
0x22cf,0xc711,0x6c5b,0xdcfb,0x3ff9, XPD
};
static short S[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0x5de6,0x17d7,0x54d6,0xaba9,0x4002, XPD
0x55d5,0xd300,0xe71e,0xf564,0x4002, XPD
0xb611,0x8f76,0xf020,0xd255,0x4001, XPD
0x3684,0x3798,0xb793,0x80b0,0x3fff, XPD
0xf5af,0x2fb2,0x1e57,0xc3d7,0x3ffa, XPD
};
#endif
#if MIEEE
static long R[15] = {
0x40000000,0xe7c42fc7,0xab95260a,
0x40010000,0xe58edf6d,0x613e4761,
0x40000000,0xdc7b575f,0x4b000615,
0x3ffe0000,0x8dc23435,0x8527521d,
0x3ff90000,0xdcfb6c5b,0xc71122cf,
};
static long S[15] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x40020000,0xaba954d6,0x17d75de6,
0x40020000,0xf564e71e,0xd30055d5,
0x40010000,0xd255f020,0x8f76b611,
0x3fff0000,0x80b0b793,0x37983684,
0x3ffa0000,0xc3d71e57,0x2fb2f5af,
};
#endif

/* erf(x)  = x P(x^2)/Q(x^2)
   0 <= x <= 1
   Peak relative error 7.6e-23  */
#if UNK
static long double T[7] = {
 1.097496774521124996496E-1L,
 5.402980370004774841217E0L,
 2.871822526820825849235E2L,
 2.677472796799053019985E3L,
 4.825977363071025440855E4L,
 1.549905740900882313773E5L,
 1.104385395713178565288E6L,
};
static long double U[6] = {
/* 1.000000000000000000000E0L, */
 4.525777638142203713736E1L,
 9.715333124857259246107E2L,
 1.245905812306219011252E4L,
 9.942956272177178491525E4L,
 4.636021778692893773576E5L,
 9.787360737578177599571E5L,
};
#endif
#if IBMPC
static short T[] = {
0xfd7a,0x3a1a,0x705b,0xe0c4,0x3ffb, XPD
0x3128,0xc337,0x3716,0xace5,0x4001, XPD
0x9517,0x4e93,0x540e,0x8f97,0x4007, XPD
0x6118,0x6059,0x9093,0xa757,0x400a, XPD
0xb954,0xa987,0xc60c,0xbc83,0x400e, XPD
0x7a56,0xe45a,0xa4bd,0x975b,0x4010, XPD
0xc446,0x6bab,0x0b2a,0x86d0,0x4013, XPD
};
static short U[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0x3453,0x1f8e,0xf688,0xb507,0x4004, XPD
0x71ac,0xb12f,0x21ca,0xf2e2,0x4008, XPD
0xffe8,0x9cac,0x3b84,0xc2ac,0x400c, XPD
0x481d,0x445b,0xc807,0xc232,0x400f, XPD
0x9ad5,0x1aef,0x45b1,0xe25e,0x4011, XPD
0x71a7,0x1cad,0x012e,0xeef3,0x4012, XPD
};
#endif
#if MIEEE
static long T[21] = {
0x3ffb0000,0xe0c4705b,0x3a1afd7a,
0x40010000,0xace53716,0xc3373128,
0x40070000,0x8f97540e,0x4e939517,
0x400a0000,0xa7579093,0x60596118,
0x400e0000,0xbc83c60c,0xa987b954,
0x40100000,0x975ba4bd,0xe45a7a56,
0x40130000,0x86d00b2a,0x6babc446,
};
static long U[18] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x40040000,0xb507f688,0x1f8e3453,
0x40080000,0xf2e221ca,0xb12f71ac,
0x400c0000,0xc2ac3b84,0x9cacffe8,
0x400f0000,0xc232c807,0x445b481d,
0x40110000,0xe25e45b1,0x1aef9ad5,
0x40120000,0xeef3012e,0x1cad71a7,
};
#endif
#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern long double expl ( long double );
extern long double logl ( long double );
extern long double erfl ( long double );
extern long double erfcl ( long double );
extern long double fabsl ( long double );
extern long double expx2l ( long double, int );
extern long double sqrtl (long double);
static long double erfcel (long double);
#else
long double polevll(), p1evll(), expl(), logl(), erfl(), erfcl(), fabsl();
long double expx2l(), sqrtl();
static long double erfcel();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif

long double ndtrl(a)
long double a;
{
long double x, y, z;

x = a * SQRTHL;
z = fabsl(x);

if( z < 1.0 )
	y = 0.5L + 0.5L * erfl(x);

else
	{
	/* See below for erfcel. */
	y = 0.5L * erfcel(z);
	/* Multiply by exp(-x^2 / 2)  */
	z = expx2l(a, -1);
	y = y * sqrtl(z);
	if( x > 0.0L )
		y = 1.0L - y;
	}

return(y);
}


long double erfcl(a)
long double a;
{
long double p,q,x,y,z;

#ifdef INFINITIES
if( a == INFINITYL )
	return(0.0L);
if( a == -INFINITYL )
	return(2.0L);
#endif
if( a < 0.0L )
	x = -a;
else
	x = a;

if( x < 1.0L )
	return( 1.0L - erfl(a) );

z = -a * a;

if( z < -MAXLOGL )
	{
under:
	mtherr( "erfcl", UNDERFLOW );
	if( a < 0 )
		return( 2.0L );
	else
		return( 0.0L );
	}

/* Compute z = expl(z).  */
z = expx2l(a, -1);
y = 1.0L/x;

if( x < 8.0L )
	{
	p = polevll( y, P, 9 );
	q = p1evll( y, Q, 10 );
	}
else
	{
	q = y * y;
	p = y * polevll( q, R, 4 );
	q = p1evll( q, S, 5 );
	}
y = (z * p)/q;

if( a < 0.0L )
	y = 2.0L - y;

if( y == 0.0L )
	goto under;

return(y);
}


/* Exponentially scaled erfc function
   exp(x^2) erfc(x)
   valid for x > 1.
   Use with ndtrl and expx2l.  */

static long double erfcel(x)
long double x;
{
long double p, q, y;

y = 1.0L/x;

if( x < 8.0L )
	{
	p = polevll( y, P, 9 );
	q = p1evll( y, Q, 10 );
	}
else
	{
	q = y * y;
	p = y * polevll( q, R, 4 );
	q = p1evll( q, S, 5 );
	}
y = p/q;
return(y);
}



long double erfl(x)
long double x;
{
long double y, z;

#if MINUSZERO
if( x == 0.0L )
	return(x);
#endif
#ifdef INFINITIES
if( x == -INFINITYL )
	return(-1.0L);
if( x == INFINITYL )
	return(1.0L);
#endif
if( fabsl(x) > 1.0L )
	return( 1.0L - erfcl(x) );

z = x * x;
y = x * polevll( z, T, 6 ) / p1evll( z, U, 6 );
return( y );
}
