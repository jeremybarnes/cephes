/* Main program to perform a point-mass integration of the
 * Solar system, with relativistic corrections.  The initial
 * conditions are those of the JPL DE200 numerical integration.
 * The final solution, given in the vector yn1[], is taken from
 * the DE200 for 100 days later.  A test run of 400 steps in double
 * precison IEEE arithmetic with step size = 1/4 day and 11th
 * order Adams integration yielded agreement to about 10^-10 au
 * for the inner planets, 10^-12 au for Pluto, and 10^-8 au
 * for the Moon.  The DE200, of course, included Earth-Moon figure
 * effects and five of the asteroids, and its arithmetic was
 * probably higher than double precision.
 *
 * If the Adams order is set to N, the first N+1 steps are
 * performed by the Runge-Kutta subroutine rungek.c.  These
 * take about 7 times longer than the subsequent ones done
 * by the Adams-Bashforth-Moulton routine adams.c.
 *
 * Neither the step size nor the approximation order has been
 * made self-adjusting.  Some experimentation is recommended, to
 * achieve the desired balance of speed and accuracy.  The
 * example above was chosen to give about the same theoretical
 * error per day as specified in the DE102 reference below.
 *
 * -Steve Moshier
 */

#include "int.h"

/* Compute relativity corrections, or not: */
#define DOREL 1

/* Include the Moon as a separate body, or not: */
#define MOON 1

/* Constrain the relativistic barycenter of the solar
 * system to stay at the origin.  Note, with the asteroids
 * (10^-9 solar masses) missing, this step is possibly useless
 * and was omitted in the test example.  The code for it is
 * supplied at the end of this file.
 */
#define BARYC 0

double sqrt();

#if MOON
#define NBODY 11
#define IMOON 9
#else
#define NBODY 10
#endif
#define ISUN (NBODY-1)
#define NTOTAL (NBODY)
#define NEQ (6*NBODY)
#define MAXORD 13

#if DOREL
#if BARYC
#define NEQC (NEQ-6)
#else
#define NEQC NEQ
#endif
#else
#define NEQC (NEQ)
#endif

static double vnewt[NEQ];
static double awork[ (NEQ*(MAXORD+2))+(MAXORD+2)+(6*NEQ) ];
static double rwork[(MAXRUNG+2)*NEQ];
static double Rij[NTOTAL][NTOTAL];

/* Speed of light. Note, all dimensions are in astronomical
 * units (au) and days (86,400 seconds).
 */
#define C 173.1446328

/* Solar system barycentric velocity and position
 * state of Mercury, Venus, EMB, ..., Pluto, MOON, Sun
 * in the order dx/dt, x, dy/dt, y, dz/dt, z for each object.
 *
 * EMB is the arithmetic average of Earth and Moon, weighted by
 * their masses. The coordinates of the MOON variable are the difference
 * between the solar system barycentric coordinates of the Moon
 * minus those of the Earth.
 */

/* Julian date of the initial state yn[]
 * June 28.0, 1969:
 */
#define JD0 2440400.50

