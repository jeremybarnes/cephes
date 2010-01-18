
/* monotl.c
   Floating point function test vectors for real functions of two variables..

   Arguments and function values are synthesized for NPTS points in
   the vicinity of each given tabulated test point.  The points are
   chosen to be near and on either side of the likely function algorithm
   domain boundaries.  Since the function programs change their methods
   at these points, major coding errors or monotonicity failures might be
   detected.

   August, 1998
   S. L. Moshier  */


/* Unit of error tolerance in test[i].thresh.  */
static long double MACHEPL =
  5.42101086242752217003726400434970855712890625E-20L;
/* How many times the above error to allow before printing a complaint.
   If TOL < 0, consider absolute error instead of relative error. */
#define TOL 8
/* Number of test points to generate on each side of tabulated point.  */
#define NPTS 10


#include <stdio.h>


/* Avoid including math.h.  */
long double frexpl (long double, int *);
long double ldexpl (long double, int);
long double fabsl (long double);

/* Power function, a = x^y, da/dx = y x^(y-1), da/dy = x^y ln(x).  */
long double powl (long double, long double);
/* Arctan, a=atan(y,x), da/dx = -y/(z x^2), da/dy = 1/(z x), z = (y/x)^2 + 1 */
long double atan2l (long double, long double);


/* Data structure of the test parameters.  */
struct oneargument
  {
    char *name;			/* Name of the function. */
    long double (*func) (long double, long double); /* Function call.  */
    long double argx;		/* Function argument, assumed exact.  */
    long double argy;		/* Function argument, assumed exact.  */
    long double answer1;	/* Exact number, close to function value.  */
    long double answer2;	/* answer1 + answer2 has extended precision. */
    long double derivx;         /* da/dx evaluated at x = argx, y = argy. */
    long double derivy;         /* da/dy evaluated at x = argx, y = argy. */
    /* Error report threshold. 2 => 1 ulp approximately
       if thresh < 0 then consider absolute error instead of relative error. */
    int thresh;
  };



struct oneargument test1[] =
{

  {"pow", powl, 2.0L, 2.0L,
   4.0L, 0.0L,
   4.0, 2.7725887222397812376689284858327062723020E0, TOL},

  {"pow", powl, 1.0L, 1.0L,
   1.0L, 0.0L,
   1.0, 0.0L, TOL},

  {"pow", powl, 1.0L, -1.0L,
   1.0L, 0.0L,
   -1.0, 0.0L, TOL},

  {"pow", powl, 1.2247314453125L, 100.25L,
   6.70034366e8, 6.51791572932896622403738102113103165E-1L,
   5.48454483747683171222959718285483015E10L,
   1.35830433265387651661998457847818722E8L, TOL},

  {"pow", powl, 1.732025146484375L, 100.25L,
   8.22352190053644e23L, 2.29126955833058414993363587912443115E7L,
   4.75979273281420171987670106527182253E25L,
   4.51710927208146658483757152447209421E23L, TOL},

  {"atan2", atan2l, 2.0L, 1.0L,
   1.107147216796875L, 1.50099721550301706546017853704007005E-6L,
   0.2L, -0.4L,TOL},
  {"atan2", atan2l, 1.0L, 1.0L,
   7.853851318359375e-1L, 1.30315615108096156608458198757210493E-5L,
   0.5L, -0.5L, TOL},

  /*
  {"pow", powl, 1.00000762939453125L, 7.47061248e8L,
   2.024155560489166427408196e2475L,
   1.49425948466361441820864586444388248e2450L,
   1.51215664232555880975860314827295147E2484L,
   1.54430224532147078846312445020772189E2470L, TOL},
  */
  {"null", NULL, 0.0L, 0.0L,
   0.0L, 0.0L,
   0.0L, 0.0L, 1},
};

/* These take care of extra-precise floating point register problems.  */
volatile long double volat1;
volatile long double volat2;

long double argsx[2*NPTS+2];
long double argsy[2*NPTS+2];

/* Return the next nearest floating point value to X
   in the direction of UPDOWN (+1 or -1).
   (Might fail if X is denormalized.)  */

long double
nextval (x, updown)
     long double x;
     int updown;
{
  long double m;
  int i;

  volat1 = x;
  m = 0.25L * MACHEPL * volat1 * updown;
  volat2 = volat1 + m;
  if (volat2 != volat1)
    printf ("successor failed\n");

  for (i = 2; i < 10; i++)
    {
      volat2 = volat1 + i * m;
      if (volat1 != volat2)
	return volat2;
    }

  printf ("nextval failed\n");
  return volat1;
}




int
main ()
{
  long double (*fun1) (long double, long double);
  int i, jx, jy, errs, tests, err_thresh;
  int idir, direction, itest;
  long double x, x0, dx, y, y0, dy, a, da, err;

  errs = 0;
  tests = 0;
  itest = 0;

  for (;;)
    {
      /* Function call reference.  */
      fun1 = test1[itest].func;
      if (fun1 == NULL)
	break;

      /* Make lookup tables of all x arguments and all y arguments
	 to be used. The arguments scan from slightly below to slightly
         above x0 and y0.  */
      for (idir = 0; idir < 2; idir++)
	{
	  if (idir == 0)
	    direction = 1;
	  else
	    direction = -1;
	  volat1 = test1[itest].argx;
	  x0 = volat1;
	  x = volat1;
	  argsx[NPTS] = x0;
	  volat1 = test1[itest].argy;
	  y0 = volat1;
	  y = volat1;
	  argsy[NPTS] = y0;
	  for (i = 1; i <= NPTS; i++)
	    {
	      x = nextval (x, direction);
	      argsx[NPTS + i * direction] = x;
	      y = nextval (y, direction);
	      argsy[NPTS + i * direction] = y;
	    }
	}

      /* Test at all points (x,y) in the lookup tables.  */
      for (jx = 0; jx <= 2*NPTS; jx++)
	{
	  /* delta x */
	  x = argsx[jx];
	  volat1 = x - x0;
	  dx = volat1;
	  for (jy = 0; jy <= 2*NPTS; jy++)
	    {
	      /* delta y */
	      y = argsy[jy];
	      volat1 = y - y0;
	      dy = volat1;
	      /* Estimated delta answer */
	      da = dx * test1[itest].derivx + dy * test1[itest].derivy;
	      /* a + delta a */
	      a = test1[itest].answer2 + da;
	      volat1 = test1[itest].answer1 + a;
	      a = volat1;
	      /* Run the function under test.  */
	      volat2 = (*(fun1)) (x, y);
	      if (volat2 != volat1)
		{
		  /* Estimate difference between program result
		     and extended precision function value.  */
		  err = volat2 - test1[itest].answer1;
		  err = err - (test1[itest].answer2 + da);
		  /* Compare difference with reporting threshold.  */
		  err_thresh = test1[itest].thresh;
		  if (err_thresh >= 0)
		    err = err / volat1; /* relative error */
		  else
		    {
		      err_thresh = -err_thresh; /* absolute error */
		      /* ...but relative error if function value > 1 */
		      if (fabsl(volat1) > 1.0L)
			err = err / volat1;
		    }
		  if (fabsl (err) > (err_thresh * MACHEPL))
		    {
		      printf ("%s(%.20Le, %.20Le)\n", test1[itest].name, x, y);
		      printf (" = %.20Le, s.b. %.20Le, %.3Le\n",
			      volat2, volat1, err);
		      errs += 1;
		    }
		}
	      tests += 1;
	    }
	}
      itest += 1;
    }
  printf ("%d errors in %d tests\n", errs, tests);
  exit (0);
}
