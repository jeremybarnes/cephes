/*   qkne.c
 *
 *  exp(x) sqrt(x) Kn(x)
 */

/* Series expansions are set to terminate at less than full
   working precision.  */
/* Relative accuracy about 22 decimals at crossover point. */
/* See qkn.c.  */

#include "mconf.h"
#include "qhead.h"

extern QELT qone[], qtwo[], qeul[], qpi[];
#define MAXFAC 150
static QELT k[NQ];
static QELT kf[NQ];
static QELT nk1f[NQ];
static QELT nkf[NQ];
static QELT zn[NQ];
static QELT t[NQ];
static QELT s[NQ];
static QELT z0[NQ];
static QELT z[NQ];
static QELT ans[NQ];
static QELT fn[NQ];
static QELT pn[NQ];
static QELT pk[NQ];
static QELT zmn[NQ];
static QELT t1[NQ];
static QELT t2[NQ];
static QELT tlg[NQ];
static QELT qspi[NQ];
static QELT qsflg = 0;
#ifndef ANSIPROT
int qtoe();
#endif

int qkne( nn, x, y )
int nn;
QELT x[], y[];
{
long i, n, lk, lj;
union
  {
    unsigned short s[4];
    double d;
  } dx;

if( nn < 0 )
	n = -nn;
else
	n = nn;

if( (x[0] != 0.0) || (x[1] < 3) || (n > MAXFAC) )
	{
/* Special fudge: return sqrt(pi/2) */
	qmov( qpi, z );
	z[1] -= 1;
	qsqrt( z, y );
	return 0;
	}


qtoe( x, dx.s );
if( dx.d > 24.0 )
	goto asymp;

qclear(ans);		 /* ans = 0.0;*/
qmul( x, x, z0 );	/* z0 = 0.25 * x * x; */
z0[1] -= 2;
qmov( qone, fn );	/* fn = 1.0; */
qclear(pn);		/* pn = 0.0; */
qmov( qone, zmn );	/* zmn = 1.0; */

if( n > 0 )
	{
	/* compute factorial of n and psi(n) */
	qmov( qeul, pn );	/* pn = -EUL; */
	qneg(pn);
	qmov( qone, k );	/* k = 1.0; */
	for( i=1; i<n; i++ )
		{
		qdiv( k, qone, t );	/* pn += 1.0/k; */
		qadd( pn, t, pn );
		qadd( qone, k, k );	/* k += 1.0; */
		qmul( fn, k, fn );	/* fn *= k; */
		}

	qdiv( x, qtwo, zmn );		/* zmn = 2.0/x; */

	if( n == 1 )
		{
		qdiv( x, qone, ans );	/* ans = 1.0/x; */
		}
	else
		{
		ltoq( &n, t );
		qdiv( t, fn, nk1f );	/* nk1f = fn/n; */
		qmov( qone, kf );	/* kf = 1.0; */
		qmov( nk1f, s );	/* s = nk1f; */
		qmov( z0, z );		/* z = -z0; */
		qneg( z );
		qmov( qone, zn );	/* zn = 1.0; */
		for( i=1; i<n; i++ )
			{
			lk = n - i;		/* nk1f = nk1f/(n-i); */
			ltoq( &lk, t );
			qdiv( t, nk1f, nk1f );
			ltoq( &i, t );		/* kf = kf * i; */
			qmul( kf, t, kf );
			qmul( zn, z, zn );	/* zn *= z; */
			qmul( nk1f, zn, t );	/* t = nk1f * zn / kf; */
			qdiv( kf, t, t );
			qadd( s, t, s );	/* s += t; */
			qdiv( x, zmn, zmn );	/* zmn *= 2.0/x; */
			zmn[1] += 1;
			}
		qmul( s, zmn, ans );		/* ans = s * zmn * 0.5; */
		ans[1] -= 1;
		}
	}


qmov( x, s );	/* 2 log(x/2) */
s[1] -= 1;
qlog( s, tlg );
tlg[1] += 1;

qmov( qeul, pk );		/* pk = -EUL; */
qneg( pk );
if( n == 0 )
	{
	qmov( pk, pn );		/* pn = pk; */
	qmov( qone, t );	/* t = 1.0; */
	}
else
	{
	ltoq( &n, t );		/* pn = pn + 1.0/n; */
	qdiv( t, qone, t );
	qadd( pn, t, pn );	
	qdiv( fn, qone, t );	/* t = 1.0/fn; */
	}
qadd( pk, pn, s );		/* s = (pk+pn)*t; */
qsub( tlg, s, s );		/* pk + pn - 2log(x/2) */
qmul( t, s, s );
lk = 1;		/* k = 1.0; */
do
	{
	lj = lk + n;		/* t *= z0 / (k * (k+n)); */
	ltoq( &lj, t1 );
	ltoq( &lk, t2 );
	qmul( t2, t1, z );
	qdiv( z, z0, z );
	qmul( t, z, t );
	qdiv( t2, qone, z );	/* pk += 1.0/k; */
	qadd( pk, z, pk );
	qdiv( t1, qone, z );	/* pn += 1.0/(k+n); */
	qadd( pn, z, pn );
	qadd( pk, pn, z );	/* s += (pk+pn)*t; */
	qsub( tlg, z, z );	/* pk + pn - 2log(x/2) */
	qmul( z, t, z );
	qadd( s, z, s );
	lk += 1.0;
	}
while( ((int) s[1] - (int) t[1]) < NBITS/2 ); /* fabs(t/s) > MACHEP ); */

if( n > 0 )
	qdiv( zmn, s, s );		/* s = 0.5 * s / zmn; */
s[1] -= 1;
if( n & 1 )
	qneg( s );		/* s = -s; */
qadd( ans, s, y );		/* ans += s; */

/* multiply by exp(x) sqrt(x) */
qexp( x, t );
qmul( t, y, y );
qsqrt( x, t );
qmul( t, y, y );

return 0;

/* Asymptotic expansion for Kn(x) */
/* Converges to 1.4e-17 for x > 18.4 */

asymp:

lk = 4 * n * n;			/* pn = 4.0 * n * n; */
ltoq( &lk, pn );
qmov( qone, pk );		/* pk = 1.0; */
qmov( x, z0 );			/* z0 = 8.0 * x; */
z0[1] += 3;
qmov( qone, fn );		/* fn = 1.0; */
qmov( qone, t );		/* t = 1.0; */
qmov( t, s );			/* s = t; */
qmov( qone, nkf );		/* nkf = MAXNUM; */
nkf[1] += 16000;
i = 0;
do
	{
	qmul( pk, pk, t1 );	/* z = pn - pk * pk; */
	qsub( t1, pn, z );
	qmul( fn, z0, t1 );	/* t = t * z /(fn * z0); */
	qdiv( t1, z, t1 );
	qmul( t, t1, t );
	qmov( t, nk1f );	/* nk1f = fabs(t); */
	nk1f[0] = 0;
	qsub( nkf, nk1f, t1 );
	if( (i >= n) && (t1[0] == 0) ) /* nk1f > nkf */
		{
/*		printf( "qkn: i=%D, %d\n", i, t[1]-s[1] );*/
		goto adone;
		}
	qmov( nk1f, nkf );	/* nkf = nk1f; */
	qadd( s, t, s );	/* s += t; */
	qadd( qone, fn, fn );	/* fn += 1.0; */
	qadd( qtwo, pk, pk );	/* pk += 2.0; */
	i += 1;
	}
while( ((int) s[1] - (int) t[1]) < NBITS/2 );

adone:

/* Normal result  = exp(-x) * sqrt( PI/(2.0*x) ) * s; */
/*
qdiv( x, qpi, z );
z[1] -= 1;
qsqrt( z, z );
qexp( x, t );
qdiv( t, z, ans );
qmul( s, ans, y );
*/

/* Instead, just multiply by sqrt(PI/2) */
if( qsflg == 0 )
	{
	qmov( qpi, z );
	z[1] -= 1;
	qsqrt( z, qspi );
	qsflg = 1;
	}
qmul( qspi, s, y );
return 0;
}
