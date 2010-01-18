/*							unityl.c
 *
 * Relative error approximations for function arguments near
 * unity.
 *
 *    cosm1(x) = cos(x) - 1
 *
 */

#include "mconf.h"
#ifdef ANSIPROT
extern long double cosl ( long double );
extern long double polevll ( long double, void *, int );
extern long double p1evll ( long double,  void *, int );
#else
long double cosl(), polevll(), p1evll();
#endif


/* cosm1(x) = cos(x) - 1  */

static long double coscof[7] = {
 4.7377507964246204691685E-14L,
-1.1470284843425359765671E-11L,
 2.0876754287081521758361E-9L,
-2.7557319214999787979814E-7L,
 2.4801587301570552304991E-5L,
-1.3888888888888872993737E-3L,
 4.1666666666666666609054E-2L,
};

extern long double PIO4L;

long double cosm1l(x)
long double x;
{
long double xx;

if( (x < -PIO4L) || (x > PIO4L) )
	return( cosl(x) - 1.0L );
xx = x * x;
xx = -0.5L*xx + xx * xx * polevll( xx, coscof, 6 );
return xx;
}
