/*							j1l.c
 *
 *	Bessel function of order one
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, j1l();
 *
 * y = j1l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of order one of the argument.
 *
 * The domain is divided into the intervals [0, 9] and
 * (9, infinity). In the first interval the rational approximation
 * is (x^2 - r^2) (x^2 - s^2) (x^2 - t^2) x P8(x^2) / Q8(x^2),
 * where r, s, t are the first three zeros of the function.
 * In the second interval the expansion is in terms of the
 * modulus M1(x) = sqrt(J1(x)^2 + Y1(x)^2) and phase  P1(x)
 * = atan(Y1(x)/J1(x)).  M1 is approximated by sqrt(1/x)P7(1/x)/Q8(1/x).
 * The approximation to j1 is M1 * cos(x -  3 pi/4 + 1/x P5(1/x^2)/Q6(1/x^2)).
 *
 *
 * ACCURACY:
 *
 *                      Absolute error:
 * arithmetic   domain      # trials      peak         rms
 *    IEEE      0, 30        40000      1.8e-19      5.0e-20
 *
 *
 */
/*							y1l.c
 *
 *	Bessel function of the second kind, order zero
 *
 *
 *
 * SYNOPSIS:
 *
 * double x, y, y1l();
 *
 * y = y1l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of the second kind, of order
 * zero, of the argument.
 *
 * The domain is divided into the intervals [0, 4.5>, [4.5,9> and
 * [9, infinity). In the first interval a rational approximation
 * R(x) is employed to compute y0(x)  = R(x) + 2/pi * log(x) * j0(x).
 *
 * In the second interval, the approximation is
 *     (x - p)(x - q)(x - r)(x - s)P9(x)/Q10(x)
 * where p, q, r, s are zeros of y1(x).
 *
 * The third interval uses the same approximations to modulus
 * and phase as j1(x), whence y1(x) = modulus * sin(phase).
 *
 * ACCURACY:
 *
 *  Absolute error, when y0(x) < 1; else relative error:
 *
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0, 30       36000       2.7e-19     5.3e-20
 *
 */

/* Copyright 1994 by Stephen L. Moshier (moshier@world.std.com).  */

#include "mconf.h"

