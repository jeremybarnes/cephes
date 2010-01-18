#ifndef __QHEAD_H
#define __QHEAD_H

/* Type of the array elements in a Q-type number */
typedef unsigned int QELT;

/* Number of bits in a word of that type */
#define WORDSIZE 32

/* Most significant bit of that type.  */
#define SIGNBIT ((unsigned int) 1 << (WORDSIZE-1))

/* Largest exponent value */
#define MAXEXP 65536

/* The exponent of 1.0 */
#define EXPONE 0x8001

/* Number of WORDSIZE-bit words in a q type number (12 or 24) */
#define NQ 14

/* Number of words in significand area */
#define OMG (NQ-2)

/* Byte offset to least significant word of significand */
#define OFFS (4*OMG+4)

/* Number of bits of precision */
#define NBITS ((OMG-1)*WORDSIZE)

/* Maximum number of decimal digits in conversion */
#define NDEC (NBITS*8/27)

typedef struct
	{
	QELT r[NQ];
	QELT i[NQ];
	}qcmplx;

typedef struct
	{
	QELT n[NQ]; /* numerator */
	QELT d[NQ]; /* denominator */
	}qfract;

#ifndef __MCONF_H
#include "mconf.h"
#endif
#endif /* __QHEAD_H */
