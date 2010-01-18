/*							incbi()
 *
 *      Inverse of imcomplete beta integral
 *
 *
 *
 * SYNOPSIS:
 *
 * double a, b, x, y, incbi();
 *
 * x = incbi( a, b, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Given y, the function finds x such that
 *
 *  incbet( a, b, x ) = y .
 *
 * The routine performs interval halving or Newton iterations to find the
 * root of incbet(a,b,x) - y = 0.
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 *                x     a,b
 * arithmetic   domain  domain  # trials    peak       rms
 *    IEEE      0,1   .25,100     50000    1.4e-13   4.0e-15
 *    IEEE      0,1    .5,10000    5000    4.0e-12   2.0e-13
 *    IEEE      0,1     0,5       45000    7.0e-13   5.4e-15
 * With a and b constrained to half-integer or integer values:
 *    IEEE      0,1    .5,100     10000    2.8e-14   1.4e-15
 * With a = .5, b constrained to half-integer or integer values:
 *    IEEE      0,1    .5,10000    2000    1.1e-10   1.6e-11
 */


/*
Cephes Math Library Release 2.3:  March,1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"

extern double MACHEP, MAXNUM, MAXLOG, MINLOG;
#ifndef ANSIPROT
double ndtri(), exp(), fabs(), log(), sqrt(), lgam(), incbet();
#endif

double incbi( aa, bb, yy0 )
double aa, bb, yy0;
{
double a, b, y0, d, y, x, x0, x1, lgm, yp, di, dithresh, yl, yh;
int i, rflg, dir, nflg;


if( yy0 <= 0 )
	return(0.0);
if( yy0 >= 1.0 )
	return(1.0);

if( aa <= 1.0 || bb <= 1.0 )
	{
	nflg = 1;
	dithresh = 4.0 * MACHEP;
	rflg = 0;
	a = aa;
	b = bb;
	y0 = yy0;
	x = a/(a+b);
	y = incbet( a, b, x );
	goto ihalve;
	}
else
	{
	nflg = 0;
	dithresh = 1.0e-4;
	}
/* approximation to inverse function */

yp = -ndtri(yy0);

if( yy0 > 0.5 )
	{
	rflg = 1;
	a = bb;
	b = aa;
	y0 = 1.0 - yy0;
	yp = -yp;
	}
else
	{
	rflg = 0;
	a = aa;
	b = bb;
	y0 = yy0;
	}

lgm = (yp * yp - 3.0)/6.0;
x0 = 2.0/( 1.0/(2.0*a-1.0)  +  1.0/(2.0*b-1.0) );
y = yp * sqrt( x0 + lgm ) / x0
	- ( 1.0/(2.0*b-1.0) - 1.0/(2.0*a-1.0) )
	* (lgm + 5.0/6.0 - 2.0/(3.0*x0));
y = 2.0 * y;
if( y < MINLOG )
	{
	x0 = 1.0;
	goto under;
	}
x = a/( a + b * exp(y) );
y = incbet( a, b, x );
yp = (y - y0)/y0;
if( fabs(yp) < 1.0e-2 )
	goto newt;

ihalve:

/* Resort to interval halving if not close enough */
x0 = 0.0;
yl = 0.0;
x1 = 1.0;
yh = 1.0;
di = 0.5;
dir = 0;

for( i=0; i<400; i++ )
	{
	if( i != 0 )
		{
		x = x0  +  di * (x1 - x0);
		if( x == 1.0 )
			x = 1.0 - MACHEP;
		y = incbet( a, b, x );
		yp = (x1 - x0)/(x1 + x0);
		if( fabs(yp) < dithresh )
			{
			x0 = x;
			goto newt;
			}
		}

	if( y < y0 )
		{
		x0 = x;
		yl = y;
		if( dir < 0 )
			{
			dir = 0;
			di = 0.5;
			}
		else if( dir > 1 )
			di = 0.5 * di + 0.5; 
		else
			di = (y0 - y)/(yh - yl);
		dir += 1;
		if( x0 > 0.75 )
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
			x = 1.0 - x;
			y = incbet( a, b, x );
			goto ihalve;
			}
		}
	else
		{
		x1 = x;
		if( rflg == 1 && x1 < MACHEP )
			{
			x0 = 0.0;
			goto done;
			}
		yh = y;
		if( dir > 0 )
			{
			dir = 0;
			di = 0.5;
			}
		else if( dir < -1 )
			di = 0.5 * di;
		else
			di = (y - y0)/(yh - yl);
		dir -= 1;
		}
	}
mtherr( "incbi", PLOSS );
if( x0 >= 1.0 )
	{
	x0 = 1.0 - MACHEP;
	goto done;
	}
if( x == 0.0 )
	{
under:
	mtherr( "incbi", UNDERFLOW );
	x0 = 0.0;
	goto done;
	}

newt:

if( nflg )
	goto done;

x0 = x;
lgm = lgam(a+b) - lgam(a) - lgam(b);

for( i=0; i<10; i++ )
	{
/* Compute the function at this point. */
	if( i != 0 )
		y = incbet(a,b,x0);
/* Compute the derivative of the function at this point. */
	d = (a - 1.0) * log(x0) + (b - 1.0) * log(1.0-x0) + lgm;
	if( d < MINLOG )
		{
		x0 = 0.0;
		goto under;
		}
	d = exp(d);
/* compute the step to the next approximation of x */
	d = (y - y0)/d;
	x = x0;
	x0 = x0 - d;
	if( x0 <= 0.0 )
		{
		x0 = 0.0;
		goto under;
		}
	if( x0 >= 1.0 )
		{
		x0 = 1.0 - MACHEP;
		goto done;
		}
	if( fabs(d/x0) < 64.0 * MACHEP )
		goto done;
	}

done:
if( rflg )
	{
	if( x0 <= MACHEP )
		x0 = 1.0 - MACHEP;
	else
		x0 = 1.0 - x0;
	}
return( x0 );
}
