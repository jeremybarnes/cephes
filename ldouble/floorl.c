/*							ceill()
 *							floorl()
 *							frexpl()
 *							ldexpl()
 *							fabsl()
 *							signbitl()
 *							isnanl()
 *							isfinitel()
 *
 *	Floating point numeric utilities
 *
 *
 *
 * SYNOPSIS:
 *
 * long double ceill(), floorl(), frexpl(), ldexpl(), fabsl();
 * int signbitl(), isnanl(), isfinitel();
 * long double x, y;
 * int expnt, n;
 *
 * y = floorl(x);
 * y = ceill(x);
 * y = frexpl( x, &expnt );
 * y = ldexpl( x, n );
 * y = fabsl( x );
 * n = signbitl(x);
 * n = isnanl(x);
 * n = isfinitel(x);
 *
 *
 *
 * DESCRIPTION:
 *
 * The following routines return a long double precision floating point
 * result:
 *
 * floorl() returns the largest integer less than or equal to x.
 * It truncates toward minus infinity.
 *
 * ceill() returns the smallest integer greater than or equal
 * to x.  It truncates toward plus infinity.
 *
 * frexpl() extracts the exponent from x.  It returns an integer
 * power of two to expnt and the significand between 0.5 and 1
 * to y.  Thus  x = y * 2**expn.
 *
 * ldexpl() multiplies x by 2**n.
 *
 * fabsl() returns the absolute value of its argument.
 *
 * These functions are part of the standard C run time library
 * for some but not all C compilers.  The ones supplied are
 * written in C for IEEE arithmetic.  They should
 * be used only if your compiler library does not already have
 * them.
 *
 * The IEEE versions assume that denormal numbers are implemented
 * in the arithmetic.  Some modifications will be required if
 * the arithmetic has abrupt rather than gradual underflow.
 */


/*
Cephes Math Library Release 2.7:  May, 1998
Copyright 1984, 1987, 1988, 1992, 1998 by Stephen L. Moshier
*/


#include "mconf.h"

/* This is defined in mconf.h. */
/* #define DENORMAL 1 */

#ifdef UNK
/* Change UNK into something else.  */
#undef UNK
#if BIGENDIAN
#define MIEEE 1
#else
#define IBMPC 1
#endif
#endif

#ifdef IBMPC
#define EXPMSK 0x800f
#define MEXP 0x7ff
#define NBITS 64
#endif

#ifdef MIEEE
#define EXPMSK 0x800f
#define MEXP 0x7ff
#define NBITS 64
#endif

extern double MAXNUML;

#ifdef ANSIPROT
extern long double fabsl ( long double );
extern long double floorl ( long double );
extern int isnanl ( long double );
#else
long double fabsl(), floorl();
int isnanl();
#endif
#ifdef INFINITIES
extern long double INFINITYL;
#endif
#ifdef NANS
extern long double NANL;
#endif

long double fabsl(x)
long double x;
{
union
  {
    long double d;
    short i[6];
  } u;

u.d = x;
#ifdef IBMPC
    u.i[4] &= 0x7fff;
#endif
#ifdef MIEEE
    u.i[0] &= 0x7fff;
#endif
return( u.d );
}



long double ceill(x)
long double x;
{
long double y;

#ifdef UNK
mtherr( "ceill", DOMAIN );
return(0.0L);
#endif
#ifdef INFINITIES
if(x == -INFINITYL)
	return(x);
#endif
#ifdef MINUSZERO
if(x == 0.0L)
	return(x);
#endif
y = floorl(x);
if( y < x )
	y += 1.0L;
return(y);
}




/* Bit clearing masks: */

