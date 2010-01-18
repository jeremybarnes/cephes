 
/* qfloor - largest integer not greater than x
 * qround - nearest integer to x
 */

/* #include "mconf.h"  */
#include "qhead.h"

extern QELT qone[], qhalf[];

#if WORDSIZE == 32
static QELT bmask[] = {
0xffffffff,
0xfffffffe,
0xfffffffc,
0xfffffff8,
0xfffffff0,
0xffffffe0,
0xffffffc0,
0xffffff80,
0xffffff00,
0xfffffe00,
0xfffffc00,
0xfffff800,
0xfffff000,
0xffffe000,
0xffffc000,
0xffff8000,
0xffff0000,
0xfffe0000,
0xfffc0000,
0xfff80000,
0xfff00000,
0xffe00000,
0xffc00000,
0xff800000,
0xff000000,
0xfe000000,
0xfc000000,
0xf8000000,
0xf0000000,
0xe0000000,
0xc0000000,
0x80000000,
0x00000000
};
#else
static QELT bmask[] = {
0xffff,
0xfffe,
0xfffc,
0xfff8,
0xfff0,
0xffe0,
0xffc0,
0xff80,
0xff00,
0xfe00,
0xfc00,
0xf800,
0xf000,
0xe000,
0xc000,
0x8000,
0x0000,
};
#endif

int qfloor( x, y )
QELT x[], y[];
{
QELT t[NQ];
register QELT *p;
long e;

qmov( x, t );
if( t[1] == 0 )
	{
	qclear( y );
	return(0);
	}
e = (long)t[1] - (EXPONE-1);

if( e <= 0 )
	{
	if( t[0] != 0 )
		{
		qmov( qone, y );
		qneg( y );
		}
	else
		{
		qclear( y );
		}
	return(0);
	}

/* number of bits to clear out */
e = NBITS - e;

qmov( t, y );
p = (QELT *)&y[NQ-1];

while( e >= WORDSIZE )
	{
	*p-- = 0;
	e -= WORDSIZE;
	}

/* clear the remaining bits */
*p &= bmask[e];

/* truncate negatives toward minus infinity */
if( t[0] != 0 )
	{
	if( qcmp( t, y ) != 0 )
		qsub( qone, y, y );
	}
return(0);
}





int qround(x, y)
QELT *x, *y;
{
QELT z[NQ], f[NQ];
int r;

qfloor( x, z );
qsub( z, x, f );
r = qcmp( f, qhalf );
if( r > 0 )
	goto rndup;
if( r == 0 )
	{
/* round to even */
	z[1] -= 1;
	qfloor( z, f );
	z[1] += 1;
	f[1] += 1;
	if( qcmp(z,f) != 0 )
		{
rndup:		qadd( qone, z, z );
		}
	}
qmov( z, y );
return(0);
}
