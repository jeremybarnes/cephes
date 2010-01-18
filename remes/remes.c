/* remes.c
 *
 * This is an interactive program that computes least maximum polynomial
 * and rational approximations.
 * Last rev: 21 Sept 1986
 */
#include <stdio.h>
#include "remes.h"

void main()
{
int i;
int chgflg; /* flags changes to default values */
char s[40];
char *sp;


printf( "\nRational Approximation by Remes Algorithm\n\n" );

START:

/* Get operator commands. */
remesa();

/* Jump to operator intervention point. */
goto showg;


LOOP:

iter += 1;
printf( "Iteration %d\n", iter );

if( search != 0 )
	{
/* Search for error maxima. */
	remess();
	goto solveq;
	}

showg:
/* Display old values of guesses */
/* and let user change them if desired */
chgflg = 0;
/* First get the step size if rational form */
if( (d > 0) && (search != 0) )
	{ /* there is a denominator polynomial */
	printf( "delta = %.4E ? ", delta );
	gets( s );
/* If input is not a null line, */
/* then decode the number. */
	if( s[0] != '\0' )
		{
		chgflg = 1;
		sscanf( s, "%lf", &delta );
		}
	for( i=0; i<=neq; i++ )
		step[i] *= delta;
	}

/* Read in guesses for locations of solution. */
for( i=0; i<neq; i++ )
	{
	printf( "x[%d] = %.4E ? ", i, xx[i] );
	gets( s );
	if( s[0] != '\0' )
		{
		chgflg = 1;
		sscanf( s, "%lf", &xx[i] );
		if( (d == 0) && ((config & ZER) == 0) )
			mm[i] = xx[i];
		}
	}

if( (d > 0) || ((config & ZER) != 0) )
	{
	for( i=0; i <=neq; i++ )
		{
		printf( "peak[%d] = %.4E ? ", i, mm[i] );
		gets( s );
		if( s[0] != '\0' )
			{
			chgflg = 1;
			sscanf( s, "%lf", &mm[i] );
			}
		}
	}

/* If there were any changes to the default values */
/* then reinitialize the step size array. */
if( chgflg )
	stpini();

solveq:

/* Solve equations. */
remese();

goto whtnxt;


ptabl:

/* Display the results */
remesp();


whtnxt:
/* Test for convergence criteria. */
if( (delta < 1.0e-15) || (spread < 1.0e-15) )
	askitr = iter;
if( askitr > iter )
	goto LOOP;

/* Ask what to do next */
printf(
"Enter #(iterations), p(rint), w(rite), g(uess), x(it), t(runc), n(one) ?");
/* Get command line from operator */
sp = &s[0];
gets( sp );

if( *sp == 'w' )
	{ /* Write results to file */
	remesw();
	goto whtnxt;
	}
if( *sp == 'g' ) /* Modify the guesses */
	goto showg;
if( *sp == 'p' ) /* Display results */
	goto ptabl;
if( (*sp >= '1') && (*sp <= '9') )
	{ /* Numeric input is iteration count */
	sscanf( sp, "%d", &askitr );
	askitr += iter;
	goto LOOP;
	}
if( *sp == 't' )
	{
	config |= TRUNC;
	goto whtnxt;
	}
/* Close files and exit */
if( *sp == 'x' )
	exit(0);
else
	goto START;
}
