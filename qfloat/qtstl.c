/*							qtstl.c		*/
/*  Function test routine.
 *  Requires q type check routine and double precision function under
 *  test.  Indicate function name and range in #define statements
 *  below.  Modifications for two argument functions and absolute
 *  rather than relative accuracy report are indicated.
 */

#include <stdio.h>

int qtoasc(), ldrand();
long double fabsl(long double);

#include "mconf.h"
#include "qhead.h"

int e64toq(), qtoe64();
#define ETOQ e64toq
#define QTOE qtoe64
/*
int e113toq(), qtoe113();
#define ETOQ e113toq
#define QTOE qtoe113
*/
/* define if relative error, don't define if absolute error: */
#define RELERR 1
#define ONEARG 1
#define ONEINT 0
#define TWOARG 0
#define TWOINT 0
#define THREEARG 0
#define THREEINT 0
#define FOURARG 0
#define FOURANS 0
#define TWOANS 0
#define PROB 0
#define EXPSCALE 0
#define EXPSC2 0
/* insert function to be tested here: */
#define FUNC log1pl
long double FUNC();
/* int FUNC(); */
#define QFUNC qlog1
int QFUNC();
/*extern int aiconf;*/

extern long double MAXLOGL;
extern long double MINLOGL;
extern long double MAXNUML;
#define LTS 3.258096538
/* insert low end and width of test interval */
#define LOW 0.5L
#define WIDTH 1.5L
#define LOWA  0.0L
#define WIDTHA 30.0L
/* 1.073741824e9 */
/* 2.147483648e9 */
extern QELT qone[], qzero[];
QELT q[NQ];
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
static long double y2, y3, y4;
static long double a, b, c, x, y, z, e;
/* static long double max, rms, ave; */
static QELT qe[NQ];
static QELT qmax[NQ];
static QELT qrmsa[NQ];
/* static QELT qrms[NQ]; */
static QELT qave[NQ];

char strave[20];
char strrms[20];
char strmax[20];
char str[80];

long double floorl();
long double underthresh =   3.362103143112093506263E-4932L;
#if FOURANS
long double qmaxes[4][NQ], qrmses[4][NQ], qaves[4][NQ];
#endif

