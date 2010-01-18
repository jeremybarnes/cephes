/*						qpow
 *
 *	Power function check routine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qpow( x, y, z );
 * QELT *x, *y, *z;
 *
 * qpow( x, y, z );
 *
 *
 *
 * DESCRIPTION:
 *
 * Computes x raised to the yth power.
 *
 *       y
 *      x  =  exp( y log(x) ).
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "qhead.h"
#include "mconf.h"

extern QELT qone[], qtwo[];
int qpowi(), mtherr();

int qpow( x, y, z )
QELT *x, *y, *z;
{
QELT w[NQ];
long li;

qfloor( y, w );
if( qcmp(y,w) == 0 )
	{
	qifrac( y, &li, w );
	if( li < 0 )
		li = -li;
	if( li <= 0x65537 )
		{
		qpowi( x, y, z );
		return 0;
		}
	}
/* z = exp( y * log(x) ) */

qlog( x, w );
qmul( y, w, w );
qexp( w, z );
return 0;
}


/*							qpowi
 *
 *	Real raised to integer power
 *
 *
 *
 * SYNOPSIS:
 *
 * int qpowi( x, n, z );
 * QELT *x, *n, *z;
 *
 * qpowi( x, n, z );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns argument x raised to the nth power, where n is integer valued.
 * The routine efficiently decomposes n as a sum of powers of
 * two. The desired power is a product of two-to-the-kth
 * powers of x.
 *
 */

int qpowi( x, y, z )
QELT x[], y[], z[];
{
QELT w[NQ];
long li, lx, e;
int signx, signy;

qifrac( y, &li, w );
qmov( x, w );
signx = w[0];
w[0] = 0;
if( li < 0 )
	lx = -li;
else
	lx = li;

/* Check for 2^N. */
if( qcmp( qtwo , w ) == 0 )
	{
	qmov( qtwo, z );
	if( signx && (lx & 1) )
		z[0] = -1;
	else
		z[0] = 0;
	e = w[1] + li - 1;
	if( e > MAXEXP )
		goto ioverf;
	if( e <= 0 )
		{
		qclear(z);
		return 0;
		}
	else
		{
		z[1] = e;
		return 0;
		}
	}

if( lx == 0x7fffffff )
	{
	qpow( x, y, z );
	return 0;
	}

if( x[1] == 0 )
	{
	if( li == 0 )
		{
		qmov( qone, z );
		return 0;
		}
	else if( li < 0 )
		{
		qinfin( z );
		return 0;
		}
	else
		{
		qclear( z );
		return 0;
		}
	}

if( li == 0L )
	{
	qmov( qone, z );
	return 0;
	}

if( li < 0 )
	{
	li = -li;
	signy = -1;
	}
else
	signy = 0;


/* First bit of the power */
if( li & 1 )
	{
	qmov( w, z );
	}	
else
	{
	qmov( qone, z );
	signx = 0;
	}

li >>= 1;
while( li != 0L )
	{
	qmul( w, w, w );	/* arg to the 2-to-the-kth power */
	if( li & 1L )	/* if that bit is set, then include in product */
		qmul( w, z, z );
	li >>= 1;
	}

if( signx )
	qneg( z ); /* odd power of negative number */

if( signy )
	{
	if( z[1] != 0 )
		{
		qdiv( z, qone, z );
		}
	else
		{
ioverf:
		qinfin( z );
		mtherr( "qpowi", OVERFLOW );
		}
	}
return 0;
}
