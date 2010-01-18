/*							exp10.c
 *
 *	Base 10 exponential function
 *      (Common antilogarithm)
 *
 *
 *
 * SYNOPSIS:
 *
 * int qexp10( x, y );
 * QELT *x, *y;
 *
 * qexp10( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns 10 raised to the x power.
 *
 *   x      x ln 10
 * 10   =  e
 *
 */

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
*/

#include "qhead.h"

#if WORDSIZE ==32
#if NQ > 8
static QELT qlog10c[] = {
0,EXPONE+1,0,0x935d8ddd,0xaaa8ac16,0xea56d62b,0x82d30a28,0xe28fecf9,
0xda5df90e,0x83c61e82,0x01f02d72,0x962f02d7,0xb1a8105c,0xcc70cbc0
};
QELT qlogt2[] = {
0,EXPONE-2,0,0x9a209a84,0xfbcff798,0x8f8959ac,0x0b7c9178,0x26ad30c5,
0x43d1f349,0x8a5e6f26,0xb7cc63cb,0x286a2d81,0x919fabd0,0x9e5cbc73
};
#else
/* 2.30258509299404568401799145468436420760110148862877E+0 */
static short qlog10c[] = {
0000000,EXPONE+1,0000000,0111535,0106735,0125250,
0126026,0165126,0153053,0101323,0005050,0161220
};

/* 3.010299956639811952137388947244930267681898814621085E-1 */
short qlogt2[] = {
0000000,EXPONE-2,0000000,0115040,0115204,0175717,
0173630,0107611,0054654,0005574,0110570,0023255};
#endif
#else
/* word size is 16 */
#if NQ > 12
static QELT qlog10c[] = {
0x0000,EXPONE+1,0x0000,0x935d,0x8ddd,0xaaa8,0xac16,0xea56,
0xd62b,0x82d3,0x0a28,0xe28f,0xecf9,0xda5d,0xf90e,0x83c6,
0x1e82,0x01f0,0x2d72,0x962f,0x02d7,0xb1a8,0x105c,0xcc71
};
QELT qlogt2[] = {
0x0000,EXPONE-2,0x0000,0x9a20,0x9a84,0xfbcf,0xf798,0x8f89,
0x59ac,0x0b7c,0x9178,0x26ad,0x30c5,0x43d1,0xf349,0x8a5e,
0x6f26,0xb7cc,0x63cb,0x286a,0x2d81,0x919f,0xabd0,0x9e5d
};
#else
/* 2.30258509299404568401799145468436420760110148862877E+0 */
static QELT qlog10c[] = {
0000000,EXPONE+1,0000000,0111535,0106735,0125250,
0126026,0165126,0153053,0101323,0005050,0161220
};

/* 3.010299956639811952137388947244930267681898814621085E-1 */
QELT qlogt2[] = {
0000000,EXPONE-2,0000000,0115040,0115204,0175717,
0173630,0107611,0054654,0005574,0110570,0023255};
#endif
#endif


extern QELT qlogt2[];
int qtanh();

int qexp10( x, y )
QELT x[], y[];
{
QELT a[NQ];

qmul( x, qlog10c, a );
qexp( a, y );
return 0;
}

/* qexp11.c
 *
 *  10**x - 1 for small x.
 *
 *             1 + tanh x/2
 *  exp(x)  =  ------------
 *             1 - tanh x/2
 *
 *
 *                    2 tanh x/2
 *  exp(x) - 1   =   ------------
 *                   1 - tanh x/2
 *
 *  exp10(x)  =  exp( x log 10 )
 *
 */

extern QELT qone[];

int qexp11( xx, y )
QELT xx[], y[];
{
QELT num[NQ], den[NQ], x[NQ];

if( xx[1] == 0 )
	{
	qclear(y);
	return 0;
	}

qmul( xx, qlog10c, x );	/*   x * log(10)   */

x[1] -= 1;		/* x/2				*/
qtanh( x, num );	/* tanh( x/2 )			*/

qsub( num, qone, den );	/* 1 - tanh			*/
num[1] += 1;
qdiv( den, num, y );	/* (2 * tanh)/(1 - tanh)	*/
return 0;
}
