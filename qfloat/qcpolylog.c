/*
   Polylogarithms.


               inf   k
                -   x
   Li (x)  =    >   ---
     n          -     n
               k=1   k


                 x
                  -
                 | |  -ln(1-t)
    Li (x)  =    |    -------- dt
      2        | |       t
                -
                 0


                1-x
                  -
                 | |  ln t
            =    |    ------ dt   =   spence(1-x)
               | |    1 - t
                -
                 1


                        2       3
                       x       x
             =  x  +  ---  +  ---  +  ...
                       4       9


  d                 1
  --   Li (x)  =   ---  Li   (x)
  dx     n          x     n-1

  */
#include "mconf.h"
#include "qhead.h"
extern QELT qzero[], qone[], qtwo[];
extern qcmplx qczero, qcone;
static int cxplog(), invformula();


int qcpolylog (n, x, y)
     int n;
     qcmplx *x, *y;
{
  qcmplx a, p, s;
  QELT k[NQ], qn[NQ];
  long ln, es;

  ln = n;
  ltoq (&ln, qn);

  if (n == 0)
    {
      /* Li_0(x) = x / (1.0 - x); */
      qsub(x->r, qone, a.r);
      qmov(x->i, a.i);
      qcdiv (&a, x, y);
      return 0;
    }

  if (n == 1)
    {
      /* Li_1(x) = -log (1.0 - x) */
      qsub(x->r, qone, a.r);
      qmov(x->i, a.i);
      qclog(&a, y);
      qcneg(y);
      return 0;
    }
  /* Argument +1 */
  if ((qcmp(qone, x->r) == 0) && (qcmp(qzero, x->i) == 0) && (n > 1))
    {
      qzetac (qn, y->r);
      qclear (y->i);
      qadd (qone, y->r, y->r);
      return 0;
    }

  /* Argument -1.
                        1-n
     Li (-z)  = - (1 - 2   ) Li (z)
       n                       n
  */
  qmov(qone, k);
  qneg(k);
  if ((qcmp(k, x->r) == 0) && (qcmp(qzero, x->i) == 0) && (n > 1))
    {
      /* Li_n(1) = zeta(n) */
      qzetac (qn, y->r);
      qadd (qone, y->r, y->r);
      qclear (y->i);
      qmov (qone, k);
      k[1] = k[1] + (1 - n);
      qsub (k, qone, k);
      qmul (k, y->r, y->r);
      qneg (y->r);
      return 0;
    }

  if (x->r[1] > qone[1] || x->i[1] > qone[1])
    {
      invformula(n, x, y);
      return 0;
    }

  /* Compare x with 3/4.  */
  qcmov(x, &a);
  a.r[0] = 0;
  a.i[0] = 0;
  qadd(a.r, a.i, k);

  ln = 3;
  ltoq (&ln, a.r);
  a.r[1] -= 2;

  if (qcmp(k, a.r) > 0)
    {
      cxplog (n, x, y);
      return 0;
    }


  /* Defining power series.  */
  qcmov (x, &p);
  qcmov (x, &s);
  qmov (qtwo, k);
  qneg(qn);
  do
    {
      qcmul( &p, x, &p);
      qpowi(k, qn, a.r);
      qclear(a.i);
      qcmul(&p, &a, &a);
      qcadd(&s, &a, &s);
      qadd(qone, k, k);
      if (k[1] > (qone[1] + 19))
	{
	  mtherr("qpolylog", PLOSS);
	  /* ln = (int) s[1] - (int) a[1];
	     printf("%ld\n", ln); */
	  break;
	}
      if (s.r[1] > s.i[1])
	es = s.r[1];
      else
	es = s.i[1];
      if (a.r[1] > a.i[1])
	es -= a.r[1];
      else
	es -= a.i[1];
    }
  while (es < NBITS / 2);
  qcmov (&s, y);
  return 0;
}

/*  This expansion in powers of log(x) is especially useful when
    x is near 1.

    See also the pari gp calculator.


                      inf           j
                       -    z(n-j) w
    polylog(n,x)  =    >   ----------
                       -       j!
                      j=0

    where

      w = log(x)

      z(j) = zeta(j), j != 1

                              n-1
                               -
      z(1) =  -log(-log(x)) +  >  1/k
                               -
                              k=1

  */

