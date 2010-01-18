/* remess.c */
#include <stdio.h>
#include "remes.h"

/* Search subroutine */

void remess()
{
double a, q, xm, ym, xn, yn, xx0, xx1;
int i, j, meq, emsign, ensign, steps;
double approx(), func(), geterr();

/* Search for maxima of error */
eclose = 1e30;
farther = 0.0;
j = 0;
meq = neq;

if( (d > 0) || ((config & ZER) != 0) )
	{
	j = 1;
	meq += 1;
	}
xx0 = apstrt;

for( i=0; i<meq; i++ )
	{
	steps = 0;
	if( (d > 0) || ((config & ZER) != 0) )
		xx1 = xx[i]; /* Next zero */
	else
		xx1 = mm[i+1]; /* Next maximum */
	if( i == meq-1 )
		xx1 = apend;
	xm = mm[i];
	ym = geterr(xm);
	emsign = esign; /* Sign of error */
	q = step[i];
	xn = xm + q;
/* Cannot skip over adjacent boundaries */
	if( xn < xx0 )
		goto revers;
	if( xn >= xx1 )
		goto revers;
	yn = geterr(xn);
	ensign = esign;
	if( yn < ym )
		{
revers:
		q = -q;
		xn = xm;
		yn = ym;
		ensign = emsign;
		}

/* March until error becomes smaller. */

	while( yn >= ym )
		{
		if( ++steps > 10 )
			goto tsear;
		ym = yn;
		xm = xn;
		emsign = ensign;
		a = xm + q;
		if( a == xm )
			goto tsear;
/* Must not skip over the zeros either side. */
		if( a <= xx0 )
			goto tsear;
		if( a >= xx1 )
			goto tsear;
		xn = a;
		yn = geterr(xn);
		ensign = esign;
		}

tsear:
	mm[i] = xm; /* Position of maximum */
	yy[i] = ym; /* Value of maximum */

	if( ym == 0.0 )
		printf( "yy[%d] = 0\n", i );
	if( eclose > ym )
		eclose = ym;

	if( farther < ym )
		farther = ym;

 /* No denominator */
	if( (d == 0) && ((config & ZER) == 0) )
		xx[i] = xm;
/* Walk to next zero. */
	if( (d > 0) || ((config & ZER) != 0) )
		xx0 = xx1;
	else
		xx0 = 0.5*(xm + xx1);
	} /* end of search loop */


/* Decrease step size if error spread increased. */
q = (farther - eclose);
/* Relative error spread */
if( eclose != 0.0 )
	q /= eclose;
if( q >= spread )
	{ /* Spread is increasing; decrease step size. */
	if( config & TRUNC )
		delta *= 0.875;
	else
		delta *= 0.5;
	printf( "delta = %.4E\n", delta );
	}
 spread = q;

printf( "peak error = %.4E, relative error spread = %.4E\n",
		farther, spread );

/* Calculate new step sizes */

if( (d == 0) && ((config & ZER) == 0) )
	{
	if( spread < 0.25 )
		q = 0.0625;
	else
		q = 0.5;

	for( i=0; i<neq; i++ )
		step[i] *= q;
	}
else
	{

	for( i=0; i<neq; i++ )
		{
		q = yy[i+1];
		if( q != 0.0 )
			q = yy[i]/q  - 1.0;
		else
			q = 0.0625;
		if( q > 0.25 )
			q = 0.25;
		q *= mm[i+1] - mm[i];
		step[i] = q * delta;
		}
	step[neq] = step[neq-1];


/* Insert new locations for the zeros. */
	for( i=0; i<neq; i++ )
		{
		xm = xx[i] - step[i];
		if( xm <= apstrt )
			continue;
		if( xm >= apend )
			continue;
		if( xm <= mm[i] )
			{
			printf( "xx[%d] < mm\n", i );
			xm = 0.5 * (mm[i] + xx[i]);
			}
		if( xm >= mm[i+1] )
			{
			printf( "xx[%d] > mm\n", i );
			xm = 0.5 * (mm[i+1] + xx[i]);
			}
		xx[i] = xm;
		}
	}
}
