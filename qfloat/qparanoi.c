
/* paranoia.c
 *
 * This is an implementation of the PARANOIA program.  It substitutes
 * subroutine calls for ALL floating point arithmetic operations.
 * This permits you to substitute your own experimental versions of
 * arithmetic routines.
 *
 * This version of PARANOIA omits the display of division by zero
 * and also omits the test for extra precise subexpressions.
 * Otherwise it includes all the
 * tests of the 27 Jan 86 distribution, plus a few additional tests.
 *
 * Steve Moshier, 28 Oct 88
 */

#define DEBUG 0
#define NOSIGNAL 1

#include <stdio.h>
#include "qhead.h"

#define KEYBOARD 0

/* Data structure of floating point number */
#define FLOAT(x) QELT x[NQ] = {0}

/* Externally defined constant 1.0 */
extern QELT qone[];
#define ONE qone

/* Coprocessor initialization, defeat underflow trap */
#define FSETUP einit
einit()
{}

/* Note all arguments of operation subroutines are pointers. */
/* c = b + a */
#define add(a,b,c) qadd(a,b,c)
/* c = b - a */
#define sub(a,b,c) qsub(a,b,c)
/* c = b * a */
#define mul(a,b,c) qmul(a,b,c)
/* c = b / a */
#define div(a,b,c) qdiv(a,b,c)
/* 1 if a>b, 0 if a==b, -1 if a<b */
#define cmp(a,b) qcmp(a,b)
/* b = a */
#define mov(a,b) qmov(a,b)
/* a = -a */
#define neg(a) qneg(a)
/* a = 0 */
#define clear(a) qclear(a)

/*define FABS(x) x[NE-1] &= 0x7fff*/
#define FABS(x) qabs(x)
#define FLOOR(x,y) qfloor(x,y)
#define LOG(x,y) qlog(x,y)
#define POW(x,y,z) qpow(x,y,z)
#define SQRT(x,y) qsqrt(x,y)

/* x = &FLOAT input, i = &long integer part, f = &FLOAT fractional part */
#define FTOL(x,i,f) qifrac(x,i,f)

/* i = &long integer input, x = &FLOAT output */
#define LTOF(i,x) ltoq(i,x)

/* Convert FLOAT a to decimal ASCII string with b digits */
#define TOASC(a,b,c) qtoasc(a,b,c)


FLOAT(Radix);
FLOAT(BInvrse);
FLOAT(RadixD2);
FLOAT(BMinusU2);
/*Small floating point constants.*/
FLOAT(Zero);
FLOAT(Half);
FLOAT(One);
FLOAT(Two);
FLOAT(Three);
FLOAT(Four);
FLOAT(Five);
FLOAT(Six);
FLOAT(Eight);
FLOAT(Nine);
FLOAT(Ten);
FLOAT(TwentySeven);
FLOAT(ThirtyTwo);
FLOAT(TwoForty);
FLOAT(MinusOne );
FLOAT(OneAndHalf);

/*Integer constants*/
int NoTrials = 20; /*Number of tests for commutativity. */
#define False 0
#define True 1

/* Definitions for declared types 
	Guard == (Yes, No);
	Rounding == (Chopped, Rounded, Other);
	Message == packed array [1..40] of char;
	Class == (Flaw, Defect, Serious, Failure);
	  */
#define Yes 1
#define No  0
#define Chopped 2
#define Rounded 1
#define Other   0
#define Flaw    3
#define Defect  2
#define Serious 1
#define Failure 0

typedef int Guard, Rounding, Class;
typedef char Message;

/* Declarations of Variables */
FLOAT(AInvrse);
FLOAT(A1);
FLOAT(C);
FLOAT(CInvrse);
FLOAT(D);
FLOAT(FourD);
FLOAT(E0);
FLOAT(E1);
FLOAT(Exp2);
FLOAT(E3);
FLOAT(MinSqEr);
FLOAT(SqEr);
FLOAT(MaxSqEr);
FLOAT(E9);
FLOAT(Third);
FLOAT(F6);
FLOAT(F9);
FLOAT(H);
FLOAT(HInvrse);
FLOAT(StickyBit);
FLOAT(J);
FLOAT(MyZero);
FLOAT(Precision);
FLOAT(Q);
FLOAT(Q9);
FLOAT(R);
FLOAT(Random9);
FLOAT(T);
FLOAT(Underflow);
FLOAT(S);
FLOAT(OneUlp);
FLOAT(UfThold);
FLOAT(U1);
FLOAT(U2);
FLOAT(V);
FLOAT(V0);
FLOAT(V9);
FLOAT(W);
FLOAT(X);
FLOAT(X1);
FLOAT(X2);
FLOAT(X8);
FLOAT(Random1);
FLOAT(Y);
FLOAT(Y1);
FLOAT(Y2);
FLOAT(Random2);
FLOAT(Z);
FLOAT(PseudoZero);
FLOAT(Z1);
FLOAT(Z2);
FLOAT(Z9);
static FLOAT(t);
FLOAT(t2);
FLOAT(Sqarg);
int ErrCnt[4];
int fpecount;
int Milestone;
int PageNo;
int I, M, N, N1, stkflg;
Guard GMult, GDiv, GAddSub;
Rounding RMult, RDiv, RAddSub, RSqrt;
int Break, Done, NotMonot, Monot, Anomaly, IEEE;
int SqRWrng, UfNGrad;
int k, k2;
int Indx;
char ch[8];

long lngint;	/* intermediate for conversion between int and FLOAT */

/* Computed constants. */
/*U1  gap below 1.0, i.e, 1.0-U1 is next number below 1.0 */
/*U2  gap above 1.0, i.e, 1.0+U2 is next number above 1.0 */


show( x )
QELT *x;
{
int i;
char s[80];

TOASC( x, s, 70 );
printf( "\n%s\n", s );
for( i=0; i<NQ; i++ )
	{
#if WORDSIZE == 32
	printf( "%08x ", x[i] );
#else
	printf( "%04x ", x[i] & 0xffff );
#endif
	if( (i & 7) == 7 )
		printf( "\n" );
	}
printf( "\n" );
}

/* define NOSIGNAL */
#ifndef NOSIGNAL
#include <signal.h>
#endif
#include <setjmp.h>
jmp_buf ovfl_buf;
typedef int (*Sig_type)();
Sig_type sigsave;

/* Floating point exception receiver */
sigfpe()
{
fpecount++;
printf( "\n* * * FLOATING-POINT ERROR * * *\n" );
/* reinitialize the floating point unit */
FSETUP();
fflush(stdout);
if( sigsave )
	{
#ifndef NOSIGNAL
	signal( SIGFPE, sigsave );
#endif
	sigsave = 0;
	longjmp( ovfl_buf, 1 );
	}
#ifndef NOSIGNAL
abort();
#else
exit(0);
#endif
}


main()
{

/* Do coprocessor or other initializations */
FSETUP();

printf(
 "This version of paranoia omits test for extra precise subexpressions\n" );
printf( "and includes a few additional tests.\n" );

clear(Zero);
printf( "0 = " );
show( Zero );
mov( ONE, One);
printf( "1 = " );
show( One );
add( One, One, Two );
printf( "1+1 = " );
show( Two );
add( Two, One, Three );
add( Three, One, Four );
add( Four, One, Five );
add( Five, One, Six );
add( Four, Four, Eight );
mul( Three, Three, Nine );
add( Nine, One, Ten );
mul( Nine, Three, TwentySeven );
mul( Four, Eight, ThirtyTwo );
mul( Four, Five, t );
mul( t, Three, t );
mul( t, Four, TwoForty );
mov( One, MinusOne );
neg( MinusOne );
div( Two, One, Half );
add( One, Half, OneAndHalf );
ErrCnt[Failure] = 0;
ErrCnt[Serious] = 0;
ErrCnt[Defect] = 0;
ErrCnt[Flaw] = 0;
PageNo = 1;
#ifndef NOSIGNAL
signal( SIGFPE, sigfpe );
#endif
printf("Program is now RUNNING tests on small integers:\n");

add( Zero, Zero, t );
if( cmp( t, Zero ) != 0)
	{
	printf( "0+0 != 0\n" );
	ErrCnt[Failure] += 1;
	}
sub( One, One, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "1-1 != 0\n" );
	ErrCnt[Failure] += 1;
	}
if( cmp( One, Zero ) <= 0 )
	{
	printf( "1 <= 0\n" );
	ErrCnt[Failure] += 1;
	}
add( One, One, t );
if( cmp( t, Two ) != 0 )
	{
	printf( "1+1 != 2\n" );
	ErrCnt[Failure] += 1;
	}
mov( Zero, Z );
neg( Z );
FLOOR( Z, t );
if( cmp(Z,Zero) != 0 )
	{
	ErrCnt[Serious] += 1;
	printf( "FLOOR(0) should equal 0, is = " );
	show( t );
	}
if( cmp(Z, Zero) != 0)
	{
	ErrCnt[Failure] += 1;
	printf("Comparison alleges that -0.0 is Non-zero!\n");
	}
else
	{
	div( TwoForty, One, U1 ); /* U1 = 0.001 */
	mov( One, Radix );
	TstPtUf();
	}
add( Two, One, t );
if( cmp( t, Three ) != 0 )
	{
	printf( "2+1 != 3\n" );
	ErrCnt[Failure] += 1;
	}
add( Three, One, t );
if( cmp( t, Four ) != 0 )
	{
	printf( "3+1 != 4\n" );
	ErrCnt[Failure] += 1;
	}
mov( Two, t );
neg( t );
mul( Two, t, t );
add( Four, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "4+2*(-2) != 0\n" );
	ErrCnt[Failure] += 1;
	}
sub( Three, Four, t );
sub( One, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "4-3-1 != 0\n" );
	ErrCnt[Failure] += 1;
	}
	sub( One, Zero, t );
if( cmp( t, MinusOne ) != 0 )
	{
	printf( "-1 != 0-1\n" );
	ErrCnt[Failure] += 1;
	}
add( One, MinusOne, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "1+(-1) != 0\n" );
	ErrCnt[Failure] += 1;
	}
mov( One, t );
FABS( t );
add( MinusOne, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "-1+abs(1) != 0\n" );
	ErrCnt[Failure] += 1;
	}
mul( MinusOne, MinusOne, t );
add( MinusOne, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "-1+(-1)*(-1) != 0\n" );
	ErrCnt[Failure] += 1;
	}
add( Half, MinusOne, t );
add( Half, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "1/2 + (-1) + 1/2 != 0\n" );
	ErrCnt[Failure] += 1;
	}
Milestone = 10;
mul( Three, Three, t );
if( cmp( t, Nine ) != 0 )
	{
	printf( "3*3 != 9\n" );
	ErrCnt[Failure] += 1;
	}
mul( Nine, Three, t );
if( cmp( t, TwentySeven ) != 0 )
	{
	printf( "3*9 != 27\n" );
	ErrCnt[Failure] += 1;
	}
