

/* Runge-Kutta numerical integration
 * of a system of ordinary differential equations.
 *
 * Reference:
 * Thomas, Benku, "The Runge-Kutta Methods," BYTE 11, #4, April 1986
 * This program is adapted from code supplied with that article.
 */

#include "int.h"
#define ORDER 7
#define ERRTERM 0

#if !(ORDER-4)
/*
Fourth order Runge-Kutta method specified by
Felberg, E., COMPUTING,	6(1970)p61-71
*/
#define INUM 6
#define HMAXL 0.5
#define HLIM1 3.0

double al[INUM-1] = {
1.0/4.0, 3.0/8.0, 12.0/13.0, 1.0, 1.0/2.0
};

double b[] = {
1.0/4.0, /* b(1,1) */

3.0/32.0, /* b(1,2) */
9.0/32.0, /* b(2,2) */

1932.0/2197.0, /* b(1,3) */
-7200.0/2197.0, /* b(2,3) */
7296.0/2197.0, /* b(3,3) */

439.0/216.0, /* b(1,4) */
-8.0,          /* b(2,4) */
3680.0/513.0, /* b(3,4) */
-845.0/4104.0, /* b(4,4) */

-8.0/27.0,      /* b(1,5) */
2.0,             /* b(2,5) */
-3544.0/2565.0, /* b(3,5) */
1859.0/4104.0, /* b(4,5) */
-11.0/40.0,    /* b(5,5) */
};

#if ERRTERM
double er[INUM] = {
1.0/360.0, 0.0, -384.0/12825.0, -41743.0/1429560.0, 1.0/50.0, 2.0/55.0
};
#endif

double a[INUM] = {
16.0/135.0, 0.0, 6656.0/12825.0, 28561.0/56430.0, -9.0/50.0, 2.0/55.0
};
#endif


#if !(ORDER-5)
/*
Fifth order Runge-Kutta method specified by
Verner J.H., SIAM J. Numer. Anal. V15,(1978),p772.(table 5)
*/
#define INUM 8
#define HMAXL 1.0
#define HLIM1 4.0
double al[INUM-1] = {
1.0/18.0, 1.0/6.0, 2.0/9.0, 2.0/3.0, 1.0, 8.0/9.0, 1.0
};

double b[] = {
1.0/18.0, /* b(1,1) */

-1.0/12.0, /* b(1,2) */
1.0/4.0, /* b(2,2) */

-2.0/81.0, /* b(1,3) */
4.0/27.0, /* b(2,3) */
8.0/81.0, /* b(3,3) */

40.0/33.0, /* b(1,4) */
-4.0/11.0, /* b(2,4) */
-56.0/11.0, /* b(3,4) */
54.0/11.0, /* b(4,4) */

-369.0/73.0, /* b(1,5) */
72.0/73.0, /* b(2,5) */
5380.0/219.0, /* b(3,5) */
-12285.0/584.0, /* b(4,5) */
2695.0/1752.0, /* b(5,5) */

-8716.0/891.0, /* b(1,6) */
656.0/297.0, /* b(2,6) */
39520.0/891.0, /* b(3,6) */
-416.0/11.0, /* b(4,6) */
52.0/27.0, /* b(5,6) */
0.0, /* b(6,6) */

3015.0/256.0, /* b(1,7) */
-9.0/4.0, /* b(2,7) */
-4219.0/78.0, /* b(3,7) */
5985.0/128.0, /* b(4,7) */
-539.0/384.0, /* b(5,7) */
0.0, /* b(6,7) */
693.0/3328.0, /* b(7,7) */
};

#if ERRTERM
double er[INUM] = {
33.0/640.0, 0.0, -132.0/325.0, 891.0/2240.0, -33.0/320.0,
-73.0/700.0, 891.0/8320.0, 2.0/35.0
};
#endif

double a[INUM] = {
57.0/640.0, 0.0, -16.0/65.0, 1377.0/2240.0, 121.0/320.0, 0.0,
891.0/8320.0, 2.0/35.0
};
#endif


