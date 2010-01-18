/*							qfresnl
 *
 *	Fresnel integral
 *
 *
 *
 * SYNOPSIS:
 *
 * int qfresnl( x, s, c );
 * QELT *x, *s, *c;
 *
 * qfresnl( x, s, c );
 *
 *
 * DESCRIPTION:
 *
 * Evaluates the Fresnel integrals
 *
 *           x
 *           -
 *          | |
 * C(x) =   |   cos(pi/2 t**2) dt,
 *        | |
 *         -
 *          0
 *
 *           x
 *           -
 *          | |
 * S(x) =   |   sin(pi/2 t**2) dt.
 *        | |
 *         -
 *          0
 *
 *
 * The integrals are evaluated by a power series for x < 1.
 * For large x auxiliary functions f(x) and g(x) are employed
 * such that
 *
 * C(x) = 0.5 + f(x) sin( pi/2 x**2 ) - g(x) cos( pi/2 x**2 )
 * S(x) = 0.5 - f(x) cos( pi/2 x**2 ) - g(x) sin( pi/2 x**2 )
 *
 * Routine qfresfg computes f and g.
 *
 *
 * ACCURACY:
 *
 * Series expansions are truncated at less than full working precision.
 */

/*
Cephes Math Library Release 2.1:  January, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
*/

#include <stdio.h>
#include "qhead.h"

/* asymptotic expansion converges to 144 bits at BRKPT >= 8.2 */
#if NBITS > 144
#define BRKPT 8.2
#else
#define BRKPT 5.7
#endif
extern QELT qpi[], qone[], qtwo[];

static union
  {
    unsigned short s[4];
    double d;
  } dtemp;

static QELT x2[NQ];
static QELT c[NQ];
static QELT q[NQ];
static QELT d[NQ];
static QELT t[NQ];
static QELT x4[NQ];
static QELT x3[NQ];
static QELT t1[NQ];
static QELT cc0[NQ];
static QELT ss0[NQ];
static QELT f[NQ];
static QELT g[NQ];

#ifdef ANSIPROT
int qfresfg( QELT *, QELT *, QELT * );
#else
int qfresfg();
#endif

int qfresnl( x, ss, cc )
QELT x[], ss[], cc[];
{
QELT max;
int sign, i, iconv, ic2;
long il;

qtoe( x, dtemp.s );
if( dtemp.d > BRKPT ) /* 7.0 5.7 */
	goto asymp0;

max = 0;
qmul( x, x, x2 );
qmul( x, x2, x3 );	/* x3 = x**3		*/
qmul( x2, x2, x4 );	/* x4 = x**4		*/
sign = -1;
qmov( qone, c );	/* c=1			*/
qmov( qpi, q );
q[1] -= 1;
qmul( q, x3, x3 );	/* x3 = pi/2 * x**3	*/
qmul( q, q, q );	/* q = (pi/2)**2	*/

qmov( qone, t );
qmov( qone, cc0 );
qmov( qone, ss0 );
il = 3;
ltoq( &il, d );
qdiv( d, ss0, ss0 );	/* ss = pi/2 * x**3 / 3	*/
i = 1;
do
	{
	qadd( c, qone, c );	/* c += 1		*/
	qdiv( c, t, t );	/* t /= c		*/
	qmul( q, t, t );	/* t *= (pi/2)**2	*/
	qmul( t, x4, t );	/* t *= x**4		*/
	il = 4*i + 1;
	ltoq( &il, d );
	qmov( t, f );
	qdiv( d, f, f );
	if( f[1] > max )
		max = f[1];
	if( sign < 0 )
		qsub( f, cc0, cc0 );
	else
		qadd( cc0, f, cc0 );
	iconv = cc0[1] - f[1];
	qadd( c, qone, c );
	qdiv( c, t, t );
	qmov( t, f );
	il = 4*i + 3;
	ltoq( &il, d );
	qdiv( d, f, f );
	if( sign < 0 )
		qsub( f, ss0, ss0 );
	else
		qadd( ss0, f, ss0 );
	if( f[1] > max )
		max = f[1];
	ic2 = (int) ss0[1] - (int) f[1];
	if( ic2 < iconv )
		iconv = ic2;
	sign = -sign;
	++i;
	}
while( iconv < 144 );

max = NBITS - max;
if( ss0[1] < cc0[1] )
	max += ss0[1];
else
	max += cc0[1];
if( max < (QELT) (3*NBITS/7) )
	printf( "qfresf: prec %d bits\n", max );

qmul( x, cc0, cc );		/*	C(x)	*/
qmul( x3, ss0, ss );		/*	S(x)	*/
goto done;


asymp0:

qfresfg( x, f, g );

qmov( qpi, t );		/*	t = PIO2 * x * x;	*/
t[1] -= 1;
qmul( x, t, t );
qmul( x, t, t );
qcos( t, c );		/*	c = cos(t);		*/
qsin( t, t );		/*	t = sin(t);		*/
qmov( qone, cc );	/*	cc = 0.5  +  f * t  -  g * c;	*/
cc[1] -= 1;
qmul( f, t, x3 );
qadd( x3, cc, cc );
qmul( g, c, x3 );
qsub( x3, cc, cc );

qmov( qone, ss );	/*	ss = 0.5  -  f * c  -  g * t;	*/
ss[1] -= 1;
qmul( f, c, x3 );
qsub( x3, ss, ss );
qmul( g, t, x3 );
qsub( x3, ss, ss );

done:

if( x[0] != 0 )
	{
	cc[0] = -1;
	ss[0] = -1;
	}

return(0);
}


