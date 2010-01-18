
/* Floating point remainder.
   c = remainder after dividing b by a.
   If n = integer part of b/a, rounded toward zero,
   then qremain(a,b,c) gives c = b - n * a.
   Integer return value contains low order bits of the integer quotient n.  */

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

int qremain( a, b, c )
QELT a[], b[], c[];
{
QELT den[NQ+1], num[NQ+1];
QELT j;
int i;

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
/* Sign of remainder = sign of numerator. */
num[0] = b[0];
qmov( num, c );
/* Sign of quotient.  */
i = quot[NQ];
if (a[0] != b[0])
  i = -i;
return i;
}
