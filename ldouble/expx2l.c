/*							expx2l.c
 *
 *	Exponential of squared argument
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, expx2l();
 * int sign;
 *
 * y = expx2l( x, sign );
 *
 *
 *
 * DESCRIPTION:
 *
 * Computes y = exp(x*x) while suppressing error amplification
 * that would ordinarily arise from the inexactness of the
 * exponential argument x*x.
 *
 * If sign < 0, the result is inverted; i.e., y = exp(-x*x) .
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic      domain        # trials      peak         rms
 *   IEEE     -106.566, 106.566    10^5       1.6e-19     4.4e-20
 *
 */

/*
Cephes Math Library Release 2.9:  June, 2000
Copyright 2000 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double fabsl (long double);
extern long double floorl (long double);
extern long double expl (long double);
#else
double fabsl();
double floorl();
double expl();
#endif

#define M 32768.0
#define MINV 3.0517578125e-5

extern long double MAXLOGL;
extern long double INFINITYL;


long double expx2l (x, sign)
     long double x;
     int sign;
{
  long double u, u1, m, f;

  x = fabsl (x);
  if (sign < 0)
    x = -x;

  /* Represent x as an exact multiple of M plus a residual.
     M is a power of 2 chosen so that exp(m * m) does not overflow
     or underflow and so that |x - m| is small.  */
  m = MINV * floorl(M * x + 0.5L);
  f = x - m;

  /* x^2 = m^2 + 2mf + f^2 */
  u = m * m;
  u1 = 2 * m * f  +  f * f;

  if (sign < 0)
    {
      u = -u;
      u1 = -u1;
    }

  if ((u+u1) > MAXLOGL)
    return (INFINITYL);

  /* u is exact, u1 is small.  */
  u = expl(u) * expl(u1);
  return(u);
}