#if !(ORDER-7)
/*
Seventh order Runge-Kutta method specified by
Verner J.H., SIAM J. Numer. Anal. V15,(1978),p772.(table 7)
*/
double al[12] = {
1.0/4.0, 1.0/12.0, 1.0/8.0, 2.0/5.0, 1.0/2.0, 6.0/7.0, 1.0/7.0,
2.0/3.0, 2.0/7.0, 1.0, 1.0/3.0, 1.0
};

double b[] = {
1.0/4.0, /* b(1,1) */

5.0/72.0, /* b(1,2) */
1.0/72.0, /* b(2,2) */

1.0/32.0, /* b(1,3) */
0.0, /* b(2,3) */
3.0/32.0, /* b(3,3) */

106.0/125.0, /* b(1,4) */
0.0, /* b(2,4) */
-408.0/125.0, /* b(3,4) */
352.0/125.0, /* b(4,4) */

1.0/48.0, /* b(1,5) */
0.0, /* b(2,5) */
0.0, /* b(3,5) */
8.0/33.0, /* b(4,5) */
125.0/528.0, /* b(5,5) */

-1263.0/2401.0, /* b(1,6) */
0.0, /* b(2,6) */
0.0, /* b(3,6) */
39936.0/26411.0, /* b(4,6) */
-64125.0/26411.0, /* b(5,6) */
5520.0/2401.0, /* b(6,6) */

37.0/392.0, /* b(1,7) */
0.0, /* b(2,7) */
0.0, /* b(3,7) */
0.0, /* b(4,7) */
1625.0/9408.0, /* b(5,7) */
-2.0/15.0, /* b(6,7) */
61.0/6720.0, /* b(7,7) */

17176.0/25515.0, /* b(1,8) */
0.0, /* b(2,8) */
0.0, /* b(3,8) */
-47104.0/25515.0, /* b(4,8) */
1325.0/504.0, /* b(5,8) */
-41792.0/25515.0, /* b(6,8) */
20237.0/145800.0, /* b(7,8) */
4312.0/6075.0, /* b(8,8) */

-23834.0/180075.0, /* b(1,9) */
0.0, /* b(2,9) */
0.0, /* b(3,9) */
-77824.0/1980825.0, /* b(4,9) */
-636635.0/633864.0, /* b(5,9) */
254048.0/300125.0, /* b(6,9) */
-183.0/7000.0, /* b(7,9) */
8.0/11.0, /* b(8,9) */
-324.0/3773.0, /* b(9,9) */

12733.0/7600.0, /* b(1,10) */
0.0, /* b(2,10) */
0.0, /* b(3,10) */
-20032.0/5225.0, /* b(4,10) */
456485.0/80256.0, /* b(5,10) */
-42599.0/7125.0, /* b(6,10) */
339227.0/912000.0, /* b(7,10) */
-1029.0/4180.0, /* b(8,10) */
1701.0/1408.0, /* b(9,10) */
5145.0/2432.0, /* b(10,10) */

-27061.0/204120.0, /* b(1,11) */
0.0, /* b(2,11) */
0.0, /* b(3,11) */
40448.0/280665.0, /* b(4,11) */
-1353775.0/1197504.0, /* b(5,11) */
17662.0/25515.0, /* b(6,11) */
-71687.0/1166400.0, /* b(7,11) */
98.0/225.0, /* b(8,11) */
1.0/16.0, /* b(9,11) */
3773.0/11664.0, /* b(10,11) */
0.0, /* 11, 11 */

11203.0/8680.0, /* b(1,12) */
0.0, /* b(2,12) */
0.0, /* b(3,12) */
-38144.0/11935.0, /* b(4,12) */
2354425.0/458304.0, /* b(5,12) */
-84046.0/16275.0, /* b(6,12) */
673309.0/1636800.0, /* b(7,12) */
4704.0/8525.0, /* b(8,12) */
9477.0/10912.0, /* b(9,12) */
-1029.0/992.0, /* b(10,12) */
0.0, /* b(11,12) */
729.0/341.0 /* b(12,12) */
};
#if ERRTERM
double er[13] = {
-1.0/480.0, 0.0, 0.0, 0.0, 0.0, -16.0/375.0, -2401.0/528000.0,
2401.0/132000.0, 243.0/14080.0, -2401.0/19200.0, -19.0/450.0,
243.0/1760.0, 31.0/720.0
};
#endif
double a[13] = {
31.0/720.0, 0.0, 0.0, 0.0, 0.0, 16.0/75.0, 16807.0/79200.0,
16807.0/79200.0, 243.0/1760.0, 0.0, 0.0, 243.0/1760.0, 31.0/720.0
};
#define INUM 13
#define HMAXL 2.0
#define HLIM1 5.0
#endif

