/*							qpsi.c
 *	Psi (digamma) function check routine
 *
 *
 * SYNOPSIS:
 *
 * int qpsi( x, y );
 * QELT *x, *y;
 *
 * qpsi( x, y );
 *
 *
 * DESCRIPTION:
 *
 *              d      -
 *   psi(x)  =  -- ln | (x)
 *              dx
 *
 * is the logarithmic derivative of the gamma function.
 * For general positive x, the argument is made greater than 16
 * using the recurrence  psi(x+1) = psi(x) + 1/x.
 * Then the following asymptotic expansion is applied:
 *
 *                           inf.   B
 *                            -      2k
 * psi(x) = log(x) - 1/2x -   >   -------
 *                            -        2k
 *                           k=1   2k x
 *
 * where the B2k are Bernoulli numbers.
 *
 * psi(-x)  =  psi(x+1) + pi/tan(pi(x+1))
 *
 *
 * ACCURACY:
 *
 * Accuracy is about 36 decimals.
 *
 */

/*
Cephes Math Library Release 2.2:  July, 1992
Copyright 1984, 1987, 1992, 1995 by Stephen L. Moshier
*/

#include "qhead.h"
extern QELT qpi[], qeul[];

/* Expansion coefficients  */
#define NP 12
char *pstrs[3*NP] = {
 "-236364091","2730","24",
"854513","138","22",
"-174611","330","20",
"43867","798","18",
"-3617","510","16",
"7","6","14",
"-691","2730","12",
"5","66","10",
"-1","30","8",
"1","42","6",
"-1","30","4",
"1","6","2",
};

static QELT pcof[NP][NQ];

/* constants for asymptote polynomial */
#if 0
/* 8.3333333333333333333333333333333333333333333333333E-2 */
/* 1/(6*2) */
static QELT q12[NQ] = {
0,0x3ffd,0,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,
0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaab};
/* -8.3333333333333333333333333333333333333333333333333E-3 */
/* -1/(30*4) */
static QELT q11[NQ] = {
-1,0x3ffa,0,0x88888888,0x88888888,0x88888888,0x88888888,0x88888888,
0x88888888,0x88888888,0x88888888,0x88888888,0x88888888,0x88888889};
/* 3.96825396825396825396825396825396825396825396825397E-3 */
/* 1/(42*6) */
static QELT q10[NQ] = {
0,0x3ff9,0,0x82082082,0x82082082,0x82082082,0x82082082,0x82082082,
0x82082082,0x82082082,0x82082082,0x82082082,0x82082082,0x82082083};
/* -4.16666666666666666666666666666666666666666666666667E-3 */
/* -1/(30*8) */
static QELT q9[NQ] = {
-1,0x3ff9,0,0x88888888,0x88888888,0x88888888,0x88888888,0x88888888,
0x88888888,0x88888888,0x88888888,0x88888888,0x88888888,0x88888889};
/* 7.57575757575757575757575757575757575757575757575758E-3 */
/* 5/(66*10) */
static QELT q8[NQ] = {
0,0x3ff9,0,0xf83e0f83,0xe0f83e0f,0x83e0f83e,0x0f83e0f8,0x3e0f83e0,
0xf83e0f83,0xe0f83e0f,0x83e0f83e,0x0f83e0f8,0x3e0f83e0,0xf83e0f84};

/* -2.1092796092796092796092796092796092796092796092796E-2 */
/* -691/(2730*12) */
static QELT q7[NQ] = {
-1,0x3ffb,0,0xaccaccac,0xcaccacca,0xccaccacc,0xaccaccac,0xcaccacca,
0xccaccacc,0xaccaccac,0xcaccacca,0xccaccacc,0xaccaccac,0xcaccaccb,};
/* 8.3333333333333333333333333333333333333333333333333E-2 */
/* 7/(6*14) */
static QELT q6[NQ] = {
0,0x3ffd,0,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,
0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaab};
/* -4.4325980392156862745098039215686274509803921568627E-1 */
/* -3617/(510*16) */
static QELT q5[NQ] = {
-1,0x3fff,0,0xe2f2f2f2,0xf2f2f2f2,0xf2f2f2f2,0xf2f2f2f2,0xf2f2f2f2,
 0xf2f2f2f2,0xf2f2f2f2,0xf2f2f2f2,0xf2f2f2f2,0xf2f2f2f2,0xf2f2f2f3};