void main()
{
char s[80];
#if EXPSCALE || EXPSC2
long double expl();
#endif
int i, j, k;
long m, n;

#if 0
einit();	/* set up floating point coprocessor */
#endif
merror = 0;
/*aiconf = -1;*/	/* configure Airy function */
x = 1.0L;
z = x * x;
qclear( qmax );
qtoasc( qmax, strmax, 4 );
qclear( qrmsa );
qclear( qave );
#if FOURANS
for(m=0; m<4; m++)
  {
    qclear(qmaxes[m]);
    qclear(qaves[m]);
    qclear(qrmses[m]);
  }
#endif
printf(" Start at random number #:" );
gets( s );
sscanf( s, "%ld", &n );
printf("%ld\n", n );
for( m=0; m<n; m++ )
	ldrand( &x );

n = 0;
m = 0;
/* x = floorl( ave ); */

loop:

for( i=0; i<500; i++ )
{
n++;
m++;

/* make random number in desired range */
ldrand( &x );
x = WIDTH *  ( x - 1.0L )  +  LOW;
#if EXPSCALE
x = expl(x);
ldrand( &a );
a = 1.0e-13L * x * a;
if( x > 0.0 )
	x -= a;
else
	x += a;
#endif
#if ONEINT
k = x;
x = k;
#endif
ETOQ( &x, q1 );		/* double number to q type */

/* do again if second argument required */

#if TWOARG || THREEARG || FOURARG
ldrand( &a );
a = WIDTHA *  ( a - 1.0 )  +  LOWA;
/*a /= 50.0;*/
#if EXPSC2
a = expl(a);
ldrand( &y2 );
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
ETOQ( &a, qy4 );
#endif


#if THREEARG || FOURARG
ldrand( &b );
#if PROB
/*
b = b - 1.0;
b = a * b;
*/
#if 0
/* This makes b <= a, for bdtr.  */
b = (a - LOWA) *  ( b - 1.0 )  +  LOWA;
if( b > 1.0 && a > 1.0 )
  b -= 1.0;
#else
b = WIDTHA *  ( b - 1.0 )  +  LOWA;
#endif
 /* for F distribution */

x = (b / a) * x;
ETOQ( &x, q1 );

/* Half-integer a and b */
/*
a = 0.5L*floorl(2.0L*a+1.0L);
b = 0.5L*floorl(2.0L*b+1.0L);
ETOQ( &a, qy4 );
*/
#else
b = WIDTHA *  ( b - 1.0 )  +  LOWA;
#endif
#if THREEINT
j = b + 0.25;
b = j;
#endif
ETOQ( &b, qb );
#endif

#if FOURARG
ldrand( &c );
c = WIDTHA *  ( c - 1.0 )  +  LOWA;
/* for hyp2f1 to ensure c-a-b > -1 */
/*
z = c-a-b;
if( z < -1.0 )
	c -= 1.6 * z;
*/
ETOQ( &c, qc );
#endif

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
z = FUNC( k, x );
/*z = FUNC( x, k );*/
/*z = FUNC( a, x );*/
#else
#if FOURANS
/*FUNC( a, x, &z, &y2, &y3, &y4 );*/
FUNC( a, x, &y2, &y3, &y4, &z );
#else
z = FUNC( a, x );
#endif
#endif
#endif

#if THREEARG
#if THREEINT
z = FUNC( j, k, x );
/*z = FUNC( k, j, x );*/
#else
z = FUNC( a, b, x );
#endif
#endif

#if FOURARG
z = FUNC( a, b, c, x );
#endif

ETOQ( &z, q2 );

/* handle detected overflow */
if( (z == MAXNUML) || (z == -MAXNUML) )
	{
	printf("detected overflow ");
#if FOURARG
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &b, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &c, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &x, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &y, q );
	qtoasc( q, str, 8 );
	printf( "%6ld\n", n );
#else
	ETOQ( &x, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &z, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	printf( "%6ld\n", n );
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
/*QFUNC( qy4, q1, qz );*/ /* for Bessel functions */
/*QFUNC( 1, q1, qz );*/
QFUNC( q1, qz );  /* normal */
#endif
#endif
#endif

#if TWOARG
#if TWOINT
QFUNC( k, q1, qz );
/*QFUNC( q1, qy4, qz );*/
/*QFUNC( qy4, q1, qz );*/
#else
#if FOURANS
/*QFUNC( qy4, q1, qz, qy2, qy3, qc );*/
QFUNC( qy4, q1, qy2, qy3, qc, qz );
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
/*QFUNC( k, j, q1, qz );*/
#else
QFUNC( qy4, qb, q1, qz );
#endif
#endif

#if FOURARG
QFUNC( qy4, qb, qc, q1, qz );
#endif

QTOE( qz, &y ); /* correct answer, in double precision */


/* get absolute error, in extended precision */
qsub( qz, q2, qe );
QTOE( qe, &e );		/* the error in double precision */

/*  handle function result equal to zero
    or underflowed. */
if( qz[1] < 3 || merror == UNDERFLOW || fabsl(z) < underthresh )
	{
underf:
	  merror = 0;
/* Don't bother to print anything.  */
#if 0
	printf("ans 0 ");
#if TWOARG
#if TWOINT
	printf( "%d ", k );
#else
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
#endif
#endif

#if THREEARG
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &b, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
#endif

#if FOURARG
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &b, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &c, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
#endif

	ETOQ( &x, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &y, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &e, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	printf( "%6ld", n );
#endif /* 0 */
	qclear( qe );
	e = 0.0;
	m -= 1;
	goto endlup;
	}
else
#if RELERR
  {
	qdiv( qz, qe, qe );
      }
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
#if TWOARG
#if TWOINT
	printf( "%d ", k );

#else
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
#endif
#endif
#if THREEARG
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &b, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
#endif
#if FOURARG
	ETOQ( &a, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &b, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &c, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
#endif
	ETOQ( &x, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	ETOQ( &y, q );
	qtoasc( q, str, 8 );
	printf( "%s ", str );
	printf( "%s %6ld\n", strmax, n );
	}

/* accumulate rms error	*/
/* rmsa += e * e;  accumulate the square of the error */
qmul( qe, qe, q2 );
qadd( q2, qrmsa, qrmsa );
endlup:
	;
}

/* report every 500 trials */
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
if( m < 100000L )
     goto loop;
printf("\n");
}

#if 0
ldrand(x)
long double *x;
{
double ds1, ds2, ds3;
long double ls;

drand( &ds1 );
drand( &ds2 );
drand( &ds3 );
ds2 -= 1.0;
ds3 -= 1.0;
ls = (2.3e-16L * (long double) ds2
	+ (long double) ds2 ) * 2.3e-16L + (long double) ds1;
*x = ls;
}
#endif
