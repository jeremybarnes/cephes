/*							log10l.c
 *
 *	Common logarithm, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, log10l();
 *
 * y = log10l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the base 10 logarithm of x.
 *
 * The argument is separated into its exponent and fractional
 * parts.  If the exponent is between -1 and +1, the logarithm
 * of the fraction is approximated by
 *
 *     log(1+x) = x - 0.5 x**2 + x**3 P(x)/Q(x).
 *
 * Otherwise, setting  z = 2(x-1)/x+1),
 * 
 *     log(x) = z + z**3 P(z)/Q(z).
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0.5, 2.0     30000      9.0e-20     2.6e-20
 *    IEEE     exp(+-10000)  30000      6.0e-20     2.3e-20
 *
 * In the tests over the interval exp(+-10000), the logarithms
 * of the random arguments were uniformly distributed over
 * [-10000, +10000].
 *
 * ERROR MESSAGES:
 *
 * log singularity:  x = 0; returns MINLOG
 * log domain:       x < 0; returns MINLOG
 */

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"
static char fname[] = {"log10l"};

/* Coefficients for log(1+x) = x - x**2/2 + x**3 P(x)/Q(x)
 * 1/sqrt(2) <= x < sqrt(2)
 * Theoretical peak relative error = 6.2e-22
 */
#ifdef UNK
static long double P[] = {
 4.9962495940332550844739E-1L,
 1.0767376367209449010438E1L,
 7.7671073698359539859595E1L,
 2.5620629828144409632571E2L,
 4.2401812743503691187826E2L,
 3.4258224542413922935104E2L,
 1.0747524399916215149070E2L,
};
static long double Q[] = {
/* 1.0000000000000000000000E0,*/
 2.3479774160285863271658E1L,
 1.9444210022760132894510E2L,
 7.7952888181207260646090E2L,
 1.6911722418503949084863E3L,
 2.0307734695595183428202E3L,
 1.2695660352705325274404E3L,
 3.2242573199748645407652E2L,
};
#endif

#ifdef IBMPC
static short P[] = {
0xfe72,0xce22,0xd7b9,0xffce,0x3ffd, XPD
0xb778,0x0e34,0x2c71,0xac47,0x4002, XPD
0xea8b,0xc751,0x96f8,0x9b57,0x4005, XPD
0xfeaf,0x6a02,0x67fb,0x801a,0x4007, XPD
0x6b5a,0xf252,0x51ff,0xd402,0x4007, XPD
0x39ce,0x9f76,0x8704,0xab4a,0x4007, XPD
0x1b39,0x740b,0x532e,0xd6f3,0x4005, XPD
};
static short Q[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0x2f3a,0xbf26,0x93d5,0xbbd6,0x4003, XPD
0x13c8,0x031a,0x2d7b,0xc271,0x4006, XPD
0x449d,0x1993,0xd933,0xc2e1,0x4008, XPD
0x5b65,0x574e,0x8301,0xd365,0x4009, XPD
0xa65d,0x3bd2,0xc043,0xfdd8,0x4009, XPD
0x3b21,0xffea,0x1cf5,0x9eb2,0x4009, XPD
0x545c,0xd708,0x7e62,0xa136,0x4007, XPD
};
#endif

#ifdef MIEEE
static long P[] = {
0x3ffd0000,0xffced7b9,0xce22fe72,
0x40020000,0xac472c71,0x0e34b778,
0x40050000,0x9b5796f8,0xc751ea8b,
0x40070000,0x801a67fb,0x6a02feaf,
0x40070000,0xd40251ff,0xf2526b5a,
0x40070000,0xab4a8704,0x9f7639ce,
0x40050000,0xd6f3532e,0x740b1b39,
};
static long Q[] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40030000,0xbbd693d5,0xbf262f3a,
0x40060000,0xc2712d7b,0x031a13c8,
0x40080000,0xc2e1d933,0x1993449d,
0x40090000,0xd3658301,0x574e5b65,
0x40090000,0xfdd8c043,0x3bd2a65d,
0x40090000,0x9eb21cf5,0xffea3b21,
0x40070000,0xa1367e62,0xd708545c,
};
#endif

/* Coefficients for log(x) = z + z^3 P(z^2)/Q(z^2),
 * where z = 2(x-1)/(x+1)
 * 1/sqrt(2) <= x < sqrt(2)
 * Theoretical peak relative error = 6.16e-22
 */

