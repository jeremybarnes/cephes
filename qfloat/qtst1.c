/*							qtst1.c		*/
/*  Function test routine.
 *  Requires q type check routine and double precision function under
 *  test.  Indicate function name and range in #define statements
 *  below.  Modifications for two argument functions and absolute
 *  rather than relative accuracy report are indicated.
 */

#include <stdio.h>
/* int printf(), gets(), sscanf(); */

#include "mconf.h"
#include "qhead.h"
int dprec(), drand();
int qtoasc();

/* define if relative error, don't define if absolute error: */
#define RELERR 1
#define ONEARG 0
#define ONEINT 0
#define TWOARG 0
#define TWOINT 0
#define THREEARG 1
#define THREEINT 0
#define FOURARG 0
#define FOURANS 0
#define TWOANS 0
#define PROB 0
#define EXPSCALE 0
#define EXPSC2 0
/* insert function to be tested here: */
#define FUNC hyperg
double FUNC();
#define QFUNC qhyp
int QFUNC();
/*extern int aiconf;*/

extern double MAXLOG;
extern double MINLOG;
extern double MAXNUM;
#define LTS 3.258096538
/* insert low end and width of test interval */
#define LOW 0.0
#define WIDTH 30.0
#define LOWA  0.0
#define WIDTHA 30.0
/* 1.073741824e9 */
/* 2.147483648e9 */
extern QELT qone[];
QELT q1[NQ];
QELT q2[NQ];
QELT q3[NQ];
QELT qa[NQ];
QELT qb[NQ];
QELT qc[NQ];
QELT qz[NQ];
QELT qy1[NQ];
QELT qy2[NQ];
QELT qy3[NQ];
QELT qy4[NQ];
static double y2;
static double y3;
static double y4;
static double a;
static double b;
static double c;
static double x;
static double y;
static double z;
static double e;
static QELT qe[NQ];
static QELT qmax[NQ];
static QELT qrmsa[NQ];
static QELT qave[NQ];

char strave[20];
char strrms[20];
char strmax[20];
double underthresh =  2.22507385850720138309E-308; /* 2^-1022 */


