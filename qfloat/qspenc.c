/*							qspenc.c
 *
 *	Dilogarithm
 *
 *
 *
 * SYNOPSIS:
 *
 * int qspenc( x, y );
 * QELT *x, *y;
 *
 * qspenc( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Computes the integral
 *
 *                    x
 *                    -
 *                   | | log t
 * spence(x)  =  -   |   ----- dt
 *                 | |   t - 1
 *                  -
 *                  1
 *
 * for x >= 0.  A power series gives the integral in
 * the interval (0.5, 1.5).  Transformation formulas for 1/x
 * and 1-x are employed outside the basic expansion range.
 *
 *
 *
 */

/*
Cephes Math Library Release 2.1:  January, 1989
Copyright 1985, 1987, 1989, 1999 by Stephen L. Moshier
*/

#include "qhead.h"
extern QELT qpi[], qone[];

#ifdef ANSIPROT
int qspenss( QELT *, QELT * );
#else
int qspenss();
#endif

int qspenc( x, y )
QELT x[], y[];
{
QELT xxx[NQ], a[NQ], w[NQ], t[NQ];
int flag;
long ll;
union
  {
    unsigned short s[4];
    double d;
  } xx;

qtoe( x, xx.s );
if( xx.d == 1.0 )
	{
	qclear( y );
	return( 0 );
	}
if( xx.d == 0.0 )
	{
	qmul( qpi, qpi, y );	/* y = pi*pi/6 */
	qmov( qone, a );
	ll = 6;
	ltoq(&ll,a);
	/*
	a[3] = 0140000;
	a[1] = EXPONE + 2;
	*/
	qdiv( a, y, y );
	}

qmov( x, xxx );
flag = 0;
if( xx.d > 2.0 )
	{
	xx.d = 1.0/xx.d;
	qdiv( xxx, qone, xxx );	/* x = 1.0/x */
	flag |= 2;
	}
if( xx.d > 1.5 )
	{
	qdiv( xxx, qone, w );	/* w = (1.0/x) - 1.0 */
	qsub( qone, w, w );
	flag |= 2;
	}
else if( xx.d < 0.5 )
	{
	qmov( xxx, w );		/* w = -x */
	w[0] = ~w[0];
	flag |= 1;
	}
else
	qsub( qone, xxx, w );	/* w = x - 1.0 */

qspenss( w, y );

if( flag & 1 )
	{
	/*	y = (PI * PI)/6.0  - log(x) * log(1.0-x) - y */
	qmul( qpi, qpi, t );	/* t = pi*pi/6 */
	qmov( qone, a );
	ll = 6;
	ltoq (&ll,a);
	/* 
	a[3] = 0140000;
	a[1] = EXPONE + 2;
	*/
	qdiv( a, t, t );

	qlog( xxx, a );
	qsub( xxx, qone, w );
	qlog( w, w );
	qmul( w, a, a );
	qsub( a, t, t );
	qsub( y, t, y );
	}	
if( flag & 2 )
	{
	qlog( xxx, t );		/* t = log(x)	*/
				/* y = -0.5 * t * t  -  y */
	qmul( t, t, t );
	t[1] -= 1;
	qadd( y, t, y );
	y[0] = ~y[0];
	}

return( 0 );
}


/* Power series for dilogarithm */

int qspenss( w, y )
QELT w[], y[];
{
QELT a[NQ], k[NQ], t[NQ];

qmov( qone, a );		/* a = 1.0 */
qclear( k );			/* k = 0.0 */
qclear( y );			/* y = 0.0 */

do
	{
	qmul( a, w, a );	/*	a *= w		*/
	qadd( k, qone, k );	/*	k += 1.0	*/
	qmul( k, k, t );	/*	y -= a/(k*k)	*/
	qdiv( t, a, t );
	qsub( t, y, y );
	qmul( a, w, a );	/*	a *= w		*/
	qadd( k, qone, k );	/*	k += 1.0	*/
	qmul( k, k, t );	/*	y += a/(k*k)	*/
	qdiv( t, a, t );
	qadd( y, t, y );
	}
while( ((int) y[1] - (int) a[1]) < NBITS );

/*printf( "%.3E ", k );*/
return 0;
}

