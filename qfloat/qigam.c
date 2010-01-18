/*							qigam.c
 *	Check routine for incomplete gamma integral
 *
 *
 *
 * SYNOPSIS:
 *
 * For the left tail:
 * int qigam( a, x, y );
 * QELT *a, *x, *y;
 * qigam( a, x, y );
 *
 * For the right tail:
 * int qigamc( a, x, y );
 * QELT *a, *x, *y;
 * qigamc( a, x, y );
 *
 *
 * DESCRIPTION:
 *
 * The function is defined by
 *
 *                           x
 *                            -
 *                   1       | |  -t  a-1
 *  igam(a,x)  =   -----     |   e   t   dt.
 *                  -      | |
 *                 | (a)    -
 *                           0
 *
 *
 * In this implementation both arguments must be positive.
 * The integral is evaluated by either a power series or
 * continued fraction expansion, depending on the relative
 * values of a and x.
 *
 *
 * ACCURACY:
 *
 * Expansions terminate at less than full working precision.
 *
 */

/*							qigam.c	*/
/*	Check routine for incomplete gamma integral */
/*	SLM, 22 Jan 84	*/



/*
 * incomplete gamma integral
 *
 *
 *          inf.
 *           -
 *   -      |   -t  a-1
 *  | (a)   |  e   t   dt  =  qigamc(a,x)
 *          |
 *         -
 *          x
 *
 *
 */

#include "qhead.h"
extern QELT qone[], qtwo[];
static QELT ans[NQ];
static QELT c[NQ];
static QELT yc[NQ];
static QELT ax[NQ];
static QELT z[NQ];
static QELT pk[NQ];
static QELT pkm1[NQ];
static QELT pkm2[NQ];
static QELT qk[NQ];
static QELT qkm1[NQ];
static QELT qkm2[NQ];
static QELT r[NQ];
static QELT t[NQ];
int qigam(), qlgam();

int qigamc( a, x, y )
QELT *a, *x, *y;
{

if( (x[0] != 0) || ( a[0] != 0) || (x[1] == 0) || (a[1] == 0) )
	{
	mtherr( "qigam", DOMAIN );
	return 0;
	}

qsub( a, x, z);		/* z = x - a; */

if( (x[1] <= (QELT) (EXPONE-1)) || (z[0] != 0 ) )
	{
	qigam( a, x, y );
	qsub( y, qone, y );
	return 0;
	}

/* 	ax = exp( a * log(x) - x - lgam(a) ); */
qlog( x, ax );
qmul( a, ax, ax );
qsub( x, ax, ax );
qlgam( a, c );
qsub( c, ax, c );
qexp( c, ax );

/* continued fraction */
qsub( a, qone, y); 	/* y = 1.0 - a; */
qadd( x, y, z );
qadd( qone, z, z);	/* z = x + y + 1.0; */
qmov( qone, c );
c[1] = 0;
c[3] = 0;		/* c = 0.0; */
qmov( qone, pkm2 );	/* pkm2 = 1.0; */
qmov( x, qkm2 );	/* qkm2 = x; */
qadd( x, qone, pkm1);	/* pkm1 = x + 1.0; */
qmul( z, x, qkm1);	/* qkm1 = z * x; */
qdiv( qkm1, pkm1, ans);	/* ans = pkm1/qkm1; */

do
	{
	qadd( qone, c, c);		/* c += 1.0; */
	qadd( qone, y, y);		/* y += 1.0; */
	qadd( qtwo, z, z);		/* z += 2.0; */
	qmul( y, c, yc );		/* yc = y * c; */
	qmul( pkm2, yc, r);
	qmul( pkm1, z, pk);
	qsub( r, pk, pk );	/* pk = pkm1 * z  -  pkm2 * yc; */
	qmul( qkm2, yc, r );
	qmul( qkm1, z, qk );
	qsub( r, qk, qk );	/* qk = qkm1 * z  -  qkm2 * yc; */
	if( qk[1] > 0 )
		{
		qdiv( qk, pk, r );	/* r = pk/qk; */
		qsub( r, ans, t );	/* t = ans - r */
		qmov( r, ans );		/* ans = r; */
		}
	else
		qmov( qone, t );		/* t = 1.0; */

	qmov( pkm1, pkm2 );		/* pkm2 = pkm1; */
	qmov( pk, pkm1 );		/* pkm1 = pk; */
	qmov( qkm1, qkm2 );		/* qkm2 = qkm1; */
	qmov( qk, qkm1 );		/* qkm1 = qk; */
	}
while( (int) ans[1] - (int) t[1] < 80 ); /* was 67 10**-20 */

qmul( ax, ans, y );  /* return ans * ax */
return 0;
}


int qigam( a, x, y )
QELT *a, *x, *y;
{
if( (x[0] != 0) || ( a[0] != 0) || (x[1] == 0) || (a[1] == 0) )
	{
	mtherr( "qigam", DOMAIN );
	return 0;
	}

qsub( a, x, z);		/* z = x - a; */

if( (x[1] > (QELT) (EXPONE-1)) && (z[0] == 0 ) )
		{
		qigamc( a, x, y );
		qsub( y, qone, y );
		return 0;
		}
/* 	ax = exp( a * log(x) - x - lgam(a) ); */
qlog( x, ax );
qmul( a, ax, ax );
qsub( x, ax, ax );
qlgam( a, c );
qsub( c, ax, c );
qexp( c, ax );

/* power series */
qmov( a, r );		/* r = a; */
qmov( qone, c );	/* c = 1.0; */
qmov( qone, ans );	/* ans = 1.0; */

do
	{
	qadd( qone, r, r );		/* r += 1.0; */
	qdiv( r, x, z );
	qmul( z, c, c );		/* c *= x/r; */
	qadd( c, ans, ans );		/* ans += c; */
	}
while( (int) ans[1] - (int) c[1] < 80 ); /* was 67 while( c/ans > stop ); */

qdiv( a, ax, z );	/* ans * ax / a  */
qmul( z, ans, y );
return 0;
}
