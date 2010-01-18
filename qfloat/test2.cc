#include <stdio.h>
#include "qfloat.h"
char str[80];

extern "C"
{
  double exp (double);
}

int
main ()
{
  qfloat fac, sum, n, one;
  double d;
  int i;

  one = 1.0;
  n = one;
  fac = one;
  sum = 1.0;
  for (i = 0; i < 14; i++)
    {
      fac *= n;
      sum += one / fac;
      n += one;
    }
  qtoasc (&sum, str, 10);
  printf ("series: %s\n", str);
  sum = exp (one);
  qtoasc (&sum, str, 10);
  printf ("qexp: %s\n", str);
  d = exp (1.0);
  printf ("double: %.10e\n", d);
}


int 
mtherr (char *prog, int code)
{
  printf ("%s code %d\n", prog, code);
  return 0;
}