/*
j1(x) = (x^2-r0^2)(x^2-r1^2)(x^2-r2^2) x P(x**2)/Q(x**2)
0 <= x <= 9
Relative error
n=8, d=8
Peak error =  2e-21
*/
#if UNK
static long double j1n[9] = {
-2.63469779622127762897E-4L,
 9.31329762279632791262E-1L,
-1.46280142797793933909E3L,
 1.32000129539331214495E6L,
-7.41183271195454042842E8L,
 2.626500686552841932403E11L,
-5.68263073022183470933E13L,
 6.80006297997263446982E15L,
-3.41470097444474566748E17L,
};
static long double j1d[8] = {
/* 1.00000000000000000000E0L,*/
 2.95267951972943745733E3L,
 4.78723926343829674773E6L,
 5.37544732957807543920E9L,
 4.46866213886267829490E12L,
 2.76959756375961607085E15L,
 1.23367806884831151194E18L,
 3.57325874689695599524E20L,
 5.10779045516141578461E22L,
};
#endif
#if IBMPC
static short j1n[] = {
0xf72f,0x18cc,0x50b2,0x8a22,0xbff3, XPD
0x6dc3,0xc850,0xa096,0xee6b,0x3ffe, XPD
0x29f3,0x496b,0xa54c,0xb6d9,0xc009, XPD
0x38f5,0xf72b,0x0a5c,0xa122,0x4013, XPD
0x1ac8,0xc825,0x3c9c,0xb0b6,0xc01c, XPD
0x038e,0xbd23,0xa7fa,0xf49c,0x4024, XPD
0x636c,0x4d29,0x9f71,0xcebb,0xc02c, XPD
0xd3c2,0xf8f0,0xf852,0xc144,0x4033, XPD
0xd8d8,0x7311,0xa7d2,0x97a4,0xc039, XPD
};
static short j1d[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0xbaf9,0x146e,0xdf50,0xb88a,0x400a, XPD
0x6a17,0xe162,0x4e86,0x9218,0x4015, XPD
0x6041,0xc9fe,0x6890,0xa033,0x401f, XPD
0xb498,0xfdd5,0x209e,0x820e,0x4029, XPD
0x0122,0x56c0,0xf2ef,0x9d6e,0x4032, XPD
0xe6c0,0xa725,0x3d56,0x88f7,0x403b, XPD
0x665d,0xb178,0x242e,0x9af7,0x4043, XPD
0xdd67,0xf5b3,0x0522,0xad0f,0x404a, XPD
};
#endif
#if MIEEE
static long j1n[27] = {
0xbff30000,0x8a2250b2,0x18ccf72f,
0x3ffe0000,0xee6ba096,0xc8506dc3,
0xc0090000,0xb6d9a54c,0x496b29f3,
0x40130000,0xa1220a5c,0xf72b38f5,
0xc01c0000,0xb0b63c9c,0xc8251ac8,
0x40240000,0xf49ca7fa,0xbd23038e,
0xc02c0000,0xcebb9f71,0x4d29636c,
0x40330000,0xc144f852,0xf8f0d3c2,
0xc0390000,0x97a4a7d2,0x7311d8d8,
};
static long j1d[24] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x400a0000,0xb88adf50,0x146ebaf9,
0x40150000,0x92184e86,0xe1626a17,
0x401f0000,0xa0336890,0xc9fe6041,
0x40290000,0x820e209e,0xfdd5b498,
0x40320000,0x9d6ef2ef,0x56c00122,
0x403b0000,0x88f73d56,0xa725e6c0,
0x40430000,0x9af7242e,0xb178665d,
0x404a0000,0xad0f0522,0xf5b3dd67,
};
#endif
/*
sqrt(j0^2(1/x^2) + y0^2(1/x^2)) = z P(z**2)/Q(z**2)
z(x) = 1/sqrt(x)
Relative error
n=7, d=8
Peak error =  1.35e=20
Relative error spread =  9.9e0
*/
#if UNK
static long double modulusn[8] = {
-5.041742205078442098874E0L,
 3.918474430130242177355E-1L,
 2.527521168680500659056E0L,
 7.172146812845906480743E0L,
 2.859499532295180940060E0L,
 1.014671139779858141347E0L,
 1.255798064266130869132E-1L,
 1.596507617085714650238E-2L,
};
static long double modulusd[8] = {
/* 1.000000000000000000000E0L,*/
-6.233092094568239317498E0L,
-9.214128701852838347002E-1L,
 2.531772200570435289832E0L,
 8.755081357265851765640E0L,
 3.554340386955608261463E0L,
 1.267949948774331531237E0L,
 1.573909467558180942219E-1L,
 2.000925566825407466160E-2L,
};
#endif
#if IBMPC
static short modulusn[] = {
0x3d53,0xb598,0xf3bf,0xa155,0xc001, XPD
0x3111,0x863a,0x3a61,0xc8a0,0x3ffd, XPD
0x7d55,0xdb8c,0xe825,0xa1c2,0x4000, XPD
0xe5e2,0x6914,0x3a08,0xe582,0x4001, XPD
0x71e6,0x88a5,0x0a53,0xb702,0x4000, XPD
0x2cb0,0xc657,0xbe70,0x81e0,0x3fff, XPD
0x6de4,0x8fae,0xfe26,0x8097,0x3ffc, XPD
0xa905,0x05fb,0x3101,0x82c9,0x3ff9, XPD
};
static short modulusd[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0x2603,0x640e,0x7d8d,0xc775,0xc001, XPD
0x77b5,0x8f2d,0xb6bf,0xebe1,0xbffe, XPD
0x6420,0x97ce,0x8e44,0xa208,0x4000, XPD
0x0260,0x746b,0xd030,0x8c14,0x4002, XPD
0x77b6,0x34e2,0x501a,0xe37a,0x4000, XPD
0x37ce,0x79ae,0x2f15,0xa24c,0x3fff, XPD
0xfc82,0x02c7,0x17a4,0xa12b,0x3ffc, XPD
0x1237,0xcc6c,0x7356,0xa3ea,0x3ff9, XPD
};
#endif
#if MIEEE
static long modulusn[24] = {
0xc0010000,0xa155f3bf,0xb5983d53,
0x3ffd0000,0xc8a03a61,0x863a3111,
0x40000000,0xa1c2e825,0xdb8c7d55,
0x40010000,0xe5823a08,0x6914e5e2,
0x40000000,0xb7020a53,0x88a571e6,
0x3fff0000,0x81e0be70,0xc6572cb0,
0x3ffc0000,0x8097fe26,0x8fae6de4,
0x3ff90000,0x82c93101,0x05fba905,
};
static long modulusd[24] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0xc0010000,0xc7757d8d,0x640e2603,
0xbffe0000,0xebe1b6bf,0x8f2d77b5,
0x40000000,0xa2088e44,0x97ce6420,
0x40020000,0x8c14d030,0x746b0260,
0x40000000,0xe37a501a,0x34e277b6,
0x3fff0000,0xa24c2f15,0x79ae37ce,
0x3ffc0000,0xa12b17a4,0x02c7fc82,
0x3ff90000,0xa3ea7356,0xcc6c1237,
};
#endif
/*
atan(y1(x)/j1(x))  =  x - 3pi/4 + z P(z**2)/Q(z**2)
z(x) = 1/x
Absolute error
n=5, d=6
Peak error =  4.83e-21
Relative error spread =  1.9e0
*/
#if UNK
static long double phasen[6] = {
 2.010456367705144783933E0L,
 1.587378144541918176658E0L,
 2.682837461073751055565E-1L,
 1.472572645054468815027E-2L,
 2.884976126715926258586E-4L,
 1.708502235134706284899E-6L,
};
static long double phased[6] = {
/* 1.000000000000000000000E0L,*/
 6.809332495854873089362E0L,
 4.518597941618813112665E0L,
 7.320149039410806471101E-1L,
 3.960155028960712309814E-2L,
 7.713202197319040439861E-4L,
 4.556005960359216767984E-6L,
};
#endif
#if IBMPC
static short phasen[] = {
0xebc0,0x5506,0x512f,0x80ab,0x4000, XPD
0x6050,0x98aa,0x3500,0xcb2f,0x3fff, XPD
0xe907,0x28b9,0x7cb7,0x895c,0x3ffd, XPD
0xa830,0xf4a3,0x2c60,0xf144,0x3ff8, XPD
0xf74f,0xbe87,0x7e7d,0x9741,0x3ff3, XPD
0x540c,0xc1d5,0xb096,0xe54f,0x3feb, XPD
};
static short phased[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0xefe3,0x292c,0x0d43,0xd9e6,0x4001, XPD
0xb1f2,0xe0d2,0x5ab5,0x9098,0x4001, XPD
0xc39e,0x9c8c,0x5428,0xbb65,0x3ffe, XPD
0x98f8,0xd610,0x3c35,0xa235,0x3ffa, XPD
0xa853,0x55fb,0x6c79,0xca32,0x3ff4, XPD
0x8d72,0x2be3,0xcb0f,0x98df,0x3fed, XPD
};
#endif
#if MIEEE
static long phasen[18] = {
0x40000000,0x80ab512f,0x5506ebc0,
0x3fff0000,0xcb2f3500,0x98aa6050,
0x3ffd0000,0x895c7cb7,0x28b9e907,
0x3ff80000,0xf1442c60,0xf4a3a830,
0x3ff30000,0x97417e7d,0xbe87f74f,
0x3feb0000,0xe54fb096,0xc1d5540c,
};
static long phased[18] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40010000,0xd9e60d43,0x292cefe3,
0x40010000,0x90985ab5,0xe0d2b1f2,
0x3ffe0000,0xbb655428,0x9c8cc39e,
0x3ffa0000,0xa2353c35,0xd61098f8,
0x3ff40000,0xca326c79,0x55fba853,
0x3fed0000,0x98dfcb0f,0x2be38d72,
};
#endif
#define JZ1 1.46819706421238932572e1L
#define JZ2 4.92184563216946036703e1L
#define JZ3 1.03499453895136580332e2L

