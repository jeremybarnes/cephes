/*							qellpj.c
 *
 *	Jacobian Elliptic Functions
 *
 *
 *
 * SYNOPSIS:
 *
 * int qellpj( u, m, sn, cn, dn, ph );
 * QELT *u, *m;
 * QELT *sn, *cn, *dn, *ph;
 *
 * qellpj( u, m, sn, cn, dn, ph );
 *
 *
 *
 * DESCRIPTION:
 *
 *
 * Evaluates the Jacobian elliptic functions sn(u|m), cn(u|m),
 * and dn(u|m) of parameter m between 0 and 1, and real
 * argument u.
 *
 * These functions are periodic, with quarter-period on the
 * real axis equal to the complete elliptic integral
 * ellpk(1.0-m).
 *
 * Relation to incomplete elliptic integral:
 * If u = ellik(phi,m), then sn(u|m) = sin(phi),
 * and cn(u|m) = cos(phi).  Phi is called the amplitude of u.
 *
 * Computation is by means of the arithmetic-geometric mean
 * algorithm, except when m is within 1e-9 of 0 or 1.  In the
 * latter case with m close to 1, the approximation applies
 * only for phi < pi/2.
 *
 * ACCURACY:
 *
 * Truncated at 70 bits.
 *
 */

/*
Cephes Math Library, Release 2.1:  February, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
*/


#include "qhead.h"
static QELT a[15][NQ];
static QELT c[15][NQ];
extern QELT qone[], qpi[];
int qasin();

int qellpj( u, m, sn, cn, dn, ph )
QELT u[], m[];
QELT sn[], cn[], dn[], ph[];
{
QELT ai[NQ], b[NQ], phi[NQ], t[NQ], twon[NQ];
QELT temp[NQ];
int i;


/*	A. G. M. scale		*/

qmov( qone, &a[0][0] );		/* a[0] = 1.0 */
qsub( m, qone, b );		/* b = sqrt(1.0 - m) */
qsqrt( b, b );
qsqrt( m, &c[0][0] );		/* c[0] = sqrt(m) */
qmov( qone, twon );		/* twon = 1.0 */
i = 0;

while( ((int) c[i][1] - (int) a[i][1]) > -70 )
	{
	if( i > 13 )
		{
		mtherr( "qellpj", OVERFLOW );
		goto done;
		}
	qmov( &a[i][0], ai );	/* ai = a[i] */
	++i;
	qsub( b, ai, temp );	/* c[i] = ( ai - b )/2.0 */
	temp[1] -= 1;
	qmov( temp, &c[i][0] );
	qmul( ai, b, t );	/* t = sqrt( ai * b ) */
	qsqrt( t, t );
	qadd( ai, b, temp );	/* a[i] = ( ai + b )/2.0 */
	temp[1] -= 1;
	qmov( temp, &a[i][0] );
	qmov( t, b );		/* b = t */
	twon[1] += 1;		/* twon *= 2.0; */
	}

done:

/* backward recurrence */

qmul( &a[i][0], u, temp );	/* phi = twon * a[i] * u */
qmul( twon, temp, phi );

do
	{
	qsin( phi, temp );	/* t = c[i] * sin(phi) / a[i] */
	qmul( temp, &c[i][0], temp );
	qdiv( &a[i][0], temp, t );
	qmov( phi, b );		/* b = phi */
	qasin( t, temp );	/* phi = (arcsin(t) + phi)/2.0 */
	qadd( phi, temp, temp );
	temp[1] -= 1;
	qmov( temp, phi );
	}
while( --i );

qsin( phi, sn );	/* *sn = sin(phi) */

qcos( phi, t );
qmov( t, cn );
qsub( b, phi, temp );
qcos( temp, temp );
qdiv( temp, t, dn );
qmov( phi, ph );

return 0;
}
