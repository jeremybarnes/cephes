/*						qctst1.c */
/*  Function test routine, complex variable version.
 *  Requires q type check routine and double precision function under
 *  test.  Indicate function name and range in #define statements
 *  below.  Modifications for two argument functions and absolute
 *  rather than relative accuracy report are indicated.
 */
#include <stdio.h>
#include <complex.h>
#include "qhead.h"

/*
typedef struct
	{
	QELT r[NQ];
	QELT i[NQ];
	}qcmplx;
*/
/* define if relative error, don't define if absolute error: */
#define RELERR 0
#define ONEARG 1
#define REALANS 0
#define TWOARG 0
#define THREEARG 0
#define FOURARG 0
#define PROB 0
#define EXPSCALE 0
#define EXPSC2 0
/* insert function to be tested here: */
#define FUNC cgamma
#define QFUNC qcgamma
extern double MAXLOG;
extern double MAXNUM;
#define LTS 3.258096538
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

static double complex y2;
static double complex y3;
static double complex y4;
static double complex a;
static double complex b;
static double complex c;
static double complex x;
static double complex y;
static double complex z;
static double ya;
static double yr;
static double yi;
static double e;
static double max;
static double rmsa;
static double rms;
static double ave;

int
main()
{
char s[80];
double tr, ti;
int i, j, k;
long m, n;
double cabs();
double fabs();
double exp();
double sqrt();	/* required to compute rms error */
double complex FUNC();

#if 0
einit();
#endif
n = 0;
m = 0;
max = 0.0;
rmsa = 0.0;
ave = 0.0;
printf(" Start at random number #:" );
gets( s );
sscanf( s, "%d", &k );
for( i=0; i<k; i++ )
	drand( (double *) &x );

loop:

for( i=0; i<500; i++ )
{
n++;
m++;

/* make random number in desired range */
drand( &tr );
tr = WIDTH *  ( tr - 1.0 )  +  LOW;
#if EXPSCALE
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
x = tr;
etoq( &tr, q1.r );

drand( &tr );
tr = WIDTH *  ( tr - 1.0 )  +  LOW;
#if EXPSCALE
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
x = x + tr * I;
etoq( &tr, q1.i );

/* do again if second argument required */

#if TWOARG || THREEARG || FOURARG
drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
#if EXPSC2
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
a = tr;
etoq( &tr, qy4.r );

drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
#if EXPSC2
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
a = a + tr * I;
etoq( &tr, qy4.i );
#endif


#if THREEARG || FOURARG
drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
b = tr;
etoq( &tr, qb.r );

drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
b = b + tr * I;
etoq( &tr, qb.i );
#endif

#if FOURARG
drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
c = tr;
etoq( &tr, qc.r );
drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
c = c + tr * I;
etoq( &tr, qc.i );
#endif

/* compute function under test */
#if ONEARG
#if REALANS
ya = FUNC( x );
z = ya + 0.0 * I;
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

yr = creal(z);
yi = cimag(z);
etoq( &yr, q2.r );
etoq( &yi, q2.i );

/* handle detected overflow */
if( cabs(z) == MAXNUM )
	{
	printf("detected overflow ");
/*	printf("%.16E %.4E %.4E %6ld \n", x, a, z, n);*/
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

qtoe( qy1.r, &yr ); /* correct answer, in double precision */
qtoe( qy1.i, &yi );
y = yr + yi * I;


/*							qtst1.c 2	*/


/* get absolute error, in extended precision */
qcsub( &q2, &qy1, &q3 );
qtoe( q3.r, &yr );		/* the error in double precision */
qtoe( q3.i, &yi );
y3 = yr + yi * I;
e = cabs(y3);
/*  handle function result equal to zero */
ya = cabs(y);
if( ya == 0.0 )
	{
	printf("ans 0 ");
#if ONEARG
	qtoe( qy1.r, &yr );
	qtoe( qy1.i, &yi );
	printf("%.3E,%.3E %.3E,%.3E s.b. %.3E %.3E, %.3E %6ld \n",
	       creal(x), cimag(x), creal(z), cimag(z), yr, yi, e, n);
#else
#if THREEARG
	printf("(%.1E,%.1E) (%.1E,%.1E) (%.1E,%.1E) (%.1E,%.1E) %.3E %6ld \n",
	       creal(a), cimag(a), creal(b), creal(b), creal(x), cimag(x),
	       creal(y), cimag(y), e, n);
#else
	printf("(%.1E,%.1E) (%.1E,%.1E) (%.1E,%.1E) %.3E %6ld \n",
	       creal(a), cimag(a), creal(x), cimag(x), creal(y), cimag(y),
	       e, n);
#endif
#endif
	e = 0.0;
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
	if( ya > 1.0 )
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
	qtoe( qy1.r, &yr );
	qtoe( qy1.i, &yi );
	printf("(%.2E,%.2E) (%.2E,%.2E) s.b. (%.2E %.2E), %.3E %6ld \n",
	       creal(x), cimag(x), creal(z), cimag(z), yr, yi, max, n);
#else
	printf("(%.3E,%.3E) (%.3E,%.3E) %.3E %6ld \n",
	       creal(x), cimag(x), creal(z), cimag(z), max, n);
#endif /* 0 */
#else
#if THREEARG
	printf("(%.1f,%.1f) (%.1f,%.1f) (%.1f,%.1f) (%.1f,%.1f) %.3E %6ld \n",
	       creal(a), cimag(a), creal(b), creal(b), creal(x), cimag(x),
	       creal(y), cimag(y), max, n);
#else
	printf("%.1E,%.1E %.1E,%.1E %.1E,%.1E %.3E %6ld \n",
	       creal(a), cimag(a), creal(x), cimag(x), creal(y), cimag(y),
	       max, n);
#endif
#endif

	}

/* accumulate rms error	*/
e *= 1.0e16;	/* adjust range */
rmsa += e * e;	/* accumulate the square of the error */
endlup:
	;
}

/* report every 100 trials */
rms = 1.0e-16 * sqrt( rmsa/m );
printf("%6ld   max = %.4E   rms = %.4E  ave = %.4E \n", m, max, rms, ave/m );
goto loop;
}
