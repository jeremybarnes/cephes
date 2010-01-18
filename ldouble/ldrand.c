/*							ldrand.c
 *
 *	Pseudorandom number generator
 *
 *
 *
 * SYNOPSIS:
 *
 * double y;
 * int ldrand();
 *
 * ldrand( &y );
 *
 *
 *
 * DESCRIPTION:
 *
 * Yields a random number 1.0 <= y < 2.0.
 *
 * The three-generator congruential algorithm by Brian
 * Wichmann and David Hill (BYTE magazine, March, 1987,
 * pp 127-8) is used.
 *
 * Versions invoked by the different arithmetic compile
 * time options IBMPC, and MIEEE, produce the same sequences.
 *
 */



#include "mconf.h"
#ifdef ANSIPROT
int ranwh ( void );
#else
int ranwh();
#endif
#ifdef UNK
#undef UNK
#if BIGENDIAN
#define MIEEE
#else
#define IBMPC
#endif
#endif

/*  Three-generator random number algorithm
 * of Brian Wichmann and David Hill
 * BYTE magazine, March, 1987 pp 127-8
 *
 * The period, given by them, is (p-1)(q-1)(r-1)/4 = 6.95e12.
 */

static int sx = 1;
static int sy = 10000;
static int sz = 3000;

static union {
 long double d;
 unsigned short s[8];
} unkans;

/* This function implements the three
 * congruential generators.
 */
 
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

/*	ldrand.c
 *
 * Random double precision floating point number between 1 and 2.
 *
 * C callable:
 *	drand( &x );
 */

int ldrand( a )
long double *a;
{
unsigned short r;

/* This algorithm of Wichmann and Hill computes a floating point
 * result:
 */
ranwh();
unkans.d = sx/30269.0L  +  sy/30307.0L  +  sz/30323.0L;
r = unkans.d;
unkans.d -= r;
unkans.d += 1.0L;

if( sizeof(long double) == 16 )
  {
#ifdef MIEEE
    ranwh();
    r = sx * sy + sz;
    unkans.s[7] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[6] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[5] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[4] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[3] = r;
#endif
#ifdef IBMPC
    ranwh();
    r = sx * sy + sz;
    unkans.s[0] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[1] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[2] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[3] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[4] = r;
#endif
  }
else
  {
#ifdef MIEEE
    ranwh();
    r = sx * sy + sz;
    unkans.s[5] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[4] = r;
#endif
#ifdef IBMPC
    ranwh();
    r = sx * sy + sz;
    unkans.s[0] = r;
    ranwh();
    r = sx * sy + sz;
    unkans.s[1] = r;
#endif
  }
*a = unkans.d;
return 0;
}
