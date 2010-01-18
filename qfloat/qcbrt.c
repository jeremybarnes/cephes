/*							qcbrt.c
 *
 *	Cube root
 *
 *
 *
 * SYNOPSIS:
 *
 * int qcbrt( x, y );
 * QELT *x, *y;
 *
 * qcbrt( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the cube root of the argument, which may be negative.
 *
 */

/*
Cephes Math Library Release 2.2:  January, 1991
Copyright 1984, 1991 by Stephen L. Moshier
*/


/* cube root */
/* 21 December 83, SLM */

#include "qhead.h"

#ifndef ANSIPROT
int qfrexp(), qldexp();
#endif

extern QELT qzero[], qone[], qtwo[], qthree[];

#define CBRT2 1.25992104989487316477
#define CBRT4 1.58740105196819947475
#define CBRT2I 0.793700525984099737376
#define CBRT4I 0.629960524947436582384

/* ??? For some reason, VAX C miscompiles this file if these
   variables are made local.  */
static QELT a[NQ], b[NQ], c[NQ];

int qcbrt( xx, y )
QELT *xx, *y;
{
int sign;
long e, rem;
double x, z;
union
  {
    unsigned short s[4];
    double d;
  } dz;

if( xx[1] == 0 )
	{
	qclear( y );
	return( 0 );
	}

sign = xx[0];
qmov( xx, c );
c[0] = 0;

/* extract power of 2, leaving
 * mantissa between 0.5 and 1
 */
qfrexp( c, &e, a );

qtoe( a, dz.s );
z = dz.d;

/* Approximate cube root of number between .5 and 1,
 * peak relative error = 9.2e-6
 */
x = (((-1.3466110473359520655053e-1  * z
      + 5.4664601366395524503440e-1) * z
      - 9.5438224771509446525043e-1) * z
      + 1.1399983354717293273738e0 ) * z
      + 4.0238979564544752126924e-1;
/* Newton iterations. */
x -= ( x - (z/(x*x)) )*0.33333333333333333333;
x -= ( x - (z/(x*x)) )*0.33333333333333333333;

/* exponent divided by 3 */
if( e >= 0 )
	{
	rem = e;
	e = e / 3;
	rem = rem - 3 * e;
	if( rem == 1 )
		x = x * CBRT2;
	else if( rem == 2 )
		x = x * CBRT4;
	}
else
	{
	/* argument less than 1 */
	e = -e;
	rem = e;
	e /= 3;
	rem -= 3*e;
	if( rem == 1 )
		x *= CBRT2I;
	else if( rem == 2 )
		x *= CBRT4I;
	e = -e;
	}
dz.d = x;
etoq( dz.s, a );

/* multiply by power of 2 */
qldexp( a, e, a );

/* More Newton iterations. */
qmul( a, a, b );	/* 	x * x		*/
qdiv( b, c, b );	/*	z / x*x	*/
qsub( b, a, b );	/*	x - z/x*x	*/
qdiv( qthree, b, b );	/*	.../3		*/
qsub( b, a, a );	/*	x - ...		*/

qmul( a, a, b );
qdiv( b, c, b );
qsub( b, a, b );
qdiv( qthree, b, b );
qsub( b, a, a );

qmul( a, a, b );
qdiv( b, c, b );
qsub( b, a, b );
qdiv( qthree, b, b );
qsub( b, a, a );


if( sign != 0 )
	a[0] = -1;
qmov( a, y );
return( 0 );
}
