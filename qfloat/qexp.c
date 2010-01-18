/*							qexp.c
 *
 *	Exponential function check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qexp( x, y );
 * QELT *x, *y;
 *
 * qexp( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns e (2.71828...) raised to the x power.
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
#else /* WORDSIZE 32 */
static QELT C1[NQ] = {0,EXPONE-1,0,0xb17217f7};
static QELT C2[NQ] = {
0x0000,EXPONE-33,0x0000,0xd1cf79ab,0xc9e3b398,0x03f2f6af,0x40f34326,0x7298b62d,
0x8a0d175b,0x8baafa2b,0xe7b87620,0x6debac98,0x559552fb,0x4afa1b11};
#endif /* WORDSIZE 32 */

int qexp( x, y )
QELT *x, *y;
{
QELT num[NQ], den[NQ], x2[NQ];
long i;

/* range reduction theory: x = i + f, 0<=f<1;
 * e^x = e^i * e^f 
 * e^i = 2^(i/log 2).
 * Let i/log2 = i1 + f1, 0 <= f1 < 1.
 * Then e^i = 2^i1 * 2^f1, so
 * e^x = 2^i1 * e^(f1 log 2) * e^f.
 */

/* Catch overflow that might cause an endless recursion below.  */
if( x[1] >= EXPONE + 15 )
	{
	if( x[0] != 0 )
		goto underf;
	else
		goto overf;
	}
if( x[1] == 0 )
	{
	qmov( qone, y );
	return(0);
	}
qmov(x, x2);
qdiv( qlog2, x2, den );
qadd( qhalf, den, den );
qfloor( den, num );
qifrac( num, &i, den );
qmul( num, C1, den );
qsub( den, x2, x2 );
qmul( num, C2, den );
qsub( den, x2, x2 );

#if 0
sign = x2[0];
if( sign != 0 )
	x2[0] = 0;
qifrac( x2, &i, num );		/* x = i + f		*/
if( i != 0 )
	{
	ltoq( &i, den );		/* floating point i	*/
	qdiv( qlog2, den, den );	/* i/log 2		*/
	qifrac( den, &i, den );	/* i/log 2  =  i1 + f1	*/
	qmul( qlog2, den, den );	/* log 2 * f1		*/
	qadd( den, num, x2 );		/* log 2 * f1  + f	*/
	}
#endif

x2[1] -= 1;		/* x/2				*/
qtanh( x2, x2 );	/* tanh( x/2 )			*/
qadd( x2, qone, num );	/* 1 + tanh			*/
qneg( x2 );
qadd( x2, qone, den );	/* 1 - tanh			*/
qdiv( den, num, y );	/* (1 + tanh)/(1 - tanh)	*/

i += y[1];
if( i > MAXEXP )
	{
overf:
	mtherr( "qexp", OVERFLOW );
	qinfin(y);
	return 0;
	}
if( i <= 0 )
	{
underf:
	qclear(y);
	return 0;
	}
y[1] = i;
return 0;
}