add( Four, Four, t );
if( cmp( t, Eight ) != 0 )
	{
	printf( "4+4 != 8\n" );
	ErrCnt[Failure] += 1;
	}
mul( Eight, Four, t );
if( cmp( t, ThirtyTwo ) != 0 )
	{
	printf( "8*4 != 32\n" );
	ErrCnt[Failure] += 1;
	}
sub( TwentySeven, ThirtyTwo, t );
sub( Four, t, t );
sub( One, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "32-27-4-1 != 0\n" );
	ErrCnt[Failure] += 1;
	}
add( Four, One, t );
if( cmp( t, Five ) != 0 )
	{
	printf( "4+1 != 5\n" );
	ErrCnt[Failure] += 1;
	}
mul( Four, Five, t );
mul( Three, t, t );
mul( Four, t, t );
if( cmp( t, TwoForty ) != 0 )
	{
	printf( "4*5*3*4 != 240\n" );
	ErrCnt[Failure] += 1;
	}
div( Three, TwoForty, t );
mul( Four, Four, t2 );
mul( Five, t2, t2 );
sub( t2, t2, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "240/3 - 4*4*5 != 0\n" );
	ErrCnt[Failure] += 1;
	}
div( Four, TwoForty, t );
mul( Five, Three, t2 );
mul( Four, t2, t2 );
sub( t2, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "240/4 - 5*3*4 != 0\n" );
	ErrCnt[Failure] += 1;
	}
div( Five, TwoForty, t );
mul( Four, Three, t2 );
mul( Four, t2, t2 );
sub( t2, t, t );
if( cmp( t, Zero ) != 0 )
	{
	printf( "240/5 - 4*3*4 != 0\n" );
	ErrCnt[Failure] += 1;
	}
if(ErrCnt[Failure] == 0)
	{
printf("-1, 0, 1/2, 1, 2, 3, 4, 5, 9, 27, 32 & 240 are O.K.\n\n");
	}
printf("Searching for Radix and Precision.\n");
mov( One, W );
do
	{
	add( W, W, W );
	add( W, One, Y );
	sub( W, Y, Z );
	sub( One, Z, Y );
	mov( Y, t );
	FABS(t);
	add( MinusOne, t, t );
	k = cmp( t, Zero );
	}
while( k < 0 );
/*.. now W is just big enough that |((W+1)-W)-1| >= 1 ...*/
mov( Zero, Precision );
mov( One, Y );
do
	{
	add( W, Y, Radix );
	add( Y, Y, Y );
	sub( W, Radix, Radix );
	k = cmp( Radix, Zero );
	}
while( k == 0);

if( cmp(Radix, Two) < 0 )
	mov( One, Radix );
printf("Radix = " );
show( Radix );
if( cmp(Radix, One) != 0)
	{
	mov( One, W );
	do
		{
		add( One, Precision, Precision );
		mul( W, Radix, W );
		add( W, One, Y );
		sub( W, Y, t );
		k = cmp( t, One );
		}
	while( k == 0 );
	}
/* now W == Radix^Precision is barely too big to satisfy (W+1)-W == 1 */
div( W, One, U1 );
mul( Radix, U1, U2 );
printf( "Closest relative separation found is U 1 = " );
show( U1 );
printf( "Recalculating radix and precision." );
	
/*save old values*/
mov( Radix, E0 );
mov( U1, E1 );
mov( U2, E9 );
mov( Precision, E3 );
	
div( Three, Four, X );
sub( One, X, Third );
sub( Third, Half, F6 );
add( F6, F6, X );
sub( Third, X, X );
FABS( X );
if( cmp(X, U2) < 0 )
	mov( U2, X );
	
/*... now X = (unknown no.) ulps of 1+...*/
do
	{
	mov( X, U2 );
/* Y = Half * U2 + ThirtyTwo * U2 * U2; */
	mul( ThirtyTwo, U2, t );
	mul( t, U2, t );
	mul( Half, U2, Y );
	add( t, Y, Y );
	add( One, Y, Y );
	sub( One, Y, X );
	k = cmp( U2, X );
	k2 = cmp( X, Zero );
	}
while ( ! ((k <= 0) || (k2 <= 0)));
	
/*... now U2 == 1 ulp of 1 + ... */
div( Three, Two, X );
sub( Half, X, F6 );
add( F6, F6, Third );
sub( Half, Third, X );
add( F6, X, X );
FABS( X );
if( cmp(X, U1) < 0 )
	mov( U1, X );
	
/*... now  X == (unknown no.) ulps of 1 -... */
do
	{
	mov( X, U1 );
 /* Y = Half * U1 + ThirtyTwo * U1 * U1;*/
	mul( ThirtyTwo, U1, t );
	mul( U1, t, t );
	mul( Half, U1, Y );
	add( t, Y, Y );
	sub( Y, Half, Y );
	add( Half, Y, X );
	sub( X, Half, Y );
	add( Half, Y, X );
	k = cmp( U1, X );
	k2 = cmp( X, Zero );
	} while ( ! ((k <= 0) || (k2 <= 0)));
/*... now U1 == 1 ulp of 1 - ... */
if( cmp( U1, E1 ) == 0 )
	printf("confirms closest relative separation U1 .\n");
else
	{
	printf("gets better closest relative separation U1 = " );
	show( U1 );
	}
div( U1, One, W );
sub( U1, Half, F9 );
add( F9, Half, F9 );
div( U1, U2, t );
div( TwoForty, One, t2 );
add( t2, t, t );
FLOOR( t, Radix );
if( cmp(Radix, E0) == 0 )
	printf("Radix confirmed.\n");
else
	{
	printf("MYSTERY: recalculated Radix = " );
	show( Radix );
	mov( E0, Radix );
	}
add( Eight, Eight, t );
if( cmp( Radix, t ) > 0 )
	{
	printf( "Radix is too big: roundoff problems\n" );
	ErrCnt[Defect] += 1;
	}
k = 1;
if( cmp( Radix, Two ) == 0 )
	k = 0;
if( cmp( Radix, Ten ) == 0 )
	k = 0;
if( cmp( Radix, One ) == 0 )
	k = 0;
if( k != 0 )
	{
	printf( "Radix is not as good as 2 or 10\n" );
	ErrCnt[Flaw] += 1;
	}
/*=============================================*/
Milestone = 20;
/*=============================================*/
sub( Half, F9, t );
if( cmp( t, Half ) >= 0 )
	{
	printf( "(1-U1)-1/2 < 1/2 is FALSE, prog. fails?\n" );
	ErrCnt[Failure] += 1;
	}
mov( F9, X );
I = 1;
sub( Half, X, Y );
sub( Half, Y, Z );
if( (cmp( X, One ) == 0) && (cmp( Z, Zero) != 0) )
	{
	printf( "Comparison is fuzzy ,X=1 but X-1/2-1/2 != 0\n" );
	ErrCnt[Failure] += 1;
	}
add( One, U2, X );
I = 0;
/*=============================================*/
Milestone = 25;
/*=============================================*/
/*... BMinusU2 = nextafter(Radix, 0) */

sub( One, Radix, BMinusU2 );
sub( U2, BMinusU2, t );
add( One, t, BMinusU2 );
/* Purify Integers */
if( cmp(Radix,One) != 0 )
	{
/*X = - TwoForty * LOG(U1) / LOG(Radix);*/
	LOG( U1, X );
	LOG( Radix, t );
	div( t, X, X );
	mul( TwoForty, X, X );
	neg( X );	

	add( Half, X, Y );
	FLOOR( Y, Y );
	sub( Y, X, t );
	FABS( t );
	mul( Four, t, t );
	if( cmp( t, One ) < 0 )
		mov( Y, X );
	div( TwoForty, X, Precision );
	add( Half, Precision, Y );
	FLOOR( Y, Y );
	sub( Y, Precision, t );
	FABS( t );
	mul( TwoForty, t, t );
	if( cmp( t, Half ) < 0 )
		mov( Y, Precision );
	}
FLOOR( Precision, t );
if( (cmp( Precision, t ) != 0) || (cmp( Radix, One ) == 0) )
	{
	printf("Precision cannot be characterized by an Integer number\n");
	printf("of significant digits but, by itself, this is a minor flaw.\n");
	}
if( cmp(Radix, One) == 0 ) 
	printf("logarithmic encoding has precision characterized solely by U1.\n");
else
	{
	printf("The number of significant digits of the Radix is" );
	show( Precision );
	}
mul( U2, Nine, t );
mul( Nine, t, t );
mul( TwoForty, t, t );
if( cmp( t, One ) >= 0 )
	{
	printf( "Precision worse than 5 decimal figures\n" );
	ErrCnt[Serious] += 1;
	}
/*=============================================*/
Milestone = 30;
/*=============================================*/
/* Test for extra-precise subepressions has been deleted. */
Milestone = 35;
/*=============================================*/
if( cmp(Radix,Two) >= 0 )
	{
	mul( Radix, Radix, t );
	div( t, W, X );
	add( X, One, Y );
	sub( X, Y, Z );
	add( Z, U2, T );
	sub( Z, T, X );
	if( cmp( X, U2 ) != 0 )
		{
		printf( "Subtraction is not normalized X=Y,X+Z != Y+Z!\n" );
		ErrCnt[Failure] += 1;
		}
	if( cmp(X,U2) == 0 )
	 printf("Subtraction appears to be normalized, as it should be.");
	}

printf("\nChecking for guard digit in *, /, and -.\n");
mul( F9, One, Y );
mul( One, F9, Z );
sub( Half, F9, X );
sub( Half, Y, Y );
sub( X, Y, Y );
sub( Half, Z, Z );
sub( X, Z, Z );
add( One, U2, X );
mul( X, Radix, T );
mul( Radix, X, R );
sub( Radix, T, X );
mul( Radix, U2, t );
sub( t, X, X );
sub( Radix, R, T );
mul( Radix, U2, t );
sub( t, T, T );
sub( One, Radix, t );
mul( t, X, X );
sub( One, Radix, t );
mul( t, T, T );

k = cmp(X,Zero);
k |= cmp(Y,Zero);
k |= cmp(Z,Zero);
k |= cmp(T,Zero);
if( k == 0 )
	GMult = Yes;
else
	{
	GMult = No;
	ErrCnt[Serious] += 1;
	printf( "* lacks a Guard Digit, so 1*X != X\n" );
	}
mul( Radix, U2, Z );
add( One, Z, X );
add( X, Z, Y );
mul( X, X, t );
sub( t, Y, Y );
FABS( Y );
sub( U2, Y, Y );
sub( U2, One, X );
sub( U2, X, Z );
mul( X, X, t );
sub( t, Z, Z );
FABS( Z );
sub( U1, Z, Z );
if( (cmp(Y,Zero) > 0) || (cmp(Z,Zero) > 0) )
	{
	ErrCnt[Failure] += 1;
	printf( "* gets too many final digits wrong.\n" );
	}
