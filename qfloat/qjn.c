/*							qjn.c
 *
 *	Bessel function of noninteger order
 *
 *
 *
 * SYNOPSIS:
 *
 * int qjn( v, x, y );
 * QELT *v, *x, *y;
 *
 * qjn( v, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of order v of the argument,
 * where v is real.  Negative x is allowed if v is an integer.
 *
 * Two expansions are used: the ascending power series and the
 * Hankel expansion for large v.  If v is not too large, it
 * is reduced by recurrence to a region of better accuracy.
 *
 */

/*
Cephes Math Library Release 2.2:  July, 1992
Copyright 1984, 1987, 1989, 1992 by Stephen L. Moshier
*/

/*							qjn.c	*/
/* jn.c		1 Dec 83
 * Bessel function of order n
 */

#define DEBUG 0
#define ERRCK 1
#include <stdio.h>
#include "qhead.h"

extern QELT qhalf[], qone[], qtwo[], qpi[];

static QELT t[NQ];
static QELT u[NQ];
QELT hankzz[NQ];
static QELT k[NQ];
static QELT ans[NQ];
QELT hankc[NQ];
QELT hanks[NQ];
static QELT j[NQ];
static QELT m[NQ];
QELT hankpp[NQ];
QELT hankqq[NQ];
static QELT rans[NQ];
static QELT ru[NQ];

#ifdef ANSIPROT
int qhank( QELT *, QELT *, QELT * );
static int qrecur( QELT *, QELT *, QELT *, QELT * );
#else
double floor(), sqrt();
static int qrecur();
int mtherr(), qhank(), qgamma();
#endif

#define fabs(x) ((x) < 0 ? -(x) : (x))
#if DEBUG
static QELT yh[NQ];
#endif

