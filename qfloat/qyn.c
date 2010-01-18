/*							qyn.c
 *
 *	Real bessel function of second kind and general order.
 *
 *
 *
 * SYNOPSIS:
 *
 * int qyn( v, x, y );
 * QELT *v, *x, *y;
 *
 * qyn( v, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns Bessel function of order v.
 * If v is not an integer, the result is
 *
 *    Y (z) = ( cos(pi v) * J (x) - J  (x) )/sin(pi v)
 *     v                     v       -v
 *
 * Hankel's expansion is used for large x:
 *
 * Y (z) = sqrt(2/(pi z)) (P sin w + Q cos w)
 *  v
 *
 * w = z - (.5 v + .25) pi
 *
 *
 *         (u-1)(u-9)   (u-1)(u-9)(u-25)(u-49)
 * P = 1 - ---------- + ---------------------- - ...
 *                 2                  4
 *          2! (8z)            4! (8z)
 *
 *
 *      (u-1)    (u-1)(u-9)(u-25)
 * Q =  -----  - ---------------- + ...
 *        8z                3
 *                   3! (8z)
 *
 *         2
 *  u = 4 v
 *
 * (AMS55 #9.2.6).
 *
 *
 *                 -n   n-1
 *           -(z/2)      -  (n-k-1)!   2    k
 * Y (z)  =  -------     >  -------- (z / 4)   +  (2/pi) ln (z/2) J (z)
 *  n           pi       -     k!                                  n
 *                      k=0
 *
 *
 *                  n     inf                             2    k
 *             (z/2)       -                          (- z / 4)
 *           - ------  -   >  (psi(k+1) + psi(n+k+1)) ----------
 *               pi        -                            k!(n+k)!
 *                        k=0
 *
 *  (AMS55 #9.1.11).
 *
 * ACCURACY:
 *
 * Series expansions are set to terminate at less than full working
 * precision.
 *
 */

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1996 by Stephen L. Moshier
*/

/* enable compilation of auxiliary functions */
#define AUXFUN 0
/* debugging  */
#define ERRCK 1
#define DEBUG 0

#include <stdio.h>
#include "qhead.h"

extern QELT qeul[], qone[], qtwo[], qpi[];

/* values computed by qhank() (see qjn.c): */
extern QELT hankc[];
extern QELT hanks[];
extern QELT hankpp[];
extern QELT hankqq[];
extern QELT hankzz[];

static QELT jn[NQ] = {0};	/* answer saved from qjn() */
#if AUXFUN
static QELT yn[NQ] = {0};	/* answer saved from qyn() */

static QELT m[NQ] = {0};
static QELT j[NQ] = {0};
static QELT k[NQ] = {0};
#endif
static QELT z[NQ] = {0};

#ifdef ANSIPROT
int qhank( QELT *, QELT *, QELT * );
int qynrecur( long, long, QELT *, QELT * );
#else
int qhank(), qynrecur();
int qlog(), qjn();
#endif

static QELT nfac[NQ], nm1fac[NQ], f[NQ], a[NQ], psi1[NQ], psin[NQ];
static QELT g[NQ], h[NQ], s[NQ];


