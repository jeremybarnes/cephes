/*   mtst.c
 Consistency tests for math functions.

 With NTRIALS=10000, the following are typical results for
 14x32 bit arithmetic:

Consistency test of math functions.
Max and rms errors for 1000 random arguments.
A = absolute error criterion (but relative if >1):
Otherwise, estimate is of relative error
x =   sqrt( square(x) ):  max =  1.541E-106   rms =  2.667E-107
x =   atan(    tan(x) ):  max =  5.449E-106   rms =  1.262E-106
x =   cbrt(   cube(x) ):  max =  2.152E-106   rms =  5.761E-107
x =    sin(   asin(x) ):  max =  1.814E-105   rms =  9.178E-106
x =    log(    exp(x) ):  max =  1.042E-105   rms =  1.109E-106
x =   log2(   exp2(x) ):  max =  6.311E-106 A rms =  1.257E-106 A
x =  log10(  exp10(x) ):  max =  3.761E-106   rms =  1.093E-106
x =  acosh(   cosh(x) ):  max =  2.862E-106   rms =  9.864E-107
x = pow( pow(x,a),1/a ):  max =  2.739E-104   rms =  1.402E-105
x =   tanh(  atanh(x) ):  max =  1.433E-103   rms =  5.784E-105
x =  asinh(   sinh(x) ):  max =  2.860E-106   rms =  9.945E-107
x =    cos(   acos(x) ):  max =  1.744E-105 A rms =  4.624E-106 A
x =  ndtri(   ndtr(x) ):  max =  1.567E-93   rms =  9.056E-95
Legendre  ellpk,  ellpe:  max =  8.796E-104   rms =  6.872E-105
Absolute error and only 200 trials:
Wronksian of   Yn,   Jn:  max =  2.512E-54 A rms =  3.864E-55 A
Absolute error and only 200 trials:
Wronksian of   Kn,   Iv:  max =  1.276E-25 A rms =  9.033E-27 A
lgam(x) = log(gamma(x)):  max =  1.422E-104 A rms =  3.050E-105 A

*/

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1988, 1994 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/

#include <stdio.h>
#include <stdlib.h>
#include "qhead.h"
#include "mconf.h"

#define NTRIALS 100
#define WTRIALS (NTRIALS/5)
#define STRTST 0

#ifndef ANSIPROT
int qsqrt ();
int qcbrt (), qexp (), qlog (), qtan (), qatn ();
int qsin (), qasin (), qcos (), qacos (), qpow ();
int qtanh (), qatanh (), qsinh (), qasinh (), qcosh (), qacosh ();
int qexp2 (), qlogtwo (), qexp10 (), qlog10 ();
int qjn(), qyn(), qin(), qkn();
int qndtr(), qndtri(), qellpe(), qellpk(), qgamma(), qlgam();
int qtoasc();
int drand();
#endif

extern int merror;
extern QELT qzero[], qone[], qtwo[], qhalf[], qpi[];

/* Provide inverses for square root and cube root: */
int 
qsquare (x, y)
     QELT x[], y[];
{
  qmul (x, x, y);
  return 0;
}

int 
qcube (x, y)
     QELT x[], y[];
{
  qmul (x, x, y);
  qmul (x, y, y);
  return 0;
}

/* lookup table for each function */
struct fundef
  {
    char *nam1;			/* the function */
    int (*name) ();
    char *nam2;			/* its inverse  */
    int (*inv) ();
    int nargs;			/* number of function arguments */
    int tstyp;			/* type code of the function */
    long ctrl;			/* relative error flag */
    double arg1w;		/* width of domain for 1st arg */
    double arg1l;		/* lower bound domain 1st arg */
    long arg1f;			/* flags, e.g. integer arg */
    double arg2w;		/* same info for args 2, 3, 4 */
    double arg2l;
    long arg2f;
  };


/* fundef.ctrl bits: */
#define RELERR 1
#define EXPSCAL 4

/* fundef.tstyp  test types: */
#define POWER 1
#define ELLIP 2
#define GAMMA 3
#define WRONK1 4
#define WRONK2 5
#define WRONK3 6
#define NDTR 7

/* fundef.argNf  argument flag bits: */
#define INT 2

