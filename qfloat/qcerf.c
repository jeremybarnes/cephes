/* qcerf.c
   Complex error function.

             2 z                      2
  erf z =  -------- 1F1 ( 1/2, 3/2, -z )
           sqrt(pi)
  */

#include "qhead.h"
#include "mconf.h"

extern QELT oneopi[], qhalf[], qone[], qthree[];
#ifdef ANSIPROT
int qchyp1f1(qcmplx *, qcmplx *, qcmplx *, qcmplx *);
#else
int qchyp1f1();
int qcneg(), qcsin(), qcmul(), qcmov(), qcdiv(), qcsub(), qcadd();
#endif

int
qcerf (zz, y)
qcmplx *zz, *y;
{
qcmplx a, b, z, z2;
int neg, cj;

qcmov(zz, &z);
neg = 0;
if (z.r[0])
  {
    qneg(z.r);
    qneg(z.i);
    neg = 1;
  }
cj = 0;
if (z.i[0])
  {
    qneg(z.i);
    cj = 1;
  }
qmov(qhalf,a.r);
qclear(a.i);

qmov(qthree,b.r);
b.r[1] -= 1;
qclear(b.i);

qcmul(&z, &z, &z2);
qneg(z2.r);
qneg(z2.i);

qchyp1f1(&a, &b, &z2, &b);

qmul(oneopi, z.r, a.r);
qmul(oneopi, z.i, a.i);
qcmul(&a, &b, y);
if (neg)
  {
    qneg(y->r);
    qneg(y->i);
  }
if (cj)
  qneg(y->i);
return 0;
}

/* qcerfw.c
   Faddeeva error function.

                2
  w(z) =  exp(-z )  erfc(-iz)
          
  */


int
qcerfw(x, y)
qcmplx *x, *y;
{
  qcmplx z, w;

  /* -iz */
  qmov(x->i,z.r);
  qmov(x->r,z.i);
  qneg(z.i);
    /* 1 - erf(-iz) */
  qcerf(&z, &w);
  qsub( w.r, qone, w.r);
  qneg(w.i);
  /* exp(-z^2) */
  qcmul(x, x, &z);
  qneg(z.r);
  qneg(z.i);
  qcexp(&z, y);

  qcmul(y, &w, y);
  return 0;
}
