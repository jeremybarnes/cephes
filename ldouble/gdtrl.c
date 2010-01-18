/*							gdtrl.c
 *
 *	Gamma distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double a, b, x, y, gdtrl();
 *
 * y = gdtrl( a, b, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the integral from zero to x of the gamma probability
 * density function:
 *
 *
 *                x
 *        b       -
 *       a       | |   b-1  -at
 * y =  -----    |    t    e    dt
 *       -     | |
 *      | (b)   -
 *               0
 *
 *  The incomplete gamma integral is used, according to the
 * relation
 *
 * y = igam( b, ax ).
 *
 *
 * ACCURACY:
 *
 * See igam().
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * gdtrl domain        x < 0            0.0
 *
 */
/*							gdtrcl.c
 *
 *	Complemented gamma distribution function
 *
 *
 *
 * SYNOPSIS:
 *
 * long double a, b, x, y, gdtrcl();
 *
 * y = gdtrcl( a, b, x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the integral from x to infinity of the gamma
 * probability density function:
 *
 *
 *               inf.
 *        b       -
 *       a       | |   b-1  -at
 * y =  -----    |    t    e    dt
 *       -     | |
 *      | (b)   -
 *               x
 *
 *  The incomplete gamma integral is used, according to the
 * relation
 *
 * y = igamc( b, ax ).
 *
 *
 * ACCURACY:
 *
 * See igamc().
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * gdtrcl domain        x < 0            0.0
 *
 */

/*							gdtrl()  */


/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double igaml ( long double, long double );
extern long double igamcl ( long double, long double );
#else
long double igaml(), igamcl();
#endif

long double gdtrl( a, b, x )
long double a, b, x;
{

if( x < 0.0L )
	{
	mtherr( "gdtrl", DOMAIN );
	return( 0.0L );
	}
return(  igaml( b, a * x )  );
}



long double gdtrcl( a, b, x )
long double a, b, x;
{

if( x < 0.0L )
	{
	mtherr( "gdtrcl", DOMAIN );
	return( 0.0L );
	}
return(  igamcl( b, a * x )  );
}
