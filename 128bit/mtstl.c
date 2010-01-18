/*   mtst.c
 Consistency tests for math functions.

 The following are typical results for
 an alleged IEEE long double precision arithmetic:

Consistency test of math functions.
Max and rms errors for 1000 random arguments.
A = absolute error criterion (but relative if >1):
Otherwise, estimate is of relative error
x =    cos(   acos(x) ):  max = 2.89E-34 A rms = 8.36E-35 A
x =   atan(    tan(x) ):  max = 2.41E-34   rms = 5.55E-35
x =    sin(   asin(x) ):  max = 3.70E-34   rms = 4.82E-35
x =   sqrt( square(x) ):  max = 0.00E+00   rms = 0.00E+00
x =    log(    exp(x) ):  max = 2.93E-34   rms = 1.62E-35
x =   log2(   exp2(x) ):  max = 9.63E-35 A rms = 3.40E-36 A
x =  log10(  exp10(x) ):  max = 2.41E-35 A rms = 1.01E-36 A
x =  acosh(   cosh(x) ):  max = 1.33E-34   rms = 4.20E-36
x = pow( pow(x,a),1/a ):  max = 8.62E-34   rms = 1.85E-34
x =   tanh(  atanh(x) ):  max = 1.96E-34   rms = 2.01E-35
x =  asinh(   sinh(x) ):  max = 1.93E-34   rms = 3.09E-35
x =   cbrt(   cube(x) ):  max = 1.36E-34   rms = 6.61E-36
*/

/*
Cephes Math Library Release 2.7:  December, 1998
Copyright 1984 - 1998 by Stephen L. Moshier
*/


#include "mconf.h"

#define NTRIALS 1000
#define WTRIALS (NTRIALS/5)
#define STRTST 0

/* Note, fabsl may be an intrinsic function. */

long double fabsl(), sqrtl();
long double cbrtl(), expl(), logl(), tanl(), atanl();
long double sinl(), asinl(), cosl(), acosl(), powl();
long double tanhl(), atanhl(), sinhl(), asinhl(), coshl(), acoshl();
long double exp2l(), log2l(), exp10l(), log10l();
extern int merror;
int printf(), drand();

/*
NYI:
double jn(), yn(), iv(), kn();
double ndtr(), ndtri(), ellpe(), ellpk(), gamma(), lgam();
*/

/* Provide inverses for square root and cube root: */
long double squarel(x)
long double x;
{
return( x * x );
}

long double cubel(x)
long double x;
{
return( x * x * x );
}

