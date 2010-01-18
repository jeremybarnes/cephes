/*							qexpm1.c
 *
 *	Exponential function check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qexpm1( x, y );
 * QELT *x, *y;
 *
 * qexpm1( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns e (2.71828...) raised to the x power, minus 1.
 *
 */

/*
Cephes Math Library Release 2.3:  January, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "qhead.h"

extern QELT qlog2[], qhalf[], qone[];
int qtanh();


/* C1 + C2 = ln 2 */
#if WORDSIZE == 16
static QELT C1[NQ] = {0,EXPONE-1,0,0xb172, 0x17f7};
#if 0
#if NQ > 12
static QELT C2[NQ] = {
0x0000,EXPONE-33,0x0000,0xd1cf,0x79ab,0xc9e3,0xb398,0x03f2,
0xf6af,0x40f3,0x4326,0x7298,0xb62d,0x8a0d,0x175b,0x8baa,
0xfa2b,0xe7b8,0x7620,0x6deb,0xac98,0x5595,0x52fb,0x4afa};
#else /* not NQ > 12 */
static QELT C2[NQ] = {
0x0000,EXPONE-33,0x0000,0xd1cf,0x79ab,0xc9e3,0xb398,0x03f2,
0xf6af,0x40f3,0x4326,0x7299};
#endif /* not NQ > 12 */
#endif /* 0 */
#else /* WORDSIZE 32 */
static QELT C1[NQ] = {0,EXPONE-1,0,0xb17217f7};
#if 0
static QELT C2[NQ] = {
0x0000,EXPONE-33,0x0000,0xd1cf79ab,0xc9e3b398,0x03f2f6af,0x40f34326,0x7298b62d,
0x8a0d175b,0x8baafa2b,0xe7b87620,0x6debac98,0x559552fb,0x4afa1b11};
#endif /* 0 */
#endif /* WORDSIZE 32 */


int qexpm1( x, y )
QELT *x, *y;
{
QELT num[NQ], den[NQ], x2[NQ];


/* goto use_exp; */

qmov (C1, num);
num[1] -= 1;
if (qcmp(x, num) > 0)
  goto use_exp;
qmov(x, den);
qneg(den);
if (qcmp(den, num) > 0)
  goto use_exp;

qmov(x, x2);
x2[1] -= 1;		/* x/2				*/
qtanh( x2, x2 );	/* tanh( x/2 )			*/
/* 2 tanh / (1 - tanh) */
qmov( x2, num );	/* 2 tanh			*/
num[1] += 1;
qneg( x2 );
qadd( x2, qone, den );	/* 1 - tanh			*/
qdiv( den, num, y );	/* (2 tanh)/(1 - tanh)	*/
return 0;

use_exp:

qexp(x, num);
qsub(qone, num, y);
return 0;
}
