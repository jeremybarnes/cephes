/*							ndtril.c
 *
 *	Inverse of Normal distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, ndtril();
 *
 * x = ndtril( y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the argument, x, for which the area under the
 * Gaussian probability density function (integrated from
 * minus infinity to x) is equal to y.
 *
 *
 * For small arguments 0 < y < exp(-2), the program computes
 * z = sqrt( -2 log(y) );  then the approximation is
 * x = z - log(z)/z  - (1/z) P(1/z) / Q(1/z) .
 * For larger arguments,  x/sqrt(2 pi) = w + w^3 R(w^2)/S(w^2)) ,
 * where w = y - 0.5 .
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain        # trials      peak         rms
 *  Arguments uniformly distributed:
 *    IEEE       0, 1           5000       7.8e-19     9.9e-20
 *  Arguments exponentially distributed:
 *    IEEE     exp(-11355),-1  30000       1.7e-19     4.3e-20
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition    value returned
 * ndtril domain      x <= 0        -MAXNUML
 * ndtril domain      x >= 1         MAXNUML
 *
 */


/*
Cephes Math Library Release 2.3:  January, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"
extern long double MAXNUML;

/* ndtri(y+0.5)/sqrt(2 pi) = y + y^3 R(y^2)
   0 <= y <= 3/8
   Peak relative error 6.8e-21.  */
#if UNK
/* sqrt(2pi) */
static long double s2pi = 2.506628274631000502416E0L;
static long double P0[8] = {
 8.779679420055069160496E-3L,
-7.649544967784380691785E-1L,
 2.971493676711545292135E0L,
-4.144980036933753828858E0L,
 2.765359913000830285937E0L,
-9.570456817794268907847E-1L,
 1.659219375097958322098E-1L,
-1.140013969885358273307E-2L,
};
static long double Q0[7] = {
/* 1.000000000000000000000E0L, */
-5.303846964603721860329E0L,
 9.908875375256718220854E0L,
-9.031318655459381388888E0L,
 4.496118508523213950686E0L,
-1.250016921424819972516E0L,
 1.823840725000038842075E-1L,
-1.088633151006419263153E-2L,
};
#endif
#if IBMPC
static unsigned short s2p[] = {
0x2cb3,0xb138,0x98ff,0xa06c,0x4000, XPD
};
#define s2pi *(long double *)s2p
static short P0[] = {
0xb006,0x9fc1,0xa4fe,0x8fd8,0x3ff8, XPD
0x6f8a,0x976e,0x0ed2,0xc3d4,0xbffe, XPD
0xf1f1,0x6fcc,0xf3d0,0xbe2c,0x4000, XPD
0xccfb,0xa681,0xad2c,0x84a3,0xc001, XPD
0x9a0d,0x0082,0xa825,0xb0fb,0x4000, XPD
0x13d1,0x054a,0xf220,0xf500,0xbffe, XPD
0xcee9,0x2c92,0x70bd,0xa9e7,0x3ffc, XPD
0x5fee,0x4a42,0xa6cb,0xbac7,0xbff8, XPD
};
static short Q0[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0x841e,0xfec7,0x1d44,0xa9b9,0xc001, XPD
0x97e6,0xcde0,0xc0e7,0x9e8a,0x4002, XPD
0x66f9,0x8f3e,0x47fd,0x9080,0xc002, XPD
0x212f,0x2185,0x33ec,0x8fe0,0x4001, XPD
0x8e73,0x7bac,0x8df2,0xa000,0xbfff, XPD
0xc143,0xcb94,0xe3ea,0xbac2,0x3ffc, XPD
0x25d9,0xc8f3,0x9573,0xb25c,0xbff8, XPD
};
#endif
#if MIEEE
static unsigned long s2p[] = {
0x40000000,0xa06c98ff,0xb1382cb3,
};
#define s2pi *(long double *)s2p
static long P0[24] = {
0x3ff80000,0x8fd8a4fe,0x9fc1b006,
0xbffe0000,0xc3d40ed2,0x976e6f8a,
0x40000000,0xbe2cf3d0,0x6fccf1f1,
0xc0010000,0x84a3ad2c,0xa681ccfb,
0x40000000,0xb0fba825,0x00829a0d,
0xbffe0000,0xf500f220,0x054a13d1,
0x3ffc0000,0xa9e770bd,0x2c92cee9,
0xbff80000,0xbac7a6cb,0x4a425fee,
};
static long Q0[21] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0xc0010000,0xa9b91d44,0xfec7841e,
0x40020000,0x9e8ac0e7,0xcde097e6,
0xc0020000,0x908047fd,0x8f3e66f9,
0x40010000,0x8fe033ec,0x2185212f,
0xbfff0000,0xa0008df2,0x7bac8e73,
0x3ffc0000,0xbac2e3ea,0xcb94c143,
0xbff80000,0xb25c9573,0xc8f325d9,
};
#endif

