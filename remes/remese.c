/*		remese.c	*/

/* Solve equations */

#include "remes.h"

void remese()
{
double x, y, z, gx, gxsq, pade;
float sprec;
int i, j, ip;
double gofx(), func(), special();

gxsq = 0.0;
pade = 0.0;

if( (d == 0) && ((config & ZER) == 0) )
	{ /* no denominator: */
/* In this case the deviation is an unknown variable */
/* adjoined to the other unknowns; it has a coefficient */
/* of plus or minus 1. */
	dev = 1.0;
	}
else
	{
/* Otherwise dev = 0, since the solution sought is */
/* for the locations of zero error. */
	dev = 0.0;
	}

/* Set up the equations for solution by simq() */
for( i=0; i<neq; i++ )
	{
/* Offset to 1st element of this row of matrix */
	ip = neq * i;
/* The guess for this row */
	x = xx[i];
/* Right-hand-side vector */
	y = func(x);
	gx = gofx(x);
	if( config & PXCU )
		gxsq = gx * gx * gx;
	if( config & PXSQ )
		gxsq = gx * gx;
	if( config & PADE )
		{
		pade = 2.0 + y;
		}
	if( d > 0 )
		{ /* add the deviation if rational form */
/* Relative error criterion */
		if( relerr )
			y = y * (1.0+dev);
/* Absolute error criterion */
		else
			y = y + dev;
		}
	if( config & CW )
		{ /* y(1+dev) = z + z^2 P/Q */
		y = (y - gx)/(gx*gx);
		}
	if( config & SPECIAL )
		{
		y = special( y, gx );
		}
/* Insert powers of x[i] for numerator coefficients. */
	if( config & XPX )
		z = gx;
	else if( config & X2PX )
		z = gx * gx;
	else
		z = 1.0;
	for( j=0; j<=n; j++ )
		{
		if( config & PADE )
			AA[ip+j] = pade * z;
		else
			AA[ip+j] = z;
		if( config & (PXSQ | PXCU) )
			z = z * gxsq;
		else
			z = z * gx;
		}
/* Insert denominator terms, if any. */
	if( d > 0 )
		{
		z = 1.0;
		for( j=0; j<d; j++ )
			{
			AA[ip+n+1+j] = -y * z;
			if( config & (PXSQ | PXCU) )
				z = z * gxsq;
			else
				z = z * gx;
			}
/* Right hand side vector */
		BB[i] = y * z;
		}
	else
		{ /* No denominator */
/* Right hand side vector */
		BB[i] = y;
		z = dev;
		if( relerr )
			z = z * y;
		AA[ip+n+1] = z;
		}
/* Switch sign of deviation for next row. */
	dev = -1.0 * dev;
	}

/* Solve the simultaneous linear equations. */
simq( &AA[0], &BB[0], &param[0], neq, 0, &IPS[0] );
if( config & TRUNC )
	{
	for( i=0; i<neq; i++ )
		{
		sprec = (float )param[i];
		param[i] = sprec;
		}
	}
}
