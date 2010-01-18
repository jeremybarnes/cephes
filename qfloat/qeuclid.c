/* Rational arithmetic routines
 *
 * radd( a, b, c )	c = b + a
 * rsub( a, b, c )	c = b - a
 * rmul( a, b, c )	c = b * a
 * rdiv( a, b, c )	c = b / a
 * euclid( n, d )	Reduce n/d to lowest terms, return g.c.d.
 *
 * Note: arguments are assumed,
 * without checking,
 * to be integer valued.
 */

#include "qhead.h"
#include <stdlib.h>

/* Integer overflow threshold */
#define BIG (EXPONE + NBITS)
/*
typedef struct
	{
	QELT n[NQ];
	QELT d[NQ];
	}qfract;
*/
static QELT gcd[NQ];
static QELT d1[NQ];
static QELT d2[NQ];
static QELT gcn[NQ];
static QELT n1[NQ];
static QELT n2[NQ];

extern QELT qone[];
int qeuclid();


/* Add fractions. */

int qradd( ff1, ff2, ff3 )
qfract *ff1, *ff2, *ff3;
{

qmov( ff1->n, n1 );
qmov( ff1->d, d1 );
qmov( ff2->n, n2 );
qmov( ff2->d, d2 );
if( n1[1] == 0 )
	{
	qmov( n2, ff3->n );
	qmov( d2, ff3->d );
	return 0;
	}
if( n2[1] == 0 )
	{
	qmov( n1, ff3->n );
	qmov( d1, ff3->d );
	return 0;
	}
qeuclid( d1, d2, gcd );
qeuclid( n1, n2, gcn );

/* f3->n = (n2 * d1 + n1 * d2) * gcn; */
qmul( n2, d1, n2 );
qmul( n1, d2, n1 );
qadd( n1, n2, n2 );
qmul( gcn, n2, ff3->n );

/* f3->d = d1 * d2 * gcd;*/
qmul( d1, d2, d2 );
qmul( d2, gcd, ff3->d );
qeuclid( ff3->n, ff3->d, gcd );
return 0;
}




/* Subtract fractions. */

int qrsub( ff1, ff2, ff3 )
qfract *ff1, *ff2, *ff3;
{

qmov( ff1->n, n1 );
qmov( ff1->d, d1 );
qmov( ff2->n, n2 );
qmov( ff2->d, d2 );
if( n1[1] == 0 )
	{
	qmov( n2, ff3->n );
	qmov( d2, ff3->d );
	return 0;
	}
if( n2[1] == 0 )
	{
	qneg( n1 );
	qmov( n1, ff3->n );
	qmov( d1, ff3->d );
	return 0;
	}
qeuclid( d1, d2, gcd );
qeuclid( n1, n2, gcn );

/* f3->n = (n2 * d1 - n1 * d2) * gcn; */
qmul( n2, d1, n2 );
qmul( n1, d2, n1 );
qsub( n1, n2, n2 );
qmul( gcn, n2, ff3->n );

/* f3->d = d1 * d2 * gcd;*/
qmul( d1, d2, d2 );
qmul( d2, gcd, ff3->d );
qeuclid( ff3->n, ff3->d, gcd );
return 0;
}






/* Multiply fractions. */

int qrmul( ff1, ff2, ff3 )
qfract *ff1, *ff2, *ff3;
{

qmov( ff1->n, n1 );
qmov( ff1->d, d1 );
qmov( ff2->n, n2 );
qmov( ff2->d, d2 );
if( (n1[1] == 0) || (n2[1] == 0) )
	{
	qclear( ff3->n );
	qmov( qone, ff3->d );
	return 0;
	}

qeuclid( n1, d2, gcd ); /* cross cancel any common divisors */
qeuclid( n2, d1, gcd );
qmul( n1, n2, ff3->n );
qmul( d1, d2, ff3->d );

/* Check for overflow. */
if( (ff3->n[1] >= (QELT) BIG) || (ff3->d[1] >= (QELT) BIG) )
	{
	mtherr( "qrmul", OVERFLOW );
	exit(0);	/* terminate program */
	}
return 0;
}




/* Divide fractions. */

int qrdiv( ff1, ff2, ff3 )
qfract *ff1, *ff2, *ff3;
{

/* Invert ff1, then multiply */
qmov( ff1->d, n1 );
qmov( ff1->n, d1 );
if( n1[0] != 0 )
	{ /* keep denominator positive */
	qneg(n1);
	qneg(d1);
	}
qmov( ff2->n, n2 );
qmov( ff2->d, d2 );
if( (n1[1] == 0) || (n2[1] == 0) )
	{
	qclear( ff3->n );
	qmov( qone, ff3->d );
	return 0;
	}
qeuclid( n1, d2, gcd ); /* cross cancel any common divisors */
qeuclid( n2, d1, gcd );
qmul( n1, n2, ff3->n );
qmul( d1, d2, ff3->d );

/* Check for overflow. */
if( (ff3->n[1] >= (QELT) BIG) || (ff3->d[1] >= (QELT) BIG) )
	{
	mtherr( "qrdiv", OVERFLOW );
	exit(0);	/* terminate program */
	}
return 0;
}





/* Euclidean algorithm
 *   reduces fraction to lowest terms,
 *   returns greatest common divisor.
 */



int qeuclid( num, den, gcda )
QELT *num, *den, *gcda;
{
QELT nn[NQ], dd[NQ], q[NQ], r[NQ];


/* Numerator. */
qmov( num, nn );
/* Denominator. */
qmov( den, dd );

/* Make numbers positive, locally. */
nn[0] = 0;
dd[0] = 0;

/* Abort if numbers are too big for integer arithmetic. */
if( (nn[1] >= (QELT) BIG) || (dd[1] >= (QELT) BIG) )
	{
	mtherr( "qeuclid", OVERFLOW );
	exit(0);	/* terminate program */
	qmov( qone, gcda );
	return 0;
	}


/* Divide by zero, gcd = 1. */
if( dd[1] <= (QELT) (EXPONE - 1) )
	{
	qmov( qone, gcda );
	return 0;
	}

/* Zero. Return 0/1, gcd = denominator. */
if( nn[1] <= (QELT) (EXPONE - 1) )
	{
	qmov( qone, den );
	qmov( dd, gcda );
	return 0;
	}

while( dd[1] > (QELT) (EXPONE - 1) )
	{
/* Find integer part of n divided by d. */
	qdiv( dd, nn, r );
	qfloor( r, q );
/* Find remainder = n - d*q after dividing n by d. */
	qmul( dd, q, q );
	qsub( q, nn, r );
/* The next fraction is d/r. */
	qmov( dd, nn );
	qmov( r, dd );
	}

qdiv( nn, num, num );
qdiv( nn, den, den );
qmov( nn, gcda );
return 0;
}
