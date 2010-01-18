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
y = "123.0";
if(x != y)
  {
    qtoasc(&x, str, 10);
    printf("%ld = %s\n", li, str);
    errs += 1;
  }
qifrac(&x, &li, &y);
if( li !=123L )
  errs += 1;

li = qtol(x);
if( li !=123L )
  errs += 1;

dx = li;
x = dx;
dy = qtod (x);
if (dx != dy)
  errs += 1;
y = dy;
if (x != y)
  errs += 1;

ldx = li;
x = ldx;
ldy = qtold (x);
if (ldx != ldy)
  errs += 1;
y = ldy;
if (x != y)
  errs += 1;

printf("%d errors\n", errs);
}
