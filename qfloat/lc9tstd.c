/*							cltstd.c	*/
/*  Complex variable function test routine.
 *  Requires long double type check routine and double precision function
 *  under test.  Indicate function name and range in #define statements
 *  below.  Modifications for two argument functions and absolute
 *  rather than relative accuracy report are indicated.
 */

#include <stdio.h>
#include <complex.h>
/* int printf(), gets(), sscanf(); */

#include "mconf.h"
#ifndef ANSIPROT
int drand();
#endif
void dprec(), ldprec();

long double cabsl(long double complex);
long double fabsl(long double);
long double expl(long double);
long double sqrtl(long double);

/* define if relative error, don't define if absolute error: */
#define RELERR 1
#define ONEARG 1
#define ONEINT 0
#define TWOARG 0
#define TWOINT 0
#define THREEARG 0
#define THREEINT 0
#define FOURARG 0
#define VECARG 0
#define FOURANS 0
#define TWOANS 0
#define PROB 0
#define EXPSCALE 0
#define EXPSC2 0
#define SETFPU 1
/* insert function to be tested here: */
#define FUNC csqrt
double complex FUNC();
#define QFUNC csqrtl
long double complex QFUNC();
/*extern int aiconf;*/

extern double MAXLOG;
extern double MINLOG;
extern double MAXNUM;
#define LTS 3.258096538
/* insert low end and width of test interval */
#define LOW -10.0
#define WIDTH 20.0
#define LOWA  0.01
#define WIDTHA 0.49
/* 1.073741824e9 */
/* 2.147483648e9 */
long double qone = 1.0L;
static long double complex q1, q2, q3, qa, qb, qc, qz, qy1, qy2, qy3, qy4;
static double complex y2, y3, y4, a, b, c, x, y, z;
static double yr, yi, e;
static long double qe, qmax, qrmsa, qave, qt;
volatile double v;
static long double lp[3], lq[3];
static double dp[3], dq[3];

char strave[20];
char strrms[20];
char strmax[20];
double underthresh =  2.22507385850720138309E-308; /* 2^-1022 */
long double sqrtl(), fabsl();
double tr, ti;