/* 3.0539543302701197438039543302701197438039543302701E+0 */
/* 43867/(798*18) */
static QELT q4[NQ] = {
0,0x4002,0,0xc373fcdc,0xff373fcd,0xcff373fc,0xdcff373f,0xcdcff373,
0xfcdcff37,0x3fcdcff3,0x73fcdcff,0x373fcdcf,0xf373fcdc,0xff373fce};
/* -2.6456212121212121212121212121212121212121212121212E+1 */
/* -174611/(330*20) */
static QELT q3[NQ] = {
-1,0x4005,0,0xd3a6528a,0x6528a652,0x8a6528a6,0x528a6528,0xa6528a65,
0x28a6528a,0x6528a652,0x8a6528a6,0x528a6528,0xa6528a65,0x28a6528a};
/* 2.8146014492753623188405797101449275362318840579710E+2 */
/* 854513/(138*22) */
static QELT q2[NQ] = {
0,0x4009,0,0x8cbae607,0x6b981dae,0x6076b981,0xdae6076b,0x981dae60,
0x76b981da,0xe6076b98,0x1dae6076,0xb981dae6,0x076b981d,0xae6076ba};
/* -3.6075105463980463980463980463980463980463980463980E+3 */
/* -236364091/(2730*24) */
static QELT q1[NQ] = {
-1,0x400c,0,0xe1782b32,0xb32b32b3,0x2b32b32b,0x32b32b32,0xb32b32b3,
0x2b32b32b,0x32b32b32,0xb32b32b3,0x2b32b32b,0x32b32b32,0xb32b32b3};
/* 5.4827583333333333333333333333333333333333333333333E+4 */
/* 8553103/(6*26) */
/*
0,0x4010,0,0xd62b9555,0x55555555,0x55555555,0x55555555,0x55555555,
0x55555555,0x55555555,0x55555555,0x55555555,0x55555555,0x55555555};
*/
#endif

extern QELT q32[], qone[], qhalf[];
int asctoq(), qtan();

/* Initialize coefficients from ASCII strings.  */
static int qpsiini = 0;

static int initqpsi()
{
QELT den[NQ], temp[NQ];
int i, k;

k = 0;
for( i=0; i<NP; i++ )
  {
   /* One numerator coefficient */
    asctoq( pstrs[k++], pcof[i] );
   /* Two denominator factors */
    asctoq( pstrs[k++], temp );
    asctoq( pstrs[k++], den );
    qmul( den, temp, den );
    qdiv( den, pcof[i], pcof[i] );
  }
qpsiini = 1;
return 0;
}



int qpsi( x, y )
QELT *x, *y;
{
QELT v[NQ], w[NQ], g[NQ], xx[NQ], t[NQ];
int i;

if (qpsiini == 0)
  initqpsi();

if( x[0] != 0 )
	{
/* psi(-x)  =  psi(x+1) + pi/tan(pi(x+1)) */
	qmov( x, xx );
	xx[0] = 0;
	qadd( qone, xx, xx );
	qpsi( xx, t );
	qmul( xx, qpi, g );
	qtan( g, g );
	qdiv( g, qpi, w );
	qadd( t, w, y );
	return 0;
	}
/* range reduction: transform argument to be greater than 32 */
qmov( x, xx );
qclear(v);

while( xx[1] <= (QELT) (EXPONE + 4) )	/* exponent of 16 */
	{
	qdiv( xx, qone, w );
	qadd( w, v, v );
	qadd( qone, xx, xx );
	}

/*  Asymptotic polynomial in 1/x**2 for Stirling's formula	*/
qmul( xx, xx, w );
qdiv( w, pcof[0], g );
for( i = 1; i <= 11; i++ )
  {
    qadd( g, pcof[i], g );
    qdiv( w, g, g );
  }
/* g + 1/2x - log(x) */
qdiv( xx, qone, w );
w[1] -= 1;
qadd( w, g, g );
qlog( xx, w );
qsub( w, g, g );
/* negate */
qneg(g);
/* We now have psi(xx). Subtract sum of reciprocals. */
qsub( v, g, y );
return 0;
}