/* Approximation for interval z = sqrt(-2 log y ) between 2 and 8
 */
/*  ndtri(p) = z - ln(z)/z - 1/z P1(1/z)/Q1(1/z)
    z = sqrt(-2 ln(p))
    2 <= z <= 8, i.e., y between exp(-2) = .135 and exp(-32) = 1.27e-14.
    Peak relative error 5.3e-21  */
#if UNK
static long double P1[10] = {
 4.302849750435552180717E0L,
 4.360209451837096682600E1L,
 9.454613328844768318162E1L,
 9.336735653151873871756E1L,
 5.305046472191852391737E1L,
 1.775851836288460008093E1L,
 3.640308340137013109859E0L,
 3.691354900171224122390E-1L,
 1.403530274998072987187E-2L,
 1.377145111380960566197E-4L,
};
static long double Q1[9] = {
/* 1.000000000000000000000E0L, */
 2.001425109170530136741E1L,
 7.079893963891488254284E1L,
 8.033277265194672063478E1L,
 5.034715121553662712917E1L,
 1.779820137342627204153E1L,
 3.845554944954699547539E0L,
 3.993627390181238962857E-1L,
 1.526870689522191191380E-2L,
 1.498700676286675466900E-4L,
};
#endif
#if IBMPC
static short P1[] = {
0x6105,0xb71e,0xf1f5,0x89b0,0x4001, XPD
0x461d,0x2604,0x8b77,0xae68,0x4004, XPD
0x8b33,0x4a47,0x9ec8,0xbd17,0x4005, XPD
0xa0b2,0xc1b0,0x1627,0xbabc,0x4005, XPD
0x9901,0x28f7,0xad06,0xd433,0x4004, XPD
0xddcb,0x5009,0x7213,0x8e11,0x4003, XPD
0x2432,0x0fa6,0xcfd5,0xe8fa,0x4000, XPD
0x3e24,0xd53c,0x53b2,0xbcff,0x3ffd, XPD
0x4058,0x3d75,0x5393,0xe5f4,0x3ff8, XPD
0x1789,0xf50a,0x7524,0x9067,0x3ff2, XPD
};
static short Q1[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0xd901,0x2673,0x2fad,0xa01d,0x4003, XPD
0x24f5,0xc93c,0x0e9d,0x8d99,0x4005, XPD
0x8cda,0x523a,0x612d,0xa0aa,0x4005, XPD
0x602c,0xb5fc,0x7b9b,0xc963,0x4004, XPD
0xac72,0xd3e7,0xb766,0x8e62,0x4003, XPD
0x048e,0xe34c,0x927c,0xf61d,0x4000, XPD
0x6d88,0xa5cc,0x45de,0xcc79,0x3ffd, XPD
0xe6d1,0x199a,0x9931,0xfa29,0x3ff8, XPD
0x4c7d,0x3675,0x70a0,0x9d26,0x3ff2, XPD
};
#endif
#if MIEEE
static long P1[30] = {
0x40010000,0x89b0f1f5,0xb71e6105,
0x40040000,0xae688b77,0x2604461d,
0x40050000,0xbd179ec8,0x4a478b33,
0x40050000,0xbabc1627,0xc1b0a0b2,
0x40040000,0xd433ad06,0x28f79901,
0x40030000,0x8e117213,0x5009ddcb,
0x40000000,0xe8facfd5,0x0fa62432,
0x3ffd0000,0xbcff53b2,0xd53c3e24,
0x3ff80000,0xe5f45393,0x3d754058,
0x3ff20000,0x90677524,0xf50a1789,
};
static long Q1[27] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x40030000,0xa01d2fad,0x2673d901,
0x40050000,0x8d990e9d,0xc93c24f5,
0x40050000,0xa0aa612d,0x523a8cda,
0x40040000,0xc9637b9b,0xb5fc602c,
0x40030000,0x8e62b766,0xd3e7ac72,
0x40000000,0xf61d927c,0xe34c048e,
0x3ffd0000,0xcc7945de,0xa5cc6d88,
0x3ff80000,0xfa299931,0x199ae6d1,
0x3ff20000,0x9d2670a0,0x36754c7d,
};
#endif

