/* Factorial function */

#include "qhead.h"

extern QELT qone[];

int qfac( x, y )
QELT x[], y[];
{
QELT p[NQ], j[NQ];
long i, n;
double dp, dj;
union
  {
    unsigned short s[4];
    double d;
  } dn;

if( x[0] != 0 )
	{
	qinfin( y );
	mtherr( "qfac", DOMAIN );
	return 0;
	}


qtoe( x, dn.s );
n = dn.d;

if( n == 0 )
	{
	qmov( qone, y );
	return 0;
	}

if( n == 1 )
	{
	qmov( qone, y );
	return 0;
	}

if( n > 1754 )
	{
	qinfin(y);
	mtherr( "qfac", OVERFLOW );
	return 0;
	}

/* Cheat by using normal arithmetic */
dp = 1.0;
dj = 1.0;
i = 1;
do
	{
	if( i > 17 )
		goto fmore;
	i += 1;
	dj += 1.0;
	dp *= dj;
	}
while( i < n );

dn.d = dp;
etoq( dn.s, y );
return 0;


fmore:

dn.d = dj;
etoq( dn.s, j );
dn.d = dp;
etoq( dn.s, p );

do
	{
	i += 1;
	qadd( qone, j, j );
	qmuli( j, p, p );
	}
while( i < n );

qmov( p, y );
return 0;
}