int qyn( qn, x, y )
QELT qn[], x[], y[];
{
long i, k, n, sign, kpn;
volatile double du, dn;
union
  {
    unsigned short s[4];
    double d;
  } dz;
#if ERRCK
QELT bt;

bt = 0;
#endif
qfloor( qn, h );
if( qcmp(qn, h) != 0 )
	{
	/* y = (cos(PI*v) * jv( v, x ) - jv( -v, x ))/sin(PI*v); */
	qjn( qn, x, g );
	qmul( qpi, qn, a );
	qcos( a, f );
	qmul( f, g, g );

	qmov( qn, f );
	qneg( f );
	qjn( f, x, h );
	qsub( h, g, g );

	qsin( a, f );
	qdiv( f, g, y );
	return 0;
	}
qtoe( qn, dz.s );
dn = dz.d;
n = dn;
if( n < 0 )
	{
	n = -n;
	if( (n & 1) == 0 )	/* -1**n */
		sign = 1;
	else
		sign = -1;
	}
else
	sign = 1;


if( x[1] == 0 )
	{
	mtherr("qyn", OVERFLOW );
	qinfin(y);
	return( 0 );
	}


qtoe( x, dz.s );
du = dz.d;

if( (du > 64.0) && (du > 0.95*dn) )
	{
/* Hankel expansion will be invoked. */
	if( du > 1.4*dn )
		{
#if AUXFUN
		qjn( qn, x, jn );
#endif
		qhank( qn, x, g );
		qmul( hankpp, hanks, g );
		qmul( hankqq, hankc, h );
		qadd( g, h, y );
		qmul( hankzz, y, y );
		goto qyndone;
		}
	else
		{
		k = 0.7 * du;
		qmov( x, g );
		qynrecur( k, n, g, y );
		goto qyndone;
		}
	}


/* factorial of n */
qmov( qone, nfac);	/*nfac = 1.0;*/
qclear( f );		/*f = 0;*/
for( i=0; i<n-1; i++ )
	{
	qadd( qone, f, f );	/*f += 1.0;*/
	qmuli( f, nfac, nfac );	/*nfac *= f;*/
	}
qmov( nfac, nm1fac );		/*nm1fac = nfac;*/
qadd( qone, f, f );		/*f += 1.0;*/
qmul( nfac, f, nfac );		/*nfac *= f;*/
/*printf("nfac %.4E\n", nfac );*/
qdiv( nfac, qone, a );		/*a = 1.0/nfac;*/

/* psi function */
qclear( psi1 );			/*psi1 = 0;*/
qclear( psin );			/*psin = 0;*/
for( i=1; i<=n; i++ )
	{
	ltoq( &i, z );
	qdiv( z, qone, z );
	qadd( psin, z, psin );	/*psin += 1.0/i;*/
	}
/*printf("psin %.4E\n", psin );*/
qmul( x, x, z );
z[1] -= 2;
z[0] = -1;			/*z = -x*x/4.0;*/
qdiv( nfac, psin, s );		/*s = psin/nfac;*/
kpn = n+1;
qmov( qone, f );		/* k = 1 */
ltoq( &kpn, g );
do
	{
	qdiv( f, qone, h );
	qadd( psi1, h, psi1 );	/*psi1 += 1.0/k;*/
	qdiv( g, qone, h );
	qadd( psin, h, psin );	/*psin += 1.0/kpn;*/
	qmuli( f, g, h );
	qdiv( h, z, h );
	qmul( h, a, a );	/*a *= z/(k*kpn);*/
	qadd( psi1, psin, h );
	qmul( h, a, h );
	qadd( s, h, s );	/*s += a*(psi1+psin);*/
#if ERRCK
	/* remember largest term summed */
	if( h[1] > bt )
		bt = h[1];
	if( s[1] > bt )
		bt = s[1];
#endif
	qadd( qone, f, f );	/*k += 1;*/
	qadd( qone, g, g );	/*kpn += 1;*/
	}
while( ((int) s[1] - (int) a[1]) < (3*NBITS)/4 );
/*printf("infinite %.4E\n", s);*/
#if ERRCK
/* estimate cancellation error */
i = bt - s[1];
if( i > NBITS/2
    || DEBUG )
	printf( "qyn pseries: %ld bits cancellation\n", i );
#endif

/*							qyn.c 3		*/
/* finite sum */

qclear( f );			/*f = 0;*/
if( n > 0 )
{
z[0] = 0;			/*z = -z;*/
qmov( nm1fac, a );		/*a = nm1fac;*/
kpn = n - 1;
qmov( a, f );			/*f = a;*/
ltoq( &kpn, g );
qmov( qone, h );		/* k = 1 */
for( k=1; k<n; k++ )
	{
	qmul( g, h, nm1fac );
	qdiv( nm1fac, z, nm1fac );
	qmul( a, nm1fac, a );	/*a *= z/(k*kpn);*/
	qadd( f, a, f );	/*f += a;*/
	qsub( qone, g, g );	/*kpn -= 1;*/
	qadd( qone, h, h );
	}
}
/*printf("finite %.4E\n", f);*/
/* x/2**n */
qmov( x, a );
a[1] -= 1;			/*a = x/2;*/
qmov( qone, z );		/*z = 1.0;*/
for( i=1; i<=n; i++ )
	qmul( z, a, z );	/*z *= a;*/
/*printf("x/2**n %.4E\n", z );*/
/* combine the terms */
/*s = 2.0*(log(a)+EUL)*jn(n,x) - z*s - f/z;*/
qlog( a, g );
qadd( g, qeul, g );
i = n;	/* convert integer n to q type for qjn() */
ltoq( &i, h );
qjn( h, x, jn );
qmul( jn, g, g );
g[1] += 1;

qdiv( z, f, f );
qsub( f, g, g );

qmul( z, s, f );
qsub( f, g, g );

qdiv( qpi, g, y );		/*s /= PI;*/
qyndone:
if( sign < 0 )
	y[0] = ~y[0];		/*return(s*sign);*/
return( 0 );
}


/* Use forward recurrence from order k to order n
 * The asymptotic expansion would work for noninteger n
 * as long as n-k is an integer.
 */
int qynrecur( k, n, x, y )
long k, n;
QELT x[], y[];
{
QELT r[NQ], anm1[NQ], anm2[NQ], g[NQ], h[NQ];

/* Calculate Y_k(x), Y_k+1(x) for relatively small k */
ltoq( &k, r );
qhank( r, x, g );
qmul( hankpp, hanks, g );
qmul( hankqq, hankc, h );
qadd( g, h, anm2 );
qmul( hankzz, anm2, anm2 );
qadd( qone, r, r );
k += 1;
qhank( r, x, g );
qmul( hankpp, hanks, g );
qmul( hankqq, hankc, h );
qadd( g, h, anm1 );
qmul( hankzz, anm1, anm1 );
qmul( qtwo, r, r ); /* r = 2 * k; */
/* Y_k+1 = 2k/x Y_k  -  Y_k-1 */
do
	{
	qmul( anm1, r, g ); /* an = r * anm1 / x  -  anm2; */
	qdiv( x, g, g );
	qsub( anm2, g, g );
	qmov( anm1, anm2 ); /* anm2 = anm1; */
	qmov( g, anm1 );   /* anm1 = an; */
	qadd( qtwo, r, r ); /* r += 2.0; */
	}
while( ++k < n );
qmov( g, y );
return 0;
}