sub( U2, One, Y );
add( One, U2, X );
div( Y, One, Z );
sub( X, Z, Y );
div( Three, One, X );
div( Nine, Three, Z );
sub( Z, X, X );
div( TwentySeven, Nine, T );
sub( T, Z, Z );
k = cmp( X, Zero );
k |= cmp( Y, Zero );
k |= cmp( Z, Zero );
if( k )
	{
	ErrCnt[Defect] += 1;
printf( "Division lacks a Guard Digit, so error can exceed 1 ulp\n" );
printf( "or  1/3  and  3/9  and  9/27 may disagree\n" );
	}
div( One, F9, Y );
sub( Half, F9, X );
sub( Half, Y, Y );
sub( X, Y, Y );
add( One, U2, X );
div( One, X, T );
sub( X, T, X );
k = cmp( X, Zero );
k |= cmp( Y, Zero );
k |= cmp( Z, Zero );
if( k == 0 )
	GDiv = Yes;
else
	{
	GDiv = No;
	ErrCnt[Serious] += 1;
	printf( "Division lacks a Guard Digit, so X/1 != X\n" );
	}
add( One, U2, X );
div( X, One, X );
sub( Half, X, Y );
sub( Half, Y, Y );
if( cmp(Y,Zero) >= 0 )
	{
	ErrCnt[Serious] += 1;
	printf( "Computed value of 1/1.000..1 >= 1\n" );
	}
sub( U2, One, X );
mul( Radix, U2, Y );
add( One, Y, Y );
mul( X, Radix, Z );
mul( Y, Radix, T );
div( Radix, Z, R );
div( Radix, T, StickyBit );
sub( X, R, X );
sub( Y, StickyBit, Y );
k = cmp( X, Zero );
k |= cmp( Y, Zero );
if( k )
	{
	ErrCnt[Failure] += 1;
	printf( "* and/or / gets too many last digits wrong\n" );
	}
sub( U1, One, Y );
sub( F9, One, X );
sub( Y, One, Y );
sub( U2, Radix, T );
sub( BMinusU2, Radix, Z );
sub( T, Radix, T );
k = cmp( X, U1 );
k |= cmp( Y, U1 );
k |= cmp( Z, U2 );
k |= cmp( T, U2 );
if( k == 0 )
	GAddSub = Yes;
else
	{
	GAddSub = No;
	ErrCnt[Serious] += 1;
	printf( "- lacks Guard Digit, so cancellation is obscured\n" );
	}
sub( One, F9, t );
if( (cmp(F9,One) != 0) && (cmp(t,Zero) >= 0) )
	{
	ErrCnt[Serious] += 1;
	printf("comparison alleges  (1-U1) < 1  although\n");
	printf("  subtration yields  (1-U1) - 1 = 0 , thereby vitiating\n");
	printf("  such precautions against division by zero as\n");
	printf("  ...  if (X == 1.0) {.....} else {.../(X-1.0)...}\n");
	}
if (GMult == Yes && GDiv == Yes && GAddSub == Yes)
	printf(" *, /, and - appear to have guard digits, as they should.\n");
/*=============================================*/
Milestone = 40;
/*=============================================*/
printf("Checking rounding on multiply, divide and add/subtract.\n");
RMult = Other;
RDiv = Other;
RAddSub = Other;
div( Two, Radix, RadixD2 );
mov( Two, A1 );
Done = False;
do
	{
	mov( Radix, AInvrse );
	do
		{
		mov( AInvrse, X );
		div( A1, AInvrse, AInvrse );
		FLOOR( AInvrse, t );
		k = cmp( t, AInvrse );
		}
	while( ! (k != 0 ) );
	k = cmp( X, One );
	k2 = cmp( A1, Three );
	Done = (k == 0) || (k2 > 0);
	if(! Done)
		add( Nine, One, A1 );
	}
while( ! (Done));
if( cmp(X, One) == 0 )
	mov( Radix, A1 );
div( A1, One, AInvrse );
mov( A1, X );
mov( AInvrse, Y );
Done = False;
do
	{
	mul( X, Y, Z );
	sub( Half, Z, Z );
	if( cmp( Z, Half ) != 0 )
		{
		ErrCnt[Failure] += 1;
		printf( "X * (1/X) differs from 1\n" );
		}
	k = cmp( X, Radix );
	Done = (k == 0);
	mov( Radix, X );
	div( X, One, Y );
	}
while( ! (Done));

add( One, U2, Y2 );
sub( U2, One, Y1 );
sub( U2, OneAndHalf, X );
add( OneAndHalf, U2, Y );
sub( U2, X, Z );
mul( Z, Y2, Z );
mul( Y, Y1, T );
sub( X, Z, Z );
sub( X, T, T );
mul( X, Y2, X );
add( Y, U2, Y );
mul( Y, Y1, Y );
sub( OneAndHalf, X, X );
sub( OneAndHalf, Y, Y );
k = cmp( X, Zero );
k |= cmp( Y, Zero );
k |= cmp( Z, Zero );
if( cmp( T, Zero ) > 0 )
	k = 1;
if( k == 0 )
	{
	add( OneAndHalf, U2, X );
	mul( X, Y2, X );
	sub( U2, OneAndHalf, Y );
	sub( U2, Y, Y );
	add( OneAndHalf, U2, Z );
	add( U2, Z, Z );
	sub( U2, OneAndHalf, T );
	mul( T, Y1, T );
	add( Z, U2, t );
	sub( t, X, X );
	mul( Y, Y1, StickyBit );
	mul( Z, Y2, S );
	sub( Y, T, T );
	sub( Y, U2, Y );
	add( StickyBit, Y, Y );
/* Z = S - (Z + U2 + U2); */
	add( Z, U2, t );
	add( t, U2, t );
	sub( t, S, Z );
	add( Y2, U2, t );
	mul( t, Y1, StickyBit );
	mul( Y2, Y1, Y1 );
	sub( Y2, StickyBit, StickyBit );
	sub( Half, Y1, Y1 );
	k = cmp( X, Zero );
	k |= cmp( Y, Zero );
	k |= cmp( Z, Zero );
	k |= cmp( T, Zero );
	k |= cmp( StickyBit, Zero );
	k |= cmp( Y1, Half );
	if( k == 0 )
		{
		RMult = Rounded;
		printf("Multiplication appears to round correctly.\n");
		}
	else
		{
		add( X, U2, t );
		k = cmp( t, Zero );
		if( cmp( Y, Zero ) >= 0 )
			k |= 1;
		add( Z, U2, t );
		k |= cmp( t, Zero );
		if( cmp( T, Zero ) >= 0 )
			k |= 1;
		add( StickyBit, U2, t );
		k |= cmp( t, Zero );
		if( cmp(Y1, Half) >= 0 )
			k |= 1;
		if( k == 0 )
			{
			printf("Multiplication appears to chop.\n");
			}
		else
			{
		printf("* is neither chopped nor correctly rounded.\n");
			}
		if( (RMult == Rounded) && (GMult == No) )
			printf("Multiplication has inconsistent result");
		}
	}
else
	printf("* is neither chopped nor correctly rounded.\n");

/*=============================================*/
Milestone = 45;
/*=============================================*/
add( One, U2, Y2 );
sub( U2, One, Y1 );
add( OneAndHalf, U2, Z );
add( Z, U2, Z );
div( Y2, Z, X );
sub( U2, OneAndHalf, T );
sub( U2, T, T );
sub( U2, T, Y );
div( Y1, Y, Y );
add( Z, U2, Z );
div( Y2, Z, Z );
sub( OneAndHalf, X, X );
sub( T, Y, Y );
div( Y1, T, T );
add( OneAndHalf, U2, t );
sub( t, Z, Z );
sub( OneAndHalf, U2, t );
add( t, T, T );
k = 0;
if( cmp( X, Zero ) > 0 )
	k = 1;
if( cmp( Y, Zero ) > 0 )
	k = 1;
if( cmp( Z, Zero ) > 0 )
	k = 1;
if( cmp( T, Zero ) > 0 )
	k = 1;
if( k == 0 )
	{
	div( Y2, OneAndHalf, X );
	sub( U2, OneAndHalf, Y );
	add( U2, OneAndHalf, Z );
	sub( Y, X, X );
	div( Y1, OneAndHalf, T );
	div( Y1, Y, Y );
	add( Z, U2, t );
	sub( t, T, T );
	sub( Z, Y, Y );
	div( Y2, Z, Z );
	add( Y2, U2, Y1 );
	div( Y2, Y1, Y1 );
	sub( OneAndHalf, Z, Z );
	sub( Y2, Y1, Y2 );
	sub( U1, F9, Y1 );
	div( F9, Y1, Y1 );
	k = cmp( X, Zero );
	k |= cmp( Y, Zero );
	k |= cmp( Z, Zero );
	k |= cmp( T, Zero );
	k |= cmp( Y2, Zero );
	sub( Half, Y1, t );
	sub( Half, F9, t2 );
	k |= cmp( t, t2 );
	if( k == 0 )
		{
		RDiv = Rounded;
		printf("Division appears to round correctly.\n");
		if(GDiv == No)
			printf("Division test inconsistent\n");
		}
	else
		{
		k = 0;
		if( cmp( X, Zero ) >= 0 )
			k = 1;
		if( cmp( Y, Zero ) >= 0 )
			k = 1;
		if( cmp( Z, Zero ) >= 0 )
			k = 1;
		if( cmp( T, Zero ) >= 0 )
			k = 1;
		if( cmp( Y, Zero ) >= 0 )
			k = 1;
		sub( Half, Y1, t );
		sub( Half, F9, t2 );
		if( cmp( t, t2 ) >= 0 )
			k = 1;
		if( k == 0 )
			{
			RDiv = Chopped;
			printf("Division appears to chop.\n");
			}
		}
	}
if(RDiv == Other)
	printf("/ is neither chopped nor correctly rounded.\n");
div( Radix, One, BInvrse );
mul( BInvrse, Radix, t );
sub( Half, t, t );
if( cmp( t, Half ) != 0 )
	{
	ErrCnt[Failure] += 1;
	printf( "Radix * ( 1 / Radix ) differs from 1\n" );
	}

Milestone = 50;
/*=============================================*/
add( F9, U1, t );
sub( Half, t, t );
k = cmp( t, Half );
add( BMinusU2, U2, t );
sub( One, t, t );
sub( One, Radix, t2 );
k |= cmp( t, t2 );
if( k != 0 )
	{
	ErrCnt[Failure] += 1;
	printf( "Incomplete carry-propagation in Addition\n" );
	}
mul( U1, U1, X );
sub( X, One, X );
sub( U2, One, Y );
mul( U2, Y, Y );
add( One, Y, Y );
sub( Half, F9, Z );
sub( Half, X, X );
sub( Z, X, X );
sub( One, Y, Y );
if( (cmp(X,Zero) == 0) && (cmp(Y,Zero) == 0) )
	{
	RAddSub = Chopped;
	printf("Add/Subtract appears to be chopped.\n");
	}
