/*							qshici.c
 *
 *	Hyperbolic sine and cosine integrals
 *
 *
 *
 * SYNOPSIS:
 *
 * int qshici( x, si, ci );
 * QELT *x, *si, *ci;
 *
 * qshici( x, si, ci );
 *
 *
 * DESCRIPTION:
 *
 *
 *                            x
 *                            -
 *                           | |   cosh t - 1
 *   Chi(x) = eul + ln x +   |    -----------  dt
 *                         | |          t
 *                          -
 *                          0
 *
 *               x
 *               -
 *              | |  sinh t
 *   Shi(x) =   |    ------  dt
 *            | |       t
 *             -
 *             0
 *
 * where eul = 0.57721566490153286061 is Euler's constant.
 *
 * The power series are
 *
 *           inf        2n+1
 *            -        z
 * Shi(z)  =  >  --------------
 *            -  (2n+1) (2n+1)!
 *           n=0
 *
 *                             inf      2n
 *                              -      z
 * Chi(z)  =  eul +  ln(z)  +   >  -----------
 *                              -    2n (2n)!
 *                             n=1
 *
 * Asymptotically,
 *
 *           
 *     -x                1   2!   3!
 * 2x e   Shi(x)  =  1 + - + -- + -- + ...
 *                       x    2    3
 *                           x    x
 *
 * ACCURACY:
 *
 * Series expansions are set to terminate at less than full
 * working precision.
 *
 */

/*
Cephes Math Library Release 2.0:  April, 1987
Copyright 1984, 1987, 1996 by Stephen L. Moshier
*/



#include "qhead.h"
extern QELT qeul[];
extern QELT qone[], qtwo[], qpi[];

int qshici( x, si, ci )
QELT x[], si[], ci[];
{
QELT k[NQ], z[NQ], t[NQ], a[NQ];
int sign;
union
  {
    unsigned short s[4];
    double d;
  } dx;

qtoe( x, dx.s );
if( dx.d > 87.0 )
	goto asymp;

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
	ci[0] = -1;
	qinfin(ci);
	return( 0 );
	}


qmul( x, x, z );	/* z = -x * x		*/
/*z[0] = -1;*/
qmov( qone, a );	/* a = 1.0		*/
qmov( qone, t );	/* t = 1.0		*/
qmov( qone, si );	/* si = 1.0		*/
qclear( ci );		/* ci = 0.0		*/
qmov( qtwo, k );	/* k = 2.0		*/

/*							qsici.c 2	*/

do
	{
	qdiv( k, z, t );	/* a *= z/k	*/
	qmul( t, a, a );
	qdiv( k, a, t );	/* ci += a/k	*/
	qadd( ci, t, ci );
	qadd( k, qone, k );	/* k += 1.0	*/

	qdiv( k, a, a );	/* a /= k	*/
	qdiv( k, a, t );	/* si += a/k	*/
	qadd( si, t, si );
	qadd( k, qone, k );	/* k += 1.0	*/
	}
while( ((int) si[1] - (int) a[1]) < NBITS/2 );

qmul( si, x, si );	/* si *= x		*/

if( sign )
	si[0] = -1;

/* ci = EUL + log(x) + ci */

/* comment out the following to leave with ci = Chin(x): */
qlog( x, t );
qadd( ci, t, ci );
qadd( qeul, ci, ci );
return 0;


asymp:

qdiv( x, qone, z );
qmov( qone, t );
qmov( qone, k );
qmov( qone, a );
do
	{
	qmul( z, a, a );
	qmul( k, a, a );
	qadd( a, t, t );
	qadd( k, qone, k );
	}
while( ((int) qone[1] - (int) a[1]) < 119 );

qexp( x, k );
qmul( t, k, si );
qdiv( x, si, si );
si[1] -= 1;
return 0;
}
