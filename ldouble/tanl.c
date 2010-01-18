/*							tanl.c
 *
 *	Circular tangent, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, tanl();
 *
 * y = tanl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the circular tangent of the radian argument x.
 *
 * Range reduction is modulo pi/4.  A rational function
 *       x + x**3 P(x**2)/Q(x**2)
 * is employed in the basic interval [0, pi/4].
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     +-1.07e9       30000     1.9e-19     4.8e-20
 *
 * ERROR MESSAGES:
 *
 *   message         condition          value returned
 * tan total loss   x > 2^39                0.0
 *
 */
/*							cotl.c
 *
 *	Circular cotangent, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, cotl();
 *
 * y = cotl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the circular cotangent of the radian argument x.
 *
 * Range reduction is modulo pi/4.  A rational function
 *       x + x**3 P(x**2)/Q(x**2)
 * is employed in the basic interval [0, pi/4].
 *
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE     +-1.07e9      30000      1.9e-19     5.1e-20
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition          value returned
 * cot total loss   x > 2^39                0.0
 * cot singularity  x = 0                  INFINITYL
 *
 */

/*
Cephes Math Library Release 2.7:  May, 1998
Copyright 1984, 1990, 1998 by Stephen L. Moshier
*/

#include "mconf.h"

#ifdef UNK
static long double P[] = {
-1.3093693918138377764608E4L,
 1.1535166483858741613983E6L,
-1.7956525197648487798769E7L,
};
static long double Q[] = {
/* 1.0000000000000000000000E0L,*/
 1.3681296347069295467845E4L,
-1.3208923444021096744731E6L,
 2.5008380182335791583922E7L,
-5.3869575592945462988123E7L,
};
static long double DP1 = 7.853981554508209228515625E-1L;
static long double DP2 = 7.946627356147928367136046290398E-9L;
static long double DP3 = 3.061616997868382943065164830688E-17L;
#endif


#ifdef IBMPC
static short P[] = {
0xbc1c,0x79f9,0xc692,0xcc96,0xc00c, XPD
0xe5b1,0xe4ee,0x652f,0x8ccf,0x4013, XPD
0xaf9a,0x4c8b,0x5699,0x88ff,0xc017, XPD
};
static short Q[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff,*/
0x8ed4,0x9b2b,0x2f75,0xd5c5,0x400c, XPD
0xadcd,0x55e4,0xe2c1,0xa13d,0xc013, XPD
0x7adf,0x56c7,0x7e17,0xbecc,0x4017, XPD
0x86f6,0xf2d1,0x01e5,0xcd7f,0xc018, XPD
};
static short P1[] = {0x0000,0x0000,0xda80,0xc90f,0x3ffe, XPD};
static short P2[] = {0x0000,0x0000,0xa300,0x8885,0x3fe4, XPD};
static short P3[] = {0x3707,0xa2e0,0x3198,0x8d31,0x3fc8, XPD};
#define DP1 *(long double *)P1
#define DP2 *(long double *)P2
#define DP3 *(long double *)P3
#endif

#ifdef MIEEE
static long P[] = {
0xc00c0000,0xcc96c692,0x79f9bc1c,
0x40130000,0x8ccf652f,0xe4eee5b1,
0xc0170000,0x88ff5699,0x4c8baf9a,
};
static long Q[] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x400c0000,0xd5c52f75,0x9b2b8ed4,
0xc0130000,0xa13de2c1,0x55e4adcd,
0x40170000,0xbecc7e17,0x56c77adf,
0xc0180000,0xcd7f01e5,0xf2d186f6,
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
extern long double MAXNUML;

#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern long double floorl ( long double );
extern long double ldexpl ( long double, int );
extern int isnanl ( long double );
extern int isfinitel ( long double );
static long double tancotl( long double, int );
#else
long double polevll(), p1evll(), floorl(), ldexpl(), isnanl(), isfinitel();
static long double tancotl();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif

long double tanl(x)
long double x;
{

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
	mtherr( "tanl", DOMAIN );
	return(NANL);
	}
#endif
return( tancotl(x,0) );
}


long double cotl(x)
long double x;
{

if( x == 0.0L )
	{
	mtherr( "cotl", SING );
#ifdef INFINITIES
	return( INFINITYL );
#else
	return( MAXNUML );
#endif
	}
return( tancotl(x,1) );
}


static long double tancotl( xx, cotflg )
long double xx;
int cotflg;
{
long double x, y, z, zz;
int j, sign;

/* make argument positive but save the sign */
if( xx < 0.0L )
	{
	x = -xx;
	sign = -1;
	}
else
	{
	x = xx;
	sign = 1;
	}

if( x > lossth )
	{
	if( cotflg )
		mtherr( "cotl", TLOSS );
	else
		mtherr( "tanl", TLOSS );
	return(0.0L);
	}

/* compute x mod PIO4 */
y = floorl( x/PIO4L );

/* strip high bits of integer part */
z = ldexpl( y, -4 );
z = floorl(z);		/* integer part of y/16 */
z = y - ldexpl( z, 4 );  /* y - 16 * (y/16) */

/* integer and fractional part modulo one octant */
j = z;

/* map zeros and singularities to origin */
if( j & 1 )
	{
	j += 1;
	y += 1.0L;
	}

z = ((x - y * DP1) - y * DP2) - y * DP3;

zz = z * z;

if( zz > 1.0e-20L )
	y = z  +  z * (zz * polevll( zz, P, 2 )/p1evll(zz, Q, 4));
else
	y = z;
	
if( j & 2 )
	{
	if( cotflg )
		y = -y;
	else
		y = -1.0L/y;
	}
else
	{
	if( cotflg )
		y = 1.0L/y;
	}

if( sign < 0 )
	y = -y;

return( y );
}
