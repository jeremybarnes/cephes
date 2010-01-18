/*							cgamma
 *
 *	Complex gamma function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qcgamma( x, y );
 * qcmplx *x, *y;
 *
 * qcgamma( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns complex-valued gamma function of the complex argument.
 *
 * gamma(x) = exp (log(gamma(x)))
 *
 */
/*							qclgam
 *
 *	Natural logarithm of complex gamma function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qclgam( x, y );
 * qcmplx *x, *y;
 *
 * qclgam( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the base e (2.718...) logarithm of the complex gamma
 * function of the argument.
 *
 * The logarithm of the gamma function is approximated by the
 * logarithmic version of Stirling's asymptotic formula.
 * Arguments of real part less than +32 are increased by recurrence.
 * The cosecant reflection formula is employed for arguments
 * having real part less than -34.
 *
 */

/*
Cephes Math Library Release 2.7:  March, 1998
Copyright 1984, 1998 Stephen L. Moshier
*/

/* Complex variable natural logarithm of gamma function */
#include "qhead.h"
#include "mconf.h"
#if ANSIC
#define qcarg(z,a) qatn2((z)->i, (z)->r, a)
#else
#define qcarg(z,a) qatn2((z)->r, (z)->i, a)
#endif

extern QELT qhalf[], qone[], qpi[];
extern qcmplx qcone;
#ifdef ANSIPROT
int initqgam(void);
#else
int qcneg(), qcsin(), qcmul(), qcmov(), qcdiv(), qclog(), qcsub(), qcadd();
int qcexp(), initqgam();
#endif
/* See qgamma.c for coefficients. */
#define NG 55
extern QELT qgamcof[NG][NQ];
extern QELT qgam12[];

extern int qgamini;

int qclgam( x, y )
qcmplx *x, *y;
{
qcmplx v, w, g, xx, t;
QELT a[NQ], b[NQ];
QELT *p;
int i, cj;
long il;

if( qgamini == 0 )
  initqgam();

qmov( qone, &qcone.r[0] );
qclear( &qcone.i[0] );
qcmov( x, &xx );

cj = 0;
if (xx.i[0] != 0)
  {
    cj = 1;
    xx.i[0] = 0;
  }

#if NQ == 28
if( (xx.r[1] > (QELT) (EXPONE + 8))
#else
if( (xx.r[1] > (QELT) (EXPONE + 5))
#endif
    && (x->r[0] != 0) )
	{
	  qmov(&xx.r[0], a);
	  qfloor(a,b);
	  if (qcmp(a,b) == 0)
	    {
qlgover:
	      mtherr("qlgam", SING);
	      return 0;
	    }
	  qcmov(&qcone, &t);
	  qcsub(&xx, &t, &t);
	  qclgam( &t, &t );  /* ln gamma(1-z)  */
	  qneg(b);
	  qifrac(b,&il,a);
	  /*
	  if (il & 1)
	    il += 1;
	    */
	  ltoq(&il, b);
	  qadd (b, &xx.r[0], &xx.r[0]);
	qmul( &xx.r[0], qpi, &g.r[0] );	/* PI/(sin(PI*z))	*/
	qmul( &xx.i[0], qpi, &g.i[0] );
	qcsin( &g, &g );
	if(g.r[1] == 0 && g.i[1] == 0)
	  goto qlgover;
	qclog(&g, &g);
	qlog( qpi, &v.r[0] );
	qclear( &v.i[0] );
	qcsub( &g, &v, y );
	qcsub( &t, y, y );	/* ... /gamma(x)	*/
	qmul(qpi, b, b);
	qsub(b, &y->i[0], &y->i[0]);
	goto qcldone;
	}

/* range reduction: transform argument to be greater than 32.
   To satisfy Im {clgam(z)} = arg cgamma(z), accumulate
   arg v during the recurrence.  */
/*qcmov( x, &xx );*/
qclear( a );
qclear( b );
qcmov( &qcone, &v );
#if NQ == 28
while( xx.r[1] <= (QELT) (EXPONE + 8) )
#else
while( xx.r[1] <= (QELT) (EXPONE + 5) )
#endif
	{
	qcmul( &xx, &v, &v );
	qcarg( &xx, b);
	qadd( b, a, a );
	qcadd( &qcone, &xx, &xx );
	}
qcabs(&v, b);
qlog(b,&v.r[0]);
qmov(a,&v.i[0]);
qcneg(&v);

/*  Asymptotic series in 1/x**2 */
qcmul( &xx, &xx, &w );
qcdiv( &w, &qcone, &w );

p = &qgamcof[0][0];
qmul( &w.r[0], p, &g.r[0] );
qmul( &w.i[0], p, &g.i[0] );
p += NQ;
qadd( &g.r[0], p, &g.r[0] );
for( i=0; i<NG-2; i++ )
	{
	qcmul( &w, &g, &g );
	p += NQ;
	qadd( &g.r[0], p, &g.r[0] );
	}

qcdiv( &xx, &g, &g );

/* g + (x - 0.5)*log(x) - x + qgam12	*/
qsub( qhalf, &xx.r[0], &t.r[0] );
qmov( &xx.i[0], &t.i[0] );
qclog( &xx, &w );
qcmul( &t, &w, &t );
qcsub( &xx, &t, &t );
qadd( qgam12, &t.r[0], &t.r[0] );
qcadd( &g, &t, &g );

qcadd( &v, &g, y );
qcldone:
if (cj)
  {
    y->i[0] = ~(y->i[0]);
  }
return(0);
}

/*							qgamma()	*/

/* Complex variable gamma function check routine */


int qcgamma( x, y )
qcmplx *x, *y;
{
#if 0
qcmplx t, s, xx;
#else
qcmplx xx;
#endif

qcmov( x, &xx );
#if 0
if( qgamini == 0 )
  initqgam();
if( x->r[0] != 0 )
	{
	qcneg(&xx);
	qcgamma( &xx, &t );
	qmul( &xx.r[0], qpi, &s.r[0] );
	qmul( &xx.i[0], qpi, &s.i[0] );
	qcsin( &s, &s );
	qcmul( &xx, &s, &s );
	qcmul( &s, &t, &t );
	qmov( qpi, &s.r[0] );
	qclear( &s.i[0] );
	qcdiv( &t, &s, y );
/*	y[0] = ~y[0];*/
	qcneg( y );
	return(0);
	}
#endif
qclgam( &xx, y );
qcexp( y, y );
return(0);
}

