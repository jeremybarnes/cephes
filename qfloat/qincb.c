/*							qincb.c
 *
 *	Incomplete beta integral
 *
 *
 * SYNOPSIS:
 *
 * int qincb( a, b, x, y );
 * QELT *a, *b, *x, *y;
 *
 * qincb( a, b, x, y );
 *
 *
 * DESCRIPTION:
 *
 * Returns incomplete beta integral of the arguments, evaluated
 * from zero to x.
 *
 *                  x
 *     -            -
 *    | (a+b)      | |  a-1     b-1
 *  -----------    |   t   (1-t)   dt.
 *   -     -     | |
 *  | (a) | (b)   -
 *                 0
 *
 *
 * ACCURACY:
 *
 * Series expansions terminate at less than full working precision.
 *
 */

/*
Cephes Math Library, Release 2.3:  March, 1995
Copyright 1984, 1995, 1996 by Stephen L. Moshier
*/

#include "qhead.h"
#define BITSPREC 80

extern QELT qone[], qtwo[];
static QELT xk[NQ];
static QELT pk[NQ];
static QELT pkm1[NQ];
static QELT pkm2[NQ];
static QELT qk[NQ];
static QELT qkm1[NQ];
static QELT qkm2[NQ];
static QELT k1[NQ];
static QELT k2[NQ];
static QELT k3[NQ];
static QELT k4[NQ];
static QELT k5[NQ];
static QELT k6[NQ];
static QELT k7[NQ];
static QELT k8[NQ];
static QELT r[NQ];
static QELT t[NQ];
static QELT ans[NQ];
static QELT a[NQ];
static QELT b[NQ];
static QELT x[NQ];
static QELT lgmq[NQ];

#ifdef ANSIPROT
int qincps( QELT *, QELT *, QELT *, QELT * );
#else
int qlgam(), qhy2f1(), qpow();
int qincps();
#endif