#define NTESTS 17
struct fundef defs[NTESTS] =
{
  {"square", qsquare, "  sqrt", qsqrt, 1, 0, 1, 170.0, -85.0, EXPSCAL,
   0.0, 0.0, 0},
  {"   tan", qtan, "  atan", qatn, 1, 0, 1, 0.0, 0.0, 0,
   0.0, 0.0, 0},
  {"  cube", qcube, "  cbrt", qcbrt, 1, 0, 1, 2000.0, -1000.0, 0,
   0.0, 0.0, 0},
  {"  asin", qasin, "   sin", qsin, 1, 0, 1, 2.0, -1.0, 0,
   0.0, 0.0, 0},
  {"   exp", qexp, "   log", qlog, 1, 0, 1, 340.0, -170.0, 0,
   0.0, 0.0, 0},
  {"  exp2", qexp2, "  log2", qlogtwo, 1, 0, 0, 340.0, -170.0, 0,
   0.0, 0.0, 0},
  {" exp10", qexp10, " log10", qlog10, 1, 0, 1, 340.0, -170.0, 0,
   0.0, 0.0, 0},
  {"  cosh", qcosh, " acosh", qacosh, 1, 0, 1, 340.0, 0.0, 0,
   0.0, 0.0, 0},
  {"pow", qpow, "pow", qpow, 2, POWER, 1, 25.0, 0.0, 0,
   50.0, -25.0, 0},
  {" atanh", qatanh, "  tanh", qtanh, 1, 0, 1, 2.0, -1.0, 0,
   0.0, 0.0, 0},
  {"  sinh", qsinh, " asinh", qasinh, 1, 0, 1, 340.0, 0.0, 0,
   0.0, 0.0, 0},
  {"  acos", qacos, "   cos", qcos, 1, 0, 0, 2.0, -1.0, 0,
   0.0, 0.0, 0},
  {"  ndtr",   qndtr,   " ndtri",  qndtri, 1, NDTR, 1,   10.0,   -10.0,   0,
     0.0, 0.0, 0},
  {" ellpe", qellpe, " ellpk",  qellpk, 1, ELLIP, 1, 1.0,  0.0,   0,
     0.0, 0.0, 0},
  {"  Jn",     qjn,   "  Yn",     qyn, 2, WRONK1, 0, 30.0,  0.1,  0,
     40.0, -20.0, INT},
  {"  Iv",     qin,   "  Kn",     qkn, 2, WRONK2, 0,  30.0, 0.1,  0,
     20.0, 0.0, INT},
  {"gamma",  qgamma,   "lgam",   qlgam, 1, GAMMA, 0, 11.0, 1.0,   0,
     0.0, 0.0, 0},
};

static char *headrs[] =
{
  "x = %s( %s(x) ): ",
  "x = %s( %s(x,a),1/a ): ",	/* power */
  "Legendre %s, %s: ",		/* ellip */
  "%s(x) = log(%s(x)): ",	/* gamma */
  "Wronksian of %s, %s: ",	/* wronk1 */
  "Wronksian of %s, %s: ",	/* wronk2 */
  "Wronksian of %s, %s: ",	/* wronk3 */
  "x = %s( %s(x) ): ",
};

static QELT y1[NQ], y2[NQ], y3[NQ], y4[NQ], a[NQ];
static QELT x[NQ], y[NQ], z[NQ], e[NQ], max[NQ], rmsa[NQ], rms[NQ], ave[NQ];

union {
  double d;
  unsigned short i[4];
} dx, dy;

static QELT qthree[NQ], temp[NQ];
static QELT base1[NQ], width1[NQ], base2[NQ], width2[NQ], fuzz[NQ];
char str1[40], str2[40];