int qjn( nn, xx, y )
QELT nn[], xx[], y[];
{
QELT n[NQ], x[NQ];
double dx, dn, an, du;
int i, sign;
QELT bt;
union
  {
    unsigned short s[4];
    double d;
  } dz;

bt = 0;
qmov( nn, n );
qmov( xx, x );
qtoe( n, dz.s );
dn = dz.d;
qtoe( x, dz.s );
dx = dz.d;
sign = 1;
an = fabs( dn );
if( an == floor(an) )
	{
	i = an - 16384.0 * floor( an/16384.0 );
	if( n[0] != 0 )
		{
		if( i & 1 )
			sign = -sign;
		n[0] = 0;
		}
	if( x[0] != 0 )
		{
		if( i & 1 )
			sign = -sign;
		x[0] = 0;
		}
	}
else
	{
	if( x[0] != 0 )
		{
		mtherr( "qjv", DOMAIN );
		qclear( y );
		goto done;
		}
	}

dx = fabs(dx);


#if DEBUG
	qhank( n, x, yh);
#endif

#if NBITS > 336
if( dx > 81.0 )
#else
#if NBITS > 144
if( dx > 100.0 )
#else
if( dx > 34.0 )
#endif
#endif
	{
/*	if( dx > 0.95*an )*/
	if( dx > 1.4*an )
		{
		qhank( n, x, y );
		goto done;
		}
	if( dx > 0.7 * an )
		{
		du = 3.6 * sqrt(dx);
/*		du = 0.8 * dx;*/
		dz.d = du;
		etoq( dz.s, ru );
		ru[0] = 0;
		qfloor( n, m );
		qsub( m, n, m );
		qfloor( ru, ru );
		qadd( ru, m, ru );
		if( n[0] == 0 )
			{
			qrecur( n, x, ru, rans );
			}
		else
			{
			qmov( ru, m );
			qmov( n, ru );
			qrecur( m, x, ru, rans );
			qmov( m, ru );
			}
		if( rans[1] == 0 )
			{
			qclear( y );
			goto done;
			}
		qhank( ru, x, y );
		if( n[0] == 0 )
			qdiv( rans, y, y );
		else
			qmul( rans, y, y );
		goto done;
		}
	}

qmul( x, x, hankzz );	/*z = -x * x / 4.0;		*/
/* x = 0, n = 0 is special case	*/

if( hankzz[1] < 3 )
	{
	if( n[1] < 3 )
		qmov( qone, y );
	else
		qclear( y );
	goto done;
	}

hankzz[1] -= 2;
hankzz[0] = -1;

/*                   inf      2   k
 *                 v  -    (-z /4)
 *  J (z)  =  (z/2)   >  ------------
 *   v                -       -
 *                   k=0  k! | (v+k+1)
 */
qmov( n, ans );
qadd( qone, ans, ans );
if( (n[1] < 3) || (qcmp( qone, n ) == 0) )
	qmov( qone, u );
else
	{
	qgamma( ans, u );
	qdiv( u, qone, u );		/*u = 1.0/gamma(n+1);*/
	}
qmov( u, ans );		/*ans = u;*/
qmov( qone, k );	/*k = 1.0;*/

while( u[1] > (QELT) (ans[1] - NBITS)
      || u[1] > (QELT) (qone[1] - NBITS) )
	{
	qadd( n, k, t );
	qmul( t, k, t );	/*u *= z / (k * (n+k));*/
	qdiv( t, hankzz, t );
	qmul( t, u, u );
#if 0
/* #if DEBUG */
	qtoe( u, dz.s );
	du = dz.d
	printf( "pseries term: %.5e\n", du );
#endif
/* remember largest term summed */
#if ERRCK
	if( u[1] > bt )
		bt = u[1];
	if( ans[1] > bt )
		bt = ans[1];
#endif
	qadd( u, ans, ans );	/*ans += u;*/
	qadd( qone, k, k );	/*k += 1.0;*/
	}

/* estimate cancellation error */
#if ERRCK
i = bt - ans[1];
if( i > NBITS/2
    || DEBUG )
	printf( "qjn pseries: %d bits cancellation\n", i );
#endif

/* ans *= exp( n * log( x/2.0 ) );*/

if( n[1] < 3 )
	{
	qmov( ans, y );
	}
else
	{
	qmov( x, t );
	t[1] -= 1;
	qlog( t, u );
	qmul( u, n, u );
	qexp( u, t );
	qmul( ans, t, y );
	}

#if DEBUG
qsub( y, yh, yh );
qtoe( yh, dz.s );
du = dz.d
printf( "qjn - qhank = %.5e\n", du );
#endif
done:
if( sign < 0 )
	y[0] = ~y[0];
return(0);
}


/* Hankel's asymptotic expansion
 * for large x.
 * AMS55 #9.2.5.
 */

