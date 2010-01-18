/*						qctstl.c */
/*  Function test routine, long double complex variable version.
 *  Requires q type check routine and double precision function under
 *  test.  Indicate function name and range in #define statements
 *  below.  Modifications for two argument functions and absolute
 *  rather than relative accuracy report are indicated.
 */
#include <stdio.h>
#include <complex.h>
#include "qhead.h"

#define QTOE qtoe64
#define ETOQ e64toq

long double cabsl(long double complex);
long double fabsl(long double);
long double expl(long double);
long double sqrtl(long double);

int qtoasc(), ldrand();

/*
typedef struct
	{
	QELT r[NQ];
	QELT i[NQ];
	}qcmplx;
*/
/* define if relative error, don't define if absolute error: */
#define RELERR 1
#define ONEARG 0
#define REALANS 0
#define TWOARG 1
#define THREEARG 0
#define FOURARG 0
#define PROB 0
#define EXPSCALE 0
#define EXPSC2 0
/* insert function to be tested here: */
#define FUNC cdivl
#if REALANS
long double FUNC();
#else
long double complex FUNC();
#endif
#define QFUNC qcdiv
extern long double MAXLOGL;
extern long double MAXNUML;
#define LTS 3.258096538L
/* insert low end and width of test interval */
#define LOW -10.0
#define WIDTH 20.0
#define LOWA 0.0
#define WIDTHA 50.0
/* 1.073741824e9 */
/* 2.147483648e9 */
extern QELT qcone[];
static qcmplx q1;
static qcmplx q2;
static qcmplx q3;
static qcmplx qa;
static qcmplx qb;
static qcmplx qc;
static qcmplx qz;
static qcmplx qy1;
static qcmplx qy2;
static qcmplx qy3;
static qcmplx qy4;
static QELT qya[NQ];

static long double complex y2;
static long double complex y3;
static long double complex y4;
static long double complex a;
static long double complex b;
static long double complex c;
static long double complex x;
static long double complex y;
static long double complex z;
static long double ya;
static long double yr;
static long double yi;
static long double e;
static long double max;
static long double rmsa;
static long double rms;
static long double ave;

