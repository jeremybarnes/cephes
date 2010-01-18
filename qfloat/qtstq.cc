// qtstq.cc
// C++ test program for class qfloat

#include <stdio.h>
#include "qfloat.h"

/* Random numbers  */
extern "C" {
extern void qrand (qfloat *);
};
inline qfloat rand (void)
{
  qfloat y;
  qrand (&y);
  return y;
}

qfloat a, b, c, n, r0, r1, e, max;
qfloat aa, ab;
qfloat low, width;

int
main ()
{
  int m;

  // Random number interval.
  low = 2.0;
  width = 1.0e6;

  // Conversions from int, long, double to qfloat are OK.
  max = 0;
  for (m = 0; m < 1000; m++)
    {
      a = rand ();
      a = a - 1.5;
      b = rand ();
      b = (b - 1) * width + low;

      // Function under test.
      // Remainder after dividing b by a.
      r0 = fmod (b, a);

      // Compute it a different way.
      aa = fabs (a);
      ab = fabs (b);
      n = floor (ab / aa);
      r1 = ab - n * aa;
      if (b < 0)
	r1 = -r1;

      // Record differences.
      e = fabs (r1 - r0);
      if (e > max)
	{
	  max = e;
	  printf ("m=%d ", m);
	  qprint (5, "a =", a, "");
	  qprint (5, "b =", b, "");
	  qprint (5, "n =", n, "");
	  qprint (5, "r0 =", r0, "");
	  qprint (5, "r1 =", r1, "");
	  qprint (5, "max =", max, "\n");
	}
    }
  qprint (20, "max", max, "\n");
  return 0;
}