int qhank( n, x, y )
QELT n[], x[], y[];
{
QELT bt;
int flag, sign, nsum, i;
union
  {
    unsigned short s[4];
    double d;
  } dconv;

bt = 0;
nsum = 0;
qmul( n, n, m );	/* m = 4.0*n*n;*/
m[1] += 2;
qmov( qone, j );	/* j = 1.0;*/
qmov( x, hankzz );	/* z = 8.0 * x;*/
hankzz[1] += 3;
qmov( qone, k );	/* k = 1.0;*/
qmov( qone, hankc );	/* hankc = 1.0;*/
qsub( qone, m, u );	/* u = (m - 1.0)/z;*/
qdiv( hankzz, u, u );
qmov( u, hanks );	/* hanks = u;*/
sign = 1;
qmov( qone, ans );	/* conv = 1.0;*/
flag = 0;
qmov( qone, t );	/* t = 1.0;*/

while( t[1] > (qone[1] - NBITS)
    || u[1] > (qone[1] - NBITS) )
	{
	qadd( qtwo, k, k );	/* k += 2.0;*/
	qadd( qone, j, j );	/* j += 1.0;*/
	sign = -sign;
	qmul( k, k, t );	/* u *= (m - k * k)/(j * z);*/
	qsub( t, m, t );
	qdiv( j, t, t );
	qdiv( hankzz, t, t );
	qmul( t, u, u );
	if( sign < 0 )		/* hankc += sign * u;*/
		qsub( u, hankc, hankc );
	else
		qadd( u, hankc, hankc );
/* remember largest term summed */
#if ERRCK
	if( u[1] > bt )
		bt = u[1];
	if( hankc[1] > bt )
		bt = hankc[1];
#endif
/*	printf( "Hank P: %.5E %.5E", u, p ); */
	qadd( qtwo, k, k );	/* k += 2.0;*/
	qadd( qone, j, j );	/* j += 1.0;*/
	qmul( k, k, t );	/* u *= (m - k * k)/(j * z);*/
	qsub( t, m, t );
	qdiv( j, t, t );
	qdiv( hankzz, t, t );
	qmul( t, u, u );
	if( sign < 0 )		/* q += sign * u;*/
		qsub( u, hanks, hanks );
	else
		qadd( u, hanks, hanks );
/* remember largest term summed */
#if ERRCK
	if( u[1] > bt )
		bt = u[1];
	if( hanks[1] > bt )
		bt = hanks[1];
#endif
/*	printf( " Q: %.5E %.5E\n", u, q ); */
	qdiv( hankc, u, t );	/* t = fabs(u/p);*/
	t[0] = 0;
	if( qcmp( t, ans ) < 0 )	/* ( t < conv )*/
		{
		qmov( t, ans );	/* conv = t;*/
		qmov( hanks, hankqq );	/* qq = hanks; */
		qmov( hankc, hankpp );	/* pp = hankc; */
		flag = 1;
		nsum += 1;
		}
/* stop if the terms start getting larger */
	else
		{
		if( flag != 0 )
			{
			goto hank1;
			}
		}
	}	

hank1:

/* estimate cancellation error */
#if ERRCK
i = bt - hankpp[1];
if(i > NBITS/2
    || DEBUG )
  {
    qtoe( n, dconv.s );
    printf( "qhank(%.5e,", dconv.d );
    qtoe( x, dconv.s );
    printf( "%.5e): ", dconv.d );
    printf( "%d bits cancellation after %d terms\n", i, nsum );
  }
#endif

#if DEBUG
    qtoe( ans, dconv.s );
    printf( "qhank: last term / sum = %.4E\n", dconv.d );
#endif

qmov( n, t );	/* u = x - (0.5*n + 0.25) * PI;*/
qmul( qhalf, t, t );
qmov( qone, hanks );
hanks[1] -= 2;
qadd( hanks, t, t );
qmul( qpi, t, t );
qsub( t, x, u );

/* t = sqrt( 2.0/(PI*x) ) * ( pp * cos(u) - qq * sin(u) ); */
qmul( qpi, x, t );
qdiv( t, qtwo, t );
qsqrt( t, hankzz );
qsin( u, hanks );
qcos( u, hankc );
qmul( hankc, hankpp, k );
qmul( hanks, hankqq, m );
qsub( m, k, k );
qmul( k, hankzz, y );
#if 0
/* #if DEBUG */
qtoe( y, dconv.s );
printf( "%.4e\n", dconv.d );
#endif
return 0;
}



/* Reduce the order by backward recurrence.
 * AMS55 #9.1.27 and 9.1.73.
 */

static QELT pkm2[NQ];
static QELT pkm1[NQ];
static QELT pk[NQ];
static QELT pkp1[NQ];
static QELT qkm2[NQ];
static QELT qkm1[NQ];
static QELT qk[NQ];
static QELT xk[NQ];
static QELT yk[NQ];
static QELT r[NQ];
static QELT kf[NQ];
static QELT k1[NQ];

