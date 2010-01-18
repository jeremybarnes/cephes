/*							qei.c
 *
 *	Exponential integral
 *
 *
 * SYNOPSIS:
 *
 * QELT *x, *y;
 *
 * qei( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 *               x
 *                -     t
 *               | |   e
 *    Ei(x) =   -|-   ---  dt .
 *             | |     t
 *              -
 *             -inf
 * 
 * Not defined for x <= 0.
 * See also qexpn.c.
 *
 * ACCURACY:
 *
 * Series truncated at NBITS/2.
 *
 */


#include "qhead.h"
extern QELT qeul[], qone[], qzero[];


int qei(x, y)
QELT *x, *y;
{
  QELT b[NQ], c0[NQ], c[NQ], s[NQ], f[NQ], n[NQ];

  if (x[0])
    {
      printf("qei: arg < 0\n");
      qmov (qzero, y);
      return -1;
    }

  if (((int)x[1] - (int)EXPONE) > 6)
    goto eiasymp;


  /* Power series.
                            inf    n
                             -    x
     Ei(x) = EUL + ln x  +   >   ----
                             -   n n!
                            n=1
  */
  qclear(s);
  qmov(qone, f);
  qclear(n);
  qmov(qone, b);

  do
    {
      qadd( qone, n, n );
      qmul( n, f, f );
      qmul( x, b, b ); 
      qmul( n, f, c );
      qdiv( c, b, c );  /* c = b / (n * f) */
      qadd( s, c, s );
    }
  while ( ((int)s[1] - (int)c[1]) < NBITS/2);

  /*  s = EUL + log(x) + s; */
  qlog(x, c);
  qadd(qeul, c, c);
  qadd(c, s, y);
  return 1;

 eiasymp:

  /* Asymptotic expansion.
                            1       2       6
    x exp(-x) Ei(x) =  1 + ---  +  ---  +  ---- + ...
                            x        2       3
                                    x       x
  */
  qclear(n);
  qmov(qone, f);
  qmov(qone, s);
  qmov(qone, c0);
  qmov(qone, b );
  do
    {
      qadd( qone, n, n );
      qmul( n, f, f );
      qmul( x, b, b );
      qdiv( b, f, c );
      /* For debug: stop when terms begin to increase. */
#if 0
      if (qcmp(c, c0) > 0)
	{
	  double dc;
	  qtoe(c, &dc);
	  printf("c = %.2e\n", dc);
	  break;
	}
      qmov( c, c0 );
#endif
      qadd( c, s, s );
    }
  while ( ((int)s[1] - (int)c[1]) < NBITS/2);
  /*  s = s * exp(x) / x; */
  qexp(x, c);
  qdiv(x, c, c);
  qmul( s, c, y);
  return 1;
}
