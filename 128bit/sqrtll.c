/*							sqrtl.c
 *
 *	Square root, long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, sqrtl();
 *
 * y = sqrtl( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns the square root of x.
 *
 * Range reduction involves isolating the power of two of the
 * argument and using a polynomial approximation to obtain
 * a rough value for the square root.  Then Heron's iteration
 * is used three times to converge to an accurate value.
 *
 * Note, some arithmetic coprocessors such as the 8087 and
 * 68881 produce correctly rounded square roots, which this
 * routine will not.
 *
 * ACCURACY:
 *
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE      0,10        30000       8.1e-20     3.1e-20
 *
 *
 * ERROR MESSAGES:
 *
 *   message         condition      value returned
 * sqrt domain        x < 0            0.0
 *
 */

/*
Cephes Math Library Release 2.2:  December, 1990
Copyright 1984, 1990 by Stephen L. Moshier
Direct inquiries to 30 Frost Street, Cambridge, MA 02140
*/


#include "mconf.h"

#define SQRT2 1.4142135623730950488017E0L



long double sqrtl(x)
long double x;
{
int e;
long double z, w;
#ifndef UNK
short *q;
#endif
long double frexpl(), ldexpl();

if( x <= 0.0 )
	{
	if( x < 0.0 )
		mtherr( "sqrtl", DOMAIN );
	return( 0.0 );
	}
w = x;
/* separate exponent and significand */
#ifdef UNK
z = frexpl( x, &e );
#endif

/* Note, frexp and ldexp are used in order to
 * handle denormal numbers properly.
 */
#ifdef IBMPC
z = frexpl( x, &e );
q = (short *)&x; /* point to the exponent word */
q += 7;
/*
e = ((*q >> 4) & 0x0fff) - 0x3fe;
*q &= 0x000f;
*q |= 0x3fe0;
z = x;
*/
#endif
#ifdef MIEEE
z = frexpl( x, &e );
q = (short *)&x;
/*
e = ((*q >> 4) & 0x0fff) - 0x3fe;
*q &= 0x000f;
*q |= 0x3fe0;
z = x;
*/
#endif

/* approximate square root of number between 0.5 and 1
 * relative error of linear approximation = 7.47e-3
 */
/*
x = 0.4173075996388649989089L + 0.59016206709064458299663L * z;
*/

/* quadratic approximation, relative error 6.45e-4 */
x = ( -0.20440583154734771959904L  * z
     + 0.89019407351052789754347L) * z
     + 0.31356706742295303132394L;

/* adjust for odd powers of 2 */
if( (e & 1) != 0 )
	x *= SQRT2;

/* re-insert exponent */
#ifdef UNK
x = ldexpl( x, (e >> 1) );
#endif
#ifdef IBMPC
x = ldexpl( x, (e >> 1) );
/*
*q += ((e >>1) & 0x7ff) << 4;
*q &= 077777;
*/
#endif
#ifdef MIEEE
x = ldexpl( x, (e >> 1) );
/*
*q += ((e >>1) & 0x7ff) << 4;
*q &= 077777;
*/
#endif

/* Newton iterations: */
#ifdef UNK
x += w/x;
x = ldexpl( x, -1 );	/* divide by 2 */
x += w/x;
x = ldexpl( x, -1 );
x += w/x;
x = ldexpl( x, -1 );
x += w/x;
x = ldexpl( x, -1 );
#endif

/* Note, assume the square root cannot be denormal,
 * so it is safe to use integer exponent operations here.
 */
#ifdef IBMPC
x += w/x;
/* *q -= 1;*/
x *= 0.5L;
x += w/x;
/* *q -= 1;*/
x *= 0.5L;
x += w/x;
/* *q -= 1;*/
x *= 0.5L;
x += w/x;
/* *q -= 1;*/
x *= 0.5L;
#endif
#ifdef MIEEE
x += w/x;
*q -= 1;
x += w/x;
*q -= 1;
x += w/x;
*q -= 1;
x += w/x;
*q -= 1;
#endif

return(x);
}