if(GAddSub == Yes)
	{
	add( Half, U2, X );
	mul( X, U2, X );
	sub( U2, Half, Y );
	mul( Y, U2, Y );
	add( One, X, X );
	add( One, Y, Y );
	add( One, U2, t );
	sub( X, t, X );
	sub( Y, One, Y );
	k = cmp(X,Zero);
	if( k )
		printf( "1+U2-[u2(1/2+U2)+1] != 0\n" );
	k2 = cmp(Y,Zero);
	if( k2 )
		printf( "1-[U2(1/2-U2)+1] != 0\n" );
	k |= k2;
	if( k == 0 )
		{
		add( Half, U2, X );
		mul( X, U1, X );
		sub( U2, Half, Y );
		mul( Y, U1, Y );
		sub( X, One, X );
		sub( Y, One, Y );
		sub( X, F9, X );
		sub( Y, One, Y );
		k = cmp(X,Zero);
		if( k )
			printf( "F9-[1-U1(1/2+U2)] != 0\n" );
		k2 = cmp(Y,Zero);
		if( k2 )
			printf( "1-[1-U1(1/2-U2)] != 0\n" );
		k |= k2;
		if( k == 0 )
			{
			RAddSub = Rounded;
		printf("Addition/Subtraction appears to round correctly.\n");
			if(GAddSub == No)
				printf( "Add/Subtract test inconsistent\n");
			}
		else
			{
		 printf("Addition/Subtraction neither rounds nor chops.\n");
			}
		}
	else
		printf("Addition/Subtraction neither rounds nor chops.\n");
	}
else
	printf("Addition/Subtraction neither rounds nor chops.\n");

mov( One, S );
add( One, Half, X );
mul( Half, X, X );
add( One, X, X );
add( One, U2, Y );
mul( Y, Half, Y );
sub( Y, X, Z );
sub( X, Y, T );
add( Z, T, StickyBit );
if( cmp(StickyBit, Zero) != 0 )
	{
	mov( Zero, S );
	ErrCnt[Flaw] += 1;
	printf( "(X - Y) + (Y - X) is non zero!\n" );
	}
mov( Zero, StickyBit );
FLOOR( RadixD2, t );
k2 = cmp( t, RadixD2 );
k = 1;
if( (GMult == Yes) && (GDiv == Yes) && (GAddSub == Yes)
	&& (RMult == Rounded) && (RDiv == Rounded)
	&& (RAddSub == Rounded) && (k2 == 0) )
	{
	printf("Checking for sticky bit.\n");
	k = 0;
	add( Half, U1, X );
	mul( X, U2, X );
	mul( Half, U2, Y );
	add( One, Y, Z );
	add( One, X, T );
	sub( One, Z, t );
	sub( One, T, t2 );
	if( cmp(t,Zero) > 0 )
		{
		k = 1;
		printf( "[1+(1/2)U2]-1 > 0\n" );
		}
	if( cmp(t2,U2) < 0 )
		{
		k = 1;
		printf( "[1+U2(1/2+U1)]-1 < U2\n" );
		}
	add( T, Y, Z );
	sub( X, Z, Y );
	sub( T, Z, t );
	sub( T, Y, t2 );
	if( cmp(t,U2) < 0 )
		{
		k = 1;
		printf( "[[1+U2(1/2+U1)]+(1/2)U2]-[1+U2(1/2+U1)] < U2\n" );
		}
	if( cmp(t2,Zero) != 0 )
		{
		k = 1;
		printf( "(1/2)U2-[1+U2(1/2+U1)] != 0\n" );
		}
	add( Half, U1, X );
	mul( X, U1, X );
	mul( Half, U1, Y );
	sub( Y, One, Z );
	sub( X, One, T );
	sub( One, Z, t );
	sub( F9, T, t2 );
	if( cmp(t,Zero) != 0 )
		{
		k = 1;
		printf( "(1-(1/2)U1)-1 != 0\n" );
		}
	if( cmp(t2,Zero) != 0 )
		{
		k = 1;
		printf( "[1-U1(1/2+U1)]-F9 != 0\n" );
		}
	sub( U1, Half, Z );
	mul( Z, U1, Z );
	sub( Z, F9, T );
	sub( Y, F9, Q );
	sub( F9, T, t );
	if( cmp( t, Zero ) != 0 )
		{
		k = 1;
		printf( "[F9-U1(1/2-U1)]-F9 != 0\n" );
		}
	sub( U1, F9, t );
	sub( Q, t, t );
	if( cmp( t, Zero ) != 0 )
		{
		k = 1;
		printf( "(F9-U1)-(F9-(1/2)U1) != 0\n" );
		}
	add( One, U2, Z );
	mul( Z, OneAndHalf, Z );
	add( OneAndHalf, U2, T );
	sub( Z, T, T );
	add( U2, T, T );
	div( Radix, Half, X );
	add( One, X, X );
	mul( Radix, U2, Y );
	add( One, Y, Y );
	mul( X, Y, Z );
	if( cmp( T, Zero ) != 0 )
		{
		k = 1;
		printf( "(3/2+U2)-3/2(1+U2)+U2 != 0\n" );
		}
	mul( Radix, U2, t );
	add( X, t, t );
	sub( Z, t, t );
	if( cmp( t, Zero ) != 0 )
		{
		k = 1;
	printf( "(1+1/2Radix)+Radix*U2-[1+1/(2Radix)][1+Radix*U2] != 0\n" );
		}
	if( cmp(Radix, Two) != 0 )
		{
		add( Two, U2, X );
		div( Two, X, Y );
		sub( One, Y, t );
		if( cmp( t, Zero) != 0 )
			k = 1;
		}
	}
if( k == 0 )
	{
	printf("Sticky bit apparently used correctly.\n");
	mov( One, StickyBit );
	}
else
	{
	printf("Sticky bit used incorrectly or not at all.\n");
	}

if( GMult == No || GDiv == No || GAddSub == No ||
		RMult == Other || RDiv == Other || RAddSub == Other)
	{
	ErrCnt[Flaw] += 1;
 printf("lack(s) of guard digits or failure(s) to correctly round or chop\n");
printf( "(noted above) count as one flaw in the final tally below\n" );
	}
/*=============================================*/
Milestone = 60;
/*=============================================*/
printf("\n");
printf("Does Multiplication commute?  ");
printf("Testing on %d random pairs.\n", NoTrials);
SQRT( Three, Random9 );
mov( Third, Random1 );
I = 1;
do
	{
	Random();
	mov( Random1, X );
	Random();
	mov( Random1, Y );
	mul( Y, X, Z9 );
	mul( X, Y, Z );
	sub( Z9, Z, Z9 );
	I = I + 1;
	}
while ( ! ((I > NoTrials) || (cmp(Z9,Zero) != 0)));
if(I == NoTrials)
	{
	div( Three, Half, t );
	add( One, t, Random1 );
	add( U2, U1, t );
	add( t, One, Random2 );
	mul( Random1, Random2, Z );
	mul( Random2, Random1, Y );
/* Z9 = (One + Half / Three) * ((U2 + U1) + One) - (One + Half /
 *			Three) * ((U2 + U1) + One);
 */
	div( Three, Half, t2 );
	add( One, t2, t2 );
	add( U2, U1, t );
	add( t, One, t );
	mul( t2, t, Z9 );
	mul( t2, t, t );
	sub( t, Z9, Z9 );
	}
if(! ((I == NoTrials) || (cmp(Z9,Zero) == 0)))
	{
	ErrCnt[Defect] += 1;
	printf( "X * Y == Y * X trial fails.\n");
	}
else
	{
	printf("     No failures found in %d integer pairs.\n", NoTrials);
	}
/*=============================================*/
Milestone = 70;
/*=============================================*/
sqtest();
Milestone = 90;
pow1test();

Milestone = 110;

printf("Seeking Underflow thresholds UfThold and E0.\n");
mov( U1, D );
FLOOR( Precision, t );
if( cmp(Precision, t) != 0 )
	{
	mov( BInvrse, D );
	mov( Precision, X );
	do
		{
		mul( D, BInvrse, D );
		sub( One, X, X );
		}
	while( cmp(X, Zero) > 0 );
	}
mov( One, Y );
mov( D, Z );
/* ... D is power of 1/Radix < 1. */
sigsave = sigfpe;
if( setjmp(ovfl_buf) )
	goto under0;
do
	{
	mov( Y, C );
	mov( Z, Y );
	mul( Y, Y, Z );
	add( Z, Z, t );
	}
while( (cmp(Y,Z) > 0) && (cmp(t,Z) > 0) );

under0:
sigsave = 0;

mov( C, Y );
mul( Y, D, Z );
sigsave = sigfpe;
if( setjmp(ovfl_buf) )
	goto under1;
do
	{
	mov( Y, C );
	mov( Z, Y );
	mul( Y, D, Z );
	add( Z, Z, t );
	}
while( (cmp(Y,Z) > 0) && (cmp(t,Z) > 0) );

under1:
sigsave = 0;

if( cmp(Radix,Two) < 0 )
	mov( Two, HInvrse );
else
	mov( Radix, HInvrse );
div( HInvrse, One, H );
/* ... 1/HInvrse == H == Min(1/Radix, 1/2) */
div( C, One, CInvrse );
mov( C, E0 );
mul( E0, H, Z );
/* ...1/Radix^(BIG Integer) << 1 << CInvrse == 1/C */
sigsave = sigfpe;
if( setjmp(ovfl_buf) )
	goto under2;
do
	{
	mov( E0, Y );
	mov( Z, E0 );
	mul( E0, H, Z );
	add( Z, Z, t );
	}
while( (cmp(E0,Z) > 0) && (cmp(t,Z) > 0) );

under2:
sigsave = 0;

mov( E0, UfThold );
mov( Zero, E1 );
mov( Zero, Q );
mov( U2, E9 );
add( One, E9, S );
mul( C, S, D );
if( cmp(D,C) <= 0 )
	{
	mul( Radix, U2, E9 );
	add( One, E9, S );
	mul( C, S, D );
	if( cmp(D, C) <= 0 )
		{
		ErrCnt[Failure] += 1;
		printf( "multiplication gets too many last digits wrong.\n" );
		mov( E0, Underflow );
		mov( Zero, Y1 );
		mov( Z, PseudoZero );
		}
	}
else
	{
	mov( D, Underflow );
	mul( Underflow, H, PseudoZero );
	mov( Zero, UfThold );
	do
		{
		mov( Underflow, Y1 );
		mov( PseudoZero, Underflow );
		add( E1, E1, t );
		if( cmp(t, E1) <= 0)
			{
			mul( Underflow, HInvrse, Y2 );
			sub( Y2, Y1, E1 );
			FABS( E1 );
			mov( Y1, Q );
			if( (cmp( UfThold, Zero ) == 0)
				&& (cmp(Y1, Y2) != 0) )
				mov( Y1, UfThold );
			}
		mul( PseudoZero, H, PseudoZero );
		add( PseudoZero, PseudoZero, t );
		}
	while( (cmp(Underflow, PseudoZero) > 0)
		&& (cmp(t, PseudoZero) > 0) );
	}