#define THPIO4L  2.35619449019234492885L
#ifdef ANSIPROT
extern long double sqrtl ( long double );
extern long double fabsl ( long double );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double, void *, int );
extern long double cosl ( long double );
extern long double sinl ( long double );
extern long double logl ( long double );
long double j1l (long double );
#else
long double sqrtl(), fabsl(), polevll(), p1evll(), cosl(), sinl(), logl();
long double j1l();
#endif

long double j1l(x)
long double x;
{
long double xx, y, z, modulus, phase;

xx = x * x;
if( xx < 81.0L )
  {
    y = (xx - JZ1) * (xx - JZ2) * (xx - JZ3);
    y *= x * polevll( xx, j1n, 8 ) / p1evll( xx, j1d, 8 );
    return y;
  }

y = fabsl(x);
xx = 1.0/xx;
phase = polevll( xx, phasen, 5 ) / p1evll( xx, phased, 6 );

z = 1.0/y;
modulus = polevll( z, modulusn, 7 ) / p1evll( z, modulusd, 8 );

y = modulus * cosl( y -  THPIO4L + z*phase) / sqrtl(y);
if( x < 0 )
  y = -y;
return y;
}

/*
y1(x) = 2/pi * (log(x) * j1(x) - 1/x) + R(x^2) z P(z**2)/Q(z**2)
0 <= x <= 4.5
z(x) = x
Absolute error
n=6, d=7
Peak error =  7.25e-22
Relative error spread =  4.5e-2
*/
#if UNK
static long double y1n[7] = {
-1.288901054372751879531E5L,
 9.914315981558815369372E7L,
-2.906793378120403577274E10L,
 3.954354656937677136266E12L,
-2.445982226888344140154E14L,
 5.685362960165615942886E15L,
-2.158855258453711703120E16L,
};
static long double y1d[7] = {
/* 1.000000000000000000000E0L,*/
 8.926354644853231136073E2L,
 4.679841933793707979659E5L,
 1.775133253792677466651E8L,
 5.089532584184822833416E10L,
 1.076474894829072923244E13L,
 1.525917240904692387994E15L,
 1.101136026928555260168E17L,
};
#endif
#if IBMPC
static short y1n[] = {
0x5b16,0xf7f8,0x0d7e,0xfbbd,0xc00f, XPD
0x53e4,0x194c,0xbefa,0xbd19,0x4019, XPD
0x7607,0xa687,0xaf0a,0xd892,0xc021, XPD
0x5633,0xaa6b,0x79e5,0xe62c,0x4028, XPD
0x69fd,0x1242,0xf62d,0xde75,0xc02e, XPD
0x7f8b,0x4757,0x75bd,0xa196,0x4033, XPD
0x3a10,0x0848,0x5930,0x9965,0xc035, XPD
};
static short y1d[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0xdd1a,0x3b8e,0xab73,0xdf28,0x4008, XPD
0x298c,0x29ef,0x0630,0xe482,0x4011, XPD
0x0e86,0x117b,0x36d6,0xa94a,0x401a, XPD
0x57e0,0x1d92,0x90a9,0xbd99,0x4022, XPD
0xaaf0,0x342b,0xd098,0x9ca5,0x402a, XPD
0x8c6a,0x397e,0x0963,0xad7a,0x4031, XPD
0x7302,0xb91b,0xde7e,0xc399,0x4037, XPD
};
#endif
#if MIEEE
static long y1n[21] = {
0xc00f0000,0xfbbd0d7e,0xf7f85b16,
0x40190000,0xbd19befa,0x194c53e4,
0xc0210000,0xd892af0a,0xa6877607,
0x40280000,0xe62c79e5,0xaa6b5633,
0xc02e0000,0xde75f62d,0x124269fd,
0x40330000,0xa19675bd,0x47577f8b,
0xc0350000,0x99655930,0x08483a10,
};
static long y1d[21] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0x40080000,0xdf28ab73,0x3b8edd1a,
0x40110000,0xe4820630,0x29ef298c,
0x401a0000,0xa94a36d6,0x117b0e86,
0x40220000,0xbd9990a9,0x1d9257e0,
0x402a0000,0x9ca5d098,0x342baaf0,
0x40310000,0xad7a0963,0x397e8c6a,
0x40370000,0xc399de7e,0xb91b7302,
};
#endif
/*
y1(x) = (x-YZ1)(x-YZ2)(x-YZ3)(x-YZ4)R(x) P(z)/Q(z)
z(x) = x
4.5 <= x <= 9
Absolute error
n=9, d=10
Peak error =  3.27e-22
Relative error spread =  4.5e-2
*/
#if UNK
static long double y159n[10] = {
-6.806634906054210550896E-1L,
 4.306669585790359450532E1L,
-9.230477746767243316014E2L,
 6.171186628598134035237E3L,
 2.096869860275353982829E4L,
-1.238961670382216747944E5L,
-1.781314136808997406109E6L,
-1.803400156074242435454E6L,
-1.155761550219364178627E6L,
 3.112221202330688509818E5L,
};
static long double y159d[10] = {
/* 1.000000000000000000000E0L,*/
-6.181482377814679766978E1L,
 2.238187927382180589099E3L,
-5.225317824142187494326E4L,
 9.217235006983512475118E5L,
-1.183757638771741974521E7L,
 1.208072488974110742912E8L,
-8.193431077523942651173E8L,
 4.282669747880013349981E9L,
-1.171523459555524458808E9L,
 1.078445545755236785692E8L,
};
#endif
#if IBMPC
static short y159n[] = {
0xb5e5,0xbb42,0xf667,0xae3f,0xbffe, XPD
0xfdf1,0x41e5,0x4beb,0xac44,0x4004, XPD
0xe917,0x8486,0x0ebd,0xe6c3,0xc008, XPD
0xdf40,0x226b,0x7e37,0xc0d9,0x400b, XPD
0xb2bf,0x4296,0x65af,0xa3d1,0x400d, XPD
0xa33b,0x8229,0x1561,0xf1fc,0xc00f, XPD
0xcd43,0x2f50,0x1118,0xd972,0xc013, XPD
0x3811,0xa3da,0x413f,0xdc24,0xc013, XPD
0xf62f,0xd968,0x8c66,0x8d15,0xc013, XPD
0x539b,0xf305,0xc3d8,0x97f6,0x4011, XPD
};
static short y159d[] = {
/*0x0000,0x0000,0x0000,0x8000,0x3fff, XPD*/
0x1a6c,0x1c93,0x612a,0xf742,0xc004, XPD
0xd0fe,0x2487,0x01c0,0x8be3,0x400a, XPD
0xbed4,0x3ad5,0x2da1,0xcc1d,0xc00e, XPD
0x3c4f,0xdc46,0xb802,0xe107,0x4012, XPD
0xe5e5,0x4172,0x8863,0xb4a0,0xc016, XPD
0x6de5,0xb797,0xea1c,0xe66b,0x4019, XPD
0xa46a,0x0273,0xbc0f,0xc358,0xc01c, XPD
0x8e0e,0xe148,0x5ab3,0xff44,0x401e, XPD
0xb3ad,0x1c6d,0x0f07,0x8ba8,0xc01d, XPD
0xa231,0x6ab0,0x7952,0xcdb2,0x4019, XPD
};
#endif
#if MIEEE
static long y159n[30] = {
0xbffe0000,0xae3ff667,0xbb42b5e5,
0x40040000,0xac444beb,0x41e5fdf1,
0xc0080000,0xe6c30ebd,0x8486e917,
0x400b0000,0xc0d97e37,0x226bdf40,
0x400d0000,0xa3d165af,0x4296b2bf,
0xc00f0000,0xf1fc1561,0x8229a33b,
0xc0130000,0xd9721118,0x2f50cd43,
0xc0130000,0xdc24413f,0xa3da3811,
0xc0130000,0x8d158c66,0xd968f62f,
0x40110000,0x97f6c3d8,0xf305539b,
};
static long y159d[30] = {
/*0x3fff0000,0x80000000,0x00000000,*/
0xc0040000,0xf742612a,0x1c931a6c,
0x400a0000,0x8be301c0,0x2487d0fe,
0xc00e0000,0xcc1d2da1,0x3ad5bed4,
0x40120000,0xe107b802,0xdc463c4f,
0xc0160000,0xb4a08863,0x4172e5e5,
0x40190000,0xe66bea1c,0xb7976de5,
0xc01c0000,0xc358bc0f,0x0273a46a,
0x401e0000,0xff445ab3,0xe1488e0e,
0xc01d0000,0x8ba80f07,0x1c6db3ad,
0x40190000,0xcdb27952,0x6ab0a231,
};
#endif