int main()
{
char s[80];
double fabs(), floor();
#if EXPSCALE || EXPSC2
double exp();
#endif
double sqrt();	/* required to compute rms error */
int i;
long m, n;

merror = 0;
#if SETFPU
ldprec();   /* set up coprocessor.  */
#endif
/*aiconf = -1;*/	/* configure Airy function */
x = 1.0;
z = x * x;
qmax = 0.0L;
sprintf(strmax, "%.4Le", qmax );
qrmsa = 0.0L;
qave = 0.0L;

#if 1
printf(" Start at random number #:" );
gets( s );
sscanf( s, "%ld", &n );
printf("%ld\n", n );
#else
n = 0;
#endif

for( m=0; m<n; m++ )
	drand( &tr );
n = 0;
m = 0;
tr = floor( tr );

loop:

for( i=0; i<500; i++ )
{
n++;
m++;

#if SETFPU
ldprec();	/* set up floating point coprocessor */
#endif

#if ONEARG || TWOARG || THREEARG || FOURARG
/* make random number in desired range */
drand( &tr );
tr = WIDTH *  ( tr - 1.0 )  +  LOW;
#if EXPSCALE
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
v = tr;
x = v;
q1 = v;

drand( &tr );
tr = WIDTH *  ( tr - 1.0 )  +  LOW;
#if EXPSCALE
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
v = tr;
x = x + I * v;
q1 = q1 + I * v;
#if ONEINT
k = x;
x = k;
#endif
#endif

/* do again if second argument required */

#if TWOARG || THREEARG || FOURARG
drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
/*a /= 50.0;*/
#if EXPSC2
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
#if TWOINT || THREEINT
k = a + 0.25;
a = k;
#endif
v = tr;
a = v;
qy4 = v;

drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
/*a /= 50.0;*/
#if EXPSC2
tr = exp(tr);
drand( &ti );
tr -= 1.0e-13 * tr * ti;
#endif
#if TWOINT || THREEINT
k = a + 0.25;
a = k;
#endif
v = tr;
a = a + I * v;
qy4 = qy4 + I * v;
#endif

#if THREEARG || FOURARG
drand( &tr );
#if PROB
/*
b = b - 1.0;
b = a * b;
*/
#if 1
/* This makes b <= a, for bdtr.  */
a = (a - LOWA) *  ( b - 1.0 )  +  LOWA;
if( b > 1.0 && a > 1.0 )
  b -= 1.0;
else
  {
    a += 1.0;
    k = a;
    a = k;
    v = a;
    qy4 = v;
  }
#else /* not 1 */
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
v = tr;
b = v
#endif /* not 1 */

/* Half-integer a and b */
/*
a = 0.5*floor(2.0*a+1.0);
b = 0.5*floor(2.0*b+1.0);
*/
v = a;
qy4 = v;
/*x = (a / (a+b));*/

#else /* not PROB */
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
#endif /* not PROB */
#if THREEINT
j = b + 0.25;
b = j;
#endif
v = tr;
b = v;
qb = v;

ti = WIDTHA *  ( ti - 1.0 )  +  LOWA;
v = ti;
b = b + I * v;
qb = qb + I * v;
#endif

#if FOURARG
drand( &tr );
tr = WIDTHA *  ( tr - 1.0 )  +  LOWA;
/* for hyp2f1 to ensure c-a-b > -1 */
/*
z = c-a-b;
if( z < -1.0 )
	c -= 1.6 * z;
*/
v = tr;
c = v;
qc = v;

drand( &ti );
ti = WIDTHA *  ( ti - 1.0 )  +  LOWA;
v = ti;
c = c + I * v;
qc = qc + I * v;

#endif

#if VECARG
for( j=0; j<3; j++)
  {
    drand( &x );
    x = WIDTH *  ( x - 1.0 )  +  LOW;
    v = x;
    dp[j] = v;
    q1 = v;		/* double number to q type */
    lp[j] = q1;
    drand( &x );
    x = WIDTH *  ( x - 1.0 )  +  LOW;
    v = x;
    dq[j] = v;
    q1 = v;		/* double number to q type */
    lq[j] = q1;
  }
#endif /* VECARG */

/*printf("%.16E %.16E\n", a, x);*/
/* Compute function under test
   with math coprocessor set to double precision. */
#if SETFPU
dprec();
#endif

#if ONEARG
#if FOURANS
/*FUNC( x, &z, &y2, &y3, &y4 );*/
FUNC( x, &y4, &y2, &y3, &z );
#else
#if TWOANS
FUNC( x, &z, &y2 );
/*FUNC( x, &y2, &z );*/
#else
#if ONEINT
z = FUNC( k );
#else
z = FUNC( x );
#endif
#endif
#endif
#endif

#if TWOARG
#if TWOINT
z = FUNC( k, x );
/*z = FUNC( x, k );*/
/*z = FUNC( a, x );*/
#else
#if FOURANS
FUNC( a, x, &z, &y2, &y3, &y4 );
#else
z = FUNC( a, x );
#endif
#endif
#endif

#if THREEARG
#if THREEINT
z = FUNC( j, k, x );
#else
z = FUNC( a, b, x );
#endif
#endif

#if FOURARG
z = FUNC( a, b, c, x );
#endif

#if VECARG
z = FUNC( dp, dq );
#endif

yr = creal(z);
yi = cimag(z);
q2 = yr + I * yi;

/* handle detected overflow */
if (cabs(z) == MAXNUM)
	{
	printf("detected overflow ");
#if 0
#if FOURARG
	printf("%.4E %.4E %.4E %.4E %.4E %6ld \n",
		a, b, c, x, y, n);
#else
	printf("%.16E %.4E %.4E %6ld \n", x, a, z, n);
#endif
#endif
	e = 0.0;
	m -= 1;
	goto endlup;
	}
/* Skip high precision if underflow.  */
if( merror == UNDERFLOW )
  goto underf;

/* Compute high precision function
   with math coprocessor set to long double precision.  */
#if SETFPU
ldprec();
#endif

#if ONEARG
#if FOURANS
/*qy4 = QFUNC( q1, qz, qy2, qy3 );*/
qz = QFUNC( q1, qy4, qy2, qy3 );
#else
#if TWOANS
qy2 = QFUNC( q1, qz );
/*qz = QFUNC( q1, qy2 );*/
#else
/* qy4 = 0.0L;*/
/* qy4 = 1.0L;*/
/*qz = QFUNC( qy4, q1 );*/
/*qz = QFUNC( 1, q1 );*/
qz = QFUNC( q1 );  /* normal */
#endif
#endif
#endif

#if TWOARG
#if TWOINT
qz = QFUNC( k, q1 );
/*qz = QFUNC( q1, qy4 );*/
/*qz = QFUNC( qy4, q1 );*/
#else
#if FOURANS
qc = QFUNC( qy4, q1, qz, qy2, qy3 );
#else
/*qy4 = 0.0L;;*/
/*qy4 = 1.0L );*/
qz = QFUNC( qy4, q1 );
#endif
#endif
#endif

#if THREEARG
#if THREEINT
qz = QFUNC( j, k, q1 );
#else
qz = QFUNC( qy4, qb, q1 );
#endif
#endif

#if FOURARG
qz = QFUNC( qy4, qb, qc, q1 );
#endif

#if VECARG
qz = QFUNC( lp, lq );
#endif

yr = creall(qz);
yi = cimagl(qz);
y = yr + yi * I; /* correct answer, in double precision */

/* get absolute error, in extended precision */
qe = cabsl(q2 - qz);
e = qe; /* the error in double precision */

/*  handle function result equal to zero
    or underflowed. */
if( qz == 0.0L || merror == UNDERFLOW || cabs(z) < underthresh )
	{
underf:
	  merror = 0;
/* Don't bother to print anything.  */
	printf("ans 0 ");
#if ONEARG
	printf("%.3e,%.3e %.3e,%.3e s.b. %.3e %.3e, %.3e %6ld \n",
	       creal(x), cimag(x), creal(z), cimag(z), yr, yi, e, n);
#else
#if THREEARG
	printf(
       "(%.1e,%.1e) (%.1e,%.1e) (%.1e,%.1e) (%.1e,%.1e) %.3e %6ld \n",
	       creal(a), cimag(a), creal(b), creal(b),
	       creal(x), cimag(x),
	       creal(y), cimag(y), e, n);
#else
	printf("(%.1e,%.1e) (%.1e,%.1e) (%.1e,%.1e) %.3e %6ld \n",
	       creal(a), cimag(a), creal(x), cimag(x), creal(y),
	       cimag(y), e, n);
#endif
#endif
	qe = 0.0L;
	e = 0.0;
	m -= 1;
	goto endlup;
	}

else

/*	relative error	*/

/* comment out the following two lines if absolute accuracy report */

#if RELERR
  qe = qe / cabsl(qz);
#else
	{
	  q2 = qz;
	  qt = cabsl(q2);
	  if( qt > 1.0L )
	    qe = qe / qt;
	}
#endif

qave = qave + qe;
/* absolute value of error */
qe = fabsl(qe);

/* peak detect the error */
if( qe > qmax )
	{
	  qmax = qe;
	  sprintf(strmax, "%.4Le", qmax );
#if ONEARG
	  yr = creall(qz);
	  yi = cimagl(qz);
	printf("(%.2e,%.2e) (%.2e,%.2e) s.b. (%.2e %.2e), %s %6ld \n",
	       creal(x), cimag(x), creal(z), cimag(z), yr, yi, strmax, n);
#else
#if THREEARG
	printf(
       "(%.1f,%.1f) (%.1f,%.1f) (%.1f,%.1f) (%.1f,%.1f) %.3Le %6ld \n",
       creal(a), cimag(a), creal(b), creal(b), creal(x),
       cimag(x), creal(y), cimag(y), max, n);
#else
	printf("%.1e,%.1e %.1e,%.1e %.1e,%.1e %.3e %6ld \n",
	       creal(a), cimag(a), creal(x), cimag(x), creal(y),
	       cimag(y), max, n);
#endif
#endif
#if VECARG
	printf("%.8E %s %6ld \n", y, strmax, n);
#endif
	}

/* accumulate rms error	*/
/* rmsa += e * e;  accumulate the square of the error */
qt = qe * qe;
qrmsa = qrmsa + qt;
endlup:   ;
#if SETFPU
ldprec();
#endif
}

/* report every 500 trials */
/* rms = sqrt( rmsa/m ); */
qt = qrmsa / m;
qt = sqrtl(qt);
sprintf(strrms, "%.4Le", qt );

qt = qave / m;
sprintf(strave, "%.4Le", qt );
/*
printf("%6ld   max = %s   rms = %s  ave = %s \n", m, strmax, strrms, strave );
*/
printf("%6ld   max = %s   rms = %s  ave = %s \r", m, strmax, strrms, strave );
fflush(stdout);
goto loop;
}
