/*							j0l.c
 *
 *	Bessel function of order zero
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, j0l();
 *
 * y = j0l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of first kind, order zero of the argument.
 *
 * The domain is divided into the intervals [0, 9] and
 * (9, infinity). In the first interval the rational approximation
 * is (x^2 - r^2) (x^2 - s^2) (x^2 - t^2) P7(x^2) / Q8(x^2),
 * where r, s, t are the first three zeros of the function.
 * In the second interval the expansion is in terms of the
 * modulus M0(x) = sqrt(J0(x)^2 + Y0(x)^2) and phase  P0(x)
 * = atan(Y0(x)/J0(x)).  M0 is approximated by sqrt(1/x)P7(1/x)/Q7(1/x).
 * The approximation to J0 is M0 * cos(x -  pi/4 + 1/x P5(1/x^2)/Q6(1/x^2)).
 *
 *
 * ACCURACY:
 *
 *                      Absolute error:
 * arithmetic   domain      # trials      peak         rms
 *    IEEE      0, 30       100000      2.8e-19      7.4e-20
 *
 *
 */
/*							y0l.c
 *
 *	Bessel function of the second kind, order zero
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, y0l();
 *
 * y = y0l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of the second kind, of order
 * zero, of the argument.
 *
 * The domain is divided into the intervals [0, 5>, [5,9> and
 * [9, infinity). In the first interval a rational approximation
 * R(x) is employed to compute y0(x)  = R(x) + 2/pi * log(x) * j0(x).
 *
 * In the second interval, the approximation is
 *     (x - p)(x - q)(x - r)(x - s)P7(x)/Q7(x)
 * where p, q, r, s are zeros of y0(x).
 *
 * The third interval uses the same approximations to modulus
 * and phase as j0(x), whence y0(x) = modulus * sin(phase).
 *
 * ACCURACY:
 *
 *  Absolute error, when y0(x) < 1; else relative error:
 *
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0, 30       100000      3.4e-19     7.6e-20
 *
 */

/* Copyright 1994 by Stephen L. Moshier (moshier@world.std.com).  */

#include "mconf.h"