int inum = INUM;

static double *w;
static double *au;
static double *ays;

/* Initialize pointers in work array.
 * neq >= neqn in rungek() below.  If neq > neqn, unused space
 * is left for extra variables that are not actually integrated.
 */
rkstart( neq, work )
int neq;
double work[];
{
int m;

w = &work[0];
m = neq*inum;
au = &work[m];
m += neq;
ays = &work[m];
}


/* Make a step.
 */
rungek(  neqn, x, yold, h, ynew, delta )
int neqn; /* number of first order equations */
double x; /* independent variable */
double yold[]; /* initial solution vector at x, size neqn */
double h; /* step to take in independent variable */
double ynew[]; /* output new solution vector at x+h, size neqn */
double delta[]; /* the step, size neqn */
/*double err[];*/ /* estimated error, size neqn */
{
double *pa, *pal, *pb, *per, *pw, *pb0, *pu, *pys;
double xs, usum, esum;
int i, j, m;

/*
 * w[]: neqn blocks of size inum
 *      block of size neqn for temp solution vector ys[]
 *      block of size neqn for output function vector u[] of func()
 */
xs = x;

pys = ays;
pb = yold;
for( i=0; i<neqn; i++ )
	*pys++ = *pb++;  /* ys[i] = yold[i] */

func( xs, ays, au );

pw = w;
pu = au;
for( i=0; i<neqn; i++ )
	{
	*pw = *pu++;  /* w[i*inum] = u[i] */
	pw += inum;
	}

pb0 = &b[0];
pal = &al[0];
for( j=1; j<inum; j++ )
	{
	xs = x + h * *pal++;
	pys = ays;
	for( i=0; i<neqn; i++ )
		{
		usum = 0.0;
		pw = w + i*inum;
		pb = pb0;
		for( m=0; m<j; m++ )
			usum += (*pb++) * (*pw++);
		*pys++ = yold[i] + h * usum;
		}
	func( xs, ays, au );
	pw = w + j;
	pu = au;
	for( i=0; i<neqn; i++ )
		{
		*pw = *pu++;  /* w[j+i*inum] = u[i] */
		pw += inum;
		}
	pb0 += j;
	}

/* evaluate YNEW[i] and the error estimates EST[i] */
pw = w;
for( i=0; i<neqn; i++ )
	{
	pa = &a[0];
#if ERRTERM
	per = &er[0];
#endif
	usum = 0.0;
	esum = 0.0;
	for( j=0; j<inum; j++ )
		{
		if( *pa != 0 )
			{
#if ERRTERM
			esum += (*per++) * (*pw);
#endif
			usum += (*pa++) * (*pw++);
			}
		else
			{ /* skip if coefficient = 0 */
			++pa;
#if ERRTERM
			++per;
#endif
			++pw;
			}
		}
	xs = h * usum; /* the step in this coordinate */
	delta[i] = xs;
	ynew[i] = yold[i] + xs;

#if ERRTERM
	err[i] = h * esum;
#endif
	}
}
