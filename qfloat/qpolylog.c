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

  Series expansions are set to terminate at less than full
  working precision.

  */
#include "mconf.h"
#include "qhead.h"
extern QELT qzero[], qone[], qtwo[];
static int cxplog();

int qpolylog (n, x, y)
     int n;
      QELT *x, *y;
{
  QELT a[NQ], p[NQ], s[NQ], k[NQ], qn[NQ];
  long ln;

  ln = n;
  ltoq (&ln, qn);


  if ((qcmp(qone, x) == 0) && (n > 1))
    {
      qzetac (qn, y);
      qadd (qone, y, y);
      return 0;
    }

  /*
  if (n == 2)
    {
      qmov(x, s);
      qsub(qone, s, s);
      qspenc (s, y);
      return 0;
    }
  */

  /* Compare x with 3/4.  */
  ln = 3;
  ltoq (&ln, a);
  a[1] -= 2;
  if (qcmp(x,a) > 0)
    {
      cxplog (n, x, y);
      return 0;
    }


  /* Defining power series.  */
  qmov (x, p);
  qmov (x, s);
  qmov (qtwo, k);
  qneg(qn);
  do
    {
      qmul( p, x, p);
      qpowi(k, qn, a);
      qmul(p, a, a);
      qadd(s, a, s);
      qadd(qone, k, k);
      if (k[1] > (qone[1] + 19))
	{
	  mtherr("qpolylog", PLOSS);
	  /* ln = (int) s[1] - (int) a[1];
	     printf("%ld\n", ln); */
	  break;
	}
    }
  while (((int) s[1] - (int) a[1]) < NBITS / 2);
  qmov (s, y);
  return 0;
}

/*  This expansion in powers of log(x) is especially useful when
    x is near 1.

    See also the pari calculator.


                      inf           j
                       -    z(n-j) w
    polylog(n,x)  =    >   ----------
                       -       j!
                      j=0

    where

      w = log(x)

      z(j) = zeta(j), j != 1

                               n
                               -
      z(1) =  -log(-log(x)) +  >  1/k
                               -
                              k=1

  */

static int
cxplog(n, x, y)
     int n;
     QELT *x, *y;
{
  QELT z[NQ], h[NQ], q[NQ], p[NQ], s[NQ];
  long j, li;

  qlog (x, z);  /* z = log(x); */
  qmov (z, q);  /* h = -log(-z); */
  qneg (q);
  qlog (q, h);
  qneg(h);

  for (j = 1; j < n; j++)
    {
      /* h = h + 1.0/i; */
      ltoq (&j, q);
      qdiv (q, qone, q);
      qadd (h, q, h);
    }

  qmov (qone, q); /* q = 1.0; */
  j = n;  /* s = zetac((double)n) + 1.0; */
  ltoq (&j, p);
  qzetac (p, s);
  qadd (qone, s, s);

  for (j=1; j<=n+1; j++)
  {
    ltoq (&j, p);  /* q = q * z / j; */
    qdiv (p, z, p);
    qmul (q, p, q );
    if (j == n-1)
      {
	/* s = s + h * q; */
	qmul (h, q, p);
	qadd (s, p, s);
      }
    else
      {
	/* s = s + (zetac((double)(n-j)) + 1.0) * q; */
	li = n - j;
	ltoq (&li, p);
	qzetac (p, p);
	qadd (qone, p, p);
	qmul (q, p, p);
	qadd (s, p, s);
      }
  }
  j = n + 3;
  qmul (z, z, z); /* z = z * z; */
  for(;;)
    {
      /* q = q * z / ((j-1)*j); */
      li = (j-1) * j;
      ltoq (&li, p);
      qdiv (p, z, p);
      qmul (q, p, q);
      /* p1 = (zetac((double)(n-j)) + 1.0); */
      li = n - j;
      ltoq (&li, p);
      qzetac (p, p);
      qadd (qone, p, p);
      /* p1 = p1 * q; */
      qmul (p, q, p);
      /* s = s + p1; */
      qadd (s, p, s);
      if (((int)s[1] - (int)p[1]) > NBITS/2)
	break;
      j += 2;
    }
  qmov (s, y);
  return 0;
}
