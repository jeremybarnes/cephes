/*							qsici.c
 *	Sine and cosine integrals
 *
 *
 *
 * SYNOPSIS:
 *
 * int qsici( x, si, ci );
 * QELT *x, *si, *ci;
 *
 * qsici( x, si, ci );
 *
 *
 * DESCRIPTION:
 *
 * Evaluates the integrals
 *
 *                          x
 *                          -
 *                         |  cos t - 1
 *   Ci(x) = eul + ln x +  |  --------- dt,
 *                         |      t
 *                        -
 *                         0
 *             x
 *             -
 *            |  sin t
 *   Si(x) =  |  ----- dt
 *            |    t
 *           -
 *            0
 *
 * where eul = 0.57721566490153286061 is Euler's constant.
 *
 * The power series are
 *
 *          inf      n  2n+1
 *           -   (-1)  z
 * Si(z)  =  >  --------------
 *           -  (2n+1) (2n+1)!
 *          n=0
 *
 *                            inf      n  2n
 *                             -   (-1)  z
 * Ci(z)  =  eul +  ln(z)  +   >  -----------
 *                             -    2n (2n)!
 *                            n=1
 *
 * ACCURACY:
 *
 * Series expansions are set to terminate at less than full
 * working precision.
 *
 */

/*
Cephes Math Library Release 2.1:  January, 1989
Copyright 1984, 1987, 1989, 1996 by Stephen L. Moshier
*/
#include "qhead.h"
extern QELT qeul[];
extern QELT qone[], qtwo[], qpi[];

int qsici( x, si, ci )
QELT x[], si[], ci[];
{
QELT k[NQ], z[NQ], t[NQ], a[NQ];
int sign;
int jmax;

if( x[0] != 0 )
	{
	sign = -1;
	x[0] = 0;
	}
else
	sign = 0;

if( x[1] < 3 )
	{
	qclear( si );
	qinfin(ci);
	ci[0] = -1;
	return( 0 );
	}

qmul( x, x, z );	/* z = -x * x		*/
z[0] = -1;
qmov( qone, a );	/* a = 1.0		*/
qmov( qone, t );	/* t = 1.0		*/
qmov( qone, si );	/* si = 1.0		*/
qclear( ci );		/* ci = 0.0		*/
qmov( qtwo, k );	/* k = 2.0		*/
jmax = 0;

/* 	x	Bits of precision lost
 *
 *	10	11
 *	20	25
 *	30	39
 *	40	53
 *	50	68
 *	55	75
 *	60	82
 */


do
	{
	qdiv( k, z, t );	/* a *= z/k	*/
	qmul( t, a, a );
	qdiv( k, a, t );	/* ci += a/k	*/
	qadd( ci, t, ci );
	qadd( k, qone, k );	/* k += 1.0	*/
/*
	if( t[1] > jmax )
		jmax = t[1];
*/
	qdiv( k, a, a );	/* a /= k	*/
	qdiv( k, a, t );	/* si += a/k	*/
	qadd( si, t, si );
	qadd( k, qone, k );	/* k += 1.0	*/
/*
	if( t[1] > jmax )
		jmax = t[1];
*/
	}
while( ((int) si[1] - (int) a[1]) < NBITS/2 );

/*
jmax = jmax - si[1];
if( jmax > 0 )
	printf( "%d bits lost\n", jmax );
*/

qmul( si, x, si );	/* si *= x		*/

if( sign )
	si[0] = -1;

/* ci = EUL + log(x) + ci */
qlog( x, t );
qadd( ci, t, ci );
qadd( qeul, ci, ci );
return 0;
}

/*							qsicix
 *
 *	Auxiliary functions for sine and cosine integrals
 *
 *
 *
 * SYNOPSIS:
 *
 * int qsicix( x, f, g );
 * QELT *x, *f, *g;
 *
 * qsicix( x, f, g );
 *
 *
 * DESCRIPTION:
 *
 * Auxiliary functions f(x) and g(x) such that
 *
 * Ci(x) = f(x) sin(x) - g(x) cos(x)
 * Si(x) = pi/2 - f(x) cos(x) - g(x) sin(x)
 *
 * f(x) = Ci * sin  -  (si - pi/2) * cos
 * g(x) = -Ci * cos -  (si - pi/2) * sin
 *
 * The asymptotic expansions are
 *
 *
 *                2!   4!   6!
 *  z f(z) =  1 - -- + -- - -- + ...
 *                 2    4    6
 *                z    z    z
 *
 *
 *   2             3!   5!   7!
 *  z  g(z) =  1 - -- + -- - -- + ...
 *                  2    4    6
 *                 z    z    z
 *
 * (AMS55 #5.2.34)
 */

int qsicix( x, f, g )
QELT x[], f[], g[];
{
QELT sit[NQ], cit[NQ], t[NQ], c[NQ], s[NQ];
int jmin, jt;
union
  {
    unsigned short s[4];
    double d;
  } dx;

qtoe( x, dx.s );

/* error at crossover is 10**-21 */
if( dx.d > 55 )
	goto asymp;

qsici( x, sit, cit );
qmov( qpi, t );
t[1] -= 1;
qsub( t, sit, sit );

qsin( x, s );
qcos( x, c );

qmul( cit, s, t );	/* ci * sin */
qmul( sit, c, f );	/* si * cos */
qsub( f, t, f );	/*  f */

qmul( cit, c, c );	/* ci * cos */
qmul( sit, s, s );	/* si * sin */
qadd( c, s, g );	/* -g */
qneg( g );
return 0;

asymp:
/* Asymptotic expansion of auxiliary functions */

qmul( x, x, sit );	/* z = -x*x */
qneg( sit );
qmov( qone, cit );	/* a = 1.0;*/
qmov( qone, s );	/* s = 1.0;*/
qmov( qone, c );	/* c = 1.0;*/
qmov( qone, t );	/* k = 2.0;*/
t[1] += 1;
jmin = MAXEXP;

do
	{
	qdiv( sit, cit, cit );	/* a *= k/z;*/
	qmul( t, cit, cit );
	qadd( qone, t, t );	/* k += 1.0;*/
	qadd( cit, s, s );	/* s += a;*/
	qmul( cit, t, cit );	/* a *= k;*/
	qadd( t, qone, t );	/* k += 1.0;*/
	qadd( c, cit, c );	/* c += a;*/
	jt = cit[1];
	if( jt > jmin )
		goto asydon;
	jmin = jt;
	}
while( ((int) qone[1] - (int) cit[1]) < NBITS/2 );
/* converges to 43 decimals for x > 108 */

asydon:
/*
qtoe( cit, dx.s );
printf( " %d %.5e\n", qone[1] - jmin, dx.d );
*/
qdiv( x, s, f );	/* f = s / x;*/
sit[0] = 0;
qdiv( sit, c, g );	/* ci = c / fabs(z);*/
return 0;
}
