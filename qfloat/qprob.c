/*  qprob.c  */
/* various probability integrals
 * computed via incomplete beta and gamma integrals
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1987, 1995 by Stephen L. Moshier
*/

#include "qhead.h"
extern QELT qone[];
int qigam(), qigamc(), qigami(), qincbi(), qincb();

/* binomial distribution */
/*							qbdtr
 *
 *	Binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qbdtr( k, n, p, y );
 * int k, n;
 * QELT *p, *y;
 *
 * qbdtr( k, n, p, y );
 *
 * DESCRIPTION:
 *
 * Returns (in y) the sum of the terms 0 through k of the Binomial
 * probability density:
 *
 *   k
 *   --  ( n )   j      n-j
 *   >   (   )  p  (1-p)
 *   --  ( j )
 *  j=0
 *
 * The terms are not summed directly; instead the incomplete
 * beta integral is employed, according to the formula
 *
 * y = bdtr( k, n, p ) = incbet( n-k, k+1, 1-p ).
 *
 * The arguments must be positive, with p ranging from 0 to 1.
 *
 */


int qbdtr( k, n, p, y )
int k, n;
QELT p[], y[];
{
QELT dk[NQ], dn[NQ], dp[NQ];
long li;

if( k >= n )
	{
	qmov( qone, y );
	return 0;
	}

li = k + 1;
ltoq( &li, dk );	/* dk = k */

li = n - k;
ltoq( &li, dn );

qmov( p, dp );
qsub( dp, qone, dp );

qincb( dn, dk, dp, y );
return 0;
}

/*							qbdtrc
 *
 *	Complemented binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qbdtrc( k, n, p, y );
 * int k, n;
 * QELT *p, *y;
 *
 * y = qbdtrc( k, n, p, y );
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms k+1 through n of the Binomial
 * probability density:
 *
 *   n
 *   --  ( n )   j      n-j
 *   >   (   )  p  (1-p)
 *   --  ( j )
 *  j=k+1
 *
 * The terms are not summed directly; instead the incomplete
 * beta integral is employed, according to the formula
 *
 * y = bdtrc( k, n, p ) = incbet( k+1, n-k, p ).
 *
 * The arguments must be positive, with p ranging from 0 to 1.
 *
 */

int qbdtrc( k, n, p, y )
int k, n;
QELT p[], y[];
{
QELT dk[NQ], dn[NQ];
long li;

if( k < 0 )
	{
	qmov( qone, y );
	return 0;
	}
if( k == n )
	{
	qclear( y );
	return 0;
	}
li = k + 1;
ltoq( &li, dk );	/* dk = k */

li = n - k;
ltoq( &li, dn );

qincb( dk, dn, p, y );
return 0;
}

/*							qbdtri
 *
 *	Inverse binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qbdtri( k, n, y, p );
 * int k, n;
 * QELT *p, *y;
 *
 * qbdtri( k, n, y, p );
 *
 * DESCRIPTION:
 *
 * Finds the event probability p such that the sum of the
 * terms 0 through k of the Binomial probability density
 * is equal to the given cumulative probability y.
 *
 * This is accomplished using the inverse beta integral
 * function and the relation
 *
 * 1 - p = incbi( n-k, k+1, y ).
 *
 */

int qbdtri( k, n, y, p )
int k, n;
QELT p[], y[];
{
QELT dk[NQ], dn[NQ];
long li;


if( (n <= k) || (k < 0) )
	{
	qclear( y );
	return 0;
	}
li = k + 1;
ltoq( &li, dk );	/* dk = k */

li = n - k;
ltoq( &li, dn );

qincbi( dn, dk, y, p );
qsub( p, qone, p );
return 0;
}


/*							qchdtr
 *
 *	Chi-square distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qchdtr( df, x, y );
 * QELT *df, *x, *y;
 *
 * qchdtr( df, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the area under the left hand tail (from 0 to x)
 * of the Chi square probability density function with
 * v degrees of freedom.
 *
 *
 *                                  inf.
 *                                    -
 *                        1          | |  v/2-1  -t/2
 *  P( x | v )   =   -----------     |   t      e     dt
 *                    v/2  -       | |
 *                   2    | (v/2)   -
 *                                   x
 *
 * where x is the Chi-square variable.
 *
 * The incomplete gamma integral is used, according to the
 * formula
 *
 *	y = chdtr( v, x ) = igam( v/2.0, x/2.0 ).
 *
 *
 * The arguments must both be positive.
 *
 */

