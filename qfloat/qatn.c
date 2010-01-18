/*							qatn
 *
 *	Inverse circular tangent
 *      (arctangent)
 *
 *
 *
 * SYNOPSIS:
 *
 * int qatn( x, y );
 * QELT *x, *y;
 *
 * qatn( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns radian angle between -pi/2 and +pi/2 whose tangent
 * is x.
 *
 * Range reduction is from three intervals into the interval
 * from zero to pi/8.
 *
 *                     2     2     2
 *               x    x   4 x   9 x
 * arctan(x) =  ---  ---  ----  ----  ...
 *              1 -  3 -  5 -   7 -
 *
 */
/*							qatn2
 *
 *	Quadrant correct inverse circular tangent
 *
 *
 *
 * SYNOPSIS:
 *
 * int qatn2( y, x, z );
 * QELT *x, *y, *z;
 *
 * qatn2( y, x, z );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns radian angle -PI < z < PI whose tangent is y/x.
 *
 */

/*
Cephes Math Library Release 2.3:  April, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

/* arctangent check routine */

#include "qhead.h"
#include "mconf.h"
#ifndef ANSIC
#define ANSIC 0
#endif

extern QELT qone[], qtwo[], qpi[];

#if WORDSIZE == 32
#if NQ < 14

QELT qtp8[NQ] = {
0,EXPONE-2,0,0xd413cccf,0xe7799211,0x65f626cd,0xd52afa7c,0x75bd82ea,
};

QELT qt3p8[NQ] = {
0,EXPONE+1,0,0x9a827999,0xfcef3242,0x2cbec4d9,0xbaa55f4f,0x8eb7b05d,
};

#else

/* tan(pi/8) = sqrt(2) - 1 =
 * 4.1421356237309504880168872420969807856967187537694807317667973799073248E-1
 */
QELT qtp8[NQ] = {
0,EXPONE-2,0,0xd413cccf,0xe7799211,0x65f626cd,0xd52afa7c,0x75bd82ea,
0x24eea133,0xb45eb216,0x0cce6455,0x2bf20c10,0xeae28b0e,0xa2c7f9bf
/*0xa2c7,0xf9bf,0x720f,0x6ce4,0x3dd4,*/
};

/* tan(3pi/8) = sqrt(2) + 1 =
 * 2.4142135623730950488016887242096980785696718753769480731766797379907325E0
 */
QELT qt3p8[NQ] = {
0,EXPONE+1,0,0x9a827999,0xfcef3242,0x2cbec4d9,0xbaa55f4f,0x8eb7b05d,
0x449dd426,0x768bd642,0xc199cc8a,0xa57e4182,0x1d5c5161,0xd458ff38
/*0xd458,0xff37,0xee41,0xed9c,0x87bb,*/
};
#endif
#else
/* word size is 16 */
#if NQ < 24

QELT qtp8[NQ] = {
0x0000,EXPONE-2,0x0000,0xd413,0xcccf,0xe779,0x9211,0x65f6,
0x26cd,0xd52a,0xfa7c,0x75be,
};

QELT qt3p8[NQ] = {
0x0000,EXPONE+1,0x0000,0x9a82,0x7999,0xfcef,0x3242,0x2cbe,
0xc4d9,0xbaa5,0x5f4f,0x8eb8,
};

#else

/* tan(pi/8) = sqrt(2) - 1 =
 * 4.1421356237309504880168872420969807856967187537694807317667973799073248E-1
 */
QELT qtp8[NQ] = {
0x0000,EXPONE-2,0x0000,0xd413,0xcccf,0xe779,0x9211,0x65f6,
0x26cd,0xd52a,0xfa7c,0x75bd,0x82ea,0x24ee,0xa133,0xb45e,
0xb216,0x0cce,0x6455,0x2bf2,0x0c10,0xeae2,0x8b0e,
0xa2c8,
/*0xa2c7,0xf9bf,0x720f,0x6ce4,0x3dd4,*/
};

/* tan(3pi/8) = sqrt(2) + 1 =
 * 2.4142135623730950488016887242096980785696718753769480731766797379907325E0
 */