int
main()
{
char s[80];
long double tr, ti;
int i, j, k;
long m, n;

#if 0
einit();
#endif
n = 0;
m = 0;
max = 0.0;
rmsa = 0.0;
ave = 0.0;

printf ("Testing FUNC = caddl\n");
printf ("LOW = %e, WIDTH = %e\n", LOW, WIDTH);

/*
printf(" Start at random number #:" );
gets( s );
sscanf( s, "%d", &k );
for( i=0; i<k; i++ )
	drand( (double *) &x );
*/

loop:

for( i=0; i<500; i++ )
{
n++;
m++;

/* make random number in desired range */
ldrand( &tr );
tr = WIDTH *  ( tr - 1.0L )  +  LOW;
#if EXPSCALE
tr = expl(tr);
ldrand( &ti );
tr -= 1.0e-13L * tr * ti;
#endif
x = tr;
ETOQ( &tr, q1.r );

ldrand( &tr );
tr = WIDTH *  ( tr - 1.0 )  +  LOW;
#if EXPSCALE
tr = expl(tr);
ldrand( &ti );
tr -= 1.0e-13L * tr * ti;
#endif
x = x + tr * I;
ETOQ( &tr, q1.i );

/* do again if second argument required */

#if TWOARG || THREEARG || FOURARG
ldrand( &tr );
tr = WIDTHA *  ( tr - 1.0L )  +  LOWA;
#if EXPSC2
tr = expl(tr);
ldrand( &ti );
tr -= 1.0e-13L * tr * ti;
#endif
a = tr;
ETOQ( &tr, qy4.r );

ldrand( &tr );
tr = WIDTHA *  ( tr - 1.0L )  +  LOWA;
#if EXPSC2
tr = expl(tr);
ldrand( &ti );
tr -= 1.0e-13L * tr * ti;
#endif
a = a + tr * I;
ETOQ( &tr, qy4.i );
#endif


#if THREEARG || FOURARG
ldrand( &tr );
tr = WIDTHA *  ( tr - 1.0L )  +  LOWA;
b = tr;
ETOQ( &tr, qb.r );

ldrand( &tr );
tr = WIDTHA *  ( tr - 1.0L )  +  LOWA;
b = b + tr * I;
ETOQ( &tr, qb.i );
#endif

#if FOURARG
ldrand( &tr );
tr = WIDTHA *  ( tr - 1.0L )  +  LOWA;
c = tr;
ETOQ( &tr, qc.r );
ldrand( &tr );
tr = WIDTHA *  ( tr - 1.0L )  +  LOWA;
c = c + tr * I;
ETOQ( &tr, qc.i );
#endif

/* compute function under test */
#if ONEARG
#if REALANS
ya = FUNC( x );
z = ya + 0.0L * I;
#else
z = FUNC( x );
#endif
#endif

#if TWOARG
z = FUNC( a, x );
#endif

#if THREEARG
z = FUNC( a, b, x );
#endif

#if FOURARG
z = FUNC( a, b, c, x );
#endif

yr = creall(z);
yi = cimagl(z);
ETOQ( &yr, q2.r );
ETOQ( &yi, q2.i );

/* handle detected overflow */
if( cabsl(z) == MAXNUML )
	{
	printf("detected overflow ");
/*	printf("%.16Le %.4Le %.4Le %6ld \n", x, a, z, n);*/
	e = 0.0;
	m -= 1;
	goto endlup;
	}

/* compute high precision function */
#if ONEARG
#if REALANS
QFUNC( &q1, qya  );
qclear( &qy1.i[0] );
qmov( qya, &qy1.r[0] );
#else
QFUNC( &q1, &qy1 );
#endif
#endif

#if TWOARG
QFUNC( &qy4, &q1, &qy1 );
#endif

#if THREEARG
QFUNC( &qy4, &qb, &q1, &qy1 );
#endif

#if FOURARG
QFUNC( &qy4, &qb, &qc, &q1, &qy1 );
#endif

QTOE( qy1.r, &yr ); /* correct answer, in double precision */
QTOE( qy1.i, &yi );
y = yr + yi * I;


/*							qtst1.c 2	*/


/* get absolute error, in extended precision */
qcsub( &q2, &qy1, &q3 );
QTOE( q3.r, &yr );		/* the error in double precision */
QTOE( q3.i, &yi );
y3 = yr + yi * I;
e = cabsl(y3);
/*  handle function result equal to zero */
ya = cabsl(y);
if( ya == 0.0L )
	{
	printf("ans 0 ");
#if ONEARG
	QTOE( qy1.r, &yr );
	QTOE( qy1.i, &yi );
	printf("%.3Le,%.3Le %.3Le,%.3Le s.b. %.3Le %.3Le, %.3Le %6ld \n",
	       creall(x), cimagl(x), creall(z), cimagl(z), yr, yi, e, n);
#else
#if THREEARG
	printf(
       "(%.1Le,%.1Le) (%.1Le,%.1Le) (%.1Le,%.1Le) (%.1Le,%.1Le) %.3Le %6ld \n",
	       creall(a), cimagl(a), creall(b), creall(b),
	       creall(x), cimagl(x),
	       creall(y), cimagl(y), e, n);
#else
	printf("(%.1Le,%.1Le) (%.1Le,%.1Le) (%.1Le,%.1Le) %.3Le %6ld \n",
	       creall(a), cimagl(a), creall(x), cimagl(x), creall(y),
	       cimagl(y), e, n);
#endif
#endif
	e = 0.0L;
	m -= 1;
	goto endlup;
	}

/*	relative error	*/

/* comment out the following two lines if absolute accuracy report */

else
#if RELERR
	e /= ya;
#else
	{
	if( ya > 1.0L )
		e /= ya;
	}
#endif

ave += e;
/* absolute value of error */
if( e < 0 )
	e = -e;

/* peak detect the error */
if( e > max )
	{
	max = e;
#if ONEARG
#if 1
	QTOE( qy1.r, &yr );
	QTOE( qy1.i, &yi );
	printf("(%.2Le,%.2Le) (%.2Le,%.2Le) s.b. (%.2Le %.2Le), %.3Le %6ld \n",
	       creall(x), cimagl(x), creall(z), cimagl(z), yr, yi, max, n);
#else
	printf("(%.3Le,%.3Le) (%.3Le,%.3Le) %.3Le %6ld \n",
	       creall(x), cimagl(x), creall(z), cimagl(z), max, n);
#endif /* 0 */
#else
#if THREEARG
	printf(
       "(%.1Lf,%.1Lf) (%.1Lf,%.1Lf) (%.1Lf,%.1Lf) (%.1Lf,%.1Lf) %.3Le %6ld \n",
       creall(a), cimagl(a), creall(b), creall(b), creall(x),
       cimagl(x), creall(y), cimagl(y), max, n);
#else
	printf("%.1Le,%.1Le %.1Le,%.1Le %.1Le,%.1Le %.3Le %6ld \n",
	       creall(a), cimagl(a), creall(x), cimagl(x), creall(y),
	       cimagl(y), max, n);
#endif
#endif

	}

/* accumulate rms error	*/
e *= 1.0e16L;	/* adjust range */
rmsa += e * e;	/* accumulate the square of the error */
endlup:
	;
fflush (stdout);
}

/* report every 100 trials */
rms = 1.0e-16L * sqrtl( rmsa/m );
printf("%6ld   max = %.4Le   rms = %.4Le  ave = %.4Le \n",
       m, max, rms, ave/m );
fflush (stdout);
goto loop;
}
