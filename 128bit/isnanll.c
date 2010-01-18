/*							isnanl()
 *							isfinitel()
 *							signbitl()
 *
 *	Floating point IEEE special number tests
 *
 *
 *
 * SYNOPSIS:
 *
 * int signbitl(), isnanl(), isfinitel();
 * long double x, y;
 *
 * n = signbitl(x);
 * n = isnanl(x);
 * n = isfinitel(x);
 *
 *
 *
 * DESCRIPTION:
 *
 * These functions are part of the standard C run time library
 * for some but not all C compilers.  The ones supplied are
 * written in C for IEEE arithmetic.  They should
 * be used only if your compiler library does not already have
 * them.
 *
 */


/*
Cephes Math Library Release 2.9:  August, 2001
Copyright 2001 by Stephen L. Moshier
*/


#include "mconf.h"

#ifdef UNK
/* Change UNK into something else.  */
#undef UNK
#if BIGENDIAN
#define MIEEE 1
#else
#define IBMPC 1
#endif
#endif


/* Return 1 if the sign bit of x is 1, else 0.  */

int signbitl(x)
long double x;
{
union
	{
	long double d;
	short s[8];
	} u;

u.d = x;

#ifdef IBMPC
 return( u.s[7] < 0 );
#endif
#ifdef MIEEE
 return( u.i[0] < 0 );
#endif
}


/* Return 1 if x is a number that is Not a Number, else return 0.  */

int isnanl(x)
long double x;
{
#ifdef NANS
union
	{
	long double d;
	unsigned int i[4];
	} u;

u.d = x;

if( sizeof(int) == 4 )
	{
#ifdef IBMPC
	if( ((u.i[3] & 0x7fff0000) == 0x7fff0000)
	    && (((u.i[3] & 0xffff) | u.i[2] | u.i[1] | u.i[0]) != 0)
		return 1;
#endif
#ifdef MIEEE
	if( ((u.i[0] & 0x7fff0000) == 0x7fff0000)
	    && (((u.i[0] & 0xffff) | u.i[1] | u.i[2] | u.i[3]) != 0)
		return 1;
#endif
	return(0);
	}
else
	{ /* size int not 4 */
#ifdef IBMPC
	if( (u.s[7] & 0x7fff) == 0x7fff)
		{
		if(u.s[6] | u.s[5] | u.s[4] | u.s[3] | u.s[2] | u.s[1] | u.s[0])
			return(1);
		}
#endif
#ifdef MIEEE
	if( (u.s[0] & 0x7fff) == 0x7fff)
		{
		if(u.s[1] | u.s[2] | u.s[3] | u.s[4] | u.s[5] | u.s[6] | u.s[7])
			return(1);
		}
#endif
	return(0);
	} /* size int not 4 */

#else
/* No NANS.  */
return(0);
#endif
}


/* Return 1 if x is not infinite and is not a NaN.  */

int isfinitel(x)
long double x;
{
#ifdef INFINITIES
union
	{
	long double d;
	unsigned short s[6];
	unsigned int i[3];
	} u;

u.d = x;

if( sizeof(int) == 4 )
	{
#ifdef IBMPC
	if( (u.s[7] & 0x7fff) != 0x7fff)
		return 1;
#endif
#ifdef MIEEE
	if( (u.i[0] & 0x7fff0000) != 0x7fff0000)
		return 1;
#endif
	return(0);
	}
else
	{
#ifdef IBMPC
	if( (u.s[7] & 0x7fff) != 0x7fff)
		return 1;
#endif
#ifdef MIEEE
	if( (u.s[0] & 0x7fff) != 0x7fff)
		return 1;
#endif
	return(0);
	}
#else
/* No INFINITY.  */
return(1);
#endif
}