/* Comment line 4530 .. 4560 */
if( cmp(PseudoZero, Zero) != 0 )
	{
	printf("\n");
	mov(PseudoZero, Z );
/* ... Test PseudoZero for "phoney- zero" violates */
/* ... PseudoZero < Underflow or PseudoZero < PseudoZero + PseudoZero
		   ... */
	if( cmp(PseudoZero, Zero) <= 0 )
		{
		ErrCnt[Failure] += 1;
		printf("Positive expressions can underflow to an\n");
		printf("allegedly negative value\n");
		printf("PseudoZero that prints out as: " );
		show( PseudoZero );
		mov( PseudoZero, X );
		neg( X );
		if( cmp(X, Zero) <= 0 )
			{
			printf("But -PseudoZero, which should be\n");
			printf("positive, isn't; it prints out as " );
			show( X );
			}
		}
	else
		{
		ErrCnt[Flaw] += 1;
		printf( "Underflow can stick at an allegedly positive\n");
		printf("value PseudoZero that prints out as " );
		show( PseudoZero );
		}
/*	TstPtUf();*/
	}

/*=============================================*/
Milestone = 120;
/*=============================================*/
mul( CInvrse, Y, t );
mul( CInvrse, Y1, t2 );
if( cmp(t,t2) > 0 )
	{
	mul( H, S, S );
	mov( Underflow, E0 );
	}
if(! ((cmp(E1,Zero) == 0) || (cmp(E1,E0) == 0)) )
	{
	ErrCnt[Defect] += 1;
	if( cmp(E1,E0) < 0 )
		{
		printf("Products underflow at a higher");
		printf(" threshold than differences.\n");
		if( cmp(PseudoZero,Zero) == 0 ) 
			mov( E1, E0 );
		}
	else
		{
		printf("Difference underflows at a higher");
		printf(" threshold than products.\n");
		}
	}
printf("Smallest strictly positive number found is E0 = " );
show( E0 );
mov( E0, Z );
TstPtUf();
mov( E0, Underflow );
if(N == 1)
	mov( Y, Underflow );
I = 4;
if( cmp(E1,Zero) == 0 )
	I = 3;
if( cmp( UfThold,Zero) == 0 )
	I = I - 2;
UfNGrad = True;
switch(I)
	{
	case 1:
	mov( Underflow, UfThold );
	mul( CInvrse, Q, t );
	mul( CInvrse, Y, t2 );
	mul( t2, S, t2 );
	if( cmp( t, t2 ) != 0 )
		{
		mov( Y, UfThold );
		ErrCnt[Failure] += 1;
		printf( "Either accuracy deteriorates as numbers\n");
		printf("approach a threshold = " );
		show( UfThold );
		printf(" coming down from " );
		show( C );
	printf(" or else multiplication gets too many last digits wrong.\n");
		}
	break;
	
	case	2:
	ErrCnt[Failure] += 1;
	printf( "Underflow confuses Comparison which alleges that\n");
	printf("Q == Y while denying that |Q - Y| == 0; these values\n");
	printf("print out as Q = " );
	show( Q );
	printf( ", Y = " );
	show( Y );
	sub( Y2, Q, t );
	FABS(t);
	printf ("|Q - Y| = " );
	show( t );
	mov( Q, UfThold );
	break;
	
	case 3:
	mov( X, X );
	break;
	
	case 4:
	div( E9, E1, t );
	sub( t, UfThold, t );
	FABS(t);
	if( (cmp(Q,UfThold) == 0) && (cmp(E1,E0) == 0)
		&& (cmp(t,E1) <= 0) )
		{
		UfNGrad = False;
		printf("Underflow is gradual; it incurs Absolute Error =\n");
		printf("(roundoff in UfThold) < E0.\n");
		mul( E0, CInvrse, Y );
		add( OneAndHalf, U2, t );
		mul( Y, t, Y );
		add( One, U2, X );
		mul( CInvrse, X, X );
		div( X, Y, Y );
		IEEE = (cmp(Y,E0) == 0);
		}
	}
if(UfNGrad)
	{
	printf("\n");
	div( UfThold, Underflow, R );
	SQRT( R, R );
	if( cmp(R,H) <= 0)
		{
		mul( R, UfThold, Z );
/* X = Z * (One + R * H * (One + H));*/
		add( One, H, X );
		mul( H, X, X );
		mul( R, X, X );
		add( One, X, X );
		mul( Z, X, X );
		}
	else
		{
		mov( UfThold, Z );
/*X = Z * (One + H * H * (One + H));*/
		add( One, H, X );
		mul( H, X, X );
		mul( H, X, X );
		add( One, X, X );
		mul( Z, X, X );
		}
	sub( Z, X, t );
	if(! ((cmp(X,Z) == 0) || (cmp(t,Zero) != 0)) )
		{
		ErrCnt[Flaw] += 1;
		printf("X = " );
		show( X );
		printf( "\tis not equal to Z = " );
		show( Z );
		sub( Z, X, Z9 );
		printf("yet X - Z yields " );
		show( Z9 );
		printf("    Should this NOT signal Underflow, ");
		printf("this is a SERIOUS DEFECT\nthat causes ");
		printf("confusion when innocent statements like\n");;
		printf("    if (X == Z)  ...  else");
		printf("  ... (f(X) - f(Z)) / (X - Z) ...\n");
		printf("encounter Division by Zero although actually\n");
		printf("X / Z = 1 + " );
		div( Z, X, t );
		sub( Half, t, t );
		sub( Half, t, t );
		show(t);
		}
	}
printf("The Underflow threshold is " );
show( UfThold );
printf( "below which calculation may suffer larger Relative error than" );
printf( " merely roundoff.\n");
mul( U1, U1, Y2 );
mul( Y2, Y2, Y );
mul( Y, U1, Y2 );
if( cmp( Y2,UfThold) <= 0 )
	{
	if( cmp(Y,E0) > 0 )
		{
		ErrCnt[Defect] += 1;
		I = 5;
		}
	else
		{
		ErrCnt[Serious] += 1;
		I = 4;
		}
	printf("Range is too narrow; U1^%d Underflows.\n", I);
	}
Milestone = 130;

/*Y = - FLOOR(Half - TwoForty * LOG(UfThold) / LOG(HInvrse)) / TwoForty;*/
LOG( UfThold, Y );
LOG( HInvrse, t );
div( t, Y, Y );
mul( TwoForty, Y, Y );
sub( Y, Half, Y );
FLOOR( Y, Y );
div( TwoForty, Y, Y );
neg(Y);
sub( One, Y, Y2 ); /* ***** changed from Y2 = Y + Y */
printf("Since underflow occurs below the threshold\n");
printf("UfThold = " ); 
show( HInvrse );
printf( "\tto the power  " );
show( Y );
printf( "only underflow should afflict the expression " );
show( HInvrse );
printf( "\tto the power  " );
show( Y2 );
POW( HInvrse, Y2, V9 );
printf("Actually calculating yields: " );
show( V9 );
add( Radix, Radix, t );
add( t, E9, t );
mul( t, UfThold, t );
if( (cmp(V9,Zero) < 0) || (cmp(V9,t) > 0) )
	{
	ErrCnt[Serious] += 1;
	printf( "this is not between 0 and underflow\n");
	printf("   threshold = " );
	show( UfThold );
	}
else
	{
	add( One, E9, t );
	mul( UfThold, t, t );
	if( cmp(V9,t) <= 0 )
		printf("This computed value is O.K.\n");
	else
		{
		ErrCnt[Defect] += 1;
		printf( "this is not between 0 and underflow\n");
		printf("   threshold = " );
		show( UfThold );
		}
	}

Milestone = 140;

pow2test();
	
/*=============================================*/
Milestone = 160;
/*=============================================*/
Pause();
printf("Searching for Overflow threshold:\n");
printf("This may generate an error.\n");
sigsave = sigfpe;
I = 0;
mov( CInvrse, Y );
neg(Y);
mul( HInvrse, Y, V9 );
if (setjmp(ovfl_buf))
	goto overflow;
do
	{
	mov( Y, V );
	mov( V9, Y );
	mul( HInvrse, Y, V9 );
	}
while( cmp(V9,Y) < 0 );
I = 1;

overflow:

mov( V9, Z );
printf("Can `Z = -Y' overflow?\n");
printf("Trying it on Y = " );
show(Y);
mov( Y, V9 );
neg( V9 );
mov( V9, V0 );
sub( Y, V, t );
add( V, V0, t2 );
if( cmp(t,t2) == 0 )
	printf("Seems O.K.\n");
else
	{
	printf("finds a Flaw, -(-Y) differs from Y.\n");
	ErrCnt[Flaw] += 1;
	}
if( cmp(Z, Y) != 0 )
	{
	ErrCnt[Serious] += 1;
	printf("overflow past " );
	show( Y );
	printf( "\tshrinks to " );
	show( Z );
	}
/*Y = V * (HInvrse * U2 - HInvrse);*/
mul( HInvrse, U2, Y );
sub( HInvrse, Y, Y );
mul( V, Y, Y );
/*Z = Y + ((One - HInvrse) * U2) * V;*/
sub( HInvrse, One, Z );
mul( Z, U2, Z );
mul( Z, V, Z );
add( Y, Z, Z );
if( cmp(Z,V0) < 0 )
	mov( Z, Y );
if( cmp(Y,V0) < 0)
	mov( Y, V );
sub( V, V0, t );
if( cmp(t,V0) < 0 )
	mov( V0, V );
printf("Overflow threshold is V  = " );
show( V );
if(I)
	{
	printf("Overflow saturates at V0 = " );
	show( V0 );
	}
else
printf("There is no saturation value because the system traps on overflow.\n");

mul( V, One, V9 );
printf("No Overflow should be signaled for V * 1 = " );
show( V9 );
div( One, V, V9 );
	printf("                           nor for V / 1 = " );
	show( V9 );
	printf("Any overflow signal separating this * from the one\n");
	printf("above is a DEFECT.\n");
/*=============================================*/
Milestone = 170;
/*=============================================*/
mov( V, t );
neg( t );
k = 0;
if( cmp(t,V) >= 0 )
	k = 1;
mov( V0, t );
neg( t );
if( cmp(t,V0) >= 0 )
	k = 1;
mov( UfThold, t );
neg(t);
if( cmp(t,V) >= 0 )
	k = 1;
if( cmp(UfThold,V) >= 0 )
	k = 1;
if( k != 0 )
	{
	ErrCnt[Failure] += 1;
	printf( "Comparisons involving +-");
	show( V );
	show( V0 );
	show( UfThold );
	printf("are confused by Overflow." );
	}
