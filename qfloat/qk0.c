/*							qk0.c
 *
 * Modified Bessel function K of order 0
 *
 * K (z)  =  - (ln(z/2) + eul) I (z)
 *  0                           0
 *              2                    2   2                     2   3
 *             z / 4               (z /4)                    (z /4)
 *           + -----  + (1 + 1/2) -------- + (1 + 1/2 + 1/3) ------- + ...
 *                 2                    2                          2
 *             (1!)                 (2!)                       (3!)
 *
 * (AMS55 #9.6.13)
 *
 * Series expansions are set to terminate at less than full
 * working precision.
 */

#include "qhead.h"
extern QELT qone[], qtwo[], qeul[];
int qin();

int qk0( x, y )
QELT x[], y[];
{
static QELT t[NQ], u[NQ], z[NQ], k[NQ], p[NQ];

qmul( x, x, z );	/*z = x * x / 4.0;		*/
z[1] -= 2;

qmov( qone, p );	/* psi function */
qmov( qtwo, k );	/*k = 1.0;*/

qmov( z, y );		/* y = u;*/
qmov( z, u );
qmov( z, t );

while( ((int) y[1] - (int) t[1]) < 64 )
	{
	qdiv( k, qone, t );
	qadd( t, p, p );	/* psi function */
	qmul( k, k, t );
	qdiv( t, u, u );	/* (k!)**2	*/
	qmul( z, u, u );	/* z**k		*/
	qmul( p, u, t );	/* psi * u	*/
	qadd( t, y, y );	/*ans += u;*/
	qadd( qone, k, k );	/*k += 1.0;*/
	}


qmov( x, t );		/* log(x/2) + eul	*/
t[1] -= 1;
qlog( t, u );
qadd( qeul, u, u );

qclear( p );		/* I0(x)		*/
qin( p, x, t );
qmul( t, u, u );
qsub( u, y, y );
return(0);
}