#ifdef UNK
static long double R[4] = {
 1.9757429581415468984296E-3L,
-7.1990767473014147232598E-1L,
 1.0777257190312272158094E1L,
-3.5717684488096787370998E1L,
};
static long double S[4] = {
/* 1.00000000000000000000E0L,*/
-2.6201045551331104417768E1L,
 1.9361891836232102174846E2L,
-4.2861221385716144629696E2L,
};
/* log10(2) */
#define L102A 0.3125L
#define L102B -1.1470004336018804786261e-2L
/* log10(e) */
#define L10EA 0.5L
#define L10EB -6.5705518096748172348871e-2L
#endif
#ifdef IBMPC
static short R[] = {
0x6ef4,0xf922,0x7763,0x817b,0x3ff6, XPD
0x15fd,0x1af9,0xde8f,0xb84b,0xbffe, XPD
0x8b96,0x4f8d,0xa53c,0xac6f,0x4002, XPD
0x8932,0xb4e3,0xe8ae,0x8ede,0xc004, XPD
};
static short S[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0x7ce4,0x1fc9,0xbdc5,0xd19b,0xc003, XPD
0x0af3,0x0d10,0x716f,0xc19e,0x4006, XPD
0x4d7d,0x0f55,0x5d06,0xd64e,0xc007, XPD
};
static short LG102A[] = {0x0000,0x0000,0x0000,0xa000,0x3ffd, XPD};
#define L102A *(long double *)LG102A
static short LG102B[] = {0x0cee,0x8601,0xaf60,0xbbec,0xbff8, XPD};
#define L102B *(long double *)LG102B
static short LG10EA[] = {0x0000,0x0000,0x0000,0x8000,0x3ffe, XPD};
#define L10EA *(long double *)LG10EA
static short LG10EB[] = {0x39ab,0x235e,0x9d5b,0x8690,0xbffb, XPD};
#define L10EB *(long double *)LG10EB
#endif

#ifdef MIEEE
static long R[12] = {
0x3ff60000,0x817b7763,0xf9226ef4,
0xbffe0000,0xb84bde8f,0x1af915fd,
0x40020000,0xac6fa53c,0x4f8d8b96,
0xc0040000,0x8edee8ae,0xb4e38932,
};
static long S[9] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0xc0030000,0xd19bbdc5,0x1fc97ce4,
0x40060000,0xc19e716f,0x0d100af3,
0xc0070000,0xd64e5d06,0x0f554d7d,
};
static long LG102A[] = {0x3ffd0000,0xa0000000,0x00000000};
#define L102A *(long double *)LG102A
static long LG102B[] = {0xbff80000,0xbbecaf60,0x86010cee};
#define L102B *(long double *)LG102B
static long LG10EA[] = {0x3ffe0000,0x80000000,0x00000000};
#define L10EA *(long double *)LG10EA
static long LG10EB[] = {0xbffb0000,0x86909d5b,0x235e39ab};
#define L10EB *(long double *)LG10EB
#endif


#define SQRTH 0.70710678118654752440L
#ifdef ANSIPROT
extern long double frexpl ( long double, int * );
extern long double ldexpl ( long double, int );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern int isnanl ( long double );
#else
long double frexpl(), ldexpl(), polevll(), p1evll(), isnanl();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif

long double log10l(x)
long double x;
{
long double y;
VOLATILE long double z;
int e;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
/* Test for domain */
if( x <= 0.0L )
	{
	if( x == 0.0L )
		{
		mtherr( fname, SING );
#ifdef INFINITIES
		return(-INFINITYL);
#else
		return( -4.9314733889673399399914e3L );
#endif
		}
	else
		{
		mtherr( fname, DOMAIN );
#ifdef NANS
		return(NANL);
#else
		return( -4.9314733889673399399914e3L );
#endif
		}
	}
#ifdef INFINITIES
if( x == INFINITYL )
	return(INFINITYL);
#endif
/* separate mantissa from exponent */

/* Note, frexp is used so that denormal numbers
 * will be handled properly.
 */
x = frexpl( x, &e );


/* logarithm using log(x) = z + z**3 P(z)/Q(z),
 * where z = 2(x-1)/x+1)
 */
if( (e > 2) || (e < -2) )
{
if( x < SQRTH )
	{ /* 2( 2x-1 )/( 2x+1 ) */
	e -= 1;
	z = x - 0.5L;
	y = 0.5L * z + 0.5L;
	}	
else
	{ /*  2 (x-1)/(x+1)   */
	z = x - 0.5L;
	z -= 0.5L;
	y = 0.5L * x  + 0.5L;
	}
x = z / y;
z = x*x;
y = x * ( z * polevll( z, R, 3 ) / p1evll( z, S, 3 ) );
goto done;
}


/* logarithm using log(1+x) = x - .5x**2 + x**3 P(x)/Q(x) */

if( x < SQRTH )
	{
	e -= 1;
	x = ldexpl( x, 1 ) - 1.0L; /*  2x - 1  */
	}	
else
	{
	x = x - 1.0L;
	}
z = x*x;
y = x * ( z * polevll( x, P, 6 ) / p1evll( x, Q, 7 ) );
y = y - ldexpl( z, -1 );   /* -0.5x^2 + ... */

done:

/* Multiply log of fraction by log10(e)
 * and base 2 exponent by log10(2).
 *
 * ***CAUTION***
 *
 * This sequence of operations is critical and it may
 * be horribly defeated by some compiler optimizers.
 */
z = y * (L10EB);
z += x * (L10EB);
z += e * (L102B);
z += y * (L10EA);
z += x * (L10EA);
z += e * (L102A);

return( z );
}
