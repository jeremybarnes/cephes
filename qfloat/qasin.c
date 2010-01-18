/*							qasin.c
 *
 *	Inverse circular sine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qasin( x, y );
 * QELT *x, *y;
 *
 * qasin( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns radian angle between -pi/2 and +pi/2 whose sine is x.
 *
 *    asin(x) = arctan (x / sqrt(1 - x^2))
 *
 * If |x| > 0.5 it is transformed by the identity
 *
 *    asin(x) = pi/2 - 2 asin( sqrt( (1-x)/2 ) ).
 *
 */
/*							qacos
 *
 *	Inverse circular cosine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qacos( x, y );
 * QELT x[], y[];
 *
 * qacos( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns radian angle between 0 and pi whose cosine
 * is x.
 *
 * acos(x) = pi/2 - asin(x)
 *
 */

/*
Cephes Math Library Release 2.3:  April, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

/* arc sine */

#include "qhead.h"

extern QELT qhalf[], qone[], qpi[];

int qasin( x, y )
QELT x[], y[];
{
QELT a[NQ], z[NQ], zz[NQ], temp[NQ];
int sign, flg;

sign = x[0];
qmov( x, a );
a[0] = 0;
if( qcmp( a, qone ) > 0 )
	{
	mtherr( "qasin", DOMAIN );
	qclear(y);
	return 0;
	}
if( qcmp( a, qhalf) > 0 )
	{
	qsub( a, qhalf, zz );	/*zz = 0.5 -a;*/
	qadd( qhalf, zz, zz );	/*zz = (zz + 0.5)/2.0;*/
	if( zz[1] > 0 )
		zz[1] -= 1;
	qsqrt( zz, z );		/* z = sqrt( zz );*/
	flg = 1;
	}
else
	{
	qmov( a, z );		/*z = a;*/
	qmul( z, z, zz );	/* zz = z * z;*/
	flg = 0;
	}

qsub( zz, qone, temp );	/* 1 - x**2 */
qsqrt( temp, temp );
qdiv( temp, z, temp );
qatn( temp, y );
if( flg != 0 )
	{
	y[1] += 1;
	qmov( qpi, a );
	a[1] -= 1;
	qsub( y, a, y );	/* z = PIO2 - z;*/
	}
y[0] = sign;
return 0;
}



int qacos( x, y )
QELT x[], y[];
{
QELT r[NQ], temp[NQ];

qmov( x, temp );
temp[0] = 0;
if( qcmp( temp, qone ) > 0 )
	{
	mtherr( "qacos", DOMAIN );
	qclear(y);
	return 0;
	}
qasin( x, r );
qmov( qpi, temp );
temp[1] -= 1;
qsub( r, temp, y );
return 0;
}
