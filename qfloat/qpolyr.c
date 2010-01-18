/* Arithmetic operations on polynomials
 *
 * In the following descriptions a, b, c are polynomials of degree
 * na, nb, nc respectively.  The degree of a polynomial cannot
 * exceed a run-time value MAXPOL.  An operation that attempts
 * to use or generate a polynomial of higher degree may produce a
 * result that suffers truncation at degree MAXPOL.  The value of
 * MAXPOL is set by calling the function
 *
 *     polini( maxpol );
 *
 * where maxpol is the desired maximum degree.  This must be
 * done prior to calling any of the other functions in this module.
 * Memory for internal temporary polynomial storage is allocated
 * by polini().
 *
 * Each polynomial is represented by an array containing its
 * coefficients, together with a separately declared integer equal
 * to the degree of the polynomial.  The coefficients appear in
 * ascending order; that is,
 *
 *                                        2                      na
 * a(x)  =  a[0]  +  a[1] * x  +  a[2] * x   +  ...  +  a[na] * x  .
 *
 *
 *
 * sum = poleva( a, na, x );	Evaluate polynomial a(t) at t = x.
 * polprt( a, na, D );		Print the coefficients of a to D digits.
 * polclr( a, na );		Set a identically equal to zero, up to a[na].
 * polmov( a, na, b );		Set b = a.
 * poladd( a, na, b, nb, c );	c = b + a, nc = max(na,nb)
 * polsub( a, na, b, nb, c );	c = b - a, nc = max(na,nb)
 * polmul( a, na, b, nb, c );	c = b * a, nc = na+nb
 *
 *
 * Division:
 *
 * i = poldiv( a, na, b, nb, c );	c = b / a, nc = MAXPOL
 *
 * returns i = the degree of the first nonzero coefficient of a.
 * The computed quotient c must be divided by x^i.  An error message
 * is printed if a is identically zero.
 *
 *
 * Change of variables:
 * If a and b are polynomials, and t = a(x), then
 *     c(t) = b(a(x))
 * is a polynomial found by substituting a(x) for t.  The
 * subroutine call for this is
 *
 * polsbt( a, na, b, nb, c );
 *
 *
 * Notes:
 * poldiv() is an integer routine; poleva() is double.
 * Any of the arguments a, b, c may refer to the same array.
 *
 */

#include "qhead.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct{
	short n[NQ];
	short d[NQ];
	}qfract;

extern short qone[];

/* near pointer version of malloc() */
#define malloc _nmalloc
#define free _nfree

/* Pointers to internal arrays.  Note poldiv() allocates
 * and deallocates some temporary arrays every time it is called.
 */
static qfract *pt1 = 0;
static qfract *pt2 = 0;
static qfract *pt3 = 0;

/* Maximum degree of polynomial. */
int MAXPOL = 0;
extern int MAXPOL;

/* Number of bytes (chars) in maximum size polynomial. */
static int psize = 0;


/* Initialize max degree of polynomials
 * and allocate temporary storage.
 */
polini( maxdeg )
int maxdeg;
{
void *malloc();
int m;

MAXPOL = maxdeg;
psize = (maxdeg + 1) * sizeof(qfract);

/* Release previously allocated memory, if any. */
if( pt3 )
	free(pt3);
if( pt2 )
	free(pt2);
if( pt1 )
	free(pt1);

/* Allocate new arrays */
pt1 = (qfract * )malloc(psize); /* used by polsbt */
pt2 = (qfract * )malloc(psize); /* used by polsbt */
pt3 = (qfract * )malloc(psize); /* used by polmul */

/* Report if failure */
if( (pt1 == NULL) || (pt2 == NULL) || (pt3 == NULL) )
	{
	printf( "Unable to allocate polynomial storage\n" );
	exit(1);
	}
}



/* Print the coefficients of a, with d decimal precision.
 */
static char form[128] = {0};

polprt( a, na, d )
qfract a[];
int na, d;
{
int i, j, d1;
qfract *p;


/* Now do the printing.
 */
p = a;
for( i=0; i<=na; i++ )
	{
	printf( "%d\n", i );
	asctoq( p->n, 43, form );
	printf( "%s\n", form );
	asctoq( p->d, 43, form );
	printf( "%s\n\n", form );
	++p;
	}
}



