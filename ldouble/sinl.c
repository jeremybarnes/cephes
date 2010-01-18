/*							sinl.c
 *
 *	Circular sine, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, sinl();
 *
 * y = sinl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Range reduction is into intervals of pi/4.  The reduction
 * error is nearly eliminated by contriving an extended precision
 * modular arithmetic.
 *
 * Two polynomial approximating functions are employed.
 * Between 0 and pi/4 the sine is approximated by the Cody
 * and Waite polynomial form
 *      x + x**3 P(x**2) .
 * Between pi/4 and pi/2 the cosine is represented as
 *      1 - .5 x**2 + x**4 Q(x**2) .
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain      # trials      peak         rms
 *    IEEE     +-5.5e11      200,000    1.2e-19     2.9e-20
 * 
 * ERROR MESSAGES:
 *
 *   message           condition        value returned
 * sin total loss   x > 2**39               0.0
 *
 * Loss of precision occurs for x > 2**39 = 5.49755813888e11.
 * The routine as implemented flags a TLOSS error for
 * x > 2**39 and returns 0.0.
 */
/*							cosl.c
 *
 *	Circular cosine, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, cosl();
 *
 * y = cosl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Range reduction is into intervals of pi/4.  The reduction
 * error is nearly eliminated by contriving an extended precision
 * modular arithmetic.
 *
 * Two polynomial approximating functions are employed.
 * Between 0 and pi/4 the cosine is approximated by
 *      1 - .5 x**2 + x**4 Q(x**2) .
 * Between pi/4 and pi/2 the sine is represented by the Cody
 * and Waite polynomial form
 *      x  +  x**3 P(x**2) .
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain      # trials      peak         rms
 *    IEEE     +-5.5e11       50000      1.2e-19     2.9e-20
 */

/*							sin.c	*/

/*
Cephes Math Library Release 2.7:  May, 1998
Copyright 1985, 1990, 1998 by Stephen L. Moshier
*/

#include "mconf.h"

#ifdef UNK
static long double sincof[7] = {
-7.5785404094842805756289E-13L,
 1.6058363167320443249231E-10L,
-2.5052104881870868784055E-8L,
 2.7557319214064922217861E-6L,
-1.9841269841254799668344E-4L,
 8.3333333333333225058715E-3L,
-1.6666666666666666640255E-1L,
};
static long double coscof[7] = {
 4.7377507964246204691685E-14L,
-1.1470284843425359765671E-11L,
 2.0876754287081521758361E-9L,
-2.7557319214999787979814E-7L,
 2.4801587301570552304991E-5L,
-1.3888888888888872993737E-3L,
 4.1666666666666666609054E-2L,
};
static long double DP1 = 7.853981554508209228515625E-1L;
static long double DP2 = 7.946627356147928367136046290398E-9L;
static long double DP3 = 3.061616997868382943065164830688E-17L;
#endif

#ifdef IBMPC
static short sincof[] = {
0x4e27,0xe1d6,0x2389,0xd551,0xbfd6, XPD
0x64d7,0xe706,0x4623,0xb090,0x3fde, XPD
0x01b1,0xbf34,0x2946,0xd732,0xbfe5, XPD
0xc8f7,0x9845,0x1d29,0xb8ef,0x3fec, XPD
0x6514,0x0c53,0x00d0,0xd00d,0xbff2, XPD
0x569a,0x8888,0x8888,0x8888,0x3ff8, XPD
0xaa97,0xaaaa,0xaaaa,0xaaaa,0xbffc, XPD
};
static short coscof[] = {
0x7436,0x6f99,0x8c3a,0xd55e,0x3fd2, XPD
0x2f37,0x58f4,0x920f,0xc9c9,0xbfda, XPD
0x5350,0x659e,0xc648,0x8f76,0x3fe2, XPD
0x4d2b,0xf5c6,0x7dba,0x93f2,0xbfe9, XPD
0x53ed,0x0c66,0x00d0,0xd00d,0x3fef, XPD
0x7b67,0x0b60,0x60b6,0xb60b,0xbff5, XPD
0xaa9a,0xaaaa,0xaaaa,0xaaaa,0x3ffa, XPD
};
static short P1[] = {0x0000,0x0000,0xda80,0xc90f,0x3ffe, XPD};
static short P2[] = {0x0000,0x0000,0xa300,0x8885,0x3fe4, XPD};
static short P3[] = {0x3707,0xa2e0,0x3198,0x8d31,0x3fc8, XPD};
#define DP1 *(long double *)P1
#define DP2 *(long double *)P2
#define DP3 *(long double *)P3
#endif

