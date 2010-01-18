/*							qcosh.c
 *
 *	Hyperbolic cosine
 *
 *
 *
 * SYNOPSIS:
 *
 * int qcosh(x, y);
 * QELT *x, *y;
 *
 * qcosh(x, y);
 *
 *
 *
 * DESCRIPTION:
 *
 * cosh(x)  =  ( exp(x) + exp(-x) )/2.
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1985, 1995 by Stephen L. Moshier
*/


#include "qhead.h"

extern QELT qone[];

int qcosh(x,y)
QELT x[], y[];
{
QELT h[NQ], w[NQ];

qexp( x, w );		/* w = exp(x) */
qdiv( w, qone, h );	/* 1/exp(x) */
qadd( w, h, y );
y[1] -= 1;
return 0;
}
