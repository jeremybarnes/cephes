/* Adams-Bashforth-Moulton integration formulas.
 *
 * Reference:
 * Shampine, L. F. and M. K. Gordon, _Computer Solution of
 * Ordinary Differential Equations_, W. H. Freeman, 1975.
 *
 * Program by Steve Moshier.
 */

#include "int.h"
/* Divided differences */
#define N 19

/*Predictor coefficients*/
double precof[N] = {
1.0,
1.0 / 2.0,
5.0 / 12.0,
3.0 / 8.0,
251.0 / 720.0,
95.0 / 288.0,
19087.0 / 60480.0,
5257.0 / 17280.0,
1070017.0 / 3628800.0,
25713.0 / 89600.0,
26842253.0 / 95800320.0,
4777223.0 / 17418240.0,
703604254357.0 / 2615348736000.0,
106364763817.0 / 402361344000.0,
1166309819657.0 / 4483454976000.0,
2.5221445e7 / 9.8402304e7,
8.092989203533249e15 / 3.201186852864e16,
8.5455477715379e13 / 3.4237292544e14,
1.2600467236042756559e19 / 5.109094217170944e19,
};

/*Corrector coefficients*/
double corcof[N] = {
 1.0,
 -1.0 / 2.0,
 -1.0 / 12.0,
 -1.0 / 24.0,
 -19.0 / 720.0,
 -3.0 / 160.0,
 -863.0 / 60480.0,
 -275.0 / 24192.0,
 -33953.0 / 3628800.0,
 -8183.0 / 1036800.0,
 -3250433.0 / 479001600.0,
 -4671.0 / 788480.0,
 -13695779093.0 / 2615348736000.0,
 -2224234463.0 / 475517952000.0,
 -132282840127.0 / 31384184832000.0,
 -2639651053.0 / 689762304000.0,
 1.11956703448001e14 / 3.201186852864e16,
 5.0188465e7 / 1.5613165568e10,
 2.334028946344463e15 / 7.86014494949376e17,
};

/* Compute zeroth through kth backward differences
 * of the data in the input array
 */
divdif(vec , k, diffn)
double vec[]; /* input array of k+1 data items */
double *diffn; /* output array of ith differences */
int k;
{
double diftbl[N];
double *p, *q;
double y;
int i, o;

/* Copy the given data (zeroth difference) into temp array
 */
p = diftbl;
q = vec;
for( i=0; i<=k; i++ )
	*p++ = *q++;

/* On the first outer loop, k-1 first differences are calculated.
 * These overwrite the original data in the temp array.
 */
o = k;
for( o=k; o>0; o-- )
	{
	p = diftbl;
	q = p;
	for( i=0; i<o; i++ )
		{
		y = *p++;
		*q++ = *p - y;
		}
	*diffn++ = *p; /* copy out the last (undifferenced) item */
#if DEBUG
	printf( "%.5e ", *p );
#endif
	}
#if DEBUG
	printf( "%.5e\n", *(q-1) );
#endif
*diffn++ = *(q-1);
}


/* Update array of differences, given new data value.
 * diffn is an array of k+1 differences, starting with the
 * zeroth difference (the previous original data value).
 */
dupdate( diffn, k, f )
register double *diffn;  /* input and output array of differences */
int k; /* max order of differences */
double f; /* new data point (zeroth difference) */
{
double new, old;
int i;

new = f;
for( i=0; i<k; i++ )
	{
	old = *diffn;
	*diffn++ = new;
#if DEBUG
	printf( "%.5e ", new );
#endif
	new = new - old;
	}
#if DEBUG
	printf( "%.5e\n", new );
#endif
*diffn++ = new;
}




/* Evaluate the interpolating polynomial
 *
 *              (x - x )
 *                    n    1
 * P(x) = f  +  --------  D f   +  ...
 *         n       h         n
 *
 *     (x - x )(x - x   )...(x - x     )
 *           n       n-1          n+2-k    k-1
 *  +  ---------------------------------  D    f
 *                   k-1                        n
 *                  h     (k-1)!
 *
 *
 *         j
 *  where D denotes the jth backward difference, see dupdate(), and
 *
 *  f   =   f( x , y(x ) )  is the interpolated derivative y'(x ) .
 *   n          n     n                                        n
 *
 * The subroutine argument t is linearly scaled so that t = 1.0
 * will evaluate the polynomial at x = x_n + h,
 * t = 0.0 corresponds to x = x_n, etc.
 */
double difpol( diffn, k, t )
double *diffn;
int k; /* differences go up to order k-1 */
double t; /* scaled argument */
{
double f, fac, s, u;
int i;

f = *diffn++; /* the zeroth difference = nth data point */
u = 1.0;
/*s = x/h - n;*/
s = 1.0;	/* to evaluate the polynomial at x = xn + h */
fac = 1.0;
for( i=1; i<k; i++ )
	{
	if( s == 0.0 )
		break;
	u *= s / fac;
	f += u  *  (*diffn++);
	fac += 1.0;
	s += 1.0;
	}
return( f );
}