/*
j0(x) = (x^2-JZ1)(x^2-JZ2)(x^2-JZ3)P(x**2)/Q(x**2)
0 <= x <= 9
Relative error
n=7, d=8
Peak error =  8.49e-22
Relative error spread =  2.2e-3
*/
#if UNK
static long double j0n[8] = {
 1.296848754518641770562E0L,
-3.239201943301299801018E3L,
 3.490002040733471400107E6L,
-2.076797068740966813173E9L,
 7.283696461857171054941E11L,
-1.487926133645291056388E14L,
 1.620335009643150402368E16L,
-7.173386747526788067407E17L,
};
static long double j0d[8] = {
/* 1.000000000000000000000E0L,*/
 2.281959869176887763845E3L,
 2.910386840401647706984E6L,
 2.608400226578100610991E9L,
 1.752689035792859338860E12L,
 8.879132373286001289461E14L,
 3.265560832845194013669E17L,
 7.881340554308432241892E19L,
 9.466475654163919450528E21L,
};
#endif
#if IBMPC
static short j0n[] = {
0xf759,0x4208,0x23d6,0xa5ff,0x3fff, XPD
0xa9a8,0xe62b,0x3b28,0xca73,0xc00a, XPD
0xfe10,0xb608,0x4829,0xd503,0x4014, XPD
0x008c,0x7b60,0xd119,0xf792,0xc01d, XPD
0x943a,0x69b7,0x36ca,0xa996,0x4026, XPD
0x1b0b,0x6331,0x7add,0x8753,0xc02e, XPD
0x4018,0xad26,0x71ba,0xe643,0x4034, XPD
0xb96c,0xc486,0xfb95,0x9f47,0xc03a, XPD
};
static short j0d[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0xbdfe,0xc832,0x5b9f,0x8e9f,0x400a, XPD
0xe1a0,0x923f,0xcb5c,0xb1a2,0x4014, XPD
0x66d2,0x93fe,0x0762,0x9b79,0x401e, XPD
0xfed1,0x086d,0x3425,0xcc0a,0x4027, XPD
0x0841,0x8cb6,0x5a46,0xc9e3,0x4030, XPD
0x3d2c,0xed55,0x20e1,0x9105,0x4039, XPD
0xfdce,0xa4ca,0x2ed8,0x88b8,0x4041, XPD
0x00ac,0xfb2b,0x6f62,0x804b,0x4048, XPD
};
#endif
#if MIEEE
static long j0n[24] = {
0x3fff0000,0xa5ff23d6,0x4208f759,
0xc00a0000,0xca733b28,0xe62ba9a8,
0x40140000,0xd5034829,0xb608fe10,
0xc01d0000,0xf792d119,0x7b60008c,
0x40260000,0xa99636ca,0x69b7943a,
0xc02e0000,0x87537add,0x63311b0b,
0x40340000,0xe64371ba,0xad264018,
0xc03a0000,0x9f47fb95,0xc486b96c,
};
static long j0d[24] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x400a0000,0x8e9f5b9f,0xc832bdfe,
0x40140000,0xb1a2cb5c,0x923fe1a0,
0x401e0000,0x9b790762,0x93fe66d2,
0x40270000,0xcc0a3425,0x086dfed1,
0x40300000,0xc9e35a46,0x8cb60841,
0x40390000,0x910520e1,0xed553d2c,
0x40410000,0x88b82ed8,0xa4cafdce,
0x40480000,0x804b6f62,0xfb2b00ac,
};
#endif
/*
sqrt(j0^2(1/x^2) + y0^2(1/x^2)) = z P(z**2)/Q(z**2)
z(x) = 1/sqrt(x)
Relative error
n=7, d=7
Peak error =  1.80e-20
Relative error spread =  5.1e-2
*/
#if UNK
static long double modulusn[8] = {
 3.947542376069224461532E-1L,
 6.864682945702134624126E0L,
 1.021369773577974343844E1L,
 7.626141421290849630523E0L,
 2.842537511425216145635E0L,
 7.162842530423205720962E-1L,
 9.036664453160200052296E-2L,
 8.461833426898867839659E-3L,
};
static long double modulusd[7] = {
/* 1.000000000000000000000E0L,*/
 9.117176038171821115904E0L,
 1.301235226061478261481E1L,
 9.613002539386213788182E0L,
 3.569671060989910901903E0L,
 8.983920141407590632423E-1L,
 1.132577931332212304986E-1L,
 1.060533546154121770442E-2L,
};
#endif
#if IBMPC
static short modulusn[] = {
0x8559,0xf552,0x3a38,0xca1d,0x3ffd, XPD
0x38a3,0xa663,0x7b91,0xdbab,0x4001, XPD
0xb343,0x2673,0x4e51,0xa36b,0x4002, XPD
0x5e4b,0xe3af,0x59bb,0xf409,0x4001, XPD
0xb1cd,0x4e5e,0x2274,0xb5ec,0x4000, XPD
0xcfe9,0x74e0,0x67a1,0xb75e,0x3ffe, XPD
0x6b78,0x4cc6,0x25b7,0xb912,0x3ffb, XPD
0xcb2b,0x4b73,0x8075,0x8aa3,0x3ff8, XPD
};
static short modulusd[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0x4498,0x3d2a,0xf3fb,0x91df,0x4002, XPD
0x5e3d,0xb5f4,0x9848,0xd032,0x4002, XPD
0xb837,0x3075,0xdbc0,0x99ce,0x4002, XPD
0x775a,0x1b79,0x7d9c,0xe475,0x4000, XPD
0x7e3f,0xb8dd,0x04df,0xe5fd,0x3ffe, XPD
0xed5a,0x31cd,0xb3ac,0xe7f3,0x3ffb, XPD
0x8a83,0x1b80,0x003e,0xadc2,0x3ff8, XPD
};
#endif
#if MIEEE
static long modulusn[24] = {
0x3ffd0000,0xca1d3a38,0xf5528559,
0x40010000,0xdbab7b91,0xa66338a3,
0x40020000,0xa36b4e51,0x2673b343,
0x40010000,0xf40959bb,0xe3af5e4b,
0x40000000,0xb5ec2274,0x4e5eb1cd,
0x3ffe0000,0xb75e67a1,0x74e0cfe9,
0x3ffb0000,0xb91225b7,0x4cc66b78,
0x3ff80000,0x8aa38075,0x4b73cb2b,
};
static long modulusd[21] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40020000,0x91dff3fb,0x3d2a4498,
0x40020000,0xd0329848,0xb5f45e3d,
0x40020000,0x99cedbc0,0x3075b837,
0x40000000,0xe4757d9c,0x1b79775a,
0x3ffe0000,0xe5fd04df,0xb8dd7e3f,
0x3ffb0000,0xe7f3b3ac,0x31cded5a,
0x3ff80000,0xadc2003e,0x1b808a83,
};
#endif
/*
atan(y0(x)/j0(x)) = x - pi/4 + x P(x**2)/Q(x**2)
Absolute error
n=5, d=6
Peak error =  2.80e-21
Relative error spread =  5.5e-1
*/
#if UNK
static long double phasen[6] = {
-7.356766355393571519038E-1L,
-5.001635199922493694706E-1L,
-7.737323518141516881715E-2L,
-3.998893155826990642730E-3L,
-7.496317036829964150970E-5L,
-4.290885090773112963542E-7L,
};
static long double phased[6] = {
/* 1.000000000000000000000E0L,*/
 7.377856408614376072745E0L,
 4.285043297797736118069E0L,
 6.348446472935245102890E-1L,
 3.229866782185025048457E-2L,
 6.014932317342190404134E-4L,
 3.432708072618490390095E-6L,
};
#endif
#if IBMPC
static short phasen[] = {
0x5106,0x12a6,0x4dd2,0xbc55,0xbffe, XPD
0x1e30,0x04da,0xb769,0x800a,0xbffe, XPD
0x8d8a,0x84e7,0xdbd5,0x9e75,0xbffb, XPD
0xe514,0x8866,0x25a9,0x8309,0xbff7, XPD
0xdc17,0x325e,0x8baf,0x9d35,0xbff1, XPD
0x4c2f,0x2dd8,0x79c3,0xe65d,0xbfe9, XPD
};
static short phased[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0xf3e9,0xb2a5,0x6652,0xec17,0x4001, XPD
0x4b69,0x3f87,0x131f,0x891f,0x4001, XPD
0x6f25,0x2a95,0x2dc6,0xa285,0x3ffe, XPD
0x37bf,0xfcc8,0x9b9f,0x844b,0x3ffa, XPD
0xac5c,0x4806,0x8709,0x9dad,0x3ff4, XPD
0x4c8c,0x2dd8,0x79c3,0xe65d,0x3fec, XPD
};
#endif
#if MIEEE
static long phasen[18] = {
0xbffe0000,0xbc554dd2,0x12a65106,
0xbffe0000,0x800ab769,0x04da1e30,
0xbffb0000,0x9e75dbd5,0x84e78d8a,
0xbff70000,0x830925a9,0x8866e514,
0xbff10000,0x9d358baf,0x325edc17,
0xbfe90000,0xe65d79c3,0x2dd84c2f,
};
static long phased[18] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40010000,0xec176652,0xb2a5f3e9,
0x40010000,0x891f131f,0x3f874b69,
0x3ffe0000,0xa2852dc6,0x2a956f25,
0x3ffa0000,0x844b9b9f,0xfcc837bf,
0x3ff40000,0x9dad8709,0x4806ac5c,
0x3fec0000,0xe65d79c3,0x2dd84c8c,
};
#endif
#define JZ1 5.783185962946784521176L
#define JZ2 30.47126234366208639908L
#define JZ3 7.488700679069518344489e1L