/* ndtri(x) = z - ln(z)/z - 1/z P2(1/z)/Q2(1/z)
   z = sqrt(-2 ln(y))
   8 <= z <= 32
   i.e., y between exp(-32) = 1.27e-14 and exp(-512) = 4.38e-223
   Peak relative error 1.0e-21  */
#if UNK
static long double P2[8] = {
 3.244525725312906932464E0L,
 6.856256488128415760904E0L,
 3.765479340423144482796E0L,
 1.240893301734538935324E0L,
 1.740282292791367834724E-1L,
 9.082834200993107441750E-3L,
 1.617870121822776093899E-4L,
 7.377405643054504178605E-7L,
};
static long double Q2[7] = {
/* 1.000000000000000000000E0L, */
 6.021509481727510630722E0L,
 3.528463857156936773982E0L,
 1.289185315656302878699E0L,
 1.874290142615703609510E-1L,
 9.867655920899636109122E-3L,
 1.760452434084258930442E-4L,
 8.028288500688538331773E-7L,
};
#endif
#if IBMPC
static short P2[] = {
0xafb1,0x4ff9,0x4f3a,0xcfa6,0x4000, XPD
0xbd81,0xaffa,0x7401,0xdb66,0x4001, XPD
0x3a32,0x3863,0x9d0f,0xf0fd,0x4000, XPD
0x300e,0x633d,0x977a,0x9ed5,0x3fff, XPD
0xea3a,0x56b6,0x74c5,0xb234,0x3ffc, XPD
0x38c6,0x49d2,0x2af6,0x94d0,0x3ff8, XPD
0xc85d,0xe17d,0x5ed1,0xa9a5,0x3ff2, XPD
0x536c,0x808b,0x2542,0xc609,0x3fea, XPD
};
static short Q2[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0xaabd,0x125a,0x34a7,0xc0b0,0x4001, XPD
0x0ded,0xe6da,0x5a11,0xe1d2,0x4000, XPD
0xc742,0x9d16,0x0640,0xa504,0x3fff, XPD
0xea1e,0x4cc2,0x643a,0xbfed,0x3ffc, XPD
0x7a9b,0xfaff,0xf2dd,0xa1ab,0x3ff8, XPD
0xfd90,0x4688,0xc902,0xb898,0x3ff2, XPD
0xf003,0x032a,0xfa7e,0xd781,0x3fea, XPD
};
#endif
#if MIEEE
static long P2[24] = {
0x40000000,0xcfa64f3a,0x4ff9afb1,
0x40010000,0xdb667401,0xaffabd81,
0x40000000,0xf0fd9d0f,0x38633a32,
0x3fff0000,0x9ed5977a,0x633d300e,
0x3ffc0000,0xb23474c5,0x56b6ea3a,
0x3ff80000,0x94d02af6,0x49d238c6,
0x3ff20000,0xa9a55ed1,0xe17dc85d,
0x3fea0000,0xc6092542,0x808b536c,
};
static long Q2[21] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x40010000,0xc0b034a7,0x125aaabd,
0x40000000,0xe1d25a11,0xe6da0ded,
0x3fff0000,0xa5040640,0x9d16c742,
0x3ffc0000,0xbfed643a,0x4cc2ea1e,
0x3ff80000,0xa1abf2dd,0xfaff7a9b,
0x3ff20000,0xb898c902,0x4688fd90,
0x3fea0000,0xd781fa7e,0x032af003,
};
#endif

/*  ndtri(x) = z - ln(z)/z - 1/z P3(1/z)/Q3(1/z)
    32 < z < 2048/13
    Peak relative error 1.4e-20  */