int qincb( aa, bb, xx, y )
QELT aa[], bb[], xx[], y[];
{
double da, db, dx, dt;
int rflg, n;
union
  {
    unsigned short s[4];
    double d;
  } dz;

qtoe( aa, dz.s );
da = dz.d;
qtoe( bb, dz.s );
db = dz.d;
qtoe( xx, dz.s );
dx = dz.d;
rflg = 0;
if( db * dx < 1.0 && dx < 0.92)
  {
    qincps( aa, bb, xx, y );
    goto done;
  }

/* compare x with the mean */
if( dx < (da/(da+db)) )
	{
	qmov( aa, a );
	qmov( bb, b );
	qmov( xx, x );
	rflg = 0;
	}
else
	{
	qmov( bb, a );
	qmov( aa, b );
	qsub( xx, qone, x );
	dt = da;
	da = db;
	db = dt;
	dx = 1.0 - dx;
	rflg = 1;
	}

if( rflg == 1 && db * dx < 1.0 && dx < 0.92)
  {
    qincps( a, b, x, y );
    goto done;
  }

#if 0
dt = 1.0 - dx;
if( da * dt < 1.0 && dt < 0.8)
  {
    qsub( xx, qone, x );
    qincps( bb, aa, x, y );
    rflg = 1;
    goto done;
  }
#endif


qmov( a, k1 );		/* k1 = a */
qadd( a, b, k2 );	/* k2 = a + b */
qmov( a, k3 );		/* k3 = a */
qadd( qone, a, k4 );	/* k4 = a + 1.0 */
qmov( qone, k5 );	/* k5 = 1.0 */
qsub( qone, b, k6 );	/* k6 = b - 1.0 */
qmov( k4, k7 );		/* k7 = k4 */
qadd( qtwo, a, k8 );	/* k8 = a + 2.0 */

qclear( pkm2 );		/* pkm2 = 0.0 */
qmov( qone, qkm2 );	/* qkm2 = 1.0 */
qmov( qone, pkm1 );	/* pkm1 = 1.0 */
qmov( qone, qkm1 );	/* qkm1 = 1.0 */
qmov( qone, ans );	/* ans = 1.0 */
qmov( qone, r );
n = 0;
do
	{
	
/*	xk = -( x * k1 * k2 )/( k3 * k4 ); */
	qmul( x, k1, xk );
	qmul( k2, xk, xk );
	qneg( xk );
	qmul( k3, k4, pk );
	qdiv( pk, xk, xk );

/*	pk = pkm1 +  pkm2 * xk; */
	qmul( xk, pkm2, pk );
	qadd( pkm1, pk, pk );

/*	qk = qkm1 +  qkm2 * xk; */
	qmul( xk, qkm2, qk );
	qadd( qkm1, qk, qk );

	qmov( pkm1, pkm2 );	/* pkm2 = pkm1 */
	qmov( pk, pkm1 );	/* pkm1 = pk */
	qmov( qkm1, qkm2 );	/* qkm2 = qkm1 */
	qmov( qk, qkm1 );	/* qkm1 = qk */

/*	xk = ( x * k5 * k6 )/( k7 * k8 ); */
	qmul( x, k5, xk );
	qmul( k6, xk, xk );
	qmul( k7, k8, pk );
	qdiv( pk, xk, xk );

/*	pk = pkm1 +  pkm2 * xk */
	qmul( xk, pkm2, pk );
	qadd( pk, pkm1, pk );

/*	qk = qkm1 +  qkm2 * xk */
	qmul( xk, qkm2, qk );
	qadd( qk, qkm1, qk );

	qmov( pkm1, pkm2 );	/* pkm2 = pkm1 */
	qmov( pk, pkm1 );	/* pkm1 = pk */
	qmov( qkm1, qkm2 );	/* qkm2 = qkm1 */
	qmov( qk, qkm1 );	/* qkm1 = qk */

	if( qk[1] > 10 )
		qdiv( qk, pk, r );	/* r = pk/qk */
	if( r[1] > 10 )
	  {
	    qsub( r, ans, t );
	    if( ((int)ans[1] - (int)t[1]) > BITSPREC )
	      goto cdone;
	    qmov( r, ans );
	  }
	else
	  qmov( qone, t );
	qadd( qone, k1, k1 );	/* k1 += 1.0 */
	qadd( qone, k2, k2 );	/* k2 += 1.0 */
	qadd( qtwo, k3, k3 );	/* k3 += 2.0 */
	qadd( qtwo, k4, k4 );	/* k4 += 2.0 */
	qadd( qone, k5, k5 );	/* k5 += 1.0 */
	qsub( qone, k6, k6 );	/* k6 -= 1.0 */
	qadd( qtwo, k7, k7 );	/* k7 += 2.0 */
	qadd( qtwo, k8, k8 );	/* k8 += 2.0 */

	if( ((int)qk[1] > (EXPONE+NBITS)) || (pk[1] > (EXPONE+NBITS)) )
		{
/*		printf( "qincbet: c frac big\n" ); */
		pkm2[1] -= NBITS;
		pkm1[1] -= NBITS;
		qkm2[1] -= NBITS;
		qkm1[1] -= NBITS;
		}
	if( ((int)qk[1] < (EXPONE-NBITS)) || (pk[1] < (EXPONE-NBITS)) )
		{
/*		printf( "qincbet: c frac small\n" ); */
		pkm2[1] += NBITS;
		pkm1[1] += NBITS;
		qkm2[1] += NBITS;
		qkm1[1] += NBITS;
		}
/*	printf( "n=%3d ans=%.6E pk=%.5E qk=%.5E\n", n, ans, pk, qk );*/
	}
while( ++n < 400 );

mtherr( "qincb", PLOSS );

cdone:
/* b * log(1-x) */
qsub( x, qone, t );
qlog( t, pkm1 );
qmul( b, pkm1, pkm1 );
/* a * log(x) */
qlog( x, pk );
qmul( a, pk, pk );
/* lgam(a+b) - lgam(a) - lgam(b) */
qlgam( b, k1 );
qlgam( a, k2 );
qadd( a, b, t );
qlgam( t, t );
qsub( k2, t, t );
qsub( k1, t, t );
qmov( t, lgmq );	/* for qincbi to use */

qadd( pk, t, t );
qadd( pkm1, t, t );
qexp( t, t );
/* multiply by ans/a */
qdiv( a, t, t );
qmul( ans, t, y );

#if 0
goto idone;


incbd:
qmov( a, k1 );
qsub( qone, b, k2 );
qmov( a, k3 );
qsub( qone, a, k4 );
qmov( qone, k5 );
qadd( a, b, k6 );
qadd( a, qone, k7 );
qadd( a, qtwo, k8 );
qclear( pkm2 );
qmov( qone, qkm2 );
qmov( qone, pkm1 );
qmov( qone, qkm1 );
qsub( x, qone, z );
qdiv( z, x, z );
qmov( qone, ans );
qmov( qone, r );
n = 0;
do
  {
    qmul( z, k1, xk );
    qmul( xk, k2, xk );
    qmul( k3, k4, pk );
    qdiv( pk, xk, xk );
    qneg( xk );

    qmul( x, pkm2, pk );
    qadd( pkm1, pk, pk );

    qmul( qkm2, xk, qk );
    qadd( qkm1, qk, qk );

    qmov( pkm1, pkm2 );
    qmov( pk, pkm1 );
    qmov( qkm1, qkm2 );
    qmov( qk, qkm1 );

    qmul( z, k5, xk );
    qmul( xk, k6, xk );
    qmul( k7, k8, pk );
    qdiv( pk, xk, xk );

    qmul( pkm2, xk, pk );
    qadd( pkm1, pk, pk );

    qmul( qkm2, xk, qk );
    qadd( qkm1, qk, qk );

    qmov( pkm1, pkm2 );
    qmov( pk, pkm1 );
    qmov( qkm1, qkm2 );
    qmov( qk, qkm1 );

    if( qk[1] > 10 )
      qdiv( qk, pk, r );
    if( r[1] > 10 )
      {
	qsub( r, ans, t );
	if( ((int) ans[1] - (int) t[1]) > BITSPREC )
	   goto ddone;
      }
    else
      qmov( qtwo, r );

    qadd( qone, k1, k1 );
    qsub( qone, k2, k2 );
    qadd( qtwo, k3, k3 );
    qadd( qtwo, k4, k4 );
    qadd( qone, k5, k5 );
    qadd( qone, k6, k6 );
    qadd( qtwo, k7, k7);
    qadd( qtwo, k8, k8 );
    if( (((int)qk[1] - EXPONE) + ((int)pk[1] - EXPONE)) > NBITS)
      {
	pkm2[1] -= NBITS;
	pkm1[1] -= NBITS;
	qkm2[1] -= NBITS;
	qkm1[1] -= NBITS;
      }
    if( (((int)qk[1] - EXPONE) < -NBITS) || (((int)pk[1] - EXPONE) < -NBITS) )
      {
	pkm2[1] += NBITS;
	pkm1[1] += NBITS;
	qkm2[1] += NBITS;
	qkm1[1] += NBITS;
      }
  }
while( ++n < 400 );
mtherr("qincb", PLOSS );
ddone:

idone:
#endif

done:

if( rflg )
	qsub( y, qone, y );
return 0;
}