static int
cxplog(n, x, y)
     int n;
     qcmplx *x, *y;
{
  qcmplx z, h, q, p, s;
  long j, li, es;

  qclog (x, &z);  /* z = log(x); */
  qcmov (&z, &q);  /* h = -log(-z); */
  qcneg (&q);
  qclog (&q, &h);
  qcneg(&h);

  for (j = 1; j < n; j++)
    {
      /* h = h + 1.0/i; */
      ltoq (&j, q.r);
      qdiv (q.r, qone, q.r);
      qclear(q.i);
      qcadd (&h, &q, &h);
    }

  qmov (qone, q.r); /* q = 1.0; */
  qclear (q.i);
  j = n;  /* s = zetac((double)n) + 1.0; */
  ltoq (&j, p.r);
  qzetac (p.r, s.r);
  qadd (qone, s.r, s.r);
  qclear (s.i);

  for (j=1; j<=n+1; j++)
  {
    ltoq (&j, p.i);  /* q = q * z / j; */
    qdiv (p.i, z.r, p.r);
    qdiv (p.i, z.i, p.i);
    qcmul (&q, &p, &q );
    if (j == n-1)
      {
	/* s = s + h * q; */
	qcmul (&h, &q, &p);
	qcadd (&s, &p, &s);
      }
    else
      {
	/* s = s + (zetac((double)(n-j)) + 1.0) * q; */
	li = n - j;
	ltoq (&li, p.r);
	qzetac (p.r, p.r);
	qadd (qone, p.r, p.r);
	qclear (p.i);
	qcmul (&q, &p, &p);
	qcadd (&s, &p, &s);
      }
  }
  j = n + 3;
  qcmul (&z, &z, &z); /* z = z * z; */
  for(;;)
    {
      /* q = q * z / ((j-1)*j); */
      li = (j-1) * j;
      ltoq (&li, p.i);
      qdiv (p.i, z.r, p.r);
      qdiv (p.i, z.i, p.i);
      qcmul (&q, &p, &q);
      /* p1 = (zetac((double)(n-j)) + 1.0); */
      li = n - j;
      ltoq (&li, p.r);
      qzetac (p.r, p.r);
      qadd (qone, p.r, p.r);
      qclear (p.i);
      /* p1 = p1 * q; */
      qcmul (&p, &q, &p);
      /* s = s + p1; */
      qcadd (&s, &p, &s);
      if (s.r[1] > s.i[1])
	es = s.r[1];
      else
	es = s.i[1];
      if (p.r[1] > p.i[1])
	es -= p.r[1];
      else
	es -= p.i[1];
      if (es > NBITS/2)
	break;
      j += 2;
    }
  qcmov (&s, y);
  return 0;
}


/*  Inversion formula:
 *
 *                                                   [n/2]   n-2r
 *                n                  1     n           -  log    (z)
 *  Li (-z) + (-1)  Li (-1/z)  =  - --- log (z)  +  2  >  ----------- Li  (-1)
 *    n               n              n!                -   (n - 2r)!    2r
 *                                                    r=1
 */

static int
invformula(n, x, y)
     int n;
     qcmplx *x, *y;
{
  static qcmplx w, p, q, s, m1;
  QELT qn[NQ], t[NQ];
  long ln, r;

  qcmov(x, &q);
  qcneg(&q);
  qclog(&q, &w);

  qcmov (&qcone, &m1);
  qcneg (&m1);

  qcmov (&qczero, &s);
  for (r = 1; r <= n/2; r++)
    {
      ln = 2*r;
      qcpolylog((int)ln, &m1, &p);

      ln = n - ln;
      if (ln == 0)
	{
	  qcadd(&p, &s, &s);
	  break;
	}
      ltoq(&ln, qn);
      qfac(qn, t);
      qdiv(t, p.r, p.r);

      qmov(qn, q.r);
      qclear(q.i);
      qcpow(&w, &q, &q);

      qmul(q.r, p.r, q.r);
      qmul(q.i, p.r, q.i);
      qcadd(&q, &s, &s);
    }
  qmul(qtwo, s.r, s.r);
  qmul(qtwo, s.i, s.i);

  ln = n;
  ltoq(&ln, p.r);
  qclear(p.i);
  qcpow(&w, &p, &q);

  qfac(p.r, t);
  qdiv(t, q.r, q.r);
  qdiv(t, q.i, q.i);
  qcsub(&q, &s, &s);

  qcmov(&qcone, &q);
  qcdiv(x, &q, &q);
  qcpolylog(n, &q, &p);
  if(n & 1)
    qcneg(&p);
  qcsub(&p, &s, y);
  return 0;
}
