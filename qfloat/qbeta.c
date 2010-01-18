/*							qbeta.c
 *
 *	Beta function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qbeta( a, b, y );
 * QELT *a, *b, *y;
 *
 * qbeta( a, b, y );
 *
 *
 *
 * DESCRIPTION:
 *
 *                   -     -
 *                  | (a) | (b)
 * beta( a, b )  =  -----------.
 *                     -
 *                    | (a+b)
 *
 */

/*
Cephes Math Library Release 2.0:  April, 1987
Copyright 1984, 1987 by Stephen L. Moshier
*/

#include "qhead.h"

int qgamma();

int qbeta( a, b, y )
QELT a[], b[], y[];
{
QELT r[NQ], s[NQ];

qadd( a, b, r );
qgamma( r, s );

qgamma( a, r );
qdiv( s, r, s );

qgamma( b, r );
qmul( s, r, y );
return 0;
}