static double yn[] = {
 3.367492758813184e-003, /* Mercury */
 3.617650084026384e-001,
  2.489452047536652e-002,
 -9.077580081092690e-002,
  1.294630071041128e-002,
 -8.571250013597834e-002,

 1.095206748972985e-002, /* Venus */
 6.127542336221157e-001,
  1.561768454932838e-002,
 -3.483591895730089e-001,
  6.331105550175031e-003,
 -1.952758215816601e-001,

 1.681126759384403e-002, /* EMB */
 1.205197129002194e-001,
  1.748308899617618e-003,
 -9.258323035784091e-001,
  7.582041930175932e-004,
 -4.015377611192672e-001,

 1.448165239083299e-002, /* Mars */
-1.101837696063662e-001,
  2.424628123494023e-004,
 -1.327593278911103e+000,
 -2.815196436092114e-004,
 -6.058866834707231e-001,

 1.092015004926191e-003, /* Jupiter */
-5.379703845550703e+000,
 -6.518116265389094e-003,
 -8.304767428789135e-001,
 -2.820782997470719e-003,
 -2.248295252614748e-001,

-3.217557586964646e-003, /* Saturn */
 7.894394222587950e+000,
  4.335809727271375e-003,
  4.596483181197301e+000,
  1.928645932522133e-003,
  1.558697670343092e+000,

 2.211920155513113e-004, /* Uranus */
-1.826538634212984e+001,
 -3.762477278855801e-003,
 -1.161959795438843e+000,
 -1.651014777644869e-003,
 -2.501080003472951e-001,

 2.642773506889557e-003, /* Neptune */
-1.605499953831724e+001,
 -1.498309226803948e-003,
 -2.394216805154041e+001,
 -6.790394433396178e-004,
 -9.400159225576589e+000,

 3.221893230538327e-004, /* Pluto */
-3.048349217668479e+001,
 -3.143582327761257e-003,
 -8.724432817555443e-001,
 -1.077956399281471e-003,
  8.911620591248875e+000,
#if MOON
 6.010848314829119e-004,
-8.081772358351251e-004,
 -1.674454691500606e-004,
 -1.994630037441996e-003,
 -8.556208109904861e-005,
 -1.087262721620868e-003,
#endif
-3.524457445683394e-007, /* Sun */
 4.504795855674601e-003,
  5.177637780672221e-006,
  7.732544746890742e-004,
  2.229113252400401e-006,
  2.685039985573271e-004
};


/* Julian date of the state yn1[]: */
#define JD1 2440500.50

static double yn1[] = {
-2.457085900366332e-002,
 2.381597794693489e-001,
  1.851778146680485e-002,
  1.996323209474686e-001,
  1.244039124191612e-002,
  8.216842968554781e-002,

-1.619513532964599e-002,
-4.288838589850871e-001,
 -1.160656795724704e-002,
  5.131125475173164e-001,
 -4.195286108659672e-003,
  2.581368657564266e-001,

-4.148833933015173e-003,
 9.784941438766318e-001,
  1.532745170011609e-002,
  2.074410311959717e-001,
  6.646462310684277e-003,
  8.989091702317244e-002,

 8.350422878366247e-003,
 1.151108827619137e+000,
  1.172723634265029e-002,
 -6.894717355437423e-001,
  5.152260844982779e-003,
 -3.474143803147213e-001,

 2.061461649176091e-003,
-5.221785914203378e+000,
 -6.307486608326940e-003,
 -1.472733759859347e+000,
 -2.754121811344123e-003,
 -5.039994787357370e-001,

-3.506509603502537e-003,
 7.558106559699063e+000,
  4.155902532833776e-003,
  5.021213850462841e+000,
  1.866781421893536e-003,
  1.748532556433840e+000,

 3.093738240936483e-004,
-1.823885644320201e+001,
 -3.755962489549436e-003,
 -1.537896840580550e+000,
 -1.649410160709415e-003,
 -4.151358646421778e-001,

 2.659694712570950e-003,
-1.578987378978514e+001,
 -1.472781001534497e-003,
 -2.409072389641744e+001,
 -6.690116571980626e-004,
 -9.467562384490781e+000,

 3.504107115900905e-004,
-3.044986307301521e+001,
 -3.142629755658865e-003,
 -1.186756347889178e+000,
 -1.086164460786462e-003,
  8.803414050220775e+000,
#if MOON
-4.181662799524538e-004,
-1.749747307712349e-003,
 -3.263107309648734e-004,
  1.807047780153076e-003,
 -1.847196922506988e-004,
  9.595533885666276e-004,
#endif
-1.063378525544779e-006,
 4.434825780648206e-003,
  5.049061922605073e-006,
  1.283313197720549e-003,
  2.188756349489559e-006,
  4.887462262250162e-004
};


/* Earth's mass divided by Moon's mass
 */
#define EMRAT 81.300587


/* GM's of the solar system bodies
 * These are scaled such that GMsun = k^2
 * (k = Gaussian gravitational constant).
 */
double GMs[] = {
 4.912547451450812e-011, /* Mercury */
 7.243456209632766e-010, /* Venus */
#if MOON
 (8.997011658557308e-010*EMRAT)/(1.0+EMRAT), /* Earth */
#else
 8.997011658557308e-010, /* EMB */
#endif
 9.549528942224058e-011, /* Mars */
 2.825342103445926e-007, /* Jupiter */
 8.459468504830660e-008, /* Saturn */
 1.288816238138035e-008, /* Uranus */
 1.532112481284276e-008, /* Neptune */
 2.276247751863699e-012, /* Pluto */
#if MOON
 (8.997011658557308e-010)/(1.0+EMRAT), /* Moon */
#endif
 2.959122082855911e-004 /* Sun */
};