/* Set a = 0.
 */
polclr( a, n )
qfract *a;
int n;
{
int i;
qfract *p;

if( n > MAXPOL )
	n = MAXPOL;

p = a;
for( i=0; i<=n; i++ )
	{
	qclear( a->n );
	qmov( qone, a->d );
	++a;
	}
}



/* Set b = a.
 */
polmov( a, na, b )
qfract *a, *b;
int na;
{
int i;
qfract *p, *q;

if( na > MAXPOL )
	na = MAXPOL;

p = a;
q = b;
for( i=0; i<= na; i++ )
	{
	qmov( p->n, q->n );
	qmov( p->d, q->d );
	++p;
	++q;
	}
}


/* c = b * a.
 */
polmul( a, na, b, nb, c )
qfract a[], b[], c[];
int na, nb;
{
int i, j, k, nc;
qfract temp;
qfract *p;

nc = na + nb;
polclr( pt3, MAXPOL );

p = &a[0];

for( i=0; i<=na; i++ )
	{
	for( j=0; j<=nb; j++ )
		{
		k = i + j;
		if( k > MAXPOL )
			break;
		qrmul( p, &b[j], &temp ); /* pt3[k] += a[i] * b[j];*/
		qradd( &temp, &pt3[k], &pt3[k] );
		}
	++p;
	}

if( nc > MAXPOL )
	nc = MAXPOL;
for( i=0; i<=nc; i++ )
	{
	qmov( pt3[i].n, c[i].n ); /* c[i] = pt3[i]; */
	qmov( pt3[i].d, c[i].d );
	}
}



 
/* c = b + a.
 */
poladd( a, na, b, nb, c )
qfract a[], b[], c[];
int na, nb;
{
int i, n;


if( na > nb )
	n = na;
else
	n = nb;

if( n > MAXPOL )
	n = MAXPOL;

for( i=0; i<=n; i++ )
	{
	if( i > na )
		{
		qmov( b[i].n, c[i].n );	/*c[i] = b[i];*/
		qmov( b[i].d, c[i].d );
		}
	else if( i > nb )
		{
		qmov( a[i].n, c[i].n );	/*c[i] = a[i];*/
		qmov( a[i].d, c[i].d );
		}
	else
		{
		qradd( &a[i], &b[i], &c[i] );	/*c[i] = b[i] + a[i];*/
		}
	}
}

/* c = b - a.
 */
polsub( a, na, b, nb, c )
qfract a[], b[], c[];
int na, nb;
{
int i, n;


if( na > nb )
	n = na;
else
	n = nb;

if( n > MAXPOL )
	n = MAXPOL;

for( i=0; i<=n; i++ )
	{
	if( i > na )
		{
		qmov( b[i].n, c[i].n );	/*c[i] = b[i];*/
		qmov( b[i].d, c[i].d );
		}
	else if( i > nb )
		{
		qmov( a[i].n, c[i].n );	/*c[i] = -a[i];*/
		qneg( c[i].n );
		qmov( a[i].d, c[i].d );
		}
	else
		{
		qrsub( &a[i], &b[i], &c[i] );	/*c[i] = b[i] - a[i];*/
		}
	}
}



/* c = b/a
 */
