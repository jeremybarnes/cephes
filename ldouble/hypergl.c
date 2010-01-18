/*							hypergl.c
 *
 *	Confluent hypergeometric function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double a, b, x, y, hypergl();
 *
 * y = hypergl( a, b, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Computes the confluent hypergeometric function
 *
 *                          1           2
 *                       a x    a(a+1) x
 *   F ( a,b;x )  =  1 + ---- + --------- + ...
 *  1 1                  b 1!   b(b+1) 2!
 *
 * Many higher transcendental functions are special cases of
 * this power series.
 *
 * As is evident from the formula, b must not be a negative
 * integer or zero unless a is an integer with 0 >= a > b.
 *
 * The routine attempts both a direct summation of the series
 * and an asymptotic expansion.  In each case error due to
 * roundoff, cancellation, and nonconvergence is estimated.
 * The result with smaller estimated error is returned.
 *
 *
 *
 * ACCURACY:
 *
 * Tested at random points (a, b, x), all three variables
 * ranging from 0 to 30.
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,30        100000      3.3e-18     5.0e-19
 *
 * Larger errors can be observed when b is near a negative
 * integer or zero.  Certain combinations of arguments yield
 * serious cancellation error in the power series summation
 * and also are not in the region of near convergence of the
 * asymptotic series.  An error message is printed if the
 * self-estimated relative error is greater than 1.0e-12.
 *
 */

/*							hyperg.c */


/*
Cephes Math Library Release 2.1:  November, 1988
Copyright 1984, 1987, 1988 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include "mconf.h"

#ifdef ANSIPROT
extern long double expl ( long double );
extern long double fabsl ( long double );
extern long double logl ( long double );
extern long double gammal ( long double );
extern long double lgaml ( long double );
static long double hy1f1pl(long double, long double, long double, long double *);
static long double hy1f1al(long double, long double, long double, long double *);
long double hyp2f0l( long double, long double, long double, int, long double *);
#else
long double expl(), fabsl();
static long double hy1f1pl();
static long double hy1f1al();
long double logl(), gammal(), lgaml(), hyp2f0l();
#endif
extern double MAXNUML, MACHEPL;

long double hypergl( a, b, x)
long double a, b, x;
{
long double asum, psum, acanc, pcanc, temp;


/* See if a Kummer transformation will help */
temp = b - a;
if( fabsl(temp) < 0.001L * fabsl(a) )
	return( expl(x) * hypergl( temp, b, -x )  );

psum = hy1f1pl( a, b, x, &pcanc );
if( pcanc < 1.0e-15L )
	goto done;


/* try asymptotic series */

asum = hy1f1al( a, b, x, &acanc );


/* Pick the result with less estimated error */

if( acanc < pcanc )
	{
	pcanc = acanc;
	psum = asum;
	}

done:
if( pcanc > 1.0e-12L )
	mtherr( "hypergl", PLOSS );

return( psum );
}




/* Power series summation for confluent hypergeometric function		*/


static long double hy1f1pl( a, b, x, err )
long double a, b, x;
long double *err;
{
long double n, a0, sum, t, u, temp;
long double an, bn, maxt, pcanc;


/* set up for power series summation */
an = a;
bn = b;
a0 = 1.0L;
sum = 1.0L;
n = 1.0L;
t = 1.0L;
maxt = 0.0L;


while( t > MACHEPL )
	{
	if( bn == 0 )			/* check bn first since if both	*/
		{
		mtherr( "hypergl", SING );
		return( MAXNUML );	/* an and bn are zero it is	*/
		}
	if( an == 0 )			/* a singularity		*/
		return( sum );
	if( n > 200 )
		goto pdone;
	u = x * ( an / (bn * n) );

	/* check for blowup */
	temp = fabsl(u);
	if( (temp > 1.0L ) && (maxt > (MAXNUML/temp)) )
		{
		pcanc = 1.0L;	/* estimate 100% error */
		goto blowup;
		}

	a0 *= u;
	sum += a0;
	t = fabsl(a0);
	if( t > maxt )
		maxt = t;
/*
	if( (maxt/fabsl(sum)) > 1.0e17L )
		{
		pcanc = 1.0;
		goto blowup;
		}
*/
	an += 1.0L;
	bn += 1.0L;
	n += 1.0L;
	}

pdone:

/* estimate error due to roundoff and cancellation */
if( sum != 0.0L )
	maxt /= fabsl(sum);
maxt *= MACHEPL; 	/* this way avoids multiply overflow */
pcanc = fabsl( MACHEPL * n  +  maxt );

blowup:

*err = pcanc;

return( sum );
}