/*=============================================*/
Milestone = 175;
/*=============================================*/
printf("\n");
for(Indx = 1; Indx <= 3; ++Indx) {
	switch(Indx)
		{
		case 1: mov(UfThold, Z); break;
		case 2: mov( E0, Z); break;
		case 3: mov(PseudoZero, Z); break;
		}
if( cmp(Z, Zero) != 0 )
	{
	SQRT( Z, V9 );
	mul( V9, V9, Y );
	mul( Radix, E9, t );
	sub( t, One, t );
	div( t, Y, t );
	add( One, Radix, t2 );
	add( t2, E9, t2 );
	mul( t2, Z, t2 );
	if( (cmp(t,Z) < 0) || (cmp(Y,t2) > 0) )
		{
		if( cmp(V9,U1) > 0 )
			ErrCnt[Serious] += 1;
		else
			ErrCnt[Defect] += 1;
		printf("Comparison alleges that what prints as Z = " );
		show( Z );
		printf(" is too far from sqrt(Z) ^ 2 = " );
		show( Y );
		}
	}
}

Milestone = 180;

for(Indx = 1; Indx <= 2; ++Indx)
	{
	if(Indx == 1)
		mov( V, Z );
	else
		mov( V0, Z );
	SQRT( Z, V9 );
	mul( Radix, E9, X );
	sub( X, One, X );
	mul( X, V9, X );
	mul( V9, X, V9 );
	mul( Two, Radix, t );
	mul( t, E9, t );
	sub( t, One, t );
	mul( t, Z, t );
	if( (cmp(V9,t) < 0) || (cmp(V9,Z) > 0) )
		{
		mov( V9, Y );
		if( cmp(X,W) <  0 )
			ErrCnt[Serious] += 1;
		else
			ErrCnt[Defect] += 1;
		printf("Comparison alleges that Z = " );
		show( Z );
		printf(" is too far from sqrt(Z) ^ 2 :" );
		show( Y );
		}
	}

Milestone = 190;

Pause();
mul( UfThold, V, X ); 
mul( Radix, Radix, Y );
mul( X, Y, t );
if( (cmp(t,One) < 0) || (cmp(X,Y) > 0) )
	{
	mul( X, Y, t );
	div( U1, Y, t2 );
	if( (cmp(t,U1) < 0) || (cmp(X,t2) > 0) )
		{
		ErrCnt[Defect] += 1;
		printf( "Badly " );
		}
	else
		{
		ErrCnt[Flaw] += 1;
		printf(" unbalanced range; UfThold * V = " );
		show( X );
		printf( "\tis too far from 1.\n");
		}
	}
Milestone = 200;

for(Indx = 1; Indx <= 5; ++Indx)
	{
	mov( F9, X );
	switch(Indx)
		{
		case 2: add( One, U2, X ); break;
		case 3: mov( V, X ); break;
		case 4: mov(UfThold,X); break;
		case 5: mov(Radix,X);
		}
	mov( X, Y );

	sigsave = sigfpe;
	if (setjmp(ovfl_buf))
		{
		printf("  X / X  traps when X = " );
		show( X );
		}
	else
		{
/*V9 = (Y / X - Half) - Half;*/
		div( X, Y, t );
		sub( Half, t, t );
		sub( Half, t, V9 );
		if( cmp(V9,Zero) == 0 )
			continue;
		mov( U1, t );
		neg(t);
		if( (cmp(V9,t) == 0) && (Indx < 5) )
			{
			ErrCnt[Flaw] += 1;
			}
		else
			{
			ErrCnt[Serious] += 1;
			}
		printf("  X / X differs from 1 when X = " );
		show( X );
		printf("  instead, X / X - 1/2 - 1/2 = " );
		show( V9 );
		}
	}

	Pause();
	printf("\n");
	{
		static char *msg[] = {
			"FAILUREs  encountered =",
			"SERIOUS DEFECTs  discovered =",
			"DEFECTs  discovered =",
			"FLAWs  discovered =" };
		int i;
		for(i = 0; i < 4; i++) if (ErrCnt[i])
			printf("The number of  %-29s %d.\n",
				msg[i], ErrCnt[i]);
		}
	printf("\n");
	if ((ErrCnt[Failure] + ErrCnt[Serious] + ErrCnt[Defect]
			+ ErrCnt[Flaw]) > 0) {
		if ((ErrCnt[Failure] + ErrCnt[Serious] + ErrCnt[
			Defect] == 0) && (ErrCnt[Flaw] > 0)) {
			printf("The arithmetic diagnosed seems ");
			printf("satisfactory though flawed.\n");
			}
		if ((ErrCnt[Failure] + ErrCnt[Serious] == 0)
			&& ( ErrCnt[Defect] > 0)) {
			printf("The arithmetic diagnosed may be acceptable\n");
			printf("despite inconvenient Defects.\n");
			}
		if ((ErrCnt[Failure] + ErrCnt[Serious]) > 0) {
			printf("The arithmetic diagnosed has ");
			printf("unacceptable serious defects.\n");
			}
		if (ErrCnt[Failure] > 0) {
			printf("Fatal FAILURE may have spoiled this");
			printf(" program's subsequent diagnoses.\n");
			}
		}
	else {
		printf("No failures, defects nor flaws have been discovered.\n");
		if (! ((RMult == Rounded) && (RDiv == Rounded)
			&& (RAddSub == Rounded) && (RSqrt == Rounded))) 
			printf("The arithmetic diagnosed seems satisfactory.\n");
		else {
			k = 0;
			if( cmp( Radix, Two ) == 0 )
				k = 1;
			if( cmp( Radix, Ten ) == 0 )
				k = 1;
			if( (cmp(StickyBit,One) >= 0) && (k == 1) )
				{
				printf("Rounding appears to conform to ");
				printf("the proposed IEEE standard P");
				k = 0;
				k |= cmp( Radix, Two );
				mul( Four, Three, t );
				mul( t, Two, t );
				sub( t, Precision, t );
				sub( TwentySeven, Precision, t2 );
				sub( TwentySeven, t2, t2 );
				add( t2, One, t2 );
				mul( t2, t, t );
				if( (cmp(Radix,Two) == 0)
					&& (cmp(t,Zero) == 0) )
					printf("754");
				else
					printf("854");
				if(IEEE)
					printf(".\n");
				else
					{
			printf(",\nexcept for possibly Double Rounding");
			printf(" during Gradual Underflow.\n");
					}
				}
		printf("The arithmetic diagnosed appears to be excellent!\n");
			}
		}
	if (fpecount)
		printf("\nA total of %d floating point exceptions were registered.\n",
			fpecount);
	printf("END OF TEST.\n");
	}


/* Random */
/*  Random computes
     X = (Random1 + Random9)^5
     Random1 = X - FLOOR(X) + 0.000005 * X;
   and returns the new value of Random1
*/


static int randflg = 0;
FLOAT(C5em6);

Random()
{

if( randflg == 0 )
	{
	mov( Six, t );
	neg(t);
	POW( Ten, t, t );
	mul( Five, t, C5em6 );
	randflg = 1;
	}
add( Random1, Random9, t );
mul( t, t, t2 );
mul( t2, t2, t2 );
mul( t, t2, t );
FLOOR(t, t2 );
sub( t2, t, t2 );
mul( t, C5em6, t );
add( t, t2, Random1 );
/*return(Random1);*/
}

/* SqXMinX */

SqXMinX( ErrKind )
int ErrKind;
{
mul( X, BInvrse, t2 );
sub( t2, X, t );
/*SqEr = ((SQRT(X * X) - XB) - XA) / OneUlp;*/
mul( X, X, Sqarg );
SQRT( Sqarg, SqEr );
sub( t2, SqEr, SqEr );
sub( t, SqEr, SqEr );
div( OneUlp, SqEr, SqEr );
if( cmp(SqEr,Zero) != 0)
	{
	Showsq( 0 );
	add( J, One, J );
	ErrCnt[ErrKind] += 1;
	printf("sqrt of " );
	mul( X, X, t );
	show( t );
	printf( "minus " );
	show( X );
	printf( "equals " );
	mul( OneUlp, SqEr, t );
	show( t );
	printf("\tinstead of correct value 0 .\n");
	}
}

/* NewD */

NewD()
{
mul( Z1, Q, X );
/*X = FLOOR(Half - X / Radix) * Radix + X;*/
div( Radix, X, t );
sub( t, Half, t );
FLOOR( t, t );
mul( t, Radix, t );
add( t, X, X );
/*Q = (Q - X * Z) / Radix + X * X * (D / Radix);*/
mul( X, Z, t );
sub( t, Q, t );
div( Radix, t, t );
div( Radix, D, t2 );
mul( X, t2, t2 );
mul( X, t2, t2 );
add( t, t2, Q );
/*Z = Z - Two * X * D;*/
mul( Two, X, t );
mul( t, D, t );
sub( t, Z, Z );

if( cmp(Z,Zero) <= 0)
	{
	neg(Z);
	neg(Z1);
	}
mul( Radix, D, D );
}

/* SR3750 */

SR3750()
{
sub( Radix, X, t );
sub( Radix, Z2, t2 );
k = 0;
if( cmp(t,t2) < 0 )
	k = 1;
sub( Z2, X, t );
sub( Z2, W, t2 );
if( cmp(t,t2) > 0 )
	k = 1;
/*if (! ((X - Radix < Z2 - Radix) || (X - Z2 > W - Z2))) {*/
if( k == 0 )
	{
	I = I + 1;
	mul( X, D, X2 );
	mov( X2, Sqarg );
	SQRT( X2, X2 );
/*Y2 = (X2 - Z2) - (Y - Z2);*/
	sub( Z2, X2, Y2 );
	sub( Z2, Y, t );
	sub( t, Y2, Y2 );
	sub( Half, Y, X2 );
	div( X2, X8, X2 );
	mul( Half, X2, t );
	mul( t, X2, t );
	sub( t, X2, X2 );
/*SqEr = (Y2 + Half) + (Half - X2);*/
	add( Y2, Half, SqEr );
	sub( X2, Half, t );
	add( t, SqEr, SqEr );
	Showsq( -1 );
	sub( X2, Y2, SqEr );
	Showsq( 1 );
	}
}

/* IsYeqX */

IsYeqX()
{
if( cmp(Y,X) != 0 )
	{
	if (N <= 0)
		{
		if( (cmp(Z,Zero) == 0) && (cmp(Q,Zero) <= 0) )
			printf("WARNING:  computing\n");
		else
			{
			ErrCnt[Defect] += 1;
			printf( "computing\n");
			}
		show( Z );
		printf( "\tto the power " );
		show( Q );
		printf("\tyielded " );
		show( Y );
		printf("\twhich compared unequal to correct " );
		show( X );
		sub( X, Y, t );
		printf("\t\tthey differ by " );
		show( t );
		}
	N = N + 1; /* ... count discrepancies. */
	}
}