main()
{
double t, e0, e, err, h, ccor;
double *pdv;
int i, ii, j, nsteps, ord;
double adstep();


orlup:
printf( "Adams order ? " );
scanf( "%d", &ord );
if( (ord > MAXORD) || (ord < 1) )
	{
	printf( "order must be between 1 and %d\n", MAXORD );
	goto orlup;
	}

printf( "step size, days ? " );
scanf( "%lf", &h );

printf( "Number of steps ? " );
scanf( "%d", &nsteps );

for( i=0; i<((MAXRUNG+2)*NEQ); i++ )
	rwork[i] = 0.0;

printf( "initializing...\n" );
t = JD0;
err = 0.0;
rkstart( NEQ, rwork );
adstart( h, yn, awork, NEQ, ord, t );
printf( "initialized.\n" );


for( j=1; j<=nsteps; j++ )
	{
	err += adstep( &t, yn, NEQC );
	printf( "%5d %11.2lf %.3e\n", j, t, err );
	}

printf( "Final x, y, z, positions and errors:\n" );
ii = 0;
for (i=0; i<NTOTAL; i++)
	{
	printf("%19.15lf  %19.15lf  %19.15lf\n",
		yn[ii+1], yn[ii+3], yn[ii+5] );
	printf("%19.3e    %19.3e    %19.3e\n",
		yn[ii+1] - yn1[ii+1],
		yn[ii+3] - yn1[ii+3],
		yn[ii+5] - yn1[ii+5] );
	ii += 6;
	}
}



/* Subroutine func() calculates the forces and accelerations.
 * Data in the output vector v[] are in the order
 * d^2x/dt^2, dx/dt, d^2y/dt^2, dy/dt, d^2z/dt^2, dz/dt
 * for each object.  For this problem the velocities dx/dt, ...
 * are simply copied over from the input array y[].
 */

#if MOON
#define yxx yin
#else
#define  yxx y
#endif

