/*							ellpel.c
 *
 *	Complete elliptic integral of the second kind
 *
 *
 *
 * SYNOPSIS:
 *
 * long double m1, y, ellpel();
 *
 * y = ellpel( m1 );
 *
 *
 *
 * DESCRIPTION:
 *
 * Approximates the integral
 *
 *
 *            pi/2
 *             -
 *            | |                 2
 * E(m)  =    |    sqrt( 1 - m sin t ) dt
 *          | |    
 *           -
 *            0
 *
 * Where m = 1 - m1, using the approximation
 *
 *      P(x)  -  x log x Q(x).
 *
 * Though there are no singularities, the argument m1 is used
 * rather than m for compatibility with ellpk().
 *
 * E(1) = 1; E(0) = pi/2.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       0, 1       10000       1.1e-19     3.5e-20
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * ellpel domain     x<0, x>1            0.0
 *
 */

/*							ellpe.c		*/

/* Elliptic integral of second kind */

/*
Cephes Math Library, Release 2.3:  October, 1995
Copyright 1984, 1987, 1989, 1995 by Stephen L. Moshier
*/

#include "mconf.h"

#if UNK
static long double P[12] = {
 3.198937812032341294902E-5L,
 7.742523238588775116241E-4L,
 4.140384701571542000550E-3L,
 7.963509564694454269086E-3L,
 7.280911706839967541799E-3L,
 5.044067167184043853799E-3L,
 5.076832243257395296304E-3L,
 7.155775630578315248130E-3L,
 1.154485760526450950611E-2L,
 2.183137319801117971860E-2L,
 5.680519271556930583433E-2L,
 4.431471805599467050354E-1L,
};
static long double Q[12] = {
 6.393938134301205485085E-6L,
 2.741404591220851603273E-4L,
 2.480876752984331133799E-3L,
 8.770638497964078750003E-3L,
 1.676835725889463343319E-2L,
 2.281970801531577700830E-2L,
 2.767367465121309044166E-2L,
 3.364167778770018154356E-2L,
 4.272453406734691973083E-2L,
 5.859374951483909267451E-2L,
 9.374999999923942267270E-2L,
 2.499999999999998643587E-1L,
};
#endif
#if IBMPC
static short P[] = {
0x7a78,0x5a02,0x554d,0x862c,0x3ff0, XPD
0x34db,0xa965,0x31a3,0xcaf7,0x3ff4, XPD
0xca6c,0x6c00,0x1071,0x87ac,0x3ff7, XPD
0x4cdb,0x125d,0x6149,0x8279,0x3ff8, XPD
0xadbd,0x3d8f,0xb6d5,0xee94,0x3ff7, XPD
0x8189,0xcd0e,0xb3c2,0xa548,0x3ff7, XPD
0x32b5,0xdd64,0x8e39,0xa65b,0x3ff7, XPD
0x0344,0xc9db,0xff27,0xea7a,0x3ff7, XPD
0xba2d,0x806a,0xa476,0xbd26,0x3ff8, XPD
0xc3e0,0x30fa,0xb53d,0xb2d7,0x3ff9, XPD
0x23b8,0x4d33,0x8fcf,0xe8ac,0x3ffa, XPD
0xbc79,0xa39f,0x2fef,0xe2e4,0x3ffd, XPD
};
static short Q[] = {
0x89f1,0xe234,0x82a6,0xd68b,0x3fed, XPD
0x202a,0x96b3,0x8273,0x8fba,0x3ff3, XPD
0xc183,0xfc45,0x3484,0xa296,0x3ff6, XPD
0x683e,0xe201,0xb960,0x8fb2,0x3ff8, XPD
0x721a,0x1b6a,0xcb41,0x895d,0x3ff9, XPD
0x4eee,0x295f,0x6574,0xbaf0,0x3ff9, XPD
0x3ade,0xc98f,0xe6f2,0xe2b3,0x3ff9, XPD
0xd470,0x1784,0xdb1e,0x89cb,0x3ffa, XPD
0xa649,0xe5c1,0xebc8,0xaeff,0x3ffa, XPD
0x84c0,0xa8f5,0xffde,0xefff,0x3ffa, XPD
0x5506,0xf94f,0xffff,0xbfff,0x3ffb, XPD
0xd8e7,0xffff,0xffff,0xffff,0x3ffc, XPD
};
#endif
#if MIEEE
static long P[36] = {
0x3ff00000,0x862c554d,0x5a027a78,
0x3ff40000,0xcaf731a3,0xa96534db,
0x3ff70000,0x87ac1071,0x6c00ca6c,
0x3ff80000,0x82796149,0x125d4cdb,
0x3ff70000,0xee94b6d5,0x3d8fadbd,
0x3ff70000,0xa548b3c2,0xcd0e8189,
0x3ff70000,0xa65b8e39,0xdd6432b5,
0x3ff70000,0xea7aff27,0xc9db0344,
0x3ff80000,0xbd26a476,0x806aba2d,
0x3ff90000,0xb2d7b53d,0x30fac3e0,
0x3ffa0000,0xe8ac8fcf,0x4d3323b8,
0x3ffd0000,0xe2e42fef,0xa39fbc79,
};
static long Q[36] = {
0x3fed0000,0xd68b82a6,0xe23489f1,
0x3ff30000,0x8fba8273,0x96b3202a,
0x3ff60000,0xa2963484,0xfc45c183,
0x3ff80000,0x8fb2b960,0xe201683e,
0x3ff90000,0x895dcb41,0x1b6a721a,
0x3ff90000,0xbaf06574,0x295f4eee,
0x3ff90000,0xe2b3e6f2,0xc98f3ade,
0x3ffa0000,0x89cbdb1e,0x1784d470,
0x3ffa0000,0xaeffebc8,0xe5c1a649,
0x3ffa0000,0xefffffde,0xa8f584c0,
0x3ffb0000,0xbfffffff,0xf94f5506,
0x3ffc0000,0xffffffff,0xffffd8e7,
};
#endif

#ifdef ANSIPROT
extern long double polevll ( long double, void *, int );
extern long double logl ( long double );
#else
long double polevll(), logl();
#endif

long double ellpel(x)
long double x;
{

if( (x <= 0.0L) || (x > 1.0L) )
	{
	if( x == 0.0L )
		return( 1.0L );
	mtherr( "ellpel", DOMAIN );
	return( 0.0L );
	}
return( 1.0L + x * polevll(x,P,11) - logl(x) * (x * polevll(x,Q,11)) );
}
