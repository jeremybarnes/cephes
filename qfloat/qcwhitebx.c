/* qcwhitebx.c
   White box tests
   attempt to exercise all paths through each tested function routine.
   Q-type complex arithmetic version.

   S. L. Moshier
   May, 2004  */

#include <stdio.h>
#include <stdlib.h>
#include "qhead.h"

/* The error reporting threshold depends mainly on the precision to which
   decimal result values are provided.  */
#define ERR_THRESH 1e-40

struct oneargument
  {
    char *name;                 /* Name of the function. */
    int (*func) (qcmplx *, qcmplx *);
    char *real_arg_str;         /* Function argument, assumed exact.  */
    char *imag_arg_str;
    char *real_ans_str;
    char *imag_ans_str;
    int thresh;                 /* Error report threshold. Not used. */
  };

QELT real_arg[NQ];
QELT imag_arg[NQ];
QELT real_ans[NQ];
QELT imag_ans[NQ];

QELT real_err[NQ];
QELT imag_err[NQ];
double d_real_err, d_imag_err;
double fabs (double);


struct oneargument test1[] =
{
  {"qctan", qctan, "1.0", "0.0",
  "1.5574077246549022305069748074583601730872507723815200383839466056988614E0",
   "0.0", 0},

  {"qcsqrt", qcsqrt, "0.0", "0.0", "0.0", "0.0", 0},

  {"qcsqrt", qcsqrt, "1.0", "0.0", "1.0", "0.0", 0},

  {"qcsqrt", qcsqrt, "-1.0", "0.0", "0.0", "1.0", 0},

  {"qcsqrt", qcsqrt, "0.0", "1.0",
   "0.7071067811865475244008443621048490392848",
   "0.7071067811865475244008443621048490392848", 0},

  {"qcsqrt", qcsqrt, "0.0", "-1.0",
    "0.7071067811865475244008443621048490392848",
   "-0.7071067811865475244008443621048490392848", 0},

  {"qcsqrt", qcsqrt, "1.0", "1.0",
   "1.098684113467809966039801195240678378544",
   "0.455089860562227341304357757822468569620", 0},

  {"qcsqrt", qcsqrt, "1.0", "-1.0",
   "1.098684113467809966039801195240678378544",
   "-0.455089860562227341304357757822468569620", 0},

  {"qcsqrt", qcsqrt, "-1.0", "1.0",
   "0.455089860562227341304357757822468569620",
   "1.098684113467809966039801195240678378544", 0},

  {"qcsqrt", qcsqrt, "-1.0", "-1.0",
   "0.455089860562227341304357757822468569620",
   "-1.098684113467809966039801195240678378544", 0},

  {"qcsqrt", qcsqrt,
   "3.4028234663852885981170418348451692544e38",  /* 2^128 * (1 - 2^(-24))  */
   "3.4028234663852885981170418348451692544e38",
   "2.0267144054983168049787510174924825580748e19",
   "8.3949259381432729882118785162080155862810e18", 0},

  {"qcsqrt", qcsqrt,
   "1.4012984643248170709237295832899161312802619e-45", /* 2^-149 */
   "1.4012984643248170709237295832899161312802619e-45",
   "4.1128054643427787980970034627701752008029081e-23",
   "1.7035798027329537503686597356013897095512107e-23", 0},

  {"null",   NULL, "0.0", "0.0", "0.0", "0.0", 0},
};


int
main ()
{
  int (*fun1) (qcmplx *, qcmplx *);
  qcmplx z, w;
  /*  char num[128]; */
  int i, k, errs, tests;
  union
    {
      double d;
      unsigned short s[4];
    }  u;

  errs = 0;
  tests = 0;
  i = 0;
  for (;;)
    {
      fun1 = test1[i].func;
      if (fun1 == NULL)
	break;

      /* Convert tabulated values to binary.  */
      asctoq (test1[i].real_arg_str, real_arg);
      asctoq (test1[i].imag_arg_str, imag_arg);
      asctoq (test1[i].real_ans_str, real_ans);
      asctoq (test1[i].imag_ans_str, imag_ans);

      /* Construct the complex argument.  */
      qmov (real_arg, z.r);
      qmov (imag_arg, z.i);

      /* Call the function under test.  */
      k = (*(fun1)) (&z, &w);

      /* Estimate the error of the result.  */
      qsub (w.r, real_ans, real_err);
      if (real_ans[1] != 0)
	qdiv (real_ans, real_err, real_err);
      qtoe (real_err, u.s);
      d_real_err = u.d;

      qsub (w.i, imag_ans, imag_err);
      if (imag_ans[1] != 0)
	qdiv (imag_ans, imag_err, imag_err);
      qtoe (imag_err, u.s);
      d_imag_err = u.d;


#if 1
      if ((fabs (d_real_err) > ERR_THRESH) || (fabs (d_imag_err) > ERR_THRESH))
	{
	  errs += 1;
	  printf ("Line %d: %s error = %.3e ", i + 1, test1[i].name,
		  d_real_err);
	  if (d_imag_err >= 0.0)
	    printf ("+ ");
	  printf ("%.3e i\n", d_imag_err);
	}
#else
      if ((qcmp (w.r, real_ans) != 0) || (qcmp (w.i, imag_ans) != 0))
	{
	  errs += 1;
	  qtoasc (w.r, num, 70);
	  printf ("Line %d: %s\n", i + 1, num);
	  qtoasc (w.i, num, 70);
	  printf ("%si\n", num);
	  qtoasc (real_ans, num, 70);
	  printf ("s.b. %s\n", num);
	  qtoasc (imag_ans, num, 70);
	  printf ("%si\n", num);
	  /*	  
	  printf ("Line %d: %.9e %9e, s.b. %.9e %.9e\n", i + 1,
		  creal(w), cimag(w), test1[i].real_ans, test1[i].imag_ans);
	  */
	}
#endif
      i += 1;
      tests += 1;
    }
  printf ("%d errors in %d tests\n", errs, tests);
  exit (0);
}
