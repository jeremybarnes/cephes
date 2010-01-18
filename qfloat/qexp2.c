/*							qexp2.c
 *
 *	Check routine for base 2 exponential function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qexp2( x, y );
 * QELT *x, *y;
 *
 * qexp2( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns 2 raised to the x power.
 *
 *        x      ln 2  x      x ln 2
 * y  =  2  = ( e     )   =  e
 *
 */


/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/


#include "qhead.h"
extern QELT qlog2[];

int qexp2( x, y )
QELT x[], y[];
{
QELT a[NQ];

qmul( x, qlog2, a );
qexp( a, y );
return 0;
}

/*							qlogtwo
 *
 *	Base 2 logarithm
 *
 *
 *
 * SYNOPSIS:
 *
 * int qlogtwo( x, y );
 * QELT *x, *y;
 *
 * qlogtwo( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the base 2 logarithm of x.
 *
 */

int qlogtwo( x, y )
QELT x[], y[];
{

qlog( x, y );
qdiv( qlog2, y, y );
return 0;
}
