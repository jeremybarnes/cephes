/*							hyp2f1.c
 *
 *	Gauss hypergeometric function   F
 *	                               2 1
 *
 *
 * SYNOPSIS:
 *
 * int qhy2f1( a, b, c, x, y );
 * QELT *a, *b, *c, *x, *y;
 *
 * qhy2f1( a, b, c, x, y );
 *
 *
 * DESCRIPTION:
 *
 *
 *  hyp2f1( a, b, c, x )  =   F ( a, b; c; x )
 *                           2 1
 *
 *           inf.
 *            -   a(a+1)...(a+k) b(b+1)...(b+k)   k+1
 *   =  1 +   >   -----------------------------  x   .
 *            -         c(c+1)...(c+k) (k+1)!
 *          k = 0
 *
 *
 * ACCURACY:
 *
 * Expansions are set to terminate at less than full working precision.
 *
 */

/*
Cephes Math Library Release 2.2:  November, 1992
Copyright 1984, 1987, 1992 by Stephen L. Moshier
*/

#include "mconf.h"
#include "qhead.h"
extern QELT qone[], qhalf[], qtwo[];
static QELT q34[NQ];
static QELT an[NQ];
static QELT bn[NQ];
static QELT cn[NQ];
static QELT a0[NQ];
static QELT sum[NQ];
static QELT qn[NQ];
static QELT t[NQ];

static QELT p[NQ];
static QELT q[NQ];
static QELT r[NQ];
static QELT s[NQ];
static QELT d[NQ];

static QELT d1[NQ];
static QELT d2[NQ];
static QELT d3[NQ];

#ifdef ANSIPROT
int qhs2f1( QELT *, QELT *, QELT *, QELT *, QELT * );
int qhz2f1( QELT *, QELT *, QELT *, QELT *, QELT * );
#else
int qpow(), qgamma(), qhs2f1(), qhz2f1(), qfloor(), qcmp();
#endif

int qhy2f1( a, b, c, x, y )
QELT a[], b[], c[], x[], y[];
{
QELT e[NQ], s[NQ];
long i, n;

#if 0
	qhz2f1( a, b, c, x, y );
#else
/* Check for negative integer a or b. */
if( a[0] != 0 || a[1] < 3 )
  {
    qfloor( a, s );
    if( qcmp(a, s) == 0 )
        return qhs2f1( a, b, c, x, y );
  }
if( b[0] != 0 || b[1] < 3 )
  {
    qfloor( b, s );
    if( qcmp(b, s) == 0 )
      return qhs2f1( a, b, c, x, y );
  }

/* test c - a - b > 0 */
qmov( c, s );
qsub( a, s, s );
qsub( b, s, s );
if( s[0] == 0 )
	{
	qhz2f1( a, b, c, x, y );
	goto done;
	}
/* make d > 0 by recurrence on c */
qneg(s);
qadd( qhalf, s, s );
qfloor( s, s );
qadd( qtwo, s, s );
qadd( qtwo, s, s );
qifrac( s, &n, e ); /* the loop count */
qmov( c, e );
qadd( s, e, e ); /* starting value of e */
qhz2f1( a, b, e, x, d2 );
qadd( qone, e, e );
qhz2f1( a, b, e, x, d1 );
qsub( qone, e, e );
qsub( x, qone, s );  /* s = 1 - x */
for( i=0; i<n; i++ )
	{
/*d3 = (e*( e-1.0-(2.0*e-a-b-1.0)*x )*d2 + (e-a)*(e-b)*x*d1)/(e*(e-1)*s) */
	qmul( qtwo, e, p );
	qsub( a, p, p );
	qsub( b, p, p );
	qsub( qone, p, p );
	qmul( x, p, p );
	qneg(p);
	qsub( qone, p, p );
	qadd( e, p, p );
	qmul( e, p, p );
	qmul( d2, p, p );

	qsub( a, e, q );
	qsub( b, e, r );
	qmul( r, q, q );
	qmul( x, q, q );
	qmul( d1, q, q );
	qadd( q, p, p );

	qsub( qone, e, r );
	qmul( e, r, r );
	qmul( s, r, r );
	qdiv( r, p, d3 );

	qsub( qone, e, e ); /* e -= 1 */
	qmov( d2, d1 );
	qmov( d3, d2 );
	}
qmov( d3, y );
done:	;
#endif
return 0;
}


static QELT aa[NQ], bb[NQ], cc[NQ], xx[NQ];

