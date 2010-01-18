/*							incbetl.c
 *
 *	Incomplete beta integral
 *
 *
 * SYNOPSIS:
 *
 * long double a, b, x, y, incbetl();
 *
 * y = incbetl( a, b, x );
 *
 *
 * DESCRIPTION:
 *
 * Returns incomplete beta integral of the arguments, evaluated
 * from zero to x.  The function is defined as
 *
 *                  x
 *     -            -
 *    | (a+b)      | |  a-1     b-1
 *  -----------    |   t   (1-t)   dt.
 *   -     -     | |
 *  | (a) | (b)   -
 *                 0
 *
 * The domain of definition is 0 <= x <= 1.  In this
 * implementation a and b are restricted to positive values.
 * The integral from x to 1 may be obtained by the symmetry
 * relation
 *
 *    1 - incbet( a, b, x )  =  incbet( b, a, 1-x ).
 *
 * The integral is evaluated by a continued fraction expansion
 * or, when b*x is small, by a power series.
 *
 * ACCURACY:
 *
 * Tested at random points (a,b,x) with x between 0 and 1.
 * arithmetic   domain     # trials      peak         rms
 *    IEEE       0,5       20000        4.5e-18     2.4e-19
 *    IEEE       0,100    100000        3.9e-17     1.0e-17
 * Half-integer a, b:
 *    IEEE      .5,10000  100000        3.9e-14     4.4e-15
 * Outputs smaller than the IEEE gradual underflow threshold
 * were excluded from these statistics.
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * incbetl domain     x<0, x>1          0.0
 */


/*
Cephes Math Library, Release 2.3:  January, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"

#define MAXGAML 1755.455L
static long double big = 9.223372036854775808e18L;
static long double biginv = 1.084202172485504434007e-19L;
extern long double MACHEPL, MINLOGL, MAXLOGL;

#ifdef ANSIPROT
extern long double gammal ( long double );
extern long double lgaml ( long double );
extern long double expl ( long double );
extern long double logl ( long double );
extern long double fabsl ( long double );
extern long double powl ( long double, long double );
static long double incbcfl( long double, long double, long double );
static long double incbdl( long double, long double, long double );
static long double pseriesl( long double, long double, long double );
#else
long double gammal(), lgaml(), expl(), logl(), fabsl(), powl();
static long double incbcfl(), incbdl(), pseriesl();
#endif

long double incbetl( aa, bb, xx )
long double aa, bb, xx;
{
long double a, b, t, x, w, xc, y;
int flag;

if( aa <= 0.0L || bb <= 0.0L )
	goto domerr;

if( (xx <= 0.0L) || ( xx >= 1.0L) )
	{
	if( xx == 0.0L )
		return( 0.0L );
	if( xx == 1.0L )
		return( 1.0L );
domerr:
	mtherr( "incbetl", DOMAIN );
	return( 0.0L );
	}

flag = 0;
if( (bb * xx) <= 1.0L && xx <= 0.95L)
	{
	t = pseriesl(aa, bb, xx);
	goto done;
	}

w = 1.0L - xx;

/* Reverse a and b if x is greater than the mean. */
if( xx > (aa/(aa+bb)) )
	{
	flag = 1;
	a = bb;
	b = aa;
	xc = xx;
	x = w;
	}
else
	{
	a = aa;
	b = bb;
	xc = w;
	x = xx;
	}

if( flag == 1 && (b * x) <= 1.0L && x <= 0.95L)
	{
	t = pseriesl(a, b, x);
	goto done;
	}

/* Choose expansion for optimal convergence */
y = x * (a+b-2.0L) - (a-1.0L);
if( y < 0.0L )
	w = incbcfl( a, b, x );
else
	w = incbdl( a, b, x ) / xc;

/* Multiply w by the factor
     a      b   _             _     _
    x  (1-x)   | (a+b) / ( a | (a) | (b) ) .   */

y = a * logl(x);
t = b * logl(xc);
if( (a+b) < MAXGAML && fabsl(y) < MAXLOGL && fabsl(t) < MAXLOGL )
	{
	t = powl(xc,b);
	t *= powl(x,a);
	t /= a;
	t *= w;
	t *= gammal(a+b) / (gammal(a) * gammal(b));
	goto done;
	}
else
	{
	/* Resort to logarithms.  */
	y += t + lgaml(a+b) - lgaml(a) - lgaml(b);
	y += logl(w/a);
	if( y < MINLOGL )
		t = 0.0L;
	else
		t = expl(y);
	}

done:

if( flag == 1 )
	{
	if( t <= MACHEPL )
		t = 1.0L - MACHEPL;
	else
	t = 1.0L - t;
	}
return( t );
}

/* Continued fraction expansion #1
 * for incomplete beta integral
 */