int main ()
{
  int (*fun) ();
  int (*ifun) ();
  struct fundef *d;
  int i, k, itst;
  int ntr;
  long ll, m;

  ntr = NTRIALS;
  printf ("Consistency test of math functions.\n");
  printf ("Max and rms errors for %d random arguments.\n",
	  ntr);
  printf ("A = absolute error criterion (but relative if >1):\n");
  printf ("Otherwise, estimate is of relative error\n");

  /* Initialize machine dependent parameters to test near the
 * largest an smallest possible arguments.  To compare different
 * machines, use the same test intervals for all systems.
 */
  defs[1].arg1w = 3.14159;
  defs[1].arg1l = -3.14159 / 2.0;
  qadd (qone, qtwo, qthree);
  dx.d = 1.0e-15;
  etoq (dx.i, fuzz);

  /* Outer loop, on the test number: */

  for (itst = STRTST; itst < NTESTS; itst++)
    {
      d = &defs[itst];
      m = 0;
      qclear (max);
      qclear (rmsa);
      qclear (ave);
      fun = d->name;
      ifun = d->inv;
      etoq ((unsigned short *)&(d->arg2w), width2);
      etoq ((unsigned short *)&(d->arg2l), base2);
      etoq ((unsigned short *)&(d->arg1w), width1);
      etoq ((unsigned short *)&(d->arg1l), base1);

      /* Smaller number of trials for Wronksians
	 (put them at end of list)  */
      if (d->tstyp == WRONK1 || d->tstyp == WRONK2 || d->tstyp == NDTR)
	{
	  ntr = WTRIALS;
	  printf ("Absolute error and only %d trials:\n", ntr);
	}
      printf (headrs[d->tstyp], d->nam2, d->nam1);

      for (i = 0; i < ntr; i++)
	{
	  m++;

	  /* make random number(s) in desired range(s) */
	  k = 0;
	  switch (d->nargs)
	    {

	    default:
	      goto illegn;

	    case 2:
	      drand (&dx.d);
	      drand (&dy.d);
	      /* a = 1.0L + ((long double )dx * dy - 1.0L)/3.0L; */
	      etoq (dx.i, y1);
	      etoq (dy.i, y2);
	      qmul (y1, y2, y1);
	      qsub (qone, y1, y1);
	      qdiv (qthree, y1, y1);
	      /*qadd( qone, y1, y1 );*/
	      /* a = d->arg2w *  ( a - 1.0L )  +  d->arg2l; */
	      qmul (width2, y1, y1);
	      qadd (base2, y1, a);

	      if (d->arg2f & EXPSCAL)
		{
		  qexp (a, a);
		  drand (&dx.d);
		  drand (&dy.d);
		  /* y2 = 1.0L + ((long double )dx * dy - 1.0L)/3.0L; */
		  etoq (dx.i, y3);
		  etoq (dy.i, y2);
		  qmul (y3, y2, y2);
		  qsub (qone, y2, y2);
		  qdiv (qthree, y2, y2);
		  qadd (qone, y2, y2);
		  /*	a -= 1.0e-13L * a * y2; */
		  qmul (a, y2, y2);
		  qmul (fuzz, y2, y2);
		  qsub (y2, a, a);
		}
	      if (d->arg2f & INT)
		{
		  qadd (a, qhalf, a);
		  qifrac (a, &ll, y2);
		  ltoq (&ll, a);
		  k = ll;
		}

	    case 1:
	      drand (&dx.d);
	      drand (&dy.d);
	      /* x = 1.0L + ((long double )dx * (long double )dy - 1.0L)/3.0L; */
	      etoq (dx.i, y1);
	      etoq (dy.i, y2);
	      qmul (y1, y2, y1);
	      qsub (qone, y1, y1);
	      qdiv (qthree, y1, y1);
	      /*qadd( qone, y1, y1 );*/
	      qmul (width1, y1, y1);
	      qadd (base1, y1, x);
	      if (qcmp (x, base1) < 0)
		qmov (base1, x);
	      qadd (base1, width1, y1);
	      if (qcmp (x, y1) > 0)
		qmov (y1, x);
	      if (d->arg1f & EXPSCAL)
		{
		  qexp (x, x);
		  drand (&dx.d);
		  drand (&dy.d);
		  /*	a = 1.0L + ((long double )dx * dy - 1.0L)/3.0L; */
		  etoq (dx.i, y1);
		  etoq (dy.i, y2);
		  qmul (y1, y2, y1);
		  qsub (qone, y1, y1);
		  qdiv (qthree, y1, y1);
		  /*	x += 1.0e-13L * x * a; */
		  qmul (x, y2, y2);
		  qmul (fuzz, y2, y2);
		  qsub (y2, x, x);
		}
	    }

	  /* compute function under test */
	  switch (d->nargs)
	    {
	    case 1:
	      switch (d->tstyp)
		{
		case ELLIP:
		  (*(fun)) (x, y1);
		  qsub (x, qone, y3);
		  (*(fun)) (y3, y2);
		  (*(ifun)) (y3, y4);
		  (*(ifun)) (x, y3);
		  break;

		case GAMMA:
		  qlgam (x, y);
		  qgamma(x, z);
		  qlog(z, x);
		  break;

		default:
		  (*(fun)) (x, z);
		  (*(ifun)) (z, y);
		}
/*
if( merror )
	{
	printf( "error: x = %.15e, z = %.15e, y = %.15e\n",
	 (double )x, (double )z, (double )y );
	}
*/
	      break;

	    case 2:
	      if (d->arg2f & INT)
		{
		  switch (d->tstyp)
		    {
		    case WRONK1:
		      (*fun) (a, x, y1);	/* jn */
		      qadd (a, qone, y3);
		      (*fun) (y3, x, y2);
		      (*ifun) (y3, x, y4);
		      (*ifun) (a, x, y3);	/* yn */
		      break;

		    case WRONK2:
		      (*fun) (a, x, y1);	/* iv */
		      qadd (a, qone, y3);
		      (*fun) (y3, x, y2);
		      (*ifun) (k, x, y3);	/* kn */
		      (*ifun) (k+1, x, y4);
		      break;

		    default:
		      (*fun) (a, x, z);
		      (*ifun) (a, z, y);
		    }
		}
	      else
		{
		  if (d->tstyp == POWER)
		    {
		      (*fun) (x, a, z);
		      qdiv (a, qone, y3);
		      (*ifun) (z, y3, y);
		    }
		  else
		    {
		      (*fun) (a, x, z);
		      (*ifun) (a, z, y);
		    }
		}
	      break;


	    default:
	    illegn:
	      printf ("Illegal nargs= %d", d->nargs);
	      exit (1);
	    }

	  switch (d->tstyp)
	    {
	    case WRONK1:
	      /*	e = (y2*y3 - y1*y4) - 2.0L/(PIL*x); *//* Jn, Yn */
	      qmul (y1, y4, temp);
	      qmul (y2, y3, e);
	      qsub (temp, e, e);
	      qmov (qpi, temp);
	      qmul (temp, x, temp);
	      qdiv (temp, qtwo, temp);
	      qsub (temp, e, e);
	      break;

	    case WRONK2:
	      /*	e = (y2*y3 + y1*y4) - 1.0L/x; *//* In, Kn */
	      qmul (y1, y4, temp);
	      qmul (y2, y3, e);
	      qadd (temp, e, e);
	      qdiv (x, qone, temp);
	      qsub (temp, e, e);
	      break;

	    case ELLIP:
	      /*	e = (y1-y3)*y4 + y3*y2 - PIO2L; */
	      qsub (y3, y1, temp);
	      qmul (y4, temp, e);
	      qmul (y3, y2, temp);
	      qadd (temp, e, e);
	      qmov (qpi, temp);
	      temp[1] -= 1;
	      qsub (temp, e, e);
	      break;

	    default:
	      /*	e = y - x; */
	      qsub (x, y, e);
	      break;
	    }

	  if (d->ctrl & RELERR)
	    {
	      if (qcmp (x, qzero) != 0)
		qdiv (x, e, e);
	      else
		printf ("warning, x == 0\n");
	    }
	  else
	    {
	      qmov (x, temp);
	      temp[0] = 0;
	      if (qcmp (temp, qone) > 0)
		qdiv (x, e, e);
	    }

	  qadd (e, ave, ave);
	  /* absolute value of error */
	  e[0] = 0;
	  /* peak detect the error */
	  if (qcmp (e, max) > 0)
	    {
	      qmov (e, max);
#if 0
	      if (e > 1.0e-10L)
		{
		  da = x;
		  db = z;
		  dc = y;
		  dd = max;
		  printf ("x %.6E z %.6E y %.6E max %.4E\n",
			  da, db, dc, dd);
		}
#endif
	    }
	  /* accumulate rms error	*/
	  if( e[1] )
	    {
	      qmul (e, e, temp);
	      qadd (temp, rmsa, rmsa);
	    }
	}

      /* report after NTRIALS trials */
      ltoq (&m, temp);
      qdiv (temp, rmsa, temp);
      qsqrt (temp, rms);
      qtoasc (max, str1, 3);
      qtoasc (rms, str2, 3);
      if (d->ctrl & RELERR)
	printf (" max = %s   rms = %s\n", str1, str2);
      else
	printf (" max = %s A rms = %s A\n", str1, str2);
    }				/* loop on itst */

exit(0);
}

