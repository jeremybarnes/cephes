/* remesa.c */

#include <stdio.h>
#include "remes.h"
#define PI 3.14159265358979323846

/* Subroutine to get operator instructions. */

void remesa()
{
char s[40];
double a, r;
int i, k, ncheb;
double func(), cos(), sqrt();

/* Ask for error criterion */
printf( "Relative error (y or n) ? " );
gets( s ); /* Read in operator's response. */
relerr = 0;
if( s[0] == 'y' ) /* "y" means yes. */
	relerr = 1;

/* Enable Automatic search. */
search = 1;

getconf:

printf( "Configuration word = " );
pconfig();
printf( " ? " );
gets(s);
if( s[0] != '\0' )
	{
/* Display a menu of configuration bits */
	if( s[0] == '?' )
		{
		k = config;
		config = 0xffff;
		pconfig();
		config = k;
		goto getconf;
		}
	else
		{
		sscanf( s, "%x", &config );
		pconfig();
		}
	printf( "\n" );
	}

printf( "Degree of numerator polynomial? " );
gets( s );		/* read line */
sscanf( s, "%d", &n );	/* decode characters */

printf( "Degree of denominator polynomial? " );
gets( s );
sscanf( s, "%d", &d );

printf ( "Start of approximation interval ? " );
gets( s );
sscanf( s, "%lf", &apstrt );

getwid:
printf ( "Width of approximation interval ? " );
gets( s );
sscanf( s, "%lf", &apwidt );
if( apwidt <= 0.0 )
	{
	puts( "? must be > 0" );
	goto getwid;
	}
apend = apstrt + apwidt;
nd1 = n + d + 1;
spread = 1.0e37;
iter = 0;
askitr = 1;

/* Supply initial guesses for solution points. */

if( (d == 0) && ((config & ZER) == 0) )
	{ /* there is no denominator polynomial */
	neq = n + 2; /* The number of equations to solve */
	mm[neq] = apend;
	}
else
	{
	neq = nd1;
	}
ncheb = nd1; /* Degree of Chebyshev error estimate */

/* Find ncheb+1 extrema of Chebyshev polynomial */
a = ncheb;
mm[0] = apstrt;
for( i=1; i<ncheb; i++ )
	{
	r = 0.5 * (1.0 - cos( (PI * i) / a ) );
	if( config & SQL )
		r = r * sqrt(r);
	else if( config & SQH )
		r = sqrt(r);
	mm[i] = apstrt + r * apwidt;
	}
mm[ncheb] = apend;

if( (d == 0) && ((config & ZER) == 0) )
	{ /* The operative locations are maxima. */
	for( i=0; i<=neq; i++ )
		xx[i] = mm[i];
	}
else
	{ /* Zeros of Chebyshev polynomial */
	a = 2.0 * ncheb;
	for( i=0; i<=ncheb; i++ )
		{
		r = 0.5 * (1.0 - cos( PI * (2*i+1) / a ) );
/* Squeeze toward low end of approximation interval. */
		if( config & SQL )
			r = r * sqrt(r);
/* Squeeze toward high end. */
		else if( config & SQH )
			r = sqrt(r);
		xx[i] = apstrt + r * apwidt;
		}
/* Deviation at solution points */
	dev = 0.0;
	}

/* If form is xR(x), avoid x = 0 */
if( config & (XPX | X2PX) )
	{
	if( config & CW )
		qx = 1.0e-15 + xx[0]/2.0;
	else
		qx =  1.0e-15 + apstrt * (1.0 + 1.0e-15);
	mm[0] = qx;
	printf( "mm[0] = %.15E\n", qx );
	if( (d == 0) && ((config & ZER) == 0) )
		xx[0] = qx;
	}
/* Initialize step sizes */
stpini();
}



/* Subroutine to initialize step sizes. */
void stpini()
{
int i;

if( search )
	{
	xx[neq+1] = apend;
	delta = 0.25;
	if( (d > 0) || ((config & ZER) != 0) )
		{
		step[0] = xx[0] - apstrt;
		for( i=1; i<neq; i++ )
			step[i] = xx[i] - xx[i-1];
		}
	else
		{
		step[0] = 0.0625 * (xx[1] - xx[0]);
		for( i=1; i<neq; i++ )
			step[i] = 0.0625 * (xx[i+1] - xx[i]);
		}
	step[neq] = step[neq-1];
	}
}


/* Bits of configuration word */
#define NCNFG 11

static char *cnfmsg[NCNFG] = {
"PXSQ",
"XPX",
"PADE",
"CW",
"SQL",
"ZER",
"X2PX",
"SQH",
"SPECIAL",
"TRUNC",
"PXCU"
};

/* Subroutine to Display the configuration word */

void pconfig()
{
int i, k;

k = 1;
for( i=0; i<NCNFG; i++ )
	{
	if( k & config )
		printf( "%x:%s ", k, cnfmsg[i] );
	k <<= 1;
	}
printf( "hex value = %x ", config );
}
