/*	qsqrta.c		*/
/* Square root check routine, done by long division. */
/* Copyright (C) 1984-1988 by Stephen L. Moshier. */


#include "qhead.h"
#include "mconf.h"

extern QELT qzero[], qone[];
static QELT rndbit[NQ+1] = {0};
static QELT rndset = 0;
#ifdef ANSIPROT
int addm( QELT *, QELT * );
int subm( QELT *, QELT * );
int cmpm( QELT *, QELT * );
int qmovz( QELT *, QELT * );
int shup1( QELT * ), shdn1( QELT * );
int mdnorm( QELT *, int );
#else
int mtherr(), qmovz(), shdn1(), shup1(), addm(), cmpm(), subm(), mdnorm();
#endif

int qsqrt( x, y )
QELT *x, *y;
{
QELT temp[NQ+1], num[NQ+1], sq[NQ+1], xx[NQ+1];
int i, j, k, n;
long m, exp;

if( rndset == 0 )
	{
	qclear( rndbit );
	rndbit[NQ] = 0;
	rndbit[NQ-1] = 1;
	rndset = 1;
	}
/* Check for arg <= 0 */
if( x[0] != 0 )
	{
	if( qcmp( qzero, x ) == 0 )
		{
		qclear(y);
		qneg(y);
		}
	else
		{
		mtherr( "qsqrt", DOMAIN );
		qclear(y);
		}
	return 0;
	}
if( x[1] == 0 )
	{
	qclear(y);
	return 0;
	}

/* Bring in the arg and renormalize if it is denormal. */
qmovz( x, xx );
m = xx[1]; /* local long word exponent */
/*
if( m == 0 )
	m -= normlz( xx );
*/
/* Divide exponent by 2 */
m -= EXPONE - 1;
exp = (QELT) ( (m / 2) + (EXPONE - 1) );

/* Adjust if exponent odd */
if( (m & 1) != 0 )
	{
	if( m > 0 )
		exp += 1;
	shdn1( xx );
	}

qclear( sq );
sq[NQ] = 0;
qclear( num );
num[NQ] = 0;
n = WORDSIZE / 2;

for( j=0; j<2*NQ-6; j++ )
	{
/* bring in next word of arg */
	if( j <= NQ-3 )
		num[NQ] = xx[j+3];
/* Do additional bit on last outer loop, for roundoff. */
	if( j == 2*NQ-7 )
		n = WORDSIZE / 2  + 1;
	for( i=0; i<n; i++ )
		{
/* Next 2 bits of arg */
		shup1( num );
		shup1( num );
/* Shift up answer */
		shup1( sq );
/* Make trial divisor */
		for( k=0; k<=NQ; k++ )
			temp[k] = sq[k];
		shup1( temp );
		addm( rndbit, temp );
/* Subtract and insert answer bit if it goes in */
		if( cmpm( temp, num ) <= 0 )
			{
			subm( temp, num );
			sq[NQ-1] |= 1;
			}
		}
	}

exp -= 1; /* Adjust for extra, roundoff loop done. */
sq[1] = exp;

/* Sticky bit = 1 if the remainder is nonzero. */
k = 0;
for( i=3; i<=NQ; i++ )
	k |= num[i];

/* Renormalize and round off. */
mdnorm( sq, k );
qmov( sq, y );
return 0;
}
