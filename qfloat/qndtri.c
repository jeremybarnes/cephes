/*							qndtri.c
 *
 *	Inverse of Normal distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qndtri(y, x);
 * QELT *y, *x;
 *
 * qndtri(y, x);
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the argument, x, for which the area under the
 * Gaussian probability density function (integrated from
 * minus infinity to x) is equal to y.
 *
 * The routine refines a trial solution computed by the double
 * precision function ndtri.
 *
 */


/*
Cephes Math Library Release 2.1:  January, 1989
Copyright 1984, 1987, 1989, 1998 by Stephen L. Moshier
*/

#include "qhead.h"
#include "mconf.h"

#ifndef ANSIPROT
double sqrt(), ndtri(), exp();
int qndtr();
#endif

static QELT qy0[NQ];
static QELT qcc[NQ];
static QELT qcl[NQ];
static QELT qd[NQ];
static QELT qy[NQ];
static QELT qx[NQ];
static QELT temp[NQ];
static int qcflg = 0;

extern double PI;
extern QELT qzero[], qhalf[], qone[], qpi[];

int qndtri(qy1, qx0)
QELT *qy1, *qx0;
{
double y0, x0;
int i, k, righttail;
union
  {
    unsigned short s[4];
    double d;
  } dz;

if( qcflg == 0 )
	{
	qmov( qpi, temp );
	temp[1] += 1;
	qsqrt( temp, temp );
	qdiv( temp, qone, qcc );
	qlog( qcc, qcl );
	qcflg = 1;
	}

qmov( qy1, qy0 );

if( qcmp(qy0, qzero) <= 0 )
	{
	mtherr( "qndtri", DOMAIN );
	qinfin( qx0 );
	qneg( qx0 );
	return 0;
	}

if( qcmp(qy0, qone) >= 0 )
	{
	mtherr( "qndtri", DOMAIN );
	qinfin( qx0 );
	return 0;
	}

/* Avoid a convergence problem that happens when y is close to 1.  */
if( qcmp(qy0, qhalf) >= 0 )
	{
	qsub( qy0, qone, qy0 );
	righttail = 1;
	}
else
	{
	righttail = 0;
	}

#ifdef DEC
if( qy0[1] < (EXPONE-127) ) /* 6e-39 */
	{
#if NBITS > 144
	k = 7;
#else
	k = 6;
#endif

#else
if( qy0[1] < (QELT) (EXPONE - 1021) ) /* 4.5e-308 */
	{
#if NBITS > 144
	k = 7;
#else
	k = 5;
#endif
#endif
/* x = sqrt( -2 log y ) */
	qlog( qy0, qd );
	qd[1] += 1;
	qd[0] = 0;
	qsqrt( qd, qx );
/* fine adjustment:
 * x = x - (log x  +  log sqrt 2pi)/x
 */
	qlog( qx, temp );
	qsub( qcl, temp, temp );
	qdiv( qx, temp, temp );
	qsub( qx, temp, qx );
	qx[0] = -1;
	}
else
	{
	qtoe( qy0, dz.s );
	y0 = dz.d;
	x0 = ndtri( y0 );
	dz.d = x0;
	etoq( dz.s, qx );
#if NBITS > 144
	k = 3;
#else
	k = 2;
#endif
	}


for( i=0; i<k; i++ )
	{
	qndtr( qx, qy );
/* debugging code */
/*
	qtoasc( qx, s, 5 );
	printf( "%s", s );
	qtoasc( qy, s, 5 );
	printf( " %s", s );
	qsub( qy0, qy, temp );
	qdiv( qy0, temp, temp );
	qtoasc( temp, s, 5 );
	printf( " %s\n", s );
*/
/*   */
	qmul( qx, qx, qd );
	qd[1] -= 1;
	qneg(qd);
	qexp( qd, qd );
	qmul( qcc, qd, qd );
	if( qd[1] > 3 )
		{
		qsub( qy0, qy, temp );
		qdiv( qd, temp, temp );
		qsub( temp, qx, qx );
		}
	}

qmov( qx, qx0 );	
if( righttail )
	qneg( qx0 );
return 0;
}