int qchdtr( df, x, y )
QELT df[], x[], y[];
{
QELT a[NQ], b[NQ];

qmov( df, a );
qmov( x, b );
a[1] -= 1;
b[1] -= 1;
qigam( a, b, y );
return 0;
}

/*							qchdtc
 *
 *	Complemented Chi-square distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qchdtc( df, x, y );
 * QELT df[], x[], y[];
 *
 * qchdtc( df, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the area under the right hand tail (from x to
 * infinity) of the Chi square probability density function
 * with v degrees of freedom:
 *
 *
 *                                  inf.
 *                                    -
 *                        1          | |  v/2-1  -t/2
 *  P( x | v )   =   -----------     |   t      e     dt
 *                    v/2  -       | |
 *                   2    | (v/2)   -
 *                                   x
 *
 * where x is the Chi-square variable.
 *
 * The incomplete gamma integral is used, according to the
 * formula
 *
 *	y = chdtr( v, x ) = igamc( v/2.0, x/2.0 ).
 *
 *
 * The arguments must both be positive.
 *
 */

int qchdtc( df, x, y )
QELT df[], x[], y[];
{
QELT a[NQ], b[NQ];

qmov( df, a );
qmov( x, b );
a[1] -= 1;
b[1] -= 1;
qigamc( a, b, y );
return 0;
}


/*							qchdti
 *
 *	Inverse of complemented Chi-square distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qchdti( df, y, x );
 * QELT *df, *x, *y;
 *
 * qchdti( df, y, x );
 *
 *
 *
 *
 * DESCRIPTION:
 *
 * Finds the Chi-square argument x such that the integral
 * from x to infinity of the Chi-square density is equal
 * to the given cumulative probability y.
 *
 * This is accomplished using the inverse gamma integral
 * function and the relation
 *
 *    x/2 = igami( df/2, y );
 *
 *
 *
 *
 * ACCURACY:
 *
 * See igami.c.
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * chdtri domain   y < 0 or y > 1        0.0
 *                     v < 1
 *
 */

int qchdti( df, y, x )
QELT df[], x[], y[];
{
QELT a[NQ];

qmov( df, a );
a[1] -= 1;
qigami( a, y, x );
x[1] += 1;
return 0;
}



/*							qfdtr
 *
 *	F distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qfdtr( ia, ib, x, y );
 * int ia, ib;
 * QELT *x, *y;
 *
 * qfdtr( ia, ib, x, y );
 *
 * DESCRIPTION:
 *
 * Returns the area from zero to x under the F density
 * function (also known as Snedcor's density or the
 * variance ratio density).  This is the density
 * of x = (u1/df1)/(u2/df2), where u1 and u2 are random
 * variables having Chi square distributions with df1
 * and df2 degrees of freedom, respectively.
 *
 * The incomplete beta integral is used, according to the
 * formula
 *
 *	P(x) = incbet( df1/2, df2/2, (df1*x/(df2 + df1*x) ).
 *
 *
 * The arguments a and b are greater than zero, and x is
 * nonnegative.
 *
 */

int qfdtr( ia, ib, x, y )
int ia, ib;
QELT x[], y[];
{
QELT a[NQ], b[NQ], u[NQ], v[NQ], w[NQ];
long li;

li = ia;
ltoq( &li, a );
li = ib;
ltoq( &li, b );

qmov( a, w );		/* ax/(b+ax)  */
qmul( x, w, w );
qadd( b, w, u );
qdiv( u, w, w );
qmov( a, u );
u[1] -= 1;
qmov( b, v );
v[1] -= 1;
qincb( u, v, w, y ); /* incbet( a/2.0, b/2.0, (ax/(b+ax)) )  */
return 0;
}


/*							qfdtrc
 *
 *	Complemented F distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qfdtrc( ia, ib, x, y );
 * int ia, ib;
 * QELT x[], y[];
 *
 * qfdtrc( ia, ib, x, y );
 *
 * DESCRIPTION:
 *
 * Returns the area from x to infinity under the F density
 * function (also known as Snedcor's density or the
 * variance ratio density).
 *
 *
 *                      inf.
 *                       -
 *              1       | |  a-1      b-1
 * 1-P(x)  =  ------    |   t    (1-t)    dt
 *            B(a,b)  | |
 *                     -
 *                      x
 *
 *
 * The incomplete beta integral is used, according to the
 * formula
 *
 *	P(x) = incbet( df2/2, df1/2, (df2/(df2 + df1*x) ).
 *
 */

