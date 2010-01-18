/*							incbil()
 *
 *      Inverse of imcomplete beta integral
 *
 *
 *
 * SYNOPSIS:
 *
 * long double a, b, x, y, incbil();
 *
 * x = incbil( a, b, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Given y, the function finds x such that
 *
 *  incbet( a, b, x ) = y.
 *
 * the routine performs up to 10 Newton iterations to find the
 * root of incbet(a,b,x) - y = 0.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 *                x       a,b
 * arithmetic   domain   domain   # trials    peak       rms
 *    IEEE      0,1    .5,10000    10000    1.1e-14   1.4e-16
 */


/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"

extern long double MACHEPL, MAXNUML, MAXLOGL, MINLOGL;
#ifdef ANSIPROT
extern long double incbetl ( long double, long double, long double );
extern long double expl ( long double );
extern long double fabsl ( long double );
extern long double logl ( long double );
extern long double sqrtl ( long double );
extern long double lgaml ( long double );
extern long double ndtril ( long double );
#else
long double incbetl(), expl(), fabsl(), logl(), sqrtl(), lgaml();
long double ndtril();
#endif

long double incbil( aa, bb, yy0 )
long double aa, bb, yy0;
{
long double a, b, y0, d, y, x, x0, x1, lgm, yp, di, dithresh, yl, yh, xt;
int i, rflg, dir, nflg;


if( yy0 <= 0.0L )
	return(0.0L);
if( yy0 >= 1.0L )
	return(1.0L);
x0 = 0.0L;
yl = 0.0L;
x1 = 1.0L;
yh = 1.0L;
if( aa <= 1.0L || bb <= 1.0L )
	{
	dithresh = 1.0e-7L;
	rflg = 0;
	a = aa;
	b = bb;
	y0 = yy0;
	x = a/(a+b);
	y = incbetl( a, b, x );
	nflg = 0;
	goto ihalve;
	}
else
	{
	nflg = 0;
	dithresh = 1.0e-4L;
	}

/* approximation to inverse function */

yp = -ndtril( yy0 );

if( yy0 > 0.5L )
	{
	rflg = 1;
	a = bb;
	b = aa;
	y0 = 1.0L - yy0;
	yp = -yp;
	}
else
	{
	rflg = 0;
	a = aa;
	b = bb;
	y0 = yy0;
	}

lgm = (yp * yp - 3.0L)/6.0L;
x = 2.0L/( 1.0L/(2.0L * a-1.0L)  +  1.0L/(2.0L * b - 1.0L) );
d = yp * sqrtl( x + lgm ) / x
	- ( 1.0L/(2.0L * b - 1.0L) - 1.0L/(2.0L * a - 1.0L) )
	* (lgm + (5.0L/6.0L) - 2.0L/(3.0L * x));
d = 2.0L * d;
if( d < MINLOGL )
	{
	x = 1.0L;
	goto under;
	}
x = a/( a + b * expl(d) );
y = incbetl( a, b, x );
yp = (y - y0)/y0;
if( fabsl(yp) < 0.2 )
	goto newt;

/* Resort to interval halving if not close enough. */
ihalve:

dir = 0;
di = 0.5L;
for( i=0; i<400; i++ )
	{
	if( i != 0 )
		{
		x = x0  +  di * (x1 - x0);
		if( x == 1.0L )
			x = 1.0L - MACHEPL;
		if( x == 0.0L )
			{
			di = 0.5;
			x = x0  +  di * (x1 - x0);
			if( x == 0.0 )
				goto under;
			}
		y = incbetl( a, b, x );
		yp = (x1 - x0)/(x1 + x0);
		if( fabsl(yp) < dithresh )
			goto newt;
		yp = (y-y0)/y0;
		if( fabsl(yp) < dithresh )
			goto newt;
		}
	if( y < y0 )
		{
		x0 = x;
		yl = y;
		if( dir < 0 )
			{
			dir = 0;
			di = 0.5L;
			}
		else if( dir > 3 )
			di = 1.0L - (1.0L - di) * (1.0L - di);
		else if( dir > 1 )
			di = 0.5L * di + 0.5L; 
		else
			di = (y0 - y)/(yh - yl);
		dir += 1;
		if( x0 > 0.95L )
			{
			if( rflg == 1 )
				{
				rflg = 0;
				a = aa;
				b = bb;
				y0 = yy0;
				}
			else
				{
				rflg = 1;
				a = bb;
				b = aa;
				y0 = 1.0 - yy0;
				}
			x = 1.0L - x;
			y = incbetl( a, b, x );
			x0 = 0.0;
			yl = 0.0;
			x1 = 1.0;
			yh = 1.0;
			goto ihalve;
			}
		}
	else
		{
		x1 = x;
		if( rflg == 1 && x1 < MACHEPL )
			{
			x = 0.0L;
			goto done;
			}
		yh = y;
		if( dir > 0 )
			{
			dir = 0;
			di = 0.5L;
			}
		else if( dir < -3 )
			di = di * di;
		else if( dir < -1 )
			di = 0.5L * di;
		else
			di = (y - y0)/(yh - yl);
		dir -= 1;
		}
	}
mtherr( "incbil", PLOSS );
if( x0 >= 1.0L )
	{
	x = 1.0L - MACHEPL;
	goto done;
	}
if( x <= 0.0L )
	{
under:
	mtherr( "incbil", UNDERFLOW );
	x = 0.0L;
	goto done;
	}

newt:

if( nflg )
	goto done;
nflg = 1;
lgm = lgaml(a+b) - lgaml(a) - lgaml(b);

for( i=0; i<15; i++ )
	{
	/* Compute the function at this point. */
	if( i != 0 )
		y = incbetl(a,b,x);
	if( y < yl )
		{
		x = x0;
		y = yl;
		}
	else if( y > yh )
		{
		x = x1;
		y = yh;
		}
	else if( y < y0 )
		{
		x0 = x;
		yl = y;
		}
	else
		{
		x1 = x;
		yh = y;
		}
	if( x == 1.0L || x == 0.0L )
		break;
	/* Compute the derivative of the function at this point. */
	d = (a - 1.0L) * logl(x) + (b - 1.0L) * logl(1.0L - x) + lgm;
	if( d < MINLOGL )
		goto done;
	if( d > MAXLOGL )
		break;
	d = expl(d);
	/* Compute the step to the next approximation of x. */
	d = (y - y0)/d;
	xt = x - d;
	if( xt <= x0 )
		{
		y = (x - x0) / (x1 - x0);
		xt = x0 + 0.5L * y * (x - x0);
		if( xt <= 0.0L )
			break;
		}
	if( xt >= x1 )
		{
		y = (x1 - x) / (x1 - x0);
		xt = x1 - 0.5L * y * (x1 - x);
		if( xt >= 1.0L )
			break;
		}
	x = xt;
	if( fabsl(d/x) < (128.0L * MACHEPL) )
		goto done;
	}
/* Did not converge.  */
dithresh = 256.0L * MACHEPL;
goto ihalve;

done:
if( rflg )
	{
	if( x <= MACHEPL )
		x = 1.0L - MACHEPL;
	else
		x = 1.0L - x;
	}
return( x );
}