/*		the auxiliary functions		*/
static QELT lss[NQ] = {0};
static QELT lcc[NQ] = {0};

int qfresfg( x, f, g )
QELT x[], f[], g[];
{
int sign, max;
#define co c
#define si q

qtoe( x, dtemp.s );
if( dtemp.d <= BRKPT )
	goto nasymp;


/*		Asymptotic power series auxiliary functions
 *		for large argument
 */

qmov( qone, c );	/* 	c = 1.0;		*/
qmov( qone, t );	/*	t = 1.0;		*/
qadd( qone, qone, t1 );	/*	t1 = 2.0;		*/
qmul( qpi, x, q );	/*	q = PIO2 * 2.0 * x;	*/
qmul( q, x, x3 );	/*	x3 = nn * x;		*/
qmul( x3, x3, x4 );	/*	x4 = x3 * x3;		*/
sign = -1;
qmov( qone, f );	/*	f = 1.0;		*/
qmov( qone, g );	/*	g = 1.0;		*/

/*
if( dtemp.d > 20000.0 )
		goto done5;
*/

while( ((int) qone[1] - (int) t[1]) < NBITS/2 )
{
qadd( qtwo, c, c );	/*	c += 2.0;		*/
qmul( t, c, t );	/*	t *= c / x4;		*/
qdiv( x4, t, t );
if( sign < 0 )
	qsub( t, f, f ); /*	f -= t;			*/
else
	qadd( t, f, f ); /*	f += t;			*/
qadd( qtwo, c, c );	/*	c += 2.0;		*/
qmul( t, c, t );	/*	t *= c;			*/
/*
	qtoe( t, dtemp.s );
	printf( "%.4E\n", dtemp.d );
*/
if( t[1] > t1[1] )
	{
	max = qone[1] - t1[1];
	if( max < 3*NBITS/7 )
		printf( "qfres asymp prec %d bits\n", max );
	goto done5;
	}
qmov( t, t1 );		/*	t1 = t;			*/
if( sign < 0 )
	qsub( t, g, g ); /*	g -= t;			*/
else
	qadd( t, g, g ); /*	g += t;			*/
sign = -sign;
}


done5:
qmul( x3, q, d );	/*	g /= x3 * q;		*/
qdiv( d, g, g );
qdiv( q, f, f );	/*	f /= q;		*/
return 0;





nasymp:

qfresnl( x, lss, lcc );
qmov( qone, co );
co[1] -= 1;
qsub( lcc, co, lcc );
qsub( lss, co, lss );

qmul( qpi, x2, x2 );
x2[1] -= 1;
qcos( x2, co );
qsin( x2, si );

		/* f = (.5-ss)*cos - (.5-cc)*sin	*/

qmul( lss, co, f );
qmul( lcc, si, t );
qsub( t, f, f );

		/* g = (.5-cc)*cos + (.5-ss)*sin	*/
qmul( lcc, co, g );
qmul( lss, si, t );
qadd( t, g, g );
return 0;
}