func( t, yxx, v )
double t; /* time */
double yxx[]; /* input state: velocity and position */
double v[]; /* output: calculated acceleration, copy of input velocity */
{
int i, j, k;
int ii, jj;
double xs, ys, zs;
double xd, yd, zd, r, s, csqi;
double temp, rc, vi;
double xv, yv, zv;

/* Copy input to temp and unravel earth/moon
 */
#if MOON
static double xm, ym, zm;
static double y[NEQ];

for( i=0; i<NEQ; i++ )
	y[i] = yin[i];
/* Locally replace input variable EMB by barycentric earth
 * and input variable M by barycentric Moon.
 */
xm = yin[55]; /* M */
ym = yin[57];
zm = yin[59];
ii = 12;
jj = 54;
for( i=0; i<6; i++ )
	{
	zd = yin[ii+i];			/* EMB */
	yd = yin[jj+i]/(1.0+EMRAT);	/* M */
	y[ii+i] = zd -  yd;		/* r_e */
	y[jj+i] = zd + EMRAT * yd;	/* r_m */
	}
#endif

/* Constrain the barycenter to stay at the origin.
 * This is done by adjusting the Sun, which body is then
 * not included with the variables that are integrated.
 */
#if DOREL
#if BARYC
	fixsun( y, v );
#endif
#endif

/* Table of distances between objects i and j */
jj = 0;
for (j=0; j<NTOTAL; j++)
	{
	ii = 0;
	xv = y[jj+1];
	yv = y[jj+3];
	zv = y[jj+5];
	for (i=0; i<j; i++)
		{
		xd = xv - y[ii+1];
		yd = yv - y[ii+3];
		zd = zv - y[ii+5];
		r = sqrt(xd*xd + yd*yd + zd*zd);
		Rij[j][i] = r;
		Rij[i][j]  = r;
		ii += 6;
		}
	jj += 6;
	}

#if MOON
/* Take the input M vector for distance from Earth to Moon. */
r = sqrt( xm*xm + ym*ym + zm*zm );
Rij[2][9] = r;
Rij[9][2] = r;
#endif

/* Compute Newtonian acceleration. */
ii = 0;
for( i=0; i<NTOTAL; i++ )
	{
	xs = 0.0;
	ys = 0.0;
	zs = 0.0;
	xv = y[ii+1];
	yv = y[ii+3];
	zv = y[ii+5];
	jj = 0;
	for( j=0; j<NTOTAL; j++ )
		{
		if( j != i )
			{
			xd = y[jj+1] - xv;
			yd = y[jj+3] - yv;
			zd = y[jj+5] - zv;
			r = Rij[i][j];
			temp = GMs[j]/(r * r * r);
			xs += xd * temp;
			ys += yd * temp;
			zs += zd * temp;
			}
		jj += 6;
		}
	vnewt[ii] = xs; /* acceleration */
	vnewt[ii+2] = ys;
	vnewt[ii+4] = zs;
	vnewt[ii+1] = y[ii]; /* velocity */
	vnewt[ii+3] = y[ii+2];
	vnewt[ii+5] = y[ii+4];
	ii += 6;
	}

#if DOREL

/* Relativistic corrections.  Reference:
 *
 * Newhall, XX, E. M. Standish, and J. G. Williams, "DE102: a
 * numerically integrated ephemeris of the Moon and planets
 * spanning forty-four centuries," Astronomy and Astrophysics
 * 125, 150-167 (1983).
 */
	
csqi = 1.0/(C*C);
ii = 0;
for (i=0; i<NBODY; i++)
	{
	v[ii] = 0.0;
	v[ii+2] = 0.0;
	v[ii+4] = 0.0;

	xv = y[ii]; /* velocity */
	yv = y[ii+2];
	zv = y[ii+4];
	vi = xv*xv + yv*yv + zv*zv;

	xs = 0.0;
	ys = 0.0;
	zs = 0.0;
	jj = 0;
	for (j=0; j<NTOTAL; j++)
		{
		if( j == i )
			{
			jj += 6;
			continue; /* skip to next j if i = j */
			}

		xd = y[jj+1] - y[ii+1];
		yd = y[jj+3] - y[ii+3];
		zd = y[jj+5] - y[ii+5];

		s = 0.0;
		for (k=0; k<NTOTAL; k++)
			{
			if( k == i )
				continue;
			s += GMs[k]/Rij[i][k];
			}
		rc = -4.0 * csqi * s;

		s = 0.0;
		for (k=0; k<NTOTAL; k++)
			{
			if(k ==j )
				continue;
			s += GMs[k]/Rij[j][k];
			}
		rc -= csqi * s;

		rc += vi * csqi;

		xv = y[jj];
		yv = y[jj+2];
		zv = y[jj+4];

		r = xv * xv + yv * yv + zv * zv;
		rc += 2.0 * csqi * r;

		r = y[ii] * y[jj] + y[ii+2] * y[jj+2] + y[ii+4] * y[jj+4];
		rc -= 4.0 * csqi * r;

		s = -xd * y[jj] - yd * y[jj+2] - zd * y[jj+4];
		s /= Rij[i][j];
		rc -= 1.5 * csqi * s * s;

		s = xd * vnewt[jj] + yd * vnewt[jj+2] + zd * vnewt[jj+4];
		rc += 0.5 * csqi * s;

		r = Rij[i][j];
		temp = GMs[j]/(r * r * r);

		rc = temp * (1.0 + rc );
		xs += xd * rc;
		ys += yd * rc;
		zs += zd * rc;

		s = -xd * (4.0*y[ii] - 3.0*y[jj])
			- yd * (4.0*y[ii+2] - 3.0*y[jj+2])
			- zd * (4.0*y[ii+4] - 3.0*y[jj+4]);
		s = s * temp * csqi;
		xs += s * (y[ii] - y[jj]);
		ys += s * (y[ii+2] - y[jj+2]);
		zs += s * (y[ii+4] - y[jj+4]);

		temp = 3.5 * csqi * GMs[j] / r;
		xs += temp * vnewt[jj];
		ys += temp * vnewt[jj+2];
		zs += temp * vnewt[jj+4];
		jj += 6;
		}
	v[ii] = xs;
	v[ii+2] = ys;
	v[ii+4] = zs;
	v[ii+1] = y[ii];
	v[ii+3] = y[ii+2];
	v[ii+5] = y[ii+4];
	ii += 6;
	}
#else
/* No relativity theory. Return the Newtonian accelerations. */
ii = 0;
for( i=0; i<NTOTAL; i++ )
	{
	v[ii] = vnewt[ii];
	v[ii+2] = vnewt[ii+2];
	v[ii+4] = vnewt[ii+4];
	v[ii+1] = y[ii];
	v[ii+3] = y[ii+2];
	v[ii+5] = y[ii+4];
	ii += 6;
	}
#endif

#if MOON
/* Convert barycentric Earth and Moon to output EMB and M variables. */
ii = 12;
jj = 54;
for( i=0; i<6; i += 2 )
	{
	xd = v[ii+i]; /* Earth */
	yd = v[jj+i]; /* Moon */
	v[ii+i] = (EMRAT * xd + yd)/(EMRAT+1.0); /* EMB */
	v[jj+i] = yd - xd; /* M = Moon - Earth */
	v[ii+i+1] = yin[ii+i];
	v[jj+i+1] = yin[jj+i];
	}
#endif
}