/* SR3980 */

SR3980()
{
long li;

do
	{
/*Q = (FLOAT) I;*/
	li = I;
	LTOF( &li, Q );
	POW( Z, Q, Y );
	IsYeqX();
	if(++I > M)
		break;
	mul( Z, X, X );
	}
while( cmp(X,W) < 0 );
}

/* PrintIfNPositive */

PrintIfNPositive()
{
if(N > 0)
	printf("Similar discrepancies have occurred %d times.\n", N);
}


/* TstPtUf */

TstPtUf()
{
N = 0;
if( cmp(Z,Zero) != 0)
	{
	printf("Since comparison denies Z = 0, evaluating ");
	printf("(Z + Z) / Z should be safe.\n");
	sigsave = sigfpe;
	if (setjmp(ovfl_buf))
		goto very_serious;
	add( Z, Z, Q9 );
	div( Z, Q9, Q9 );
	printf("What the machine gets for (Z + Z) / Z is " );
	show( Q9 );
	sub( Two, Q9, t );
	FABS(t);
	mul( Radix, U2, t2 );
	if( cmp(t,t2) < 0 )
		{
		printf("This is O.K., provided Over/Underflow");
		printf(" has NOT just been signaled.\n");
		}
	else
		{
		if( (cmp(Q9,One) < 0) || (cmp(Q9,Two) > 0) )
			{
very_serious:
			N = 1;
			ErrCnt [Serious] = ErrCnt [Serious] + 1;
			printf("This is a VERY SERIOUS DEFECT!\n");
			}
		else
			{
			N = 1;
			ErrCnt[Defect] += 1;
			printf("This is a DEFECT!\n");
			}
		}
	mul( Z, One, V9 );
	mov( V9, Random1 );
	mul( One, Z, V9 );
	mov( V9, Random2 );
	div( One, Z, V9 );
	if( (cmp(Z,Random1) == 0) && (cmp(Z,Random2) == 0)
		&& (cmp(Z,V9) == 0) )
		{
		if (N > 0)
			Pause();
		}
	else
		{
		N = 1;
		ErrCnt[Defect] += 1;
		printf( "What prints as Z = ");
		show( Z );
		printf( "\tcompares different from " );
		if( cmp(Z,Random1) != 0)
			{
			printf("Z * 1 = " );
			show( Random1 );
			}
		if( (cmp(Z,Random2) != 0)
			|| (cmp(Random2,Random1) != 0) )
			{
			printf("1 * Z == " );
			show( Random2 );
			}
		if( cmp(Z,V9) != 0 )
			{
			printf("Z / 1 = " );
			show( V9 );
			}
		if( cmp(Random2,Random1) != 0 )
			{
			ErrCnt[Defect] += 1;
			printf( "Multiplication does not commute!\n");
			printf("\tComparison alleges that 1 * Z = " );
			show(Random2);
			printf("\tdiffers from Z * 1 = " );
			show(Random1);
			}
		Pause();
		}
	}
}

Pause()
{
}

Sign( x, y )
QELT x[], y[];
{

if( cmp( x, Zero ) < 0 )
	{
	mov( One, y );
	neg( y );
	}
else
	{
	mov( One, y );
	}
}

sqtest()
{
printf("\nRunning test of square root(x).\n");

RSqrt = Other;
k = 0;
SQRT( Zero, t );
k |= cmp( Zero, t );
mov( Zero, t );
neg(t);
SQRT( t, t2 );
k |= cmp( t, t2 );
SQRT( One, t );
k |= cmp( One, t );
if( k != 0 )
 	{
	ErrCnt[Failure] += 1;
	printf( "Square root of 0.0, -0.0 or 1.0 wrong\n");
	}
mov( Zero, MinSqEr );
mov( Zero, MaxSqEr );
mov( Zero, J );
mov( Radix, X );
mov( U2, OneUlp );
SqXMinX( Serious );
mov( BInvrse, X );
mul( BInvrse, U1, OneUlp );
SqXMinX( Serious );
mov( U1, X );
mul( U1, U1, OneUlp );
SqXMinX( Serious );
if( cmp(J,Zero) != 0)
	Pause();
printf("Testing if sqrt(X * X) == X for %d Integers X.\n", NoTrials);
mov( Zero, J );
mov( Two, X );
mov( Radix, Y );
if( cmp(Radix,One) != 0 )
	{
	lngint = NoTrials;
	LTOF( &lngint, t );
	do
		{
		mov( Y, X );
		mul( Radix, Y, Y );
		sub( X, Y, t2 );
		}
	while( ! (cmp(t2,t) >= 0) );
	}
mul( X, U2, OneUlp );
I = 1;
while(I < 10)
	{
	add( X, One, X );
	SqXMinX( Defect );
	if( cmp(J,Zero) > 0 )
		break;
	I = I + 1;
	}
printf("Test for sqrt monotonicity.\n");
I = - 1;
mov( BMinusU2, X );
mov( Radix, Y );
mul( Radix, U2, Z );
add( Radix, Z, Z );
NotMonot = False;
Monot = False;
while( ! (NotMonot || Monot))
	{
	I = I + 1;
	SQRT(X, X);
	SQRT(Y,Q);
	SQRT(Z,Z);
	if( (cmp(X,Q) > 0) || (cmp(Q,Z) > 0) )
		NotMonot = True;
	else
		{
		add( Q, Half, Q );
		FLOOR( Q, Q );
		mul( Q, Q, t );
		if( (I > 0) || (cmp(Radix,t) == 0) )
			Monot = True;
		else if (I > 0)
			{
			if(I > 1)
				Monot = True;
			else
				{
				mul( Y, BInvrse, Y );
				sub( U1, Y, X );
				add( Y, U1, Z );
				}
			}
		else
			{
			mov( Q, Y );
			sub( U2, Y, X );
			add( Y, U2, Z );
			}
		}
	}
if( Monot )
	printf("sqrt has passed a test for Monotonicity.\n");
else
	{
	ErrCnt[Defect] += 1;
	printf("sqrt(X) is non-monotonic for X near " );
	show(Y);
	}
/*=============================================*/
Milestone = 80;
/*=============================================*/
add( MinSqEr, Half, MinSqEr );
sub( Half, MaxSqEr, MaxSqEr);
/*Y = (SQRT(One + U2) - One) / U2;*/
add( One, U2, Sqarg );
SQRT( Sqarg, Y );
sub( One, Y, Y );
div( U2, Y, Y );
/*SqEr = (Y - One) + U2 / Eight;*/
sub( One, Y, t );
div( Eight, U2, SqEr );
add( t, SqEr, SqEr );
Showsq( 1 );
div( Eight, U2, SqEr );
add( Y, SqEr, SqEr );
Showsq( -1 );
/*Y = ((SQRT(F9) - U2) - (One - U2)) / U1;*/
mov( F9, Sqarg );
SQRT( Sqarg, Y );
sub( U2, Y, Y );
sub( U2, One, t );
sub( t, Y, Y );
div( U1, Y, Y );
div( Eight, U1, SqEr );
add( Y, SqEr, SqEr );
Showsq( 1 );
/*SqEr = (Y + One) + U1 / Eight;*/
div( Eight, U1, t );
add( Y, One, SqEr );
add( SqEr, t, SqEr );
Showsq( -1 );
mov( U2, OneUlp );
mov( OneUlp, X );
for( Indx = 1; Indx <= 3; ++Indx)
	{
/*Y = SQRT((X + U1 + X) + F9);*/
	add( X, U1, Y );
	add( Y, X, Y );
	add( Y, F9, Y );
	mov( Y, Sqarg );
	SQRT( Sqarg, Y );
/*Y = ((Y - U2) - ((One - U2) + X)) / OneUlp;*/
	sub( U2, One, t );
	add( t, X, t );
	sub( U2, Y, Y );
	sub( t, Y, Y );
	div( OneUlp, Y, Y );
/*Z = ((U1 - X) + F9) * Half * X * X / OneUlp;*/
	sub( X, U1, t );
	add( t, F9, t );
	mul( t, Half, t );
	mul( t, X, t );
	mul( t, X, t );
	div( OneUlp, t, Z );
	add( Y, Half, SqEr );
	add( SqEr, Z, SqEr );
	Showsq( -1 );
	sub( Half, Y, SqEr );
	add( SqEr, Z, SqEr );
	Showsq( 1 );
	if(((Indx == 1) || (Indx == 3))) 
		{
/*X = OneUlp * Sign (X) * FLOOR(Eight / (Nine * SQRT(OneUlp)));*/
		mov( OneUlp, Sqarg );
		SQRT( Sqarg, t );
		mul( Nine, t, t );
		div( t, Eight, t );
		FLOOR( t, t );
		Sign( X, t2 );
		mul( t2, t, t );
		mul( OneUlp, t, X );
		}
	else
		{
		mov( U1, OneUlp );
		mov( OneUlp, X );
		neg( X );
		}
	}
/*=============================================*/
Milestone = 85;
/*=============================================*/
SqRWrng = False;
Anomaly = False;
if( cmp(Radix,One) != 0 )
	{
	printf("Testing whether sqrt is rounded or chopped.\n");
/*D = FLOOR(Half + POW(Radix, One + Precision - FLOOR(Precision)));*/
	FLOOR( Precision, t2 );
	add( One, Precision, t );
	sub( t2, t, t );
	POW( Radix, t, D );
	add( Half, D, D );
	FLOOR( D, D );
/* ... == Radix^(1 + fract) if (Precision == Integer + fract. */
	div( Radix, D, X );
	div( A1, D, Y );
	FLOOR( X, t );
	FLOOR( Y, t2 );
	if( (cmp(X,t) != 0) || (cmp(Y,t2) != 0) )
		{
		Anomaly = True;
		printf( "Anomaly 1\n" );
		}
	else
		{
		mov( Zero, X );
		mov( X, Z2 );
		mov( One, Y );
		mov( Y, Y2 );
		sub( One, Radix, Z1 );
		mul( Four, D, FourD );
		do
			{
			if( cmp(Y2,Z2) >0 )
				{
				mov( Radix, Q );
				mov( Y, Y1 );
				do
					{
/*X1 = FABS(Q + FLOOR(Half - Q / Y1) * Y1);*/
					div( Y1, Q, t );
					sub( t, Half, t );
					FLOOR( t, t );
					mul( t, Y1, t );
					add( Q, t, X1 );
					FABS( X1 );
					mov( Y1, Q );
					mov( X1, Y1 );
					}
				while( ! (cmp(X1,Zero) <= 0) );
				if( cmp(Q,One) <= 0 )
					{
					mov( Y2, Z2 );
					mov( Y, Z );
					}
				}
			add( Y, Two, Y );
			add( X, Eight, X );
			add( Y2, X, Y2 );
			if( cmp(Y2,FourD) >= 0 )
				sub( FourD, Y2, Y2 );
			}
		while( ! (cmp(Y,D) >= 0) );
		sub( Z2, FourD, X8 );
		mul( Z, Z, Q );
		add( X8, Q, Q );
		div( FourD, Q, Q );
		div( Eight, X8, X8 );
		FLOOR( Q, t );
		if( cmp(Q,t) != 0 )
			{
			Anomaly = True;
			printf( "Anomaly 2\n" );
			}
		else
			{
			Break = False;
			do
				{
				mul( Z1, Z, X );
/*X = X - FLOOR(X / Radix) * Radix;*/
				div( Radix, X, t );
				FLOOR( t, t );
				mul( t, Radix, t );
				sub( t, X, X );
				if( cmp(X,One) == 0 ) 
					Break = True;
				else
					sub( One, Z1, Z1 );
				}
			while( ! (Break || (cmp(Z1,Zero) <= 0)) );
			if( (cmp(Z1,Zero) <= 0) && (! Break))
				{
				printf( "Anomaly 3\n" );
				Anomaly = True;
				}
			else
				{
				if( cmp(Z1,RadixD2) > 0)
					sub( Radix, Z1, Z1 );
				do
					{
					NewD();
					mul( U2, D, t );
					}
				while( ! (cmp(t,F9) >= 0) );
				mul( D, Radix, t );
				sub( D, t, t );
				sub( D, W, t2 );
				if (cmp(t,t2) != 0 )
					{
					printf( "Anomaly 4\n" );
					Anomaly = True;
					}
				else
					{
					mov( D, Z2 );
					I = 0;
					add( One, Z, t );
					mul( t, Half, t );
					add( D, t, Y );
					add( D, Z, X );
					add( X, Q, X );
					SR3750();
					sub( Z, One, t );
					mul( t, Half, t );
					add( D, t, Y );
					add( Y, D, Y );
					sub( Z, D, X );
					add( X, D, X );
					add( X, Q, t );
					add( t, X, X );
					SR3750();
					NewD();
					sub( Z2, D, t );
					sub( Z2, W, t2 );
					if(cmp(t,t2) != 0 )
						{
						printf( "Anomaly 5\n" );
						Anomaly = True;
						}
					else
						{
/*Y = (D - Z2) + (Z2 + (One - Z) * Half);*/
						sub( Z, One, t );
						mul( t, Half, t );
						add( Z2, t, t );
						sub( Z2, D, Y );
						add( Y, t, Y );
/*X = (D - Z2) + (Z2 - Z + Q);*/
						sub( Z, Z2, t );
						add( t, Q, t );
						sub( Z2, D, X );
						add( X, t, X );
						SR3750();
						add( One, Z, Y );
						mul( Y, Half, Y );
						mov( Q, X );
						SR3750();
						if(I == 0)
							{
							printf( "Anomaly 6\n" );
							Anomaly = True;
							}
						}
					}
				}
			}
		}
	if ((I == 0) || Anomaly)
		{
		ErrCnt[Failure] += 1;
		printf( "Anomalous arithmetic with Integer < \n");
		printf("Radix^Precision = " );
		show( W );
		printf(" fails test whether sqrt rounds or chops.\n");
		SqRWrng = True;
		}
	}
if(! Anomaly)
	{
	if(! ((cmp(MinSqEr,Zero) < 0) || (cmp(MaxSqEr,Zero) > 0))) {
	RSqrt = Rounded;
	printf("Square root appears to be correctly rounded.\n");
	}
	else
		{
		k = 0;
		add( MaxSqEr, U2, t );
		sub( Half, U2, t2 );
		if( cmp(t,t2) > 0 )
			k = 1;
		if( cmp( MinSqEr, Half ) > 0 )
			k = 1;
		add( MinSqEr, Radix, t );
		if( cmp( t, Half ) < 0 )
			k = 1;
		if( k == 1 )
			SqRWrng = True;
		else
			{
			RSqrt = Chopped;
			printf("Square root appears to be chopped.\n");
			}
		}
	}
if( SqRWrng )
	{
	printf("Square root is neither chopped nor correctly rounded.\n");
	printf("Observed errors run from " );
	sub( Half, MinSqEr, t );
	show( t );
	printf("\tto " );
	add( Half, MaxSqEr, t );
	show( t );
	printf( "ulps.\n" );
	sub( MinSqEr, MaxSqEr, t );
	mul( Radix, Radix, t2 );
	if( cmp( t, t2 ) >= 0 )
		{
		ErrCnt[Serious] += 1;
		printf( "sqrt gets too many last digits wrong\n");
		}
	}
}