/* 2F1( a, 1-b, a+1, x) */

int qincps( aa, bb, xx, y )
QELT aa[], bb[], xx[], y[];
{
qdiv(aa,qone,k8);
qsub(bb,qone,k1);
qmul(xx,k1,k1);
qadd(qone,aa,k2);
qdiv(k2,k1,k2);
qmov(k2,k3);
qmov(k1,k4);
qmov(qtwo,k5);
qclear(ans);
while( ((int) k8[1] - (int) k2[1]) <  BITSPREC )
  {
    qsub(bb,k5,pk);
    qmul(pk,xx,pk);
    qdiv(k5,pk,k1);

    qmul(k1,k4,k4);

    qadd(aa,k5,pk);
    qdiv(pk,k4,k2);

    qadd(k2,ans,ans);

    qadd(qone,k5,k5);
  }
qadd(ans,k3,ans);
qadd(ans,k8,ans);

qadd(aa,bb,k1);
qlgam(k1,k2);
qlgam(aa,k1);
qsub(k1,k2,k2);
qlgam(bb,k1);
qsub(k1,k2,k2);

qlog(xx,k1);
qmul(aa,k1,k1);
qadd(k1,k2,k2);

qlog(ans,k1);
qadd(k1,k2,k2);
qexp(k2,y);
return 0;
}