int qfdtrc( ia, ib, x, y )
int ia, ib;
QELT x[], y[];
{
QELT a[NQ], b[NQ], u[NQ], v[NQ], w[NQ];
long li;

li = ia;
ltoq( &li, a );
li = ib;
ltoq( &li, b );

/* b/(b + ax)  */
qmov( a, u );
qmul( x, u, u );
qadd( b, u, u );
qdiv( u, b, w );

qmov( a, u );
u[1] -= 1;
qmov( b, v );
v[1] -= 1;
qincb( v, u, w, y ); /* incbet( b/2.0, a/2.0, (b/(b+ax)) )  */
return 0;
}

/*							qfdtri
 *
 *	Inverse of complemented F distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qfdtri( ia, ib, y, x );
 * int ia, ib;
 * QELT x[], y[];
 *
 * qfdtri( ia, ib, y, x );
 *
 * DESCRIPTION:
 *
 * Finds the F density argument x such that the integral
 * from x to infinity of the F density is equal to the
 * given probability p.
 *
 * This is accomplished using the inverse beta integral
 * function and the relations
 *
 *      z = incbi( df2/2, df1/2, p )
 *      x = df2 (1-z) / (df1 z).
 *
 * Note: the following relations hold for the inverse of
 * the uncomplemented F distribution:
 *
 *      z = incbi( df1/2, df2/2, p )
 *      x = df2 z / (df1 (1-z)).
 *
 */

int qfdtri( ia, ib, y, x )
int ia, ib;
QELT x[], y[];
{
QELT a[NQ], b[NQ], u[NQ], v[NQ], w[NQ];
long li;

li = ia;
ltoq( &li, a );
li = ib;
ltoq( &li, b );

qmov( a, u );
u[1] -= 1;
qmov( b, v );
v[1] -= 1;
qincbi( v, u, y, w ); /* incbi( b/2.0, a/2.0, y )  */

/* x = (b - bw)/aw  */
qmul( b, w, u );
qsub( u, b, u );
qmul( a, w, v );
qdiv( v, u, x );
return 0;
}


/*							qgdtr
 *
 *	Gamma distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qgdtr( a, b, x, y );
 * QELT *a, *b, *x, *y;
 *
 * qgdtr( a, b, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the integral from zero to x of the gamma probability
 * density function:
 *
 *
 *                x
 *        b       -
 *       a       | |   b-1  -at
 * y =  -----    |    t    e    dt
 *       -     | |
 *      | (b)   -
 *               0
 *
 *  The incomplete gamma integral is used, according to the
 * relation
 *
 * y = igam( b, ax ).
 *
 */

int qgdtr( a, b, x, y )
QELT a[], b[], x[], y[];
{
QELT w[NQ];

qmul( a, x, w );
qigam( b, w, y );  /* igam( b, a * x )  */
return 0;
}

/*							qgdtrc
 *
 *	Complemented gamma distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qgdtrc( a, b, x, y );
 * QELT *a, *b, *x, *y;
 *
 * qgdtrc( a, b, x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the integral from x to infinity of the gamma
 * probability density function:
 *
 *
 *               inf.
 *        b       -
 *       a       | |   b-1  -at
 * y =  -----    |    t    e    dt
 *       -     | |
 *      | (b)   -
 *               x
 *
 *  The incomplete gamma integral is used, according to the
 * relation
 *
 * y = igamc( b, ax ).
 *
 */

int qgdtrc( a, b, x, y )
QELT a[], b[], x[], y[];
{
QELT w[NQ];

qmul( a, x, w );
qigamc( b, w, y );  /* igamc( b, a * x )  */
return 0;
}




/*							qnbdtr
 *
 *	Negative binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qnbdtr( k, n, p, y );
 * int k, n;
 * QELT *p, *y;
 *
 * qnbdtr( k, n, p, y );
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms 0 through k of the negative
 * binomial distribution:
 *
 *   k
 *   --  ( n+j-1 )   n      j
 *   >   (       )  p  (1-p)
 *   --  (   j   )
 *  j=0
 *
 * In a sequence of Bernoulli trials, this is the probability
 * that k or fewer failures precede the nth success.
 *
 * The terms are not computed individually; instead the incomplete
 * beta integral is employed, according to the formula
 *
 * y = nbdtr( k, n, p ) = incbet( n, k+1, p ).
 *
 * The arguments must be positive, with p ranging from 0 to 1.
 *
 */

