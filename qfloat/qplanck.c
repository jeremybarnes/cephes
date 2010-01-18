/* Integral of Planck's radiation formula.
 *
 *                                       1
 *                                ------------------
 *                                 5
 *                                t  (exp(1/bw) - 1)
 *
 * Set
 *   b = T/c2
 *   u = exp(1/bw)
 *
 *  In terms of polylogarithms Li_n(u)¸ the integral is
 *
 *                  (           Li (u)      Li (u)                  )
 *     1          4 (              3           2          log(1-u)  )
 *    ----  -  6 b  ( Li (u)  -  ------  +  --------  +  ---------- )
 *       4          (   4          bw              2             3  )
 *    4 w           (                        2 (bw)        6 (bw)   )
 *
 *   Since u > 1, the Li_n are complex valued.  This is not
 * the best way to calculate the result, which is real, but it
 * is adopted as a the priori formula against which other formulas
 * can be verified.
 */
#include "qhead.h"
extern QELT qone[], qthree[], qpi[];
static QELT c1[NQ], c2[NQ], u[NQ];

int
qplanck (w, T, y)
     QELT *w, *T, *y;
{
  static QELT b[NQ], d[NQ], q[NQ];
  qcmplx cs, cu, cp;
  double dd;

 dd = 3.7417749e-16;
 etoq ((void *)&dd, c1);

 dd = 0.01438769;
 etoq ((void *)&dd, c2);

 /* Temperature, degrees K.
     dd = 3000.;
     etoq (&dd, b);  */
 /* b = T / c2 */
 qdiv(c2, T, b);

 /* d = b*w */
 qmul(b, w, d);

 /* p = exp(1/d) */
  qdiv (d, qone, u);
  qexp (u, cp.r);
  qclear(cp.i);

  /* s = polylog(4,p) */
  qcpolylog (4, &cp, &cs);

  /* s = s - polylog(3,p) / d */
  qcpolylog (3, &cp, &cu);

  qdiv (d, cu.r, cu.r);
  qdiv (d, cu.i, cu.i);
  qcsub (&cu, &cs, &cs);

  /* s = s + polylog(2,p) / (2*d*d) */

  qcpolylog (2, &cp, &cu);
  qmul(d, d, q);
  q[1] += 1;
  qdiv(q, cu.r, cu.r);
  qdiv(q, cu.i, cu.i);
  qcadd(&cu, &cs, &cs);

  /* s = s + log(1-p) / (6*d*d*d) */
  qsub(cp.r, qone, cu.r);
  qclear(cu.i);
  qclog(&cu, &cu);

  qmul(d, q, q);
  qmul(qthree, q, q);
  qdiv(q, cu.r, cu.r);
  qdiv(q, cu.i, cu.i);
  qcadd(&cu, &cs, &cs);

  /* y = .25/(w*w*w*w) - 6 * b*b*b*b * s */
  qmul(b, b, d);
  qmul(d, d, d);
  qmov(d, u);
  u[1] += 1;
  qmul(qthree, u, u);
  qmul(u, cs.r, cs.r);
  qmul(u, cs.i, cs.i);

  qmul(w, w, u);
  qmul(u, u, u);
  qdiv(u, qone, u);
  u[1] -= 2;
  qsub( cs.r, u, cs.r);

  /* r = b*b*b*b*pi*pi*pi*pi/7.5; */
  qmul(qpi, qpi, u);
  qmul(u, u, u);
  qmul(d, u, u);
  dd = 7.5;
  etoq((void *)&dd, d);
  qdiv(d, u, u);

  /*  s = s + r */
  qadd(u, cs.r, cs.r);
  /* y=a*s */
  qmul(c1, cs.r, y);
  return 0;
  }


/* Right-hand tail of Planck integral.  */
int
qplanckc (w, T, y)
     QELT *w, *T, *y;
{
  static QELT p[NQ];

  qplanck(w, T, p);
  qmul(c1, u, u);
  u[1] -= 1;
  qsub (p, u, y);
  return 0;
}

