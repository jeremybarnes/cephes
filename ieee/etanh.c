/*							xtanh.c		*/
/* hyperbolic tangent check routine */
/* this subroutine is used by the exponential function routine */
/* by Stephen L. Moshier. */



#include "ehead.h"


void etanh( x, y )
unsigned short *x, *y;
{
unsigned short e[NE], r[NE], j[NE], xx[NE], m2[NE];
short i, n;
long lj;

emov( x, r );
r[NE-1] &= (unsigned short )0x7fff;
if( ecmp(r, eone) >= 0 )
	{
/* tanh(x) = (exp(x) - exp(-x)) / (exp(x) + exp(-x))
 * Note eexp() calls xtanh, but with an argument less than (1 + log 2)/2.
 */
	eexp( r, e );
	ediv( e, eone, r );
	esub( r, e, xx );
	eadd( r, e, j );
	ediv( j, xx, y );
	return;
	}

emov( etwo, m2 );
eneg( m2 );

n = NBITS/8;	/* Number of terms to do in the continued fraction */
lj = 2 * n + 1;
ltoe( &lj, j );

emov( j, e );
emul( x, x, xx );

/* continued fraction */
for( i=0; i<n; i++)
	{
	ediv( e, xx, r );
	eadd( m2, j, j );
	eadd( r, j, e );
	}

ediv( e, x, y );
}
