/*							qrand.c
 *
 *	Pseudorandom number generator
 *
 *
 *
 * SYNOPSIS:
 *
 * int qrand( q );
 * QELT q[NQ];
 *
 * qrand( q );
 *
 *
 *
 * DESCRIPTION:
 *
 * Yields a random number 1.0 <= q < 2.0.
 *
 * A three-generator congruential algorithm adapted from Brian
 * Wichmann and David Hill (BYTE magazine, March, 1987,
 * pp 127-8) is used to generate random 16-bit integers.
 * These are copied into the significand area to produce
 * a pseudorandom bit pattern.
 */



#include "qhead.h"

/* This function implements the three
   congruential generators.  */
 
static int sx = 1;
static int sy = 10000;
static int sz = 3000;

int ranwh()
{
int r, s;

/*  sx = sx * 171 mod 30269 */
r = sx/177;
s = sx - 177 * r;
sx = 171 * s - 2 * r;
if( sx < 0 )
	sx += 30269;


/* sy = sy * 172 mod 30307 */
r = sy/176;
s = sy - 176 * r;
sy = 172 * s - 35 * r;
if( sy < 0 )
	sy += 30307;

/* sz = 170 * sz mod 30323 */
r = sz/178;
s = sz - 178 * r;
sz = 170 * s - 63 * r;
if( sz < 0 )
	sz += 30323;
/* The results are in static sx, sy, sz. */
return 0;
}


int qrand( q )
QELT q[];
{
unsigned int r;
int i;

/* Positive sign, exponent of 1.0, clear the high guard word.  */
q[0] = 0;
q[1] = EXPONE;
q[2] = 0;

/* Fill significand with pseudorandom patterns.  */
for( i=3; i<NQ; i++ )
  {
#if WORDSIZE == 16
    ranwh();
    r = (sx * sy) + sz;
#else /* WORDSIZE is 32 */
    ranwh();
    r = ((sx * sy) + sz) & 0xffff;
    ranwh();
    r = r | (((sx * sy) + sz) << 16);
#endif
    q[i] = r;
  }

/* Ensure the significand is normalized.  */
#if WORDSIZE == 16
q[3] |= 0x8000;
#else
q[3] |= 0x80000000;
#endif

return 0;
}