static unsigned short bmask[] = {
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




long double floorl(x)
long double x;
{
unsigned short *p;
union
  {
    long double y;
    unsigned short sh[6];
  } u;
int e;

#ifdef UNK
mtherr( "floor", DOMAIN );
return(0.0L);
#endif
#ifdef INFINITIES
if( x == INFINITYL )
	return(x);
#endif
#ifdef MINUSZERO
if(x == 0.0L)
	return(x);
#endif
u.y = x;
/* find the exponent (power of 2) */
#ifdef IBMPC
p = (unsigned short *)&u.sh[4];
e = (*p & 0x7fff) - 0x3fff;
p -= 4;
#endif

#ifdef MIEEE
p = (unsigned short *)&u.sh[0];
e = (*p & 0x7fff) - 0x3fff;
p += 5;
#endif

if( e < 0 )
	{
	if( u.y < 0.0L )
		return( -1.0L );
	else
		return( 0.0L );
	}

e = (NBITS -1) - e;
/* clean out 16 bits at a time */
while( e >= 16 )
	{
#ifdef IBMPC
	*p++ = 0;
#endif

#ifdef MIEEE
	*p-- = 0;
#endif
	e -= 16;
	}

/* clear the remaining bits */
if( e > 0 )
	*p &= bmask[e];

if( (x < 0) && (u.y != x) )
	u.y -= 1.0L;

return(u.y);
}



long double frexpl( x, pw2 )
long double x;
int *pw2;
{
union
  {
    long double y;
    unsigned short sh[6];
  } u;
int i, k;
short *q;

u.y = x;

#ifdef NANS
if(isnanl(x))
	{
	*pw2 = 0;
	return(x);
	}
#endif
#ifdef INFINITIES
if(x == -INFINITYL)
	{
	*pw2 = 0;
	return(x);
	}
#endif
#ifdef MINUSZERO
if(x == 0.0L)
	{
	*pw2 = 0;
	return(x);
	}
#endif

#ifdef UNK
mtherr( "frexpl", DOMAIN );
return(0.0L);
#endif

/* find the exponent (power of 2) */
#ifdef IBMPC
q = (short *)&u.sh[4];
i  = *q & 0x7fff;
#endif

#ifdef MIEEE
q = (short *)&u.sh[0];
i  = *q & 0x7fff;
#endif

if( i == 0 )
	{
	if( u.y == 0.0L )
		{
		*pw2 = 0;
		return(0.0L);
		}
/* Number is denormal or zero */
#ifdef DENORMAL
/* Handle denormal number. */
do
	{
	u.y *= 2.0L;
	i -= 1;
	k  = *q & 0x7fff;
	}
while( (k == 0) && (i > -66) );
i = i + k;
#else
	*pw2 = 0;
	return(0.0L);
#endif /* DENORMAL */
	}

*pw2 = i - 0x3ffe;
/* *q = 0x3ffe; */
/* Preserve sign of argument.  */
*q &= 0x8000;
*q |= 0x3ffe;
return( u.y );
}






long double ldexpl( x, pw2 )
long double x;
int pw2;
{
union
  {
    long double y;
    unsigned short sh[6];
  } u;
unsigned short *q;
long e;

#ifdef UNK
mtherr( "ldexp", DOMAIN );
return(0.0L);
#endif

u.y = x;
#ifdef IBMPC
q = (unsigned short *)&u.sh[4];
#endif
#ifdef MIEEE
q = (unsigned short *)&u.sh[0];
#endif
while( (e = (*q & 0x7fffL)) == 0 )
	{
#ifdef DENORMAL
	if( u.y == 0.0L )
		{
		return( 0.0L );
		}
/* Input is denormal. */
	if( pw2 > 0 )
		{
		u.y *= 2.0L;
		pw2 -= 1;
		}
	if( pw2 < 0 )
		{
		if( pw2 < -64 )
			return(0.0L);
		u.y *= 0.5L;
		pw2 += 1;
		}
	if( pw2 == 0 )
		return(u.y);
#else
	return( 0.0L );
#endif
	}

e = e + pw2;

/* Handle overflow */
if( e > 0x7fffL )
	{
	return( MAXNUML );
	}
*q &= 0x8000;
/* Handle denormalized results */
if( e < 1 )
	{
#ifdef DENORMAL
	if( e < -64 )
		return(0.0L);

#ifdef IBMPC
	*(q-1) |= 0x8000;
#endif
#ifdef MIEEE
	*(q+2) |= 0x8000;
#endif

	while( e < 1 )
		{
		u.y *= 0.5L;
		e += 1;
		}
	e = 0;
#else
	return(0.0L);
#endif
	}

*q |= (unsigned short) e & 0x7fff;
return(u.y);
}

