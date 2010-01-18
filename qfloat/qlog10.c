/*							qlog10.c
 *
 *	Common logarithm
 *
 *
 *
 * SYNOPSIS:
 *
 * int qlog10( x, y );
 * QELT *x, *y;
 *
 * qlog10( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns base 10, or common, logarithm of x.
 *
 * log  (x) = log  (e) log (x)
 *    10         10       e
 *
 */

/*
Cephes Math Library Release 2.3:  March, 1995
Copyright 1984, 1995 by Stephen L. Moshier
*/

/*	qlog10	*/

#include "qhead.h"

#if WORDSIZE ==  32
#if NQ > 8
static QELT ql10e[NQ] = {
0,EXPONE-2,0,0xde5bd8a9,0x37287195,0x355baaaf,0xad33dc32,0x3ee34602,
0x45c9a202,0x3a3f2d44,0xf78ea53c,0x75424efa,0x1402f3f2,0x92235593
};
#else
/* 4.34294481903251827651128918916605082294397005803667E-1 */
static QELT ql10e[NQ] = {
0,EXPONE-2,0,0xde5bd8a9,0x37287195,0x355baaaf,0xad33dc32,0x3ee34602};
#endif
#else
/* word size is 16 */
#if NQ > 12
static short ql10e[NQ] = {
0x0000,EXPONE-2,0x0000,0xde5b,0xd8a9,0x3728,0x7195,0x355b,
0xaaaf,0xad33,0xdc32,0x3ee3,0x4602,0x45c9,0xa202,0x3a3f,
0x2d44,0xf78e,0xa53c,0x7542,0x4efa,0x1402,0xf3f2,0x9223
};
#else
/* 4.34294481903251827651128918916605082294397005803667E-1 */
static short ql10e[NQ] = {
0000000,EXPONE-2,0000000,0157133,0154251,0033450,
0070625,0032533,0125257,0126463,0156062,0037343};
#endif
#endif


int qlog10( x, y )
QELT x[], y[];
{
QELT a[NQ];

qlog( x, a );

qmul( ql10e, a, y );
return 0;
}
