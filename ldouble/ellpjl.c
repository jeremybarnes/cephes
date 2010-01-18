/*							ellpjl.c
 *
 *	Jacobian Elliptic Functions
 *
 *
 *
 * SYNOPSIS:
 *
 * long double u, m, sn, cn, dn, phi;
 * int ellpjl();
 *
 * ellpjl( u, m, _&sn, _&cn, _&dn, _&phi );
 *
 *
 *
 * DESCRIPTION:
 *
 *
 * Evaluates the Jacobian elliptic functions sn(u|m), cn(u|m),
 * and dn(u|m) of parameter m between 0 and 1, and real
 * argument u.
 *
 * These functions are periodic, with quarter-period on the
 * real axis equal to the complete elliptic integral
 * ellpk(1.0-m).
 *
 * Relation to incomplete elliptic integral:
 * If u = ellik(phi,m), then sn(u|m) = sin(phi),
 * and cn(u|m) = cos(phi).  Phi is called the amplitude of u.
 *
 * Computation is by means of the arithmetic-geometric mean
 * algorithm, except when m is within 1e-12 of 0 or 1.  In the
 * latter case with m close to 1, the approximation applies
 * only for phi < pi/2.
 *
 * ACCURACY:
 *
 * Tested at random points with u between 0 and 10, m between
 * 0 and 1.
 *
 *            Absolute error (* = relative error):
 * arithmetic   function   # trials      peak         rms
 *    IEEE      sn          10000       1.7e-18     2.3e-19
 *    IEEE      cn          20000       1.6e-18     2.2e-19
 *    IEEE      dn          10000       4.7e-15     2.7e-17
 *    IEEE      phi         10000       4.0e-19*    6.6e-20*
 *
 * Accuracy deteriorates when u is large.
 *
 */

/*
Cephes Math Library Release 2.3:  November, 1995
Copyright 1984, 1987, 1995 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double sqrtl ( long double );
extern long double fabsl ( long double );
extern long double sinl ( long double );
extern long double cosl ( long double );
extern long double asinl ( long double );
extern long double tanhl ( long double );
extern long double sinhl ( long double );
extern long double coshl ( long double );
extern long double atanl ( long double );
extern long double expl ( long double );
#else
long double sqrtl(), fabsl(), sinl(), cosl(), asinl(), tanhl();
long double sinhl(), coshl(), atanl(), expl();
#endif
extern long double PIO2L, MACHEPL;

int ellpjl( u, m, sn, cn, dn, ph )
long double u, m;
long double *sn, *cn, *dn, *ph;
{
long double ai, b, phi, t, twon;
long double a[9], c[9];
int i;


/* Check for special cases */

if( m < 0.0L || m > 1.0L )
	{
	mtherr( "ellpjl", DOMAIN );
	*sn = 0.0L;
	*cn = 0.0L;
	*ph = 0.0L;
	*dn = 0.0L;
	return(-1);
	}
if( m < 1.0e-12L )
	{
	t = sinl(u);
	b = cosl(u);
	ai = 0.25L * m * (u - t*b);
	*sn = t - ai*b;
	*cn = b + ai*t;
	*ph = u - ai;
	*dn = 1.0L - 0.5L*m*t*t;
	return(0);
	}

if( m >= 0.999999999999L )
	{
	ai = 0.25L * (1.0L-m);
	b = coshl(u);
	t = tanhl(u);
	phi = 1.0L/b;
	twon = b * sinhl(u);
	*sn = t + ai * (twon - u)/(b*b);
	*ph = 2.0L*atanl(expl(u)) - PIO2L + ai*(twon - u)/b;
	ai *= t * phi;
	*cn = phi - ai * (twon - u);
	*dn = phi + ai * (twon + u);
	return(0);
	}


/*	A. G. M. scale		*/
a[0] = 1.0L;
b = sqrtl(1.0L - m);
c[0] = sqrtl(m);
twon = 1.0L;
i = 0;

while( fabsl(c[i]/a[i]) > MACHEPL )
	{
	if( i > 7 )
		{
		mtherr( "ellpjl", OVERFLOW );
		goto done;
		}
	ai = a[i];
	++i;
	c[i] = 0.5L * ( ai - b );
	t = sqrtl( ai * b );
	a[i] = 0.5L * ( ai + b );
	b = t;
	twon *= 2.0L;
	}

done:

/* backward recurrence */
phi = twon * a[i] * u;
do
	{
	t = c[i] * sinl(phi) / a[i];
	b = phi;
	phi = 0.5L * (asinl(t) + phi);
	}
while( --i );

*sn = sinl(phi);
t = cosl(phi);
*cn = t;
*dn = t/cosl(phi-b);
*ph = phi;
return(0);
}