int qhz2f1( a, b, c, x, y )
QELT a[], b[], c[], x[], y[];
{


if( (x[0] != 0) && (x[1] >= (QELT) (EXPONE - 1)) )
	{
	qsub( b, c, r );		/* r = c - b */
	qsub( x, qone, s );		/* s = 1 - x */
	qdiv( s, x, xx );		/* -x/s  */
	qneg( xx );
	if( qcmp( b, a ) > 0 )	/* b > a  */
		{
/* y = pow( s, -a ) * hys2f1( a, r, c, -x/s ) */
		qmov(a, aa);
		qneg(aa);
		qpow( s, aa, y );
		qhs2f1( a, r, c, xx, cc );
		qmul( cc, y, y );
		return 0;
		}

	else
		{
/* y = pow( s, -b ) * hys2f1( p, b, c, -x/s ) */
		qmov( b, aa );
		qneg( aa );
		qpow( s, aa, y );
		qsub( a, c, p );	/* p = c - a */
		qhs2f1( p, b, c, xx, cc );
		qmul( cc, y, y );
		return 0;
		}
	}

/* Make 15/16.  */
qmov(qone,q34);
q34[1] -= 4;
qsub(q34,qone,q34);

if( qcmp( x, q34 ) > 0 )	/* x > 15/16  */
{
/* q = hys2f1( a, b, 1.0-d, s ) */
qsub( x, qone, s );
qadd( a, b, d );  /* d = c - a - b */
qsub( d, c, d );
/* If d is an integer, we can't use this.  */
qfloor(d, an);
if( qcmp(d,an) == 0 )
  goto doqhs;

qsub( d, qone, cc );
qhs2f1( a, b, cc, s, q );
qsub( a, c, p );	/* p = c - a */
qsub( b, c, r );	/* r = c - b */
/* q *= gamma(d) /(gamma(c-a) * gamma(c-b)) */
qgamma( r, cc );
qgamma( p, bb );
qmul( cc, bb, cc );
qgamma( d, bb );
qdiv( cc, bb, cc );
qmul( q, cc, q );

/* If either a or b is a negative integer, this term is zero. */
qfloor(a, an);
qfloor(b, bn);
if( (a[0] != 0 && qcmp(a,an) == 0)
   || (b[0] != 0 && qcmp(b,bn) == 0) )
  {
    qclear(r);
  }
else
  {
    /* r = hys2f1( c-a, c-b, d+1.0, s ) */
    qadd( qone, d, cc );
    qhs2f1( p, r, cc, s, r );

    /* r *= gamma(-d)/(gamma(a) * gamma(b)) */
    qgamma( a, cc );
    qgamma( b, bb );
    qmul( bb, cc, cc );
    qmov( d, aa );
    qneg( aa );
    qgamma( aa, bb );
    qdiv( cc, bb, cc );
    qmul( r, cc, r );
    /* r *= pow( s, d ) */
    qpow( s, d, cc );
    qmul( r, cc, r );
  }

/* y = (q + r) * gamma(c) */
qgamma( c, cc );
qadd( q, r, bb );
qmul( bb, cc, y );
return 0;
}

doqhs:

/* |x| <= 0.5 */
qhs2f1( a, b, c, x, y );
return 0;
}



/* Ascending power series for 2F1 */

int qhs2f1( a, b, c, x, y )
QELT a[], b[], c[], x[], y[];
{
QELT max;
#if DEBUG
double da, db, dc, dx;
#endif

max = 0;
qmov( a, an );		/*an = a;*/
qmov( b, bn );		/*bn = b;*/
qmov( c, cn );		/*cn = c */
qmov( qone, a0 );	/*a0 = 1.0;*/
qmov( qone, sum );	/*sum = 1.0;*/
qmov( qone, qn );	/*n = 1.0;*/

do
	{
	if( an[1] < 3 )
		goto done;
	if( bn[1] < 3 )
		goto done;
	if( cn[1] < 3 )
		{
		qinfin(sum);
		goto done;
		}
	qmul( cn, qn, t );
	qdiv( t, a0, a0 );
	qmul( an, x, t );
	qmul( bn, t, t );
	qmul( t, a0, a0 );	/*a0 *= (an * bn * x) / (cn * n);*/
	qadd( sum, a0, sum );	/*sum += a0;*/
	if( a0[1] > max )
	  max = a0[1];
	qadd( an, qone, an );	/*an += 1.0;*/
	qadd( bn, qone, bn );	/*bn += 1.0;*/
	qadd( cn, qone, cn );	/*cn += 1.0;*/
	qadd( qn, qone, qn );	/*n += 1.0;*/
	}
while( (a0[1] + 90) > sum[1] );

done:
max -= sum[1];
if( (int) max > (NBITS-66) )
  {
    mtherr( "qhy2f1", PLOSS );
#if DEBUG
    qtoe( a, &da );
    qtoe( b, &db );
    qtoe( c, &dc );
    qtoe( x, &dx );
    printf("2F1(%.3e %.3e %.3e %.3e) loses %d bits\n",
	 da, db, dc, dx, (int) max );
#endif
  }
qmov( sum, y );
return(0);
}