Showsq( arg )
int arg;
{

k = 0;
if( arg <= 0 )
	{
	if( cmp(SqEr,MinSqEr) < 0 )
		{
		k = 1;
		mov( SqEr, MinSqEr );
		}
	}
if( arg >= 0 )
	{
	if( cmp(SqEr,MaxSqEr) > 0 )
		{
		k = 2;
		mov( SqEr, MaxSqEr );
		}
	}
#if DEBUG
if( k != 0 )
	{
	printf( "Square root of " );
	show( arg );
	printf( "\tis in error by " );
	show( SqEr );
	}
#endif
}


pow1test()
{

/*=============================================*/
Milestone = 90;
/*=============================================*/
Pause();
printf("Testing powers Z^i for small Integers Z and i.\n");
N = 0;
/* ... test powers of zero. */
I = 0;
mov( Zero, Z );
neg(Z);
M = 3;
Break = False;
do
	{
	mov( One, X );
	SR3980();
	if(I <= 10)
		{
		I = 1023;
		SR3980();
		}
	if( cmp(Z,MinusOne) == 0 )
		Break = True;
	else
		{
		mov( MinusOne, Z );
		PrintIfNPositive();
		N = 0;
/* .. if(-1)^N is invalid, replace MinusOne by One. */
		I = - 4;
		}
	}
while( ! Break );
PrintIfNPositive();
N1 = N;
N = 0;
mov( A1, Z );
/*M = FLOOR(Two * LOG(W) / LOG(A1));*/
LOG( W, t );
mul( Two, t, t );
FLOOR( t, t );
LOG( A1, t2 );
div( t2, t, t );
FTOL( t, &lngint, t2 );
M = lngint;
Break = False;
do
	{
	mov( Z, X );
	I = 1;
	SR3980();
	if( cmp(Z,AInvrse) == 0 )
		Break = True;
	else
		 mov( AInvrse, Z );
	}
while( ! (Break) );
/*=============================================*/
Milestone = 100;
/*=============================================*/
/*  Powers of Radix have been tested, */
/*         next try a few primes     */

M = NoTrials;

mov( Three, Z );
do
	{
	mov( Z, X );
	I = 1;
	SR3980();
	do
		{
		add( Z, Two, Z );
		div( Three, Z, t );
		FLOOR( t, t );
		mul( Three, t, t );
		}
	while( cmp(t,Z) == 0 );
	mul( Eight, Three, t );
	}
while( cmp(Z,t) < 0 );

if(N > 0)
	{
	printf("Errors like this may invalidate financial calculations\n");
	printf("\tinvolving interest rates.\n");
	}
PrintIfNPositive();
N += N1;
if(N == 0)
	printf("... no discrepancies found.\n");
if(N > 0)
	Pause();
else printf("\n");
}



pow2test()
{
printf("\n");
/* ...calculate Exp2 == exp(2) == 7.38905 60989 30650 22723 04275-... */
mov( Zero, X );
mov( Two, t2 ); /*I = 2;*/

mul( Two, Three, Y );
mov( Zero, Q );
N = 0;
do
	{
	mov( X, Z );
	add( t2, One, t2 ); /*I = I + 1;*/
	add( t2, t2, t );
	div( t, Y, Y ); /*Y = Y / (I + I);*/
	add( Y, Q, R );
	add( Z, R, X );
	sub( X, Z, Q );
	add( Q, R, Q );
	}
while( cmp(X,Z) > 0 );

/*Z = (OneAndHalf + One / Eight) + X / (OneAndHalf * ThirtyTwo);*/
div( Eight, One, t );
add( OneAndHalf, t, Z );
mul( OneAndHalf, ThirtyTwo, t );
div( t, X, t );
add( Z, t, Z );
mul( Z, Z, X );
mul( X, X, Exp2 );
mov( F9, X );
sub( U1, X, Y );
printf("Testing X^((X + 1) / (X - 1)) vs. exp(2) = " );
show( Exp2 );
printf( "\tas X -> 1.\n" );
for(I = 1;;)
	{
	sub( BInvrse, X, Z );
/*Z = (X + One) / (Z - (One - BInvrse));*/
	add( X, One, t2 );
	sub( BInvrse, One, t );
	sub( t, Z, t );
	div( t, t2, Z );
	POW( X, Z, Sqarg );
	sub( Exp2, Sqarg, Q );
	mov( Q, t );
	FABS( t );
	mul( TwoForty, U2, t2 );
	if( cmp( t, t2 ) > 0 )
		{
		N = 1;
		sub( BInvrse, X, V9 );
		sub( BInvrse, One, t );
		sub( t, V9, V9 );
		ErrCnt[Defect] += 1;
		printf( "Calculated " );
		show( Sqarg );
		printf(" for \t(1 + " );
		show( V9 );
		printf( "\tto the power " );
		show( Z );
		printf("\tdiffers from correct value by " );
		show( Q );
		printf("\tThis much error may spoil financial\n");
		printf("\tcalculations involving tiny interest rates.\n");
		break;
		}
	else
		{
		sub( X, Y, Z );
		mul( Z, Two, Z );
		add( Z, Y, Z );
		mov( Y, X );
		mov( Z, Y );
		sub( F9, X, Z );
		mul( Z, Z, Z );
		add( Z, One, Z );
		if( (cmp(Z,One) > 0) && (I < NoTrials) )
			I++;
		else
			{
			if( cmp(X,One) > 0 )
				{
				if(N == 0)
					printf("Accuracy seems adequate.\n");
				break;
				}
			else
				{
				add( One, U2, X );
				add( U2, U2, Y );
				add( X, Y, Y );
				I = 1;
				}
			}
		}
	}
/*=============================================*/
Milestone = 150;
/*=============================================*/
printf("Testing powers Z^Q at four nearly extreme values.\n");
N = 0;
mov( A1, Z );
/*Q = FLOOR(Half - LOG(C) / LOG(A1));*/
LOG( C, t );
LOG( A1, t2 );
div( t2, t, t );
sub( t, Half, t );
FLOOR( t, Q );
Break = False;
do
	{
	mov( CInvrse, X );
	POW( Z, Q, Y );
	IsYeqX();
	neg(Q);
	mov( C, X );
	POW( Z, Q, Y );
	IsYeqX();
	if( cmp(Z,One) < 0 )
		Break = True;
	else
		mov( AInvrse, Z );
	}
while( ! (Break));
PrintIfNPositive();
if(N == 0)
	printf(" ... no discrepancies found.\n");
printf("\n");
}