extern long double MAXNUML;
/* #define MAXNUML 1.18973149535723176502e4932L */
#define TWOOPI 6.36619772367581343075535e-1L
#define THPIO4 2.35619449019234492885L
#define Y1Z1 2.19714132603101703515e0L
#define Y1Z2 5.42968104079413513277e0L
#define Y1Z3 8.59600586833116892643e0L
#define Y1Z4 1.17491548308398812434e1L

long double y1l(x)
long double x;
{
long double xx, y, z, modulus, phase;

if( x < 0.0 )
  {
    return (-MAXNUML);
  }
z = 1.0/x;
xx = x * x;
if( xx < 81.0L )
  {
    if( xx < 20.25L )
      {
	y = TWOOPI * (logl(x) * j1l(x) - z);
	y += x * polevll( xx, y1n, 6 ) / p1evll( xx, y1d, 7 );
      }
    else
      {
	y = (x - Y1Z1)*(x - Y1Z2)*(x - Y1Z3)*(x - Y1Z4);
	y *= polevll( x, y159n, 9 ) / p1evll( x, y159d, 10 );
      }
    return y;
  }

xx = 1.0/xx;
phase = polevll( xx, phasen, 5 ) / p1evll( xx, phased, 6 );

modulus = polevll( z, modulusn, 7 ) / p1evll( z, modulusd, 8 );

z = modulus * sinl( x -  THPIO4L + z*phase) / sqrtl(x);
return z;
}