/*							hy1f1a()	*/
/* asymptotic formula for hypergeometric function:
 *
 *        (    -a                         
 *  --    ( |z|                           
 * |  (b) ( -------- 2f0( a, 1+a-b, -1/x )
 *        (  --                           
 *        ( |  (b-a)                      
 *
 *
 *                                x    a-b                     )
 *                               e  |x|                        )
 *                             + -------- 2f0( b-a, 1-a, 1/x ) )
 *                                --                           )
 *                               |  (a)                        )
 */

static long double hy1f1al( a, b, x, err )
long double a, b, x;
long double *err;
{
long double h1, h2, t, u, temp, acanc, asum, err1, err2;

if( x == 0.0L )
	{
	acanc = 1.0L;
	asum = MAXNUML;
	goto adone;
	}
temp = logl( fabsl(x) );
t = x + temp * (a-b);
u = -temp * a;

if( b > 0.0L )
	{
	temp = lgaml(b);
	t += temp;
	u += temp;
	}

h1 = hyp2f0l( a, a-b+1, -1.0L/x, 1, &err1 );

temp = expl(u) / gammal(b-a);
h1 *= temp;
err1 *= temp;

h2 = hyp2f0l( b-a, 1.0L-a, 1.0L/x, 2, &err2 );

if( a < 0 )
	temp = expl(t) / gammal(a);
else
	temp = expl( t - lgaml(a) );

h2 *= temp;
err2 *= temp;

if( x < 0.0L )
	asum = h1;
else
	asum = h2;

acanc = fabsl(err1) + fabsl(err2);


if( b < 0.0L )
	{
	temp = gammal(b);
	asum *= temp;
	acanc *= fabsl(temp);
	}


if( asum != 0.0L )
	acanc /= fabsl(asum);

acanc *= 30.0L;	/* fudge factor, since error of asymptotic formula
		 * often seems this much larger than advertised */

adone:


*err = acanc;
return( asum );
}

/*							hyp2f0()	*/

long double hyp2f0l( a, b, x, type, err )
long double a, b, x;
int type;	/* determines what converging factor to use */
long double *err;
{
long double a0, alast, t, tlast, maxt;
long double n, an, bn, u, sum, temp;

an = a;
bn = b;
a0 = 1.0e0L;
alast = 1.0e0L;
sum = 0.0L;
n = 1.0e0L;
t = 1.0e0L;
tlast = 1.0e9L;
maxt = 0.0L;

do
	{
	if( an == 0.0L )
		goto pdone;
	if( bn == 0.0L )
		goto pdone;

	u = an * (bn * x / n);

	/* check for blowup */
	temp = fabsl(u);
	if( (temp > 1.0L ) && (maxt > (MAXNUML/temp)) )
		goto error;

	a0 *= u;
	t = fabsl(a0);

	/* terminating condition for asymptotic series */
	if( t > tlast )
		goto ndone;

	tlast = t;
	sum += alast;	/* the sum is one term behind */
	alast = a0;

	if( n > 200 )
		goto ndone;

	an += 1.0e0L;
	bn += 1.0e0L;
	n += 1.0e0L;
	if( t > maxt )
		maxt = t;
	}
while( t > MACHEPL );


pdone:	/* series converged! */

/* estimate error due to roundoff and cancellation */
*err = fabsl(  MACHEPL * (n + maxt)  );

alast = a0;
goto done;

ndone:	/* series did not converge */

/* The following "Converging factors" are supposed to improve accuracy,
 * but do not actually seem to accomplish very much. */

n -= 1.0L;
x = 1.0L/x;

switch( type )	/* "type" given as subroutine argument */
{
case 1:
	alast *= ( 0.5L + (0.125L + 0.25L*b - 0.5L*a + 0.25L*x - 0.25L*n)/x );
	break;

case 2:
	alast *= 2.0L/3.0L - b + 2.0L*a + x - n;
	break;

default:
	;
}

/* estimate error due to roundoff, cancellation, and nonconvergence */
*err = MACHEPL * (n + maxt)  +  fabsl ( a0 );


done:
sum += alast;
return( sum );

/* series blew up: */
error:
*err = MAXNUML;
mtherr( "hypergl", TLOSS );
return( sum );
}