/* lookup table for each function */
struct fundef
	{
	char *nam1;		/* the function */
	long double (*name )();
	char *nam2;		/* its inverse  */
	long double (*inv )();
	int nargs;		/* number of function arguments */
	int tstyp;		/* type code of the function */
	long ctrl;		/* relative error flag */
	long double arg1w;		/* width of domain for 1st arg */
	long double arg1l;		/* lower bound domain 1st arg */
	long arg1f;		/* flags, e.g. integer arg */
	long double arg2w;		/* same info for args 2, 3, 4 */
	long double arg2l;
	long arg2f;
/*
	double arg3w;
	double arg3l;
	long arg3f;
	double arg4w;
	double arg4l;
	long arg4f;
*/
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

/* fundef.argNf  argument flag bits: */
#define INT 2

extern long double MINLOGL;
extern long double MAXLOGL;
extern long double PIL;
extern long double PIO2L;
/*
define MINLOG -170.0
define MAXLOG +170.0
define PI 3.14159265358979323846
define PIO2 1.570796326794896619
*/

#define NTESTS 12
struct fundef defs[NTESTS] = {
{"  acos",   acosl,   "   cos",    cosl, 1, 0, 0,   2.0L,      -1.0L,  0,
0.0, 0.0, 0},
{"   tan",    tanl,   "  atan",   atanl, 1, 0, 1,    0.0L,     0.0L,  0,
0.0, 0.0, 0},
{"  asin",   asinl,   "   sin",    sinl, 1, 0, 1,   2.0L,     -1.0L,  0,
0.0, 0.0, 0},
{"square", squarel,   "  sqrt",   sqrtl, 1, 0, 1,  170.0L,    -85.0L, EXPSCAL,
0.0, 0.0, 0},
{"   exp",    expl,   "   log",    logl, 1, 0, 1,  340.0L,    -170.0L,  0,
0.0, 0.0, 0},
{"  exp2",   exp2l,   "  log2",   log2l, 1, 0, 0,  340.0L,    -170.0L,  0,
0.0, 0.0, 0},
{" exp10",  exp10l,   " log10",  log10l, 1, 0, 0,  340.0L,    -170.0L,  0,
0.0, 0.0, 0},
{"  cosh",   coshl,   " acosh",  acoshl, 1, 0, 1,  340.0L,     0.0L,  0,
0.0, 0.0, 0},
{"pow",       powl,      "pow",    powl, 2, POWER, 1, 25.0L, 0.0L,   0,
50.0, -25.0, 0},
{" atanh",  atanhl,   "  tanh",   tanhl, 1, 0, 1,    2.0L,    -1.0L,  0,
0.0, 0.0, 0},
{"  sinh",   sinhl,   " asinh",  asinhl, 1, 0, 1,  10.0L,   0.0L,  0,
0.0, 0.0, 0},
{"  cube",   cubel,   "  cbrt",   cbrtl, 1, 0, 1, 2000.0L, -1000.0L,   0,
0.0, 0.0, 0},
};

static char *headrs[] = {
"x = %s( %s(x) ): ",
"x = %s( %s(x,a),1/a ): ",	/* power */
"Legendre %s, %s: ",		/* ellip */
"%s(x) = log(%s(x)): ",		/* gamma */
"Wronksian of %s, %s: ",  /* wronk1 */
"Wronksian of %s, %s: ",  /* wronk2 */
"Wronksian of %s, %s: ",  /* wronk3 */
};
 
static long double y1 = 0.0;
static long double y2 = 0.0;
static long double y3 = 0.0;
static long double y4 = 0.0;
static long double a = 0.0;
static long double x = 0.0;
static long double y = 0.0;
static long double z = 0.0;
static long double e = 0.0;
static long double max = 0.0;
static long double rmsa = 0.0;
static long double rms = 0.0;
static long double ave = 0.0;
static double dx = 0.0;
static double dy = 0.0;
static double da = 0.0;
static double db = 0.0;
static double dc = 0.0;
static double dd = 0.0;

int
main()
{
long double (*fun )();
long double (*ifun )();
struct fundef *d;
int i, k, itst;
int m, ntr;

ntr = NTRIALS;
printf( "Consistency test of math functions.\n" );
printf( "Max and rms errors for %d random arguments.\n",
	ntr );
printf( "A = absolute error criterion (but relative if >1):\n" );
printf( "Otherwise, estimate is of relative error\n" );

/* Initialize machine dependent parameters to test near the
 * largest an smallest possible arguments.  To compare different
 * machines, use the same test intervals for all systems.
 */
defs[1].arg1w = PIL;
defs[1].arg1l = -PIL/2.0;
/*
defs[3].arg1w = MAXLOGL;
defs[3].arg1l = -MAXLOGL/2.0;
defs[4].arg1w = 2.0*MAXLOGL;
defs[4].arg1l = -MAXLOGL;
defs[6].arg1w = 2.0*MAXLOGL;
defs[6].arg1l = -MAXLOGL;
defs[7].arg1w = MAXLOGL;
defs[7].arg1l = 0.0;
*/

/* Outer loop, on the test number: */

for( itst=STRTST; itst<NTESTS; itst++ )
{
d = &defs[itst];
m = 0;
max = 0.0L;
rmsa = 0.0L;
ave = 0.0L;
fun = d->name;
ifun = d->inv;

/* Smaller number of trials for Wronksians
 * (put them at end of list)
 */
if( d->tstyp == WRONK1 )
	{
	ntr = WTRIALS;
	printf( "Absolute error and only %d trials:\n", ntr );
	}
/*
y1 = d->arg1l;
y2 = d->arg1w;
da = y1;
db = y2;
printf( "arg1l = %.4e, arg1w = %.4e\n", da, db );
*/
printf( headrs[d->tstyp], d->nam2, d->nam1 );

for( i=0; i<ntr; i++ )
{
m++;
k = 0;
/* make random number(s) in desired range(s) */
switch( d->nargs )
{

default:
goto illegn;
	
case 2:
drand( &dx );
drand( &dy );
a = 1.0L + ((long double )dx * dy - 1.0L)/3.0L;
a = d->arg2w *  ( a - 1.0L )  +  d->arg2l;
if( d->arg2f & EXPSCAL )
	{
	a = expl(a);
	drand( &dx );
	drand( &dy );
	y2 = 1.0L + ((long double )dx * dy - 1.0L)/3.0L;
	a -= 1.0e-13L * a * y2;
	}
if( d->arg2f & INT )
	{
	k = a + 0.25L;
	a = k;
	}

case 1:
drand( &dx );
drand( &dy );
x = 1.0L + ((long double )dx * (long double )dy - 1.0L)/3.0L;
y1 = d->arg1l;
y2 = d->arg1w;
x = y2 *  ( x - 1.0L )  +  y1;
if( x < y1 )
	x = y1;
y1 += y2;
if( x > y1 )
	x = y1;
if( d->arg1f & EXPSCAL )
	{
	x = expl(x);
	drand( &dx );
	drand( &dy );
	a = 1.0L + ((long double )dx * dy - 1.0L)/3.0L;
	x += 1.0e-13L * x * a;
	}
}

/* compute function under test */
switch( d->nargs )
	{
	case 1:
	switch( d->tstyp )
		{
		case ELLIP:
		y1 = ( *(fun) )(x);
		y2 = ( *(fun) )(1.0L-x);
		y3 = ( *(ifun) )(x);
		y4 = ( *(ifun) )(1.0L-x);
		break;
#if 0
		case GAMMA:
		y = lgaml(x);
		x = logl( gammal(x) );
		break;
#endif
		default:
		z = ( *(fun) )(x);
		y = ( *(ifun) )(z);
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
	if( d->arg2f & INT )
		{
		switch( d->tstyp )
			{
			case WRONK1:
			y1 = (*fun)( k, x ); /* jn */
			y2 = (*fun)( k+1, x );
			y3 = (*ifun)( k, x ); /* yn */
			y4 = (*ifun)( k+1, x );	
			break;

			case WRONK2:
			y1 = (*fun)( a, x ); /* iv */
			y2 = (*fun)( a+1.0L, x );
			y3 = (*ifun)( k, x ); /* kn */	
			y4 = (*ifun)( k+1, x );	
			break;

			default:
			z = (*fun)( k, x );
			y = (*ifun)( k, z );
			}
		}
	else
		{
		if( d->tstyp == POWER )
			{
			z = (*fun)( x, a );
			y = (*ifun)( z, 1.0L/a );
			}
		else
			{
			z = (*fun)( a, x );
			y = (*ifun)( a, z );
			}
		}
	break;


	default:
illegn:
	printf( "Illegal nargs= %d", d->nargs );
	exit(1);
	}	

switch( d->tstyp )
	{
	case WRONK1:
	e = (y2*y3 - y1*y4) - 2.0L/(PIL*x); /* Jn, Yn */
	break;

	case WRONK2:
	e = (y2*y3 + y1*y4) - 1.0L/x; /* In, Kn */
	break;
	
	case ELLIP:
	e = (y1-y3)*y4 + y3*y2 - PIO2L;
	break;

	default:
	e = y - x;
	break;
	}

if( d->ctrl & RELERR )
	{
	if( x != 0.0L )
		e /= x;
	else
		printf( "warning, x == 0\n" );
	}
else
	{
	if( fabsl(x) > 1.0L )
		e /= x;
	}

ave += e;
/* absolute value of error */
if( e < 0 )
	e = -e;

/* peak detect the error */
if( e > max )
	{
	max = e;

	if( e > 3.0e-15L )
		{
		  da = x;
		  db = z;
		  dc = y;
		  dd = max;
		printf("x %.16E z %.16E y %.16E max %.4E\n",
		da, db, dc, dd );
/*
		if( d->tstyp >= WRONK1 )
			{
		printf( "y1 %.4E y2 %.4E y3 %.4E y4 %.4E k %d x %.4E\n",
		 (double )y1, (double )y2, (double )y3,
		 (double )y4, k, (double )x );
			}
*/
		}

/*
	printf("%.8E %.8E %.4E %6ld \n", x, y, max, n);
	printf("%d %.8E %.8E %.4E %6ld \n", k, x, y, max, n);
	printf("%.6E %.6E %.6E %.4E %6ld \n", a, x, y, max, n);
	printf("%.6E %.6E %.6E %.6E %.4E %6ld \n", a, b, x, y, max, n);
	printf("%.4E %.4E %.4E %.4E %.4E %.4E %6ld \n",
		a, b, c, x, y, max, n);
*/
	}

/* accumulate rms error	*/
e *= 1.0e16L;	/* adjust range */
rmsa += e * e;	/* accumulate the square of the error */
}

/* report after NTRIALS trials */
rms = 1.0e-16L * sqrtl( rmsa/m );
da = max;
db = rms;
if(d->ctrl & RELERR)
	printf(" max = %.2E   rms = %.2E\n", da, db );
else
	printf(" max = %.2E A rms = %.2E A\n", da, db );
} /* loop on itst */
exit(0);
}