/* Integrate the interpolating polynomial from x[n] to x[n+1]
 * to obtain the change in the integrated function from y[n] to y[n+1]
 * given by:
 *
 *                     k-1
 *                      -       i
 *  y    =  y   +   h   >   c  D  f    .
 *   n+1     n          -    i     n
 *                     i=0
 *
 *  This subroutine returns the summation term, not multiplied by h.
 *  The coefficients c_i of the integration formula are either
 *  precof[] or corcof[], given above.
 */
double intpol( diffn, coeffs, k )
double *diffn; /* array of backward differences */
double *coeffs; /* coefficients of integration formula */
int k; /* differences used go up to order k-1 */
{
double s;
int i;

s = 0.0;
coeffs += k;
diffn += k;
for( i=0; i<k; i++ )
	{
	s += (*--coeffs) * (*--diffn);
	}
return( s );
}



/* Copy array of n elements from p to q.
 */
vcopy( p, q, n )
register double *p, *q;
register int n;
{
do
	*q++ = *p++;
while( --n );
}


/* Adams initialization program.
 */

/* Addresses within the work array */
static double *dv;
static double *dvp;
static double *vp;
static double *yp;
static double *vn;
static double *delta;
static double *sdelta;
static double *y0;

static double ccor;
static double hstep; /* step size (constant) */
static int order; /* Order of the prediction formula */
static int ordp1;
static int asiz;
static int dsiz;
static int jstep; /* counts steps taken */

/* Initialize pointers in work array, compute derivatives at
 * initial position, and start the difference tables.
 * neq >= nequat in adstep() below.  If neq > nequat, unused space
 * is left for extra variables that are not actually integrated.
 */
adstart( h, yn, work, neq, ord, t )
double h;
double yn[], work[];
int neq, ord;
double t; 
{
double *p;
int j;

hstep = h;
ccor = hstep * precof[ord];
jstep = 0;
dsiz = ord + 2;
asiz = neq * dsiz;
order = ord;
ordp1 = ord + 1;
p = work;
dv = p;
p += asiz;
dvp = p;
p += dsiz;
vp = p;
p += neq;
yp = p;
p += neq;
vn = p;
p += neq;
delta = p;
p += neq;
sdelta = p;
p += neq;
y0 = p;


func( t, yn, vn );

p = dv;
for( j=0; j<neq; j++ )
	{
	dupdate( p, 0, vn[j] );
	p += dsiz;
	}
}





/* Adams-Bashforth-Moulton step.
 */

double adstep( t, yn, nequat )
double *t;
double yn[];
int nequat;
{
double e, e0, time;
double *pdv;
int i, j;
double intpol();

time = *t;
jstep += 1;

/* Do Runge-Kutta for the first ord + 1 steps.
 */
if( jstep <= ordp1 )
	{
	rungek(  nequat, time, yn, hstep, yn, delta );
	func( time+hstep, yn, vn );
	pdv = dv;
	for( j=0; j<nequat; j++ )
		{
		dupdate( pdv, jstep, vn[j] );
		pdv += dsiz;
		}
	e = 0.0;
	goto done;
	}



/* Predict the next position
 * based on current y' difference table dv[].
 */
	pdv = dv;
	for( i=0; i<nequat; i++ )
		{
		yp[i] = yn[i] + hstep * intpol( pdv, precof, order );
		pdv += dsiz;
		}
/* Evaluate derivatives at the predicted position
 */
	func( time+hstep, yp, vp );

/* Correct the predicted position and velocity using the derivatives
 * evalutated at yp.
 */
	pdv = dv;
	for( i=0; i<nequat; i++ )
		{
		vcopy( pdv, dvp, dsiz ); /* y' difference table */
		dupdate( dvp, ordp1, vp[i] );
/* Note, the following line is equivalent to:
 *		yn[i] = yn[i] + hstep * intpol( dap, corcof, ordp1 );
 * where e is the corrected value of the next vn.
 */
		yn[i] = yp[i] + ccor * dvp[order];
		pdv += dsiz;
		}

/* Evaluate derivative at the final position
 */
	func( time+hstep, yn, vn );

	e = 0.0;
	pdv = dv;
	for( i=0; i<nequat; i++ )
		{
		dupdate( pdv, ordp1, vn[i] );
/* Note, the following line is equivalent to:
 *		yn[i] = yn[i] + hstep * intpol( dap, corcof, ordp1 );
 * where e is the corrected value of the next vn.
 */
		yn[i] = yp[i] + ccor * pdv[order];
/* Estimate the error
 */
		e0 = hstep * pdv[order] * corcof[order];
		if( e0 < 0.0 )
			e0 = -e0;
		if( e0 > e )
			e = e0;
		pdv += dsiz;
		}
done:
time += hstep;
*t = time;
return( e );
}