poldiv( a, na, b, nb, c )
qfract a[], b[], c[];
int na, nb;
{
qfract quot;
qfract temp;
qfract *ta, *tb, *tq;
int i, j, k, sing;

sing = 0;

/* Allocate temporary arrays.  This would be quicker
 * if done automatically on the stack, but stack space
 * may be hard to obtain on a small computer.
 */
ta = (qfract * )malloc( psize );
polclr( ta, MAXPOL );
polmov( a, na, ta );

tb = (qfract * )malloc( psize );
polclr( tb, MAXPOL );
polmov( b, nb, tb );

tq = (qfract * )malloc( psize );
polclr( tq, MAXPOL );

/* What to do if leading (constant) coefficient
 * of denominator is zero.
 */
if( a[0].n[1] == 0 )
	{
	for( i=0; i<=na; i++ )
		{
		if( ta[i].n[1] != 0 )
			goto nzero;
		}
	printf( "poldiv divide by zero\n" );
	goto done;

nzero:
/* Reduce the degree of the denominator. */
	for( i=0; i<na; i++ )
		{
		qmov( ta[i+1].n, ta[i].n );	/*ta[i] = ta[i+1];*/
		qmov( ta[i+1].d, ta[i].d );
		}
	qclear( ta[na].n );	/*ta[na] = 0.0;*/
	qmov( qone, ta[na].d );
	if( b[0].n[0] != 0 )
		{
/* Optional message:
		printf( "poldiv singularity, divide quotient by x\n" );
*/
		sing += 1;
		}
	else
		{
/* Reduce degree of numerator. */
		for( i=0; i<nb; i++ )
			{
			qmov( tb[i+1].n, tb[i].n );	/*tb[i] = tb[i+1];*/
			qmov( tb[i+1].d, tb[i].d );
			}
		qclear( tb[nb].n );	/*tb[nb] = 0.0;*/
		qmov( qone, tb[nb].d );
		}
/* Call self, using reduced polynomials. */
	sing += poldiv( ta, na, tb, nb, c );
	goto done;
	}

/* Long division algorithm.  ta[0] is nonzero.
 */
for( i=0; i<=MAXPOL; i++ )
	{
	qrdiv( &ta[0], &tb[0], &quot ); /*quot = tb[i]/ta[0];*/
	for( j=0; j<=MAXPOL; j++ )
		{
		k = j + i;
		if( k > MAXPOL )
			break;
		qrmul( &ta[j], &quot, &temp ); /*tb[k] -= quot * ta[j];*/
		qrsub( &temp, &tb[k], &tb[k] );
		}
	qmov( quot.n, tq[i].n ); /*tq[i] = quot;*/
	qmov( quot.d, tq[i].d );
	}
/* Send quotient to output array. */
polmov( tq, MAXPOL, c );

done:

/* Restore allocated memory. */
free(tq);
free(tb);
free(ta);
return( sing );
}




/* Change of variables
 * Substitute a(y) for the variable x in b(x).
 * x = a(y)
 * c(x) = b(x) = b(a(y)).
 */

polsbt( a, na, b, nb, c )
qfract a[], b[], c[];
int na, nb;
{
int i, j, k, n2;
qfract temp;
qfract *p;

/* 0th degree term:
 */
polclr( pt1, MAXPOL );
qmov( b[0].n, pt1[0].n ); /*pt1[0] = b[0];*/
qmov( b[0].d, pt1[0].d );

polclr( pt2, MAXPOL );
qmov( qone, pt2[0].n ); /*pt2[0] = 1.0;*/
qmov( qone, pt2[0].d );
n2 = 0;

p = &b[1];

for( i=1; i<=nb; i++ )
	{
/* Form ith power of a. */
	polmul( a, na, pt2, n2, pt2 );
	n2 += na;
/* Add the ith coefficient of b times the ith power of a. */
	for( j=0; j<=n2; j++ )
		{
		if( j > MAXPOL )
			break;
		qrmul( p, &pt2[j], &temp ); /*pt1[j] += b[i] * pt2[j];*/
		qradd( &pt1[j], &temp, &pt1[j] );
		}
	++p;
	}

k = n2 + nb;
if( k > MAXPOL )
	k = MAXPOL;
for( i=0; i<=k; i++ )
	{
	qmov( pt1[i].n, c[i].n ); /*c[i] = pt1[i];*/
	qmov( pt1[i].d, c[i].d );
	}
}




/* Evaluate polynomial a(t) at t = x.
 */
double poleva( a, na, x, s )
qfract a[];
int na;
qfract *x;
qfract *s;
{
qfract temp;
int i;

qmov( a[na].n, s->n ); /*s = a[na];*/
qmov( a[na].d, s->d );

for( i=na-1; i>=0; i-- )
	{
	qrmul( &s, &x, &temp ); /*s = s * x + a[i];*/
	qradd( &a[i], &temp, &s );
	}
}