static int qrecur( n, x, newn, ans )
QELT n[], x[], newn[], ans[];
{
int nflag, ctr;
#if DEBUG
double da, db;
#endif

nflag = n[0];

/* continued fraction for Jn(x)/Jn-1(x)  */

/* fstart: */

#if DEBUG
qtoe( n, (unsigned short *) &da );
qtoe( newn, (unsigned short *) &db );
printf( "qn = %.6e, qnewn = %.6e, qcfrac = ", da, db );
#endif

qclear( pkm2 );
qmov( qone, qkm2 );
qmov( x, pkm1 );
qadd( n, n, qkm1 );
qmul( x, x, xk );   /* xk = -x * x; */
qneg( xk );
qmov( qkm1, yk );
qmov( qone, ans );
ctr = 0;
do
	{
	qadd( qtwo, yk, yk );
/*	pk = pkm1 * yk +  pkm2 * xk; */
	qmul( pkm1, yk, pk );
	qmul( pkm2, xk, t );
	qadd( t, pk, pk );
/*	qk = qkm1 * yk +  qkm2 * xk;*/
	qmul( qkm1, yk, qk );
	qmul( qkm2, xk, t );
	qadd( t, qk, qk );
	qmov( pkm1, pkm2 );
	qmov( pk, pkm1 );
	qmov( qkm1, qkm2 );
	qmov( qk, qkm1 );
	if( qk[1] != 0 )
		qdiv( qk, pk, r );
	if( r[1] != 0 )
		{
/*		t = fabs( (ans - r)/r ); */
		qsub( r, ans, t );
		qdiv( r, t, t );
		t[0] = 0;
		qmov( r, ans );
		}
	else
		qmov( qone, t );

	if( ++ctr > 1000 )
		{
		printf( "qrecur: continued fraction did not converge\n" );
		goto done;
		}
/*
	if( t < MACHEP )
		goto done;
*/

	if( pk[1] > (qone[1]+NBITS) )
		{
		pkm2[1] -= NBITS;
		pkm1[1] -= NBITS;
		qkm2[1] -= NBITS;
		qkm1[1] -= NBITS;
		}
	}
while( t[1] > (qone[1] - NBITS) );

done:

#if DEBUG
qtoe( ans, (unsigned short *) &da );
printf( "%.6e\n", da );
#endif

/* Change n to n-1 if n < 0 and the continued fraction is small
 */
/*
if( nflag )
	{
	if( fabs(ans) < 0.125 )
		{
		nflag = -1;
		*n = *n - 1.0;
		goto fstart;
		}
	}
*/

qmov( newn, kf );

/* backward recurrence
 *              2k
 *  J   (x)  =  --- J (x)  -  J   (x)
 *   k-1         x   k         k+1
 */

qmov( qone, pk );
qdiv( ans, qone, pkm1 );
qsub( qone, n, k1 );
qadd( k1, k1, r );
do
	{
/*	pkm2 = (pkm1 * r  -  pk * x) / x;*/
	qmul( pkm1, r, pkm2 );
	qmul( pk, x, t );
	qsub( t, pkm2, pkm2 );
	qdiv( x, pkm2, pkm2 );
	qmov( pk, pkp1 );
	qmov( pkm1, pk );
	qmov( pkm2, pkm1 );
	qsub( qtwo, r, r );
/*
	t = fabs(pkp1) + fabs(pk);
	if( (k > (kf + 2.5)) && (fabs(pkm1) < 0.25*t) )
		{
		k1 -= 1.0;
		t = x*x;
		pkm2 = ( (r*(r+2.0)-t)*pk - r*x*pkp1 )/t;
		pkp1 = pk;
		pk = pkm1;
		pkm1 = pkm2;
		r -= 2.0;
		}
*/
	qsub( qone, k1, k1 );
	qadd( qhalf, kf, t );
	}
while( qcmp( k1, t ) > 0 );

/* Take the larger of the last two iterates
 * on the theory that it may have less cancellation error.
 */
/*
if( cancel )
	{
	if( (kf >= 0.0) && (fabs(pk) > fabs(pkm1)) )
		{
		k += 1.0;
		pkm2 = pk;
		}
	}
*/
qmov( k1, newn );
#if DEBUG
qtoe( k1, (unsigned short *) &da );
qtoe( pkm2, (unsigned short *) &db );
printf( "qnewn %.6e qrans %.6e\n", da, db );
#endif
qmov( pkm2, ans );
return(0);
}
