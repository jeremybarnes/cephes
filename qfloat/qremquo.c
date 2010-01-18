
/* Floating point remainder.
   c = remainder after dividing b by a.
   If n = integer part of b/a, rounded toward zero,
   then qremain(a,b,c) gives c = b - n * a.
   Integer return value contains low order bits of the integer quotient n.

   According to the C99 standard,
     189)   When y != 0, the remainder r = x REM y is defined regardless
     of the rounding mode by the mathematical relation r = x - ny,
     where n is the integer nearest the exact value of x / y;
     whenever | n - x / y | = 1/2, then n is even. Thus, the remainder
     is always exact. If r = 0, its sign shall be that of x.
     This definition is applicable for all implementations.
  */

#include "qhead.h"
#include "mconf.h"
extern QELT qzero[];

#ifdef ANSIPROT
int subm( QELT *, QELT * );
int shup1( QELT * ), shdn1( QELT * ), shup16( QELT * ), shup8( QELT * );
int cmpm( QELT *, QELT * );
#else
int cmpm(), subm(), shdn1(), shup1(), shup16(), shup8();
int qcmp(), qclear(), qmov();
#endif

static QELT quot[NQ+1];

int qremquo( a, b, c )
QELT a[], b[], c[];
{
QELT den[NQ+1], num[NQ+1], temp[NQ];
QELT j;
int i, k, qi;

if( qcmp(a,qzero) == 0 )
	{
	mtherr( "qremain", SING );
	qclear( c );
	return 0;
	}
den[NQ] = 0;
qmov( a, den );
den[0] = 0;
num[NQ] = 0;
qmov( b, num );
num[0] = 0;

/* Execute divide steps until exponent of num < exponent of den.
   Least significant integer quotient bits left in quot[].  */

quot[NQ] = 0;
qclear( quot );

while (num[1] >= den[1])
	{
	if( cmpm(den,num) <= 0 )
		{
		subm(den, num);
		j = 1;
		}
	else
		{
		j = 0;
		}
	shup1(quot);
	quot[NQ] |= j;
	shup1(num);
	num[1] -= 1;
	}
/* Normalize.  */
while( num[2] != 0 )
  {
    shdn1( num );
    num[1] += 1;
  }
i = 0;
do
  {
    if( num[3] & SIGNBIT )
      goto normok;
#if WORDSIZE == 32
    else if( (num[3] & 0xffff0000) == 0 )
#else
    else if( num[3] == 0 )
#endif
      {
	shup16(num);
	num[1] -= 16;
	i += 16;
      }
#if WORDSIZE == 32
    else if( (num[3] & 0xff000000) == 0 )
#else
    else if( (num[3] & 0xff00) == 0 )
#endif
      {
	shup8(num);
	num[1] -= 8;
	i += 8;
      }
    else
      {
	shup1(num);
	num[1] -= 1;
	i += 1;
      }
  }
while( i<=NBITS );
qclear(num);

normok:
qi = quot[NQ]; 
/* Round integer quotient to nearest or even.  */

/* Test remainder >= 0.5 * denominator  */
qmov (den, temp);
temp[1] -= 1;
k = qcmp (num, temp);
if (k >= 0)
      {
	if (k == 0)
		{
		  /* Round to even */
		  if (qi & 1)
		    goto qroundit;
		}
	  else
		{
qroundit:
		  qi += 1;
		  qsub (den, num, num);
		}
      }

/* Sign of remainder = sign of numerator. */
num[0] = b[0];

/* Sign of quotient.  */
if (a[0] != b[0])
  qi = -qi;

qmov( num, c );
return qi;
}