#ifdef MIEEE
static long sincof[] = {
0xbfd60000,0xd5512389,0xe1d64e27,
0x3fde0000,0xb0904623,0xe70664d7,
0xbfe50000,0xd7322946,0xbf3401b1,
0x3fec0000,0xb8ef1d29,0x9845c8f7,
0xbff20000,0xd00d00d0,0x0c536514,
0x3ff80000,0x88888888,0x8888569a,
0xbffc0000,0xaaaaaaaa,0xaaaaaa97,
};
static long coscof[] = {
0x3fd20000,0xd55e8c3a,0x6f997436,
0xbfda0000,0xc9c9920f,0x58f42f37,
0x3fe20000,0x8f76c648,0x659e5350,
0xbfe90000,0x93f27dba,0xf5c64d2b,
0x3fef0000,0xd00d00d0,0x0c6653ed,
0xbff50000,0xb60b60b6,0x0b607b67,
0x3ffa0000,0xaaaaaaaa,0xaaaaaa9a,
};
static long P1[] = {0x3ffe0000,0xc90fda80,0x00000000};
static long P2[] = {0x3fe40000,0x8885a300,0x00000000};
static long P3[] = {0x3fc80000,0x8d313198,0xa2e03707};
#define DP1 *(long double *)P1
#define DP2 *(long double *)P2
#define DP3 *(long double *)P3
#endif

static long double lossth = 5.49755813888e11L; /* 2^39 */
extern long double PIO4L;
#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double floorl ( long double );
extern long double ldexpl ( long double, int );
extern int isnanl ( long double );
extern int isfinitel ( long double );
#else
long double polevll(), floorl(), ldexpl(), isnanl(), isfinitel();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif

long double sinl(x)
long double x;
{
long double y, z, zz;
int j, sign;

#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
#ifdef MINUSZERO
if( x == 0.0L )
	return(x);
#endif
#ifdef NANS
if( !isfinitel(x) )
	{
	mtherr( "sinl", DOMAIN );
#ifdef NANS
	return(NANL);
#else
	return(0.0L);
#endif
	}
#endif
/* make argument positive but save the sign */
sign = 1;
if( x < 0 )
	{
	x = -x;
	sign = -1;
	}

if( x > lossth )
	{
	mtherr( "sinl", TLOSS );
	return(0.0L);
	}

y = floorl( x/PIO4L ); /* integer part of x/PIO4 */

/* strip high bits of integer part to prevent integer overflow */
z = ldexpl( y, -4 );
z = floorl(z);           /* integer part of y/8 */
z = y - ldexpl( z, 4 );  /* y - 16 * (y/16) */

j = z; /* convert to integer for tests on the phase angle */
/* map zeros to origin */
if( j & 1 )
	{
	j += 1;
	y += 1.0L;
	}
j = j & 07; /* octant modulo 360 degrees */
/* reflect in x axis */
if( j > 3)
	{
	sign = -sign;
	j -= 4;
	}

/* Extended precision modular arithmetic */
z = ((x - y * DP1) - y * DP2) - y * DP3;

zz = z * z;
if( (j==1) || (j==2) )
	{
	y = 1.0L - ldexpl(zz,-1) + zz * zz * polevll( zz, coscof, 6 );
	}
else
	{
	y = z  +  z * (zz * polevll( zz, sincof, 6 ));
	}

if(sign < 0)
	y = -y;

return(y);
}





long double cosl(x)
long double x;
{
long double y, z, zz;
long i;
int j, sign;


#ifdef NANS
if( isnanl(x) )
	return(x);
#endif
#ifdef INFINITIES
if( !isfinitel(x) )
	{
	mtherr( "cosl", DOMAIN );
#ifdef NANS
	return(NANL);
#else
	return(0.0L);
#endif
	}
#endif

/* make argument positive */
sign = 1;
if( x < 0 )
	x = -x;

if( x > lossth )
	{
	mtherr( "cosl", TLOSS );
	return(0.0L);
	}

y = floorl( x/PIO4L );
z = ldexpl( y, -4 );
z = floorl(z);		/* integer part of y/8 */
z = y - ldexpl( z, 4 );  /* y - 16 * (y/16) */

/* integer and fractional part modulo one octant */
i = z;
if( i & 1 )	/* map zeros to origin */
	{
	i += 1;
	y += 1.0L;
	}
j = i & 07;
if( j > 3)
	{
	j -=4;
	sign = -sign;
	}

if( j > 1 )
	sign = -sign;

/* Extended precision modular arithmetic */
z = ((x - y * DP1) - y * DP2) - y * DP3;

zz = z * z;
if( (j==1) || (j==2) )
	{
	y = z  +  z * (zz * polevll( zz, sincof, 6 ));
	}
else
	{
	y = 1.0L - ldexpl(zz,-1) + zz * zz * polevll( zz, coscof, 6 );
	}

if(sign < 0)
	y = -y;

return(y);
}