#if UNK
static long double P3[8] = {
 2.020331091302772535752E0L,
 2.133020661587413053144E0L,
 2.114822217898707063183E-1L,
-6.500909615246067985872E-3L,
-7.279315200737344309241E-4L,
-1.275404675610280787619E-5L,
-6.433966387613344714022E-8L,
-7.772828380948163386917E-11L,
};
static long double Q3[7] = {
/* 1.000000000000000000000E0L, */
 2.278210997153449199574E0L,
 2.345321838870438196534E-1L,
-6.916708899719964982855E-3L,
-7.908542088737858288849E-4L,
-1.387652389480217178984E-5L,
-7.001476867559193780666E-8L,
-8.458494263787680376729E-11L,
};
#endif
#if IBMPC
static short P3[] = {
0x87b2,0x0f31,0x1ac7,0x814d,0x4000, XPD
0x491c,0xcd74,0x6917,0x8883,0x4000, XPD
0x935e,0x1776,0xcba9,0xd88e,0x3ffc, XPD
0xbafd,0x8abb,0x9518,0xd505,0xbff7, XPD
0xc87e,0x2ed3,0xa84a,0xbed2,0xbff4, XPD
0x0094,0xa402,0x36b5,0xd5fa,0xbfee, XPD
0xbc53,0x0fc3,0x1ab2,0x8a2b,0xbfe7, XPD
0x30b4,0x71c0,0x223d,0xaaed,0xbfdd, XPD
};
static short Q3[] = {
/* 0x0000,0x0000,0x0000,0x8000,0x3fff, XPD */
0xdfc1,0x8a57,0x357f,0x91ce,0x4000, XPD
0xcc4f,0x9e03,0x346e,0xf029,0x3ffc, XPD
0x38b1,0x9788,0x8f42,0xe2a5,0xbff7, XPD
0xb281,0x2117,0x53da,0xcf51,0xbff4, XPD
0xf2ab,0x1d42,0x3760,0xe8cf,0xbfee, XPD
0x741b,0xf14f,0x06b0,0x965b,0xbfe7, XPD
0x37c2,0xa91f,0x16ea,0xba01,0xbfdd, XPD
};
#endif
#if MIEEE
static long P3[24] = {
0x40000000,0x814d1ac7,0x0f3187b2,
0x40000000,0x88836917,0xcd74491c,
0x3ffc0000,0xd88ecba9,0x1776935e,
0xbff70000,0xd5059518,0x8abbbafd,
0xbff40000,0xbed2a84a,0x2ed3c87e,
0xbfee0000,0xd5fa36b5,0xa4020094,
0xbfe70000,0x8a2b1ab2,0x0fc3bc53,
0xbfdd0000,0xaaed223d,0x71c030b4,
};
static long Q3[21] = {
/* 0x3fff0000,0x80000000,0x00000000, */
0x40000000,0x91ce357f,0x8a57dfc1,
0x3ffc0000,0xf029346e,0x9e03cc4f,
0xbff70000,0xe2a58f42,0x978838b1,
0xbff40000,0xcf5153da,0x2117b281,
0xbfee0000,0xe8cf3760,0x1d42f2ab,
0xbfe70000,0x965b06b0,0xf14f741b,
0xbfdd0000,0xba0116ea,0xa91f37c2,
};
#endif
#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern long double logl ( long double );
extern long double sqrtl ( long double );
#else
long double polevll(), p1evll(), logl(), sqrtl();
#endif

long double ndtril(y0)
long double y0;
{
long double x, y, z, y2, x0, x1;
int code;

if( y0 <= 0.0L )
	{
	mtherr( "ndtril", DOMAIN );
	return( -MAXNUML );
	}
if( y0 >= 1.0L )
	{
	mtherr( "ndtri", DOMAIN );
	return( MAXNUML );
	}
code = 1;
y = y0;
if( y > (1.0L - 0.13533528323661269189L) ) /* 0.135... = exp(-2) */
	{
	y = 1.0L - y;
	code = 0;
	}

if( y > 0.13533528323661269189L )
	{
	y = y - 0.5L;
	y2 = y * y;
	x = y + y * (y2 * polevll( y2, P0, 7 )/p1evll( y2, Q0, 7 ));
	x = x * s2pi; 
	return(x);
	}

x = sqrtl( -2.0L * logl(y) );
x0 = x - logl(x)/x;
z = 1.0L/x;
if( x < 8.0L )
	x1 = z * polevll( z, P1, 9 )/p1evll( z, Q1, 9 );
else if( x < 32.0L )
	x1 = z * polevll( z, P2, 7 )/p1evll( z, Q2, 7 );
else
	x1 = z * polevll( z, P3, 7 )/p1evll( z, Q3, 7 );
x = x0 - x1;
if( code != 0 )
	x = -x;
return( x );
}