#if DOREL
/* Constraint that the center of the relativistic masses = zero.
 */
static double ysun[6];

fixsun( y, v )
double y[], v[];
{
double xx, yy, zz, vx, vy, vz, ax, ay, az;
double csqi, s;
double mustar[NTOTAL];
int i, j, k, ii, jj;

for( ii=0; ii<6; ii++ )
	ysun[ii] = y[ii+(6*ISUN)];

csqi = 0.5 / (C*C);
for( k=0; k<2; k++ )
{ /* Iterate to find solution of implicit expressions. */

/* Table of distances between bodies i and j.
 * Note, most of this work may already have been done by func().
 */
ii = 6;
for( i=1; i<NTOTAL; i++ )
	{
	jj = 0;
	vx = y[ii+1]; /* position */
	vy = y[ii+3];
	vz = y[ii+5];
	for( j=0; j<i; j++ )
		{
		xx = vx - y[jj+1];
		yy = vy - y[jj+3];
		zz = vz - y[jj+5];
		xx = sqrt( xx*xx + yy*yy + zz*zz );
		Rij[i][j] = xx;
		Rij[j][i] = xx;
		jj += 6;
		}
	ii += 6;
	}
/* Relativistic GM of each body */
ii = 0;
for( i=0; i<NTOTAL; i++ )
	{
	vx = y[ii]; /* velocity */
	vy = y[ii+2];
	vz = y[ii+4];
	s = vx * vx + vy * vy + vz * vz; /* square of velocity */
	for( j=0; j<NTOTAL; j++ )
		{
		if( j == i )
			continue;
		s -= GMs[j]/Rij[i][j];
		}
	mustar[i] = GMs[i] * (1.0 + csqi * s);
	ii += 6;
	}
/* Compute center of mass with Sun omitted. */
xx = 0.0;
yy = 0.0;
zz = 0.0;
vx = 0.0;
vy = 0.0;
vz = 0.0;
ii = 0;
for( i=0; i<ISUN; i++ )
	{
	s = mustar[i];
	xx += y[ii+1] * s; /* position */
	yy += y[ii+3] * s;
	zz += y[ii+5] * s;
	vx += y[ii] * s; /* velocity */
	vy += y[ii+2] * s;
	vz += y[ii+4] * s;
	ii += 6;
	}
/* Evaluate the Sun so that the center = 0. */
s = mustar[ISUN];
jj = 6*ISUN;
y[jj+1] = -xx/s;
y[jj+3] = -yy/s;
y[jj+5] = -zz/s;
y[jj] = -vx/s;
y[jj+2] = -vy/s;
y[jj+4] = -vz/s;
v[jj+1] = y[jj];
v[jj+3] = y[jj+2];
v[jj+5] = y[jj+4];
}

/* debug
for( ii=0; ii<6; ii++ )
	{
	xx = ysun[ii] - y[ii+6*ISUN];
	printf( "%.1e\n", xx );
	}
*/
}
#endif