#define PIO4L 0.78539816339744830961566L
#ifdef ANSIPROT
extern long double sqrtl ( long double );
extern long double fabsl ( long double );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern long double cosl ( long double );
extern long double sinl ( long double );
extern long double logl ( long double );
long double j0l ( long double );
#else
long double sqrtl(), fabsl(), polevll(), p1evll(), cosl(), sinl(), logl();
long double j0l();
#endif

long double j0l(x)
long double x;
{
long double xx, y, z, modulus, phase;

xx = x * x;
if( xx < 81.0L )
  {
    y = (xx - JZ1) * (xx - JZ2) * (xx -JZ3);
    y *= polevll( xx, j0n, 7 ) / p1evll( xx, j0d, 8 );
    return y;
  }

y = fabsl(x);
xx = 1.0/xx;
phase = polevll( xx, phasen, 5 ) / p1evll( xx, phased, 6 );

z = 1.0/y;
modulus = polevll( z, modulusn, 7 ) / p1evll( z, modulusd, 7 );

y = modulus * cosl( y -  PIO4L + z*phase) / sqrtl(y);
return y;
}


/*
y0(x) = 2/pi * log(x) * j0(x) + P(z**2)/Q(z**2)
0 <= x <= 5
Absolute error
n=7, d=7
Peak error =  8.55e-22
Relative error spread =  2.7e-1
*/
#if UNK
static long double y0n[8] = {
 1.556909814120445353691E4L,
-1.464324149797947303151E7L,
 5.427926320587133391307E9L,
-9.808510181632626683952E11L,
 8.747842804834934784972E13L,
-3.461898868011666236539E15L,
 4.421767595991969611983E16L,
-1.847183690384811186958E16L,
};
static long double y0d[7] = {
/* 1.000000000000000000000E0L,*/
 1.040792201755841697889E3L,
 6.256391154086099882302E5L,
 2.686702051957904669677E8L,
 8.630939306572281881328E10L,
 2.027480766502742538763E13L,
 3.167750475899536301562E15L,
 2.502813268068711844040E17L,
};
#endif
#if IBMPC
static short y0n[] = {
0x126c,0x20be,0x647f,0xf344,0x400c, XPD
0x2ec0,0x7b95,0x297f,0xdf70,0xc016, XPD
0x2fdd,0x4b27,0xca98,0xa1c3,0x401f, XPD
0x3e3c,0xb343,0x46c9,0xe45f,0xc026, XPD
0xb219,0x37ba,0x5142,0x9f1f,0x402d, XPD
0x23c9,0x6b29,0x4244,0xc4c9,0xc032, XPD
0x501f,0x6264,0xbdf4,0x9d17,0x4036, XPD
0x5fbd,0x0171,0x135a,0x8340,0xc035, XPD
};
static short y0d[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0x9057,0x7f25,0x59b7,0x8219,0x4009, XPD
0xd938,0xb6b2,0x71d8,0x98be,0x4012, XPD
0x97a4,0x90fa,0xa7e9,0x801c,0x401b, XPD
0x553b,0x4dc8,0x8695,0xa0c3,0x4023, XPD
0x6732,0x8c1b,0xc5ab,0x9384,0x402b, XPD
0x04d3,0xa629,0xd61d,0xb410,0x4032, XPD
0x241a,0x8f2b,0x629a,0xde4b,0x4038, XPD
};
#endif
#if MIEEE
static long y0n[24] = {
0x400c0000,0xf344647f,0x20be126c,
0xc0160000,0xdf70297f,0x7b952ec0,
0x401f0000,0xa1c3ca98,0x4b272fdd,
0xc0260000,0xe45f46c9,0xb3433e3c,
0x402d0000,0x9f1f5142,0x37bab219,
0xc0320000,0xc4c94244,0x6b2923c9,
0x40360000,0x9d17bdf4,0x6264501f,
0xc0350000,0x8340135a,0x01715fbd,
};
static long y0d[21] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40090000,0x821959b7,0x7f259057,
0x40120000,0x98be71d8,0xb6b2d938,
0x401b0000,0x801ca7e9,0x90fa97a4,
0x40230000,0xa0c38695,0x4dc8553b,
0x402b0000,0x9384c5ab,0x8c1b6732,
0x40320000,0xb410d61d,0xa62904d3,
0x40380000,0xde4b629a,0x8f2b241a,
};
#endif
/*
y0(x) = (x-Y0Z1)(x-Y0Z2)(x-Y0Z3)(x-Y0Z4)P(x)/Q(x)
4.5 <= x <= 9
Absolute error
n=9, d=9
Peak error =  2.35e-20
Relative error spread =  7.8e-13
*/
#if UNK
static long double y059n[10] = {
 2.368715538373384869796E-2L,
-1.472923738545276751402E0L,
 2.525993724177105060507E1L,
 7.727403527387097461580E1L,
-4.578271827238477598563E3L,
 7.051411242092171161986E3L,
 1.951120419910720443331E5L,
 6.515211089266670755622E5L,
-1.164760792144532266855E5L,
-5.566567444353735925323E5L,
};
static long double y059d[9] = {
/* 1.000000000000000000000E0L,*/
-6.235501989189125881723E1L,
 2.224790285641017194158E3L,
-5.103881883748705381186E4L,
 8.772616606054526158657E5L,
-1.096162986826467060921E7L,
 1.083335477747278958468E8L,
-7.045635226159434678833E8L,
 3.518324187204647941098E9L,
 1.173085288957116938494E9L,
};
#endif
#if IBMPC
static short y059n[] = {
0x992f,0xab45,0x90b6,0xc20b,0x3ff9, XPD
0x1207,0x46ea,0xc3db,0xbc88,0xbfff, XPD
0x5504,0x035a,0x59fa,0xca14,0x4003, XPD
0xd5a3,0xf673,0x4e59,0x9a8c,0x4005, XPD
0x62e0,0xc25b,0x2cb3,0x8f12,0xc00b, XPD
0xe8fa,0x4b44,0x4a39,0xdc5b,0x400b, XPD
0x49e2,0xfb52,0x02af,0xbe8a,0x4010, XPD
0x8c07,0x29e3,0x11be,0x9f10,0x4012, XPD
0xfd54,0xb2fe,0x0a23,0xe37e,0xc00f, XPD
0xf90c,0x3510,0x0be9,0x87e7,0xc012, XPD
};
static short y059d[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0xdebf,0xa468,0x8a55,0xf96b,0xc004, XPD
0xad09,0x8e6a,0xa502,0x8b0c,0x400a, XPD
0xa28c,0x5563,0xd19f,0xc75e,0xc00e, XPD
0xe8b6,0xd705,0xda91,0xd62c,0x4012, XPD
0xec8a,0x4697,0xddde,0xa742,0xc016, XPD
0x27ff,0xca92,0x3d78,0xcea1,0x4019, XPD
0xe26b,0x76b9,0x250a,0xa7fb,0xc01c, XPD
0xceb6,0x3463,0x5ddb,0xd1b5,0x401e, XPD
0x3b3b,0xea0b,0xb8d1,0x8bd7,0x401d, XPD
};
#endif
#if MIEEE
static long y059n[30] = {
0x3ff90000,0xc20b90b6,0xab45992f,
0xbfff0000,0xbc88c3db,0x46ea1207,
0x40030000,0xca1459fa,0x035a5504,
0x40050000,0x9a8c4e59,0xf673d5a3,
0xc00b0000,0x8f122cb3,0xc25b62e0,
0x400b0000,0xdc5b4a39,0x4b44e8fa,
0x40100000,0xbe8a02af,0xfb5249e2,
0x40120000,0x9f1011be,0x29e38c07,
0xc00f0000,0xe37e0a23,0xb2fefd54,
0xc0120000,0x87e70be9,0x3510f90c,
};
static long y059d[27] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0xc0040000,0xf96b8a55,0xa468debf,
0x400a0000,0x8b0ca502,0x8e6aad09,
0xc00e0000,0xc75ed19f,0x5563a28c,
0x40120000,0xd62cda91,0xd705e8b6,
0xc0160000,0xa742ddde,0x4697ec8a,
0x40190000,0xcea13d78,0xca9227ff,
0xc01c0000,0xa7fb250a,0x76b9e26b,
0x401e0000,0xd1b55ddb,0x3463ceb6,
0x401d0000,0x8bd7b8d1,0xea0b3b3b,
};
#endif
#define TWOOPI 6.36619772367581343075535E-1L
#define Y0Z1 3.957678419314857868376e0L
#define Y0Z2 7.086051060301772697624e0L
#define Y0Z3 1.022234504349641701900e1L
#define Y0Z4 1.336109747387276347827e1L
/* #define MAXNUML 1.189731495357231765021e4932L */
extern long double MAXNUML;

long double y0l(x)
long double x;
{
long double xx, y, z, modulus, phase;

if( x < 0.0 )
  {
    return (-MAXNUML);
  }
xx = x * x;
if( xx < 81.0L )
  {
    if( xx < 20.25L )
      {
	y = TWOOPI * logl(x) * j0l(x);
	y += polevll( xx, y0n, 7 ) / p1evll( xx, y0d, 7 );
      }
    else
      {
	y = (x - Y0Z1)*(x - Y0Z2)*(x - Y0Z3)*(x - Y0Z4);
	y *= polevll( x, y059n, 9 ) / p1evll( x, y059d, 9 );
      }
    return y;
  }

y = fabsl(x);
xx = 1.0/xx;
phase = polevll( xx, phasen, 5 ) / p1evll( xx, phased, 6 );

z = 1.0/y;
modulus = polevll( z, modulusn, 7 ) / p1evll( z, modulusd, 7 );

y = modulus * sinl( y -  PIO4L + z*phase) / sqrtl(y);
return y;
}