main()
{
char s[80];
double fabs(), floor();
#if EXPSCALE || EXPSC2
double exp();
#endif
double sqrt();	/* required to compute rms error */
int i, j, k;
long m, n;

dprec();	/* set up floating point coprocessor */
merror = 0;
/*aiconf = -1;*/	/* configure Airy function */
x = 1.0;
z = x * x;
qclear( qmax );
qtoasc( qmax, strmax, 4 );
qclear( qrmsa );
qclear( qave );

#if 1
printf(" Start at random number #:" );
gets( s );
sscanf( s, "%ld", &n );
printf("%ld\n", n );
#else
n = 0;
#endif

for( m=0; m<n; m++ )
	drand( &x );
n = 0;
m = 0;
x = floor( x );

loop:

for( i=0; i<100; i++ )
{
n++;
m++;

/* make random number in desired range */
drand( &x );
x = WIDTH *  ( x - 1.0 )  +  LOW;
#if EXPSCALE
x = exp(x);
drand( &a );
a = 1.0e-13 * x * a;
if( x > 0.0 )
	x -= a;
else
	x += a;
#endif
#if ONEINT
k = x;
x = k;
#endif
etoq( &x, q1 );		/* double number to q type */

/* do again if second argument required */

#if TWOARG || THREEARG || FOURARG
drand( &a );
a = WIDTHA *  ( a - 1.0 )  +  LOWA;
/*a /= 50.0;*/
#if EXPSC2
a = exp(a);
drand( &y2 );
y2 = 1.0e-13 * y2 * a;
if( a > 0.0 )
	a -= y2;
else
	a += y2;
#endif
#if TWOINT || THREEINT
k = a + 0.25;
a = k;
#endif
etoq( &a, qy4 );
#endif


#if THREEARG || FOURARG
drand( &b );
#if PROB
/*
b = b - 1.0;
b = a * b;
*/
b = WIDTHA *  ( b - 1.0 )  +  LOWA;

/* Half-integer a and b */
/*a = 0.5*floor(2.0*a+1.0);*/
a = 0.5;
b = 0.5*floor(2.0*b+1.0);
etoq( &a, qy4 );
/*x = (a / (a+b));*/

#else
b = WIDTHA *  ( b - 1.0 )  +  LOWA;
#endif
#if THREEINT
j = b + 0.25;
b = j;
#endif
etoq( &b, qb );
#endif

#if FOURARG
drand( &c );
c = WIDTHA *  ( c - 1.0 )  +  LOWA;
/* for hyp2f1 to ensure c-a-b > -1 */
/*
z = c-a-b;
if( z < -1.0 )
	c -= 1.6 * z;
*/
etoq( &c, qc );
#endif

/*printf("%.16E %.16E\n", a, x);*/

/* compute function under test */
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
/*z = FUNC( k, x );*/
/*z = FUNC( x, k );*/
z = FUNC( a, x );
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

etoq( &z, q2 );

/* handle detected overflow */
if( (z == MAXNUM) || (z == -MAXNUM) )
	{
	printf("detected overflow ");
#if FOURARG
	printf("%.4E %.4E %.4E %.4E %.4E %6ld \n",
		a, b, c, x, y, n);
#else
	printf("%.16E %.4E %.4E %6ld \n", x, a, z, n);
#endif
	e = 0.0;
	m -= 1;
	goto endlup;
	}
/* Skip high precision if underflow.  */
if( merror == UNDERFLOW )
  goto underf;

/* compute high precision function */
#if ONEARG
#if FOURANS
/*QFUNC( q1, qz, qy2, qy3, qy4 );*/
QFUNC( q1, qy4, qy2, qy3, qz );
#else
#if TWOANS
QFUNC( q1, qz, qy2 );
/*QFUNC( q1, qy2, qz );*/
#else
/*qclear( qy4 );*/
/*qmov( qone, qy4 );*/
/*QFUNC( qy4, q1, qz );*/
/*QFUNC( 1, q1, qz );*/
QFUNC( q1, qz );  /* normal */
#endif
#endif
#endif

#if TWOARG
#if TWOINT
/*QFUNC( k, q1, qz );*/
/*QFUNC( q1, qy4, qz );*/
QFUNC( qy4, q1, qz );
#else
#if FOURANS
QFUNC( qy4, q1, qz, qy2, qy3, qc );
#else
/*qclear( qy4 );*/
/*qmov( qone, qy4 );*/
QFUNC( qy4, q1, qz );
#endif
#endif
#endif

#if THREEARG
#if THREEINT
QFUNC( j, k, q1, qz );
#else
QFUNC( qy4, qb, q1, qz );
#endif
#endif

#if FOURARG
QFUNC( qy4, qb, qc, q1, qz );
#endif

qtoe( qz, &y ); /* correct answer, in double precision */


/* get absolute error, in extended precision */
qsub( qz, q2, qe );
qtoe( qe, &e );		/* the error in double precision */

/*  handle function result equal to zero
    or underflowed. */
if( qz[1] < 3 || merror == UNDERFLOW || fabs(z) < underthresh )
	{
underf:
	  merror = 0;
/* Don't bother to print anything.  */
#if 0
	printf("ans 0 ");
#if ONEARG
	printf("%.8E %.8E %.4E %6ld \n", x, y, e, n);
#endif

#if TWOARG
#if TWOINT
	printf("%d %.8E %.8E %.4E %6ld \n", k, x, y, e, n);
#else
	printf("%.6E %.6E %.6E %.4E %6ld \n", a, x, y, e, n);
#endif
#endif

#if THREEARG
	printf("%.6E %.6E %.6E %.6E %.4E %6ld \n", a, b, x, y, e, n);
#endif

#if FOURARG
	printf("%.4E %.4E %.4E %.4E %.4E %.4E %6ld \n",
		a, b, c, x, y, e, n);
#endif
#endif /* 0 */
	qclear( qe );
	e = 0.0;
	m -= 1;
	goto endlup;
	}

else

/*	relative error	*/

/* comment out the following two lines if absolute accuracy report */

#if RELERR
	qdiv( qz, qe, qe );
#else
	{
	qmov( qz, q2 );
	q2[0] = 0;
	if( qcmp( q2, qone ) > 0 )
		qdiv( qz, qe, qe );
	}
#endif

qadd( qave, qe, qave );
/* absolute value of error */
qe[0] = 0;

/* peak detect the error */
if( qcmp(qe, qmax) > 0 )
	{
	qmov( qe, qmax );
	qtoasc( qmax, strmax, 4 );
#if ONEARG
	printf("%.8E %.8E %s %6ld \n", x, y, strmax, n);
#endif
#if TWOARG
#if TWOINT
	printf("%d %.8E %.8E %s %6ld \n", k, x, y, strmax, n);
#else
	printf("%.6E %.6E %.6E %s %6ld \n", a, x, y, strmax, n);
#endif
#endif
#if THREEARG
	printf("%.6E %.6E %.6E %.6E %s %6ld \n", a, b, x, y, strmax, n);
#endif
#if FOURARG
	printf("%.4E %.4E %.4E %.4E %.4E %s %6ld \n",
		a, b, c, x, y, strmax, n);
#endif
	}

/* accumulate rms error	*/
/* rmsa += e * e;  accumulate the square of the error */
qmul( qe, qe, q2 );
qadd( q2, qrmsa, qrmsa );
endlup:
	;
}

/* report every 100 trials */
/* rms = sqrt( rmsa/m ); */
ltoq( &m, q1 );
qdiv( q1, qrmsa, q2 );
qsqrt( q2, q2 );
qtoasc( q2, strrms, 4 );

qdiv( q1, qave, q2 );
qtoasc( q2, strave, 4 );
/*
printf("%6ld   max = %s   rms = %s  ave = %s \n", m, strmax, strrms, strave );
*/
printf("%6ld   max = %s   rms = %s  ave = %s \r", m, strmax, strrms, strave );
fflush(stdout);
goto loop;
}
