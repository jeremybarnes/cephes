/*							logl.c
 *
 *	Natural logarithm, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, logl();
 *
 * y = logl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the base e (2.718...) logarithm of x.
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
 *    IEEE      0.5, 2.0    150000      8.71e-20    2.75e-20
 *    IEEE     exp(+-10000) 100000      5.39e-20    2.34e-20
 *
 * In the tests over the interval exp(+-10000), the logarithms
 * of the random arguments were uniformly distributed over
 * [-10000, +10000].
 *
 * ERROR MESSAGES:
 *
 * log singularity:  x = 0; returns -INFINITYL
 * log domain:       x < 0; returns NANL
 */

/*
Cephes Math Library Release 2.7:  May, 1998
Copyright 1984, 1990, 1998 by Stephen L. Moshier
*/

#include "mconf.h"

/* Coefficients for log(1+x) = x - x**2/2 + x**3 P(x)/Q(x)
 * 1/sqrt(2) <= x < sqrt(2)
 * Theoretical peak relative error = 2.32e-20
 */
#ifdef UNK
static long double P[] = {
 4.5270000862445199635215E-5L,
 4.9854102823193375972212E-1L,
 6.5787325942061044846969E0L,
 2.9911919328553073277375E1L,
 6.0949667980987787057556E1L,
 5.7112963590585538103336E1L,
 2.0039553499201281259648E1L,
};
static long double Q[] = {
/* 1.0000000000000000000000E0,*/
 1.5062909083469192043167E1L,
 8.3047565967967209469434E1L,
 2.2176239823732856465394E2L,
 3.0909872225312059774938E2L,
 2.1642788614495947685003E2L,
 6.0118660497603843919306E1L,
};
#endif

#ifdef IBMPC
static short P[] = {
0x51b9,0x9cae,0x4b15,0xbde0,0x3ff0, XPD
0x19cf,0xf0d4,0xc507,0xff40,0x3ffd, XPD
0x9942,0xa7d2,0xfa37,0xd284,0x4001, XPD
0x4add,0x65ce,0x9c5c,0xef4b,0x4003, XPD
0x8445,0x619a,0x75c3,0xf3cc,0x4004, XPD
0x81ab,0x3cd0,0xacba,0xe473,0x4004, XPD
0x4cbf,0xcc18,0x016c,0xa051,0x4003, XPD
};
static short Q[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0xb8b7,0x81f1,0xacf4,0xf101,0x4002, XPD
0xbc31,0x09a4,0x5a91,0xa618,0x4005, XPD
0xaeec,0xe7da,0x2c87,0xddc3,0x4006, XPD
0x2bde,0x4845,0xa2ee,0x9a8c,0x4007, XPD
0x3120,0x4703,0x89f2,0xd86d,0x4006, XPD
0x7347,0x3224,0x8223,0xf079,0x4004, XPD
};
#endif

#ifdef MIEEE
static long P[] = {
0x3ff00000,0xbde04b15,0x9cae51b9,
0x3ffd0000,0xff40c507,0xf0d419cf,
0x40010000,0xd284fa37,0xa7d29942,
0x40030000,0xef4b9c5c,0x65ce4add,
0x40040000,0xf3cc75c3,0x619a8445,
0x40040000,0xe473acba,0x3cd081ab,
0x40030000,0xa051016c,0xcc184cbf,
};
static long Q[] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40020000,0xf101acf4,0x81f1b8b7,
0x40050000,0xa6185a91,0x09a4bc31,
0x40060000,0xddc32c87,0xe7daaeec,
0x40070000,0x9a8ca2ee,0x48452bde,
0x40060000,0xd86d89f2,0x47033120,
0x40040000,0xf0798223,0x32247347,
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
static long double C1 = 6.9314575195312500000000E-1L;
static long double C2 = 1.4286068203094172321215E-6L;
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
static short sc1[] = {0x0000,0x0000,0x0000,0xb172,0x3ffe, XPD};
#define C1 (*(long double *)sc1)
static short sc2[] = {0x4f1e,0xcd5e,0x8e7b,0xbfbe,0x3feb, XPD};
#define C2 (*(long double *)sc2)
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
static long sc1[] = {0x3ffe0000,0xb1720000,0x00000000};
#define C1 (*(long double *)sc1)
static long sc2[] = {0x3feb0000,0xbfbe8e7b,0xcd5e4f1e};
#define C2 (*(long double *)sc2)
#endif


#define SQRTH 0.70710678118654752440L
extern long double MINLOGL;
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

long double logl(x)
long double x;
{
long double y, z;
int e;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
#ifdef INFINITIES
if( x == INFINITYL )
	return(x);
#endif
/* Test for domain */
if( x <= 0.0L )
	{
	if( x == 0.0L )
		{
#ifdef INFINITIES
		return( -INFINITYL );
#else
		mtherr( "logl", SING );
		return( MINLOGL );
#endif
		}
	else
		{
#ifdef NANS
		return( NANL );
#else
		mtherr( "logl", DOMAIN );
		return( MINLOGL );
#endif
		}
	}

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
z = x * ( z * polevll( z, R, 3 ) / p1evll( z, S, 3 ) );
z = z + e * C2;
z = z + x;
z = z + e * C1;
return( z );
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
y = x * ( z * polevll( x, P, 6 ) / p1evll( x, Q, 6 ) );
y = y + e * C2;
z = y - ldexpl( z, -1 );   /*  y - 0.5 * z  */
/* Note, the sum of above terms does not exceed x/4,
 * so it contributes at most about 1/4 lsb to the error.
 */
z = z + x;
z = z + e * C1; /* This sum has an error of 1/2 lsb. */
return( z );
}