#if AUXFUN
/* auxiliary function for y0 approximation			*/

int qyaux0( x, y )
QELT *x, *y;
{
QELT a[NQ], c[NQ];

if( x[1] < 2 )	/* arg == 0 then ans = 2/pi * ( Eul + ln(0.5) ) */
	{
	qmov( qone, a );
	a[1] -= 1;
	qlog( a, a );
	qadd( qeul, a, a );
	a[1] += 1;
	qdiv( qpi, a, y );
	return(0);
	}

/* else ans = y0(x) - 2/pi * ln(x) * j0(x)	*/
qclear( a );
qyn( a, x, yn );
qclear( a );
/*qjn( a, x, a );*/
qlog( x, c );
qmul( jn, c, c );
c[1] += 1;
qdiv( qpi, c, c );
qsub( c, yn, y );
return(0);
}

/*	auxiliary function for y1 approximation			*/

int qyaux1( x, y )
QELT x[], y[];
{
QELT a[NQ], c[NQ];

if( x[1] < 2 )	/* arg == 0 then ans = 0 */
	{
	qclear( y );
	return(0);
	}

/* else ans = y1(x) - 2/pi * ( ln(x) * j1(x) - 1/x )	*/
qmov( qone, a );
qyn( a, x, yn );
/*
qmov( qone, a );
qjn( a, x, a );
*/
qlog( x, c );
qmul( jn, c, c );
qdiv( x, qone, a );	/* 1/x			*/
qsub( a, c, c );
c[1] += 1;
qdiv( qpi, c, c );	/*   * 2 / pi		*/
qsub( c, yn, y );
return(0);
}

/* Phase = atan( yn(x)/jn(x) */

int qyphase( qn, qx, qy )
QELT qn[], qx[], qy[];
{
QELT xx[NQ], c[NQ], z[NQ];

if( qx[1] < 3 )
	{
	qclear( qy );
	goto fdone;
	}

/*c = yn(xx)/jn(xx);*/
qyn( qn, qx, z );
qdiv( jn, z, c );


/*z = tan( (n/2 + 1/4)PI - x );*/
qmov( qone, xx );
xx[1] -= 1;
qadd( qn, xx, xx );
xx[1] -= 1;
qmul( qpi, xx, z );

qsub( qx, z, z );
qtan( z, z );

/*t = (c + z)/(1.0 - c*z);*/
qadd( c, z, xx );
qmul( c, z, z );
qsub( z, qone, z );
qdiv( z, xx, xx );

/*y = atan( t );*/
qatn( xx, qy );

fdone:	;
return 0;
}


/* Modulus**2 =  jn(xx)**2 + yn(xx)**2 */

int qymod( qn, qx, qy )
QELT qn[], qx[], qy[];
{
QELT u[NQ], ans[NQ], t[NQ];
int term, min;


if( qx[1] < 3 )
	{
	qinfin( qy );
	goto fdone;
	}

if( qx[1] < (EXPONE + 6) )	/* x < 64 */
	{ 
	qyn( qn, qx, z );
	qmul( jn, jn, jn );
	qmul( z, z, z );
	qadd( jn, z, qy );
	goto fdone;
	}

/* asymptotic expansion
 * AMS55 # 9.2.28
 */
qmul( qn, qn, m );	/* m = 4.0*n*n;*/
m[1] += 2;
qclear(  j );		/* j = 0.0;*/
qmov( qone, k );	/* k = -1.0;*/
qneg( k );
qmul( qx, qx, z );	/* z = (2x)**2*/
z[1] += 2;
qmov( qone, u );	/* u = 1.0 */
qmov( qone, ans );	/* ans = 1.0;*/
min = MAXEXP;

while( ((int) qone[1] - (int) u[1]) < NBITS/2 )
	{
	qadd( qtwo, k, k );	/* k += 2.0;*/
	qadd( qtwo, j, j );	/* j += 2.0;*/
	qmuli( k, k, t );	/* u *= k * (m - k * k)/(j * z);*/
	qsub( t, m, t );
	qmuli( k, t, t );
	qmul( t, u, u );
	qmuli( j, z, t );
	qdiv( t, u, u );
	qadd( u, ans, ans );
	term = u[1];
	if( term >= min )
		{
		printf( "qjmod converges to %d bits\n", qone[1] - term );
		goto adone;
		}
	min = term;
	}	

adone:

qmul( qx, qpi, t );
qdiv( t, ans, ans );
ans[1] += 1;
qmov( ans, qy );


fdone:		;
return 0;
}

#endif /* AUXFUN */
