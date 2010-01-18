/*							arcdot.c
 *
 *	Angle between two vectors
 *
 *
 *
 *
 * SYNOPSIS:
 *
 * long double p[3], q[3], arcdotl();
 *
 * y = arcdotl( p, q );
 *
 *
 *
 * DESCRIPTION:
 *
 * For two vectors p, q, the angle A between them is given by
 *
 *      p.q / (|p| |q|)  = cos A  .
 *
 * where "." represents inner product, "|x|" the length of vector x.
 * If the angle is small, an expression in sin A is preferred.
 * Set r = q - p.  Then
 *
 *     p.q = p.p + p.r ,
 *
 *     |p|^2 = p.p ,
 *
 *     |q|^2 = p.p + 2 p.r + r.r ,
 *
 *                  p.p^2 + 2 p.p p.r + p.r^2
 *     cos^2 A  =  ----------------------------
 *                    p.p (p.p + 2 p.r + r.r)
 *
 *                  p.p + 2 p.r + p.r^2 / p.p
 *              =  --------------------------- ,
 *                     p.p + 2 p.r + r.r
 *
 *     sin^2 A  =  1 - cos^2 A
 *
 *                   r.r - p.r^2 / p.p
 *              =  --------------------
 *                  p.p + 2 p.r + r.r
 *
 *              =   (r.r - p.r^2 / p.p) / q.q  .
 *
 * ACCURACY:
 *
 * About 1 ULP.  See arcdot.c.
 *
 */

/*
Cephes Math Library Release 2.3:  November, 1995
Copyright 1995 by Stephen L. Moshier
*/

#include "mconf.h"
#ifdef ANSIPROT
extern long double sqrtl ( long double );
extern long double acosl ( long double );
extern long double asinl ( long double );
extern long double atanl ( long double );
#else
long double sqrtl(), acosl(), asinl(), atanl();
#endif
extern long double PIL;

long double arcdotl(p,q)
long double p[], q[];
{
long double pp, pr, qq, rr, rt, pt, qt, pq;
int i;

pq = 0.0L;
qq = 0.0L;
pp = 0.0L;
pr = 0.0L;
rr = 0.0L;
for (i=0; i<3; i++)
  {
    pt = p[i];
    qt = q[i];
    pq += pt * qt;
    qq += qt * qt;
    pp += pt * pt;
    rt = qt - pt;
    pr += pt * rt;
    rr += rt * rt;
  }
if (rr == 0.0L || pp == 0.0L || qq == 0.0L)
  return 0.0L;
rt = (rr - (pr * pr) / pp) / qq;
if (rt <= 0.75L)
  {
    rt = sqrtl(rt);
    qt = asinl(rt);
    if (pq < 0.0L)
      qt = PIL - qt;
  }
else
  {
    pt = pq / sqrtl(pp*qq);
    qt = acosl(pt);
  }
return qt;
}