QELT qt3p8[NQ] = {
0x0000,EXPONE+1,0x0000,0x9a82,0x7999,0xfcef,0x3242,0x2cbe,
0xc4d9,0xbaa5,0x5f4f,0x8eb7,0xb05d,0x449d,0xd426,0x768b,
0xd642,0xc199,0xcc8a,0xa57e,0x4182,0x1d5c,0x5161,
0xd459,
/*0xd458,0xff37,0xee41,0xed9c,0x87bb,*/
};
#endif
#endif


int qatn( x, y )
QELT *x, *y;
{
QELT z[NQ], a[NQ], b[NQ], xx[NQ], qj[NQ], yy[NQ];
long i, j, nsq;
int sign;

qmov( x, xx );
if( xx[0] != 0 )
	{
	xx[0] = 0;
	sign = -1;
	}
else
	sign = 1;

/* range reduction */
if( qcmp(xx, qt3p8) > 0 )
	{
	qmov( qpi, yy );
	yy[1] -= 1;
	qdiv( xx, qone, xx );
	qneg( xx );
	}
else if( qcmp(xx, qtp8) > 0 )
	{
	qmov( qpi, yy );
	yy[1] -= 2;
	qsub( qone, xx, a );	/* x = (x-1.0)/(x+1.0) */
	qadd( qone, xx, b );
	qdiv( b, a, xx );
	}
else
	{
	qclear( yy );
	}

qmul( xx, xx, z );	/* square of x				*/
if( z[1] == 0 )
	{
	qmov( xx, y );
	goto done;
	}
/* loop count for full convergence
 * x < sqrt(2)-1: i = 2*NBITS/9
 * x < 1: i = 4*NBITS/5
 */

i = 2*NBITS/9;
j = 2 * i + 1;
ltoq( &j, qj );		/*  2 * i  +  1			*/
qmov( qj, b );

/* continued fraction expansion */
while( j > 1 )
	{
	nsq = i * i;
	ltoq( &nsq, a );	/* i**2				*/
	qmuli( a, z, a );	/* i**2 * x**2			*/
	qdiv( b, a, b );	/*  i**2 x**2 / (2*i + 1)	*/
	j -= 2;
	i -= 1;
	qsub( qtwo, qj, qj );	/* 2*i + 1			*/
	qadd( qj, b, b );
	}

qdiv( b, xx, y );

done:
qadd( yy, y, y );

if( sign < 0 )
	qneg(y);
return 0;
}

/*							qatn2	*/
/* angle whose tangent is y/x */

#if ANSIC
int qatn2( y, x, z )
#else
int qatn2( x, y, z )
#endif
QELT x[], y[], z[];
{
QELT v[NQ], w[NQ];
int code;


code = 0;

if( (x[0] != 0) && (x[1] > 0) )
	code = 2;
if( (y[0] != 0) && (y[1] > 0 ) )
	code |= 1;

if( x[1] <= 1 ) /* x zero */
	{
	if( code & 1 ) /* y negative */
		{
#if ANSIC
		  qmov (qpi, z); /* - pi/2 */
		  z[1] -= 1;
		qneg(z);
#else
		qmov( qpi, z ); /* 3*pi/2 */
		z[1] -= 1;
		qadd( qpi, z, z );
#endif
		return 0;
		}
	if( y[1] <= 1 ) /* y zero */
		{
		qclear(z);
		return 0;
		}
	qmov( qpi, z ); /* y positive */
	z[1] -= 1;		/* PI/2 */
	return 0;
	}

if( y[1] <= 1 ) /* y zero */
	{
	if( code & 2 ) /* x negative */
		{
		qmov( qpi, z );
		return 0;
		}
	qclear(z);
	return 0;
	}


switch( code )
	{
#if ANSIC
	default:
	case 0:
	case 1: qclear(w); break;
	case 2: qmov( qpi, w ); break;
	case 3: qmov( qpi, w ); qneg(w); break;
#else
	case 0: qclear(w); break;
	case 1:
		qmov( qpi, w );
		w[1] += 1;		/* 2 PI */
		break;
	case 2:
	case 3: qmov(qpi, w); break;
#endif
	}

qdiv( x, y, v );	/* z = w + arctan( y/x ) */
qatn( v, z );
qadd( w, z, z );
return 0;
}


