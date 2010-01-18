/* remesp.c */

#include <stdio.h>
#include "remes.h"

/* Display solution and tabulate errors */

void remesp()
{
int i, j, k, ip;
double a, b, x, y, z, xm, ym;
double approx(), func(), geterr();

j = 0;	/* Printout variable */
ip = 0;	/* Solution vector counter */
printf( "Numerator coefficients:\n" );
for( i=0; i<=n; i++, j++, ip++ )
	{
	if( j >= 3 )
		{
		printf( "\n" );
		j = 0;
		}
	printf( "%23.15E ", param[ip] );
	}
if( d > 0 )
	{
	j = 0;
	printf( "\nDenominator coefficients:\n" );
	for( i=0; i<d; i++, j++, ip++ )
		{
		if( j >= 3 )
			{
			printf( "\n" );
			j = 0;
			}
		printf( "%23.15E ", param[ip] );
		}
	if( j >= 3 )
		printf( "\n" );
/* Leading denominator coefficient always = 1. */
	printf( "%9.1E", 1.0 );
	}
else
	printf( "\nDeviation: %.4E", param[n+1] );

/* Display table of function and approximation error. */
printf(
"\n\n     x         func      approx      error\n"
);
a = apwidt/N;
b = apstrt;
k = 0;
j = 0;
for( i=0; i<=N; i++ )
	{
	x = b + i * a;
	if( x >= mm[k] )
		{
		xm = mm[k];
		y = geterr(xm);
		z = qyaprx;
		ym = qy;
		printf( "%11.3E %11.3E %11.3E %11.3E*\n",
			xm, ym, z, y*esign );
		k += 1;
		} 
/* Fill in the zeros also. */
	if( (d > 0) && (x >= xx[j]) )
		{
		xm = xx[j];
		y = geterr(xm);
		z = qyaprx;
		ym = qy;
		printf( "%11.3E %11.3E %11.3E %11.3Eo\n",
			 xm, ym, z, y*esign );
		j += 1;
		} 
	y = geterr(x);
	z = qyaprx;
	ym = qy;
	printf( "%11.3E %11.3E %11.3E %11.3E\n",
		x, ym, z, y*esign );
	}
}
