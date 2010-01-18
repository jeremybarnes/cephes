#include <stdio.h>
#include "qfloat.h"

qfloat x, y, z;
float fx, fy, fz;
double dx, dy, dz;
long double ldx, ldy, ldz;
long int li, lj;
int i;
char str[80];

int
main ()
{
int errs = 0;

li = 123L;
x = li;
if (x != 123.0)
  errs += 1;
y = x + 12.0;
if (y != 135.0)
  errs += 1;
if (x > 123.0)
  errs += 1;
if (x < 123.0)
  errs += 1;
if (x > y)
  errs += 1;
if (x == 123.0)
  y += 1.0;
if (y != 136.0)
  errs += 1;
if (y == 135.0)
  errs += 1;
printf("%d errors\n", errs);
}