int qnbdtr( k, n, p, y )
int k, n;
QELT p[], y[];
{
QELT dk[NQ], dn[NQ];
long li;

if( k == 0 )
	{
	qmov( qone, y );
	return 0;
	}

li = k + 1;
ltoq( &li, dk );

li = n;
ltoq( &li, dn );

qincb( dn, dk, p, y );
return 0;
}

/*							qnbdtc
 *
 *	Complemented negative binomial distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qnbdtc( k, n, p, y );
 * int k, n;
 * QELT *p, *y;
 *
 * qnbdtc( k, n, p, y );
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms k+1 to infinity of the negative
 * binomial distribution:
 *
 *   inf
 *   --  ( n+j-1 )   n      j
 *   >   (       )  p  (1-p)
 *   --  (   j   )
 *  j=k+1
 *
 * The terms are not computed individually; instead the incomplete
 * beta integral is employed, according to the formula
 *
 * y = nbdtrc( k, n, p ) = incbet( k+1, n, 1-p ).
 *
 * The arguments must be positive, with p ranging from 0 to 1.
 *
 */

int qnbdtc( k, n, p, y )
int k, n;
QELT p[], y[];
{
QELT dk[NQ], dn[NQ], w[NQ];
long li;

if( k == 0 )
	{
	qmov( qone, y );
	return 0;
	}

li = k + 1;
ltoq( &li, dk );

li = n;
ltoq( &li, dn );

qsub( p, qone, w );
qincb( dk, dn, w, y );
return 0;
}



/*							qpdtr
 *
 *	Poisson distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qpdtr( k, m, y );
 * int k;
 * QELT *m, *y;
 *
 * qpdtr( k, m, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the sum of the first k terms of the Poisson
 * distribution:
 *
 *   k         j
 *   --   -m  m
 *   >   e    --
 *   --       j!
 *  j=0
 *
 * The terms are not summed directly; instead the incomplete
 * gamma integral is employed, according to the relation
 *
 * y = pdtr( k, m ) = igamc( k+1, m ).
 *
 * The arguments must both be positive.
 *
 */

int qpdtr( k, m, y )
int k;
QELT m[], y[];
{
QELT v[NQ];
long li;


li= k+1;
ltoq( &li, v );

qigamc( v, m, y );  /* igamc( k+1, m ) */
return 0;
}

/*							qpdtrc
 *
 *	Complemented poisson distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qpdtrc( k, m, y );
 * int k;
 * QELT *m, *y;
 *
 * qpdtrc( k, m, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the sum of the terms k+1 to infinity of the Poisson
 * distribution:
 *
 *  inf.       j
 *   --   -m  m
 *   >   e    --
 *   --       j!
 *  j=k+1
 *
 * The terms are not summed directly; instead the incomplete
 * gamma integral is employed, according to the formula
 *
 * y = pdtrc( k, m ) = igam( k+1, m ).
 *
 * The arguments must both be positive.
 *
 */

int qpdtrc( k, m, y )
int k;
QELT m[], y[];
{
QELT v[NQ];
long li;


li= k+1;
ltoq( &li, v );

qigam( v, m, y );  /* igam( k+1, m ) */
return 0;
}


/*							qpdtri
 *
 *	Inverse Poisson distribution
 *
 *
 *
 * SYNOPSIS:
 *
 * int qpdtri( k, y, m );
 * int k;
 * QELT *m, *y;
 *
 * qpdtri( k, y, m );
 *
 *
 *
 *
 * DESCRIPTION:
 *
 * Finds the Poisson variable x such that the integral
 * from 0 to x of the Poisson density is equal to the
 * given probability y.
 *
 * This is accomplished using the inverse gamma integral
 * function and the relation
 *
 *    m = igami( k+1, y ).
 *
 */

int qpdtri( k, y, m )
int k;
QELT m[], y[];
{
QELT v[NQ];
long li;


li= k+1;
ltoq( &li, v );

qigami( v, y, m );
return 0;
}

