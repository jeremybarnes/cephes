/* remesw.c */

/* Write answers to disc file. */

#include <stdio.h>
#include "remes.h"

void remesw()
{
char s[40];
int i;
FILE *f, *fopen();
double log10();

reopn:

printf( "Log file name ? " );
gets(s);
f = fopen( s, "w" );
if( f == 0 )
	{
	printf( "Can't open %s\n", s );
	goto reopn;
	}

fprintf( f, "\n%s  ", funnam );
if( config & CW )
	fprintf( f, "x + x**2 * " );
if( config & XPX )
	fprintf( f, "z  " );
if( config & X2PX )
	fprintf( f, "z**2  " );
if( config & PXCU )
	fprintf( f, "P(z**3)" );
else if( config & PXSQ )
	fprintf( f, "P(z**2)" );
else
	fprintf( f, "P(z)" );
if( d > 0 )
	{
	if( config & PXCU )
		fprintf( f, "/Q(z**2)" );
	else if( config & PXSQ )
		fprintf( f, "/Q(z**2)" );
	else
		fprintf( f, "/Q(z)" );
	}
fprintf( f, "\nz(x) = %s\n", znam );
if( relerr )
	fprintf( f, "Relative error\n" );
else
	fprintf( f, "Absolute error\n" );

fprintf( f, "n = %d, d = %d\n", n, d );
fprintf( f, "precision = %23.15E  ", -log10(farther) );
fprintf( f, "error=%23.15E\n", farther );
fprintf( f, "leveled=%23.4E\n", spread );
fprintf( f, "\nNumerator:\n" );
for( i=0; i<n+d+1; i++ )
	{
	if( i == n+1 )
		fprintf( f, "\nDenominator\n" );
	fprintf( f, "%23.15E\n", param[i] );
	}

if( d > 0 )
	fprintf( f, "%23.15E\n", 1.0 );

if( (d > 0) || ((config & ZER) != 0) )
	{
	fprintf( f, "\n Locations of zero error:\n" );
	for( i=0; i<=n+d; i++ )
		fprintf( f, "%23.15E\n", xx[i] );
	}

fprintf( f, "\n Locations of peak error:\n" );
for( i=0; i<=n+d+1; i++ )
	fprintf( f, "%23.15E\n", mm[i] );
fclose(f);
}