static long double incbcfl( a, b, x )
long double a, b, x;
{
long double xk, pk, pkm1, pkm2, qk, qkm1, qkm2;
long double k1, k2, k3, k4, k5, k6, k7, k8;
long double r, t, ans, thresh;
int n;

k1 = a;
k2 = a + b;
k3 = a;
k4 = a + 1.0L;
k5 = 1.0L;
k6 = b - 1.0L;
k7 = k4;
k8 = a + 2.0L;

pkm2 = 0.0L;
qkm2 = 1.0L;
pkm1 = 1.0L;
qkm1 = 1.0L;
ans = 1.0L;
r = 1.0L;
n = 0;
thresh = 3.0L * MACHEPL;
do
	{
	
	xk = -( x * k1 * k2 )/( k3 * k4 );
	pk = pkm1 +  pkm2 * xk;
	qk = qkm1 +  qkm2 * xk;
	pkm2 = pkm1;
	pkm1 = pk;
	qkm2 = qkm1;
	qkm1 = qk;

	xk = ( x * k5 * k6 )/( k7 * k8 );
	pk = pkm1 +  pkm2 * xk;
	qk = qkm1 +  qkm2 * xk;
	pkm2 = pkm1;
	pkm1 = pk;
	qkm2 = qkm1;
	qkm1 = qk;

	if( qk != 0.0L )
		r = pk/qk;
	if( r != 0.0L )
		{
		t = fabsl( (ans - r)/r );
		ans = r;
		}
	else
		t = 1.0L;

	if( t < thresh )
		goto cdone;

	k1 += 1.0L;
	k2 += 1.0L;
	k3 += 2.0L;
	k4 += 2.0L;
	k5 += 1.0L;
	k6 -= 1.0L;
	k7 += 2.0L;
	k8 += 2.0L;

	if( (fabsl(qk) + fabsl(pk)) > big )
		{
		pkm2 *= biginv;
		pkm1 *= biginv;
		qkm2 *= biginv;
		qkm1 *= biginv;
		}
	if( (fabsl(qk) < biginv) || (fabsl(pk) < biginv) )
		{
		pkm2 *= big;
		pkm1 *= big;
		qkm2 *= big;
		qkm1 *= big;
		}
	}
while( ++n < 400 );
mtherr( "incbetl", PLOSS );

cdone:
return(ans);
}


/* Continued fraction expansion #2
 * for incomplete beta integral
 */

static long double incbdl( a, b, x )
long double a, b, x;
{
long double xk, pk, pkm1, pkm2, qk, qkm1, qkm2;
long double k1, k2, k3, k4, k5, k6, k7, k8;
long double r, t, ans, z, thresh;
int n;

k1 = a;
k2 = b - 1.0L;
k3 = a;
k4 = a + 1.0L;
k5 = 1.0L;
k6 = a + b;
k7 = a + 1.0L;
k8 = a + 2.0L;

pkm2 = 0.0L;
qkm2 = 1.0L;
pkm1 = 1.0L;
qkm1 = 1.0L;
z = x / (1.0L-x);
ans = 1.0L;
r = 1.0L;
n = 0;
thresh = 3.0L * MACHEPL;
do
	{
	
	xk = -( z * k1 * k2 )/( k3 * k4 );
	pk = pkm1 +  pkm2 * xk;
	qk = qkm1 +  qkm2 * xk;
	pkm2 = pkm1;
	pkm1 = pk;
	qkm2 = qkm1;
	qkm1 = qk;

	xk = ( z * k5 * k6 )/( k7 * k8 );
	pk = pkm1 +  pkm2 * xk;
	qk = qkm1 +  qkm2 * xk;
	pkm2 = pkm1;
	pkm1 = pk;
	qkm2 = qkm1;
	qkm1 = qk;

	if( qk != 0.0L )
		r = pk/qk;
	if( r != 0.0L )
		{
		t = fabsl( (ans - r)/r );
		ans = r;
		}
	else
		t = 1.0L;

	if( t < thresh )
		goto cdone;

	k1 += 1.0L;
	k2 -= 1.0L;
	k3 += 2.0L;
	k4 += 2.0L;
	k5 += 1.0L;
	k6 += 1.0L;
	k7 += 2.0L;
	k8 += 2.0L;

	if( (fabsl(qk) + fabsl(pk)) > big )
		{
		pkm2 *= biginv;
		pkm1 *= biginv;
		qkm2 *= biginv;
		qkm1 *= biginv;
		}
	if( (fabsl(qk) < biginv) || (fabsl(pk) < biginv) )
		{
		pkm2 *= big;
		pkm1 *= big;
		qkm2 *= big;
		qkm1 *= big;
		}
	}
while( ++n < 400 );
mtherr( "incbetl", PLOSS );

cdone:
return(ans);
}

/* Power series for incomplete gamma integral.
   Use when b*x is small.  */

static long double pseriesl( a, b, x )
long double a, b, x;
{
long double s, t, u, v, n, t1, z, ai;

ai = 1.0L / a;
u = (1.0L - b) * x;
v = u / (a + 1.0L);
t1 = v;
t = u;
n = 2.0L;
s = 0.0L;
z = MACHEPL * ai;
while( fabsl(v) > z )
	{
	u = (n - b) * x / n;
	t *= u;
	v = t / (a + n);
	s += v; 
	n += 1.0L;
	}
s += t1;
s += ai;

u = a * logl(x);
if( (a+b) < MAXGAML && fabsl(u) < MAXLOGL )
	{
	t = gammal(a+b)/(gammal(a)*gammal(b));
	s = s * t * powl(x,a);
	}
else
	{
	t = lgaml(a+b) - lgaml(a) - lgaml(b) + u + logl(s);
	if( t < MINLOGL )
		s = 0.0L;
	else
	s = expl(t);
	}
return(s);
}
