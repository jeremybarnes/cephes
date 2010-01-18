#include <stdio.h>
long double inf = 1.0f/0.0f;
long double nnn = 1.0f/0.0f - 1.0f/0.0f;
long double fin = 1.0f;
long double neg = -1.0f;
long double nn2;

int isnanl(), isfinitel(), signbitl();
void abort (void);
void exit (int);

void pvalue (char *str, long double x)
{
union
  {
    long double f;
    unsigned int i[3];
  }u;
int k;

printf("%s ", str);
u.f = x;
for (k = 0; k < 3; k++)
  printf("%08x ", u.i[k]);
printf ("\n");
}


int
main()
{

if (!isnanl(nnn))
  abort();
pvalue("nnn", nnn);
pvalue("inf", inf);
nn2 = inf - inf;
pvalue("inf - inf", nn2);
if (isnanl(fin))
  abort();
if (isnanl(inf))
  abort();
if (!isfinitel(fin))
  abort();
if (isfinitel(nnn))
  abort();
if (isfinitel(inf))
  abort();
if (!signbitl(neg))
  abort();
if (signbitl(fin))
  abort();
if (signbitl(inf))
  abort();
/*
if (signbitf(nnn))
  abort();
  */
exit (0);
return 0;
}
