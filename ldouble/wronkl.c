/* Wronksian test for Bessel functions. */

long double jnl (), ynl (), floorl ();
#define PI 3.14159265358979323846L

long double y, Jn, Jnp1, Jmn, Jmnp1, Yn, Ynp1;
long double w1, w2, err1, max1, err2, max2;
void wronk ();

main ()
{
  long double x, delta;
  int n, i, j;

  max1 = 0.0L;
  max2 = 0.0L;
  delta = 0.6 / PI;
  for (n = -30; n <= 30; n++)
    {
      x = -30.0;
      while (x < 30.0)
	{
	  wronk (n, x);
	  x += delta;
	}
      delta += .00123456;
    }
}

void 
wronk (n, x)
     int n;
     long double x;
{

  Jnp1 = jnl (n + 1, x);
  Jmn = jnl (-n, x);
  Jn = jnl (n, x);
  Jmnp1 = jnl (-(n + 1), x);
  /* This should be trivially zero.  */
  err1 = Jnp1 * Jmn + Jn * Jmnp1;
  if (err1 < 0.0)
    err1 = -err1;
  if (err1 > max1)
    {
      max1 = err1;
      printf ("1 %3d %.5Le %.3Le\n", n, x, max1);
    }
  if (x < 0.0)
    {
      x = -x;
      Jn = jnl (n, x);
      Jnp1 = jnl (n + 1, x);
    }
  Yn = ynl (n, x);
  Ynp1 = ynl (n + 1, x);
  /* The Wronksian.  */
  w1 = Jnp1 * Yn - Jn * Ynp1;
  /* What the Wronksian should be. */
  w2 = 2.0 / (PI * x);
  err2 = w1 - w2;
  if (err2 > max2)
    {
      max2 = err2;
      printf ("2 %3d %.5Le %.3Le\n", n, x, max2);
    }
}
