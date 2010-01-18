/* remesf.c */

#include "remes.h"

/* For special, nonstandard approximation forms, define one of
   the SPEC macros nonzero or write your own version.
   For standard forms, fill in a string for funnam and bits for config.  */
#define SPEC1 1

/* Insert function name and formulas for printout */
char funnam[] = {
#if SPEC1
"exp(x) = 1 + x + .5x^2 + "
#endif
#if SPEC2
"exp2(x) = 1 + "
#endif
#if SPEC3
"log gamma(x) = ( x - 0.5 ) * log(x) - x + LS2PI + 1/x P(1/x^2)"
#endif
#if SPEC4
"gamma(x) = sqrt(2 pi) x^(x-.5) exp(-x) (1 + 1/x P(1/x))"
#endif
#if SPEC5
"atan(y0/j0) + pi/4"
#endif
#if SPEC6
"j0^2(1/x) + y0^2(1/x)"
#endif
#if SPEC7
"j0(x) = (x^2-r0^2)(x^2-r1^2)R(x)"
#endif
#if SPEC8
"j1^2(1/x) + y1^2(1/x)"
#endif
#if SPEC9
"j1(x) = (x^2-r0^2)(x^2-r1^2)(x^2-r3^2)R(x)"
#endif
#if SPEC10
"atan(y1/j1) + 3pi/4"
#endif
#if SPEC11
"y0(x) = 2/pi * log(x) * j0(x) + (x^2-YZ1)*P(x^2)"
#endif
#if SPEC12
"y0(x) = (x-YZ1)(x-YZ2)(x-YZ3)(x-YZ4)R(x)"
#endif
#if SPEC13
"y1(x) = 2/pi * (log(x) * j1(x) - 1/x) + R(x^2)"
#endif
#if SPEC14
"y1(x) = (x-YZ1)(x-YZ2)(x-YZ3)(x-YZ4)R(x)"
#endif
#if SPEC15
"erf(x)"
#endif
#if SPEC16
"erfc(x) = exp(-x^2) R(1/x)"
#endif
/* "1/sqrt(x) = "*/
};

char znam[] = { "x" };


/* The flag bits for type of approximation:
   PXSQ | XPX | X2PX | SQL | SQH | PADE | CW | ZER | SPECIAL | PXCU
   See remes.h for definitions.  */

int config =
#if SPEC1
  ZER | X2PX | SQH | SPECIAL;
#endif
#if SPEC2
  ZER | XPX | SQH | SPECIAL;
#endif
#if SPEC3
  SQH | ZER | XPX | PXSQ | SPECIAL;
#endif
#if SPEC4
  SQH | ZER | XPX | SPECIAL;
#endif
#if SPEC5
  SQH | ZER | XPX | PXSQ;
#endif
#if SPEC6
  SQH | ZER | XPX | PXSQ;
#endif
#if SPEC7
  SQH | ZER | PXSQ | SPECIAL;
#endif
#if SPEC8
  SQH | ZER | XPX | PXSQ;
#endif
#if SPEC9
  SQH | ZER | XPX | SPECIAL;
#endif
#if SPEC10
  SQH | ZER | XPX | PXSQ;
#endif
#if SPEC11
  SQH | ZER | PXSQ | SPECIAL;
#endif
#if SPEC12
  ZER | SPECIAL;
#endif
#if SPEC13
  SQH | ZER | XPX | PXSQ | SPECIAL;
#endif
#if SPEC14
  ZER | SPECIAL;
#endif
#if SPEC15
  SQH | ZER | XPX | PXSQ;
#endif
#if SPEC16
  ZER | XPX | PXSQ | SPECIAL;
#endif


#if SPEC3
#define LS2PI 0.91893853320467274178
#endif
#if SPEC4
#define SQTPI  2.50662827463100050242
#endif
#if SPEC7
#define JZ1   5.783185962946784521176
#define JZ2  30.471262343662086399078
#define JZ3  74.887006790695183444889
#endif
#if SPEC9
#define JZ11 1.46819706421238932572e1
#define JZ12 4.92184563216946036703e1
#define JZ13 1.03499453895136580332e2
#endif
#if SPEC11 || SPEC12 || SPEC13 || SPEC14
/*  2/pi  */
#define TWOOPI 6.36619772367581343075535E-1
#define YZ1    7.98479794664680489965E-1
#define YZ2    1.56632184707105519425E1
#define YZ3    5.02121196292038770878E1
#define Y1Z1   2.19714132603101703515E0
#define Y1Z2   5.42968104079413513277E0
#define Y1Z3   8.59600586833116892643E0
#define Y1Z4   1.17491548308398812434E1
#endif

/* This subroutine computes the rational form P(x)/Q(x) */
/* using coefficients from the solution vector param[]. */

double approx(x)
double x;
{
double gx, z, yn, yd;
double gofx(), speci();
int i;

gx = gofx(x);
if( config & PXCU )
	z = gx * gx * gx;
else if( config & PXSQ )
	z = gx * gx;
else
	z = gx;

/* Highest order numerator coefficient */
yn = param[n];

/* Work backwards toward the constant term. */
for( i=n-1; i>=0; i-- )
	yn = z * yn  +  param[i];
	
if( d > 0 )
	{
/* Highest degree coefficient = 1.0 */
	yd = z + param[n+d];

	for( i=n+d-1; i>n; i-- )
		yd = z * yd  +  param[i];
	}
else
/* There is no denominator. */
	yd = 1.0;

if( config & XPX )
	yn = yn * gx;
if( config & X2PX )
	yn = yn * gx * gx;
if( config & PADE )
	{ /* 2P/(Q-P) */
	yd = yd - yn;
	yn = 2.0 * yn;
	}
qyaprx = yn/yd;
if( config & CW )
	qyaprx = gx + qyaprx * gx * gx;
if( config & SPECIAL )
	qyaprx = speci( qyaprx, gx );
return( qyaprx );
}



/* Subroutine to compute approximation error at x */

double geterr(x)
double x;
{
double e, f;
double fabs(), approx(), func();

f = func(x);
e = approx(x) - f;
if( relerr )
	{
	if( f != 0.0 )
		e /= f;
	}
if( e < 0.0 )
	{
	esign = -1;
	e = -e;
	}
else
	esign = 1;

return(e);
}



/* Subroutine for special argument transformations */

double gofx(x)
double x;
{

return x;
}

/* Routine for special modifications of the approximating form.
 * Example already provided by the CW flag: 
 *	y(1+dev) = gx + gx^2 R(gx)
 * would change y to
 *	R(gx) = (y - gx)/(gx*gx)
 * This function is called from remese.c.
 *
 * An inverse routine called speci() must also be supplied.
 * This finds y from R and gx (see below).
 */
extern double PI, PIO4, THPIO4;
#define SQTPI 2.50662827463100050242
#define JO1 14.6819706421238932572
#define YO1  4.66539330185668857532



double special( y, gx )
double y, gx;
{
double a = 0.0;

#if SPEC1
/* exponential, y = exp(x) = 1 + x + .5x^2 + x^2 R(x) */
double b;
if( gx == 0.0 )
	return(1.0);
b = gx * gx;
a = (y - 1.0 - gx - .5*b)/b;
#endif

#if SPEC2
/* y = exp2(x) = 1 + x R(x) */
a = y - 1.0;
#endif

#if SPEC3
/* y = log gamma(x) = q(x) + 1/x P(1/x^2)
 * configufation is SQH | ZER | XPX | PXSQ | SPECIAL
 */
double b;
double log();
b = 1.0/gx;
b = ( b - 0.5 ) * log(b) - b + LS2PI;
a = y - b;
#endif

#if SPEC4
/* y = gamma(x) = sqrt(2 pi) x^(x-.5) exp(-x) (1 + 1/x P(1/x)) */
double b;
double pow(), exp();
b = 1.0/gx;
a = SQTPI * pow( b, b-0.5 ) * exp(-b);
a = (y - a)/a;
#endif
#if SPEC5
/* Nothing special. */
#endif
#if SPEC6
/* Nothing special. */
#endif
#if SPEC7
/* y = j0(x) = (x^2 - JZ1)(x^2-JZ2)(x^2-JZ3)P(x^2) */
double b;
b=gx*gx;
a = (b-JZ1)*(b-JZ2)*(b-JZ3);
a = y/a;
#endif
#if SPEC8
/* Nothing special. */
#endif
#if SPEC9
/* y = j1(x) = (x^2 - JZ11)(x^2-JZ12)(x^2-JZ13)P(x^2) */
double b;
b=gx*gx;
a = (b-JZ11)*(b-JZ12)*(b-JZ13);
a = y/a;
#endif
#if SPEC10
/* Nothing special. */
#endif
#if SPEC11
/* y = y0(x) = TWOOPI * log(x) * j0(x) + (x^2-YZ1)*P(x^2) */
/*double b;*/
double log(), j0();
/*b=gx*gx;*/
a = y - TWOOPI * log(gx) * j0(gx);
/*a /= (b-YZ1)*(b-YZ2)*(b-YZ3);*/
#endif
#if SPEC12
double b;
b = gx;
a = y / ((b-YZ1)*(b-YZ2)*(b-YZ3)*(b-YZ4));
#endif
#if SPEC13
/* y = y1(x) = TWOOPI * (log(x) * j1(x) - 1/x) + R(x^2) */
double log(), j1();
a = y - TWOOPI * ( j1(gx) * log(gx)  -  1.0/gx );
#endif
#if SPEC14
double b;
b = gx;
a = y / ((b-Y1Z1)*(b-Y1Z2)*(b-Y1Z3)*(b-Y1Z4));
#endif
#if SPEC15
/* Nothing special. */
#endif
#if SPEC16
/* y = erfc(x) = exp(-x^2) P(x) */
double exp();
double b;
b = 1.0/(gx*gx);
a = y * exp(b);
#endif

/* y = cos(x) = 1 - .5 x^2 + x^2 x^2 P(x^2) */
/*
b = gx * gx;
a = (y - 1.0 + 0.5*b)/b;
*/

/* logarithm, y = log(1+x) = x - .5x^2 + x^2 * (xP(x))
 * configuration is  ZER | XPX | SPECIAL
 */
/*
if( gx == 0.0 )
	return(0.0);
b = gx * gx;
a = (y - gx + 0.5 * b)/b;
*/



/* acosh() */
/*
if( gx == 0.0 )
	return(0.0);
a = y/(2.0*sqrt(gx));
*/
return( a );
}



double speci( R, gx )
double R, gx;
{
double y =0.0;

#if SPEC1
/* exponential, y = exp(x) = 1 + x + .5x^2 + x^2 R(x) */
double b;
b = gx * gx;
y = 1.0 + gx + .5 * b;
y = y + b * R;
#endif

#if SPEC2
/* y = exp2(x) = 1 + x R(x) */
y = R + 1.0;
#endif

#if SPEC3
/* y = log gamma(x) = q(x) + 1/x P(1/x^2) */
double b;
double log();
b = 1.0/gx;
b = ( b - 0.5 ) * log(b) - b + LS2PI;
y = b + R;
#endif

#if SPEC4
/* y = gamma(x) = sqrt(2 pi) x^(x-.5) exp(-x) (1 + 1/x P(1/x)) */
double b;
double pow(), exp();
b = 1.0/gx;
b = SQTPI * pow( b, b-0.5 ) * exp(-b);
y = b + b * R;
#endif
#if SPEC5
/* Nothing special. */
#endif
#if SPEC6
/* Nothing special. */
#endif
#if SPEC7
/* y = j0(x) = (x^2 - JZ1)(x^2-JZ2)(x^2-JZ3)P(x^2) */
double b;
b=gx*gx;
y = (b-JZ1)*(b-JZ2)*(b-JZ3)*R;
#endif
#if SPEC8
/* Nothing special. */
#endif
#if SPEC9
/* y = j1(x) = (x^2 - JZ11)(x^2-JZ12)(x^2-JZ13)P(x^2) */
double b;
b=gx*gx;
y = (b-JZ11)*(b-JZ12)*(b-JZ13)*R;
#endif
#if SPEC10
/* Nothing special. */
#endif
#if SPEC11
/* y = y0(x) = TWOOPI * log(x) * j0(x) + (x^2-YZ1)*P(x^2) */
/*double b;*/
double log(), j0();
/*b=gx*gx;*/
y = TWOOPI * log(gx) * j0(gx) + R; /*(b-YZ1)*(b-YZ2)*(b-YZ3)*R;*/
#endif
#if SPEC12
double b;
b = gx * gx;
y = (b-YZ1)*(b-YZ2)*(b-YZ3)*(b-YZ4)*R;
#endif
#if SPEC13
/* y = y1(x) = TWOOPI * (log(x) * j1(x) - 1/x) + R(x^2) */
double log(), j1();
y =  TWOOPI * ( j1(gx) * log(gx)  -  1.0/gx ) + R;
#endif
#if SPEC14
double b;
b = gx;
y = (b-Y1Z1)*(b-Y1Z2)*(b-Y1Z3)*(b-Y1Z4)*R;
#endif
#if SPEC15
/* Nothing special. */
#endif
#if SPEC16
/* y = erfc(x) = exp(-x^2) P(x) */
double exp();
double b;
b = -1.0/(gx*gx);
y = exp(b) * R;
#endif

/* y = cos(x) = 1 - .5 x^2 +  x^2 x^2 R(x^2) */
/*
b = gx * gx;
y = 1.0 - 0.5*b + b * R;
*/

/* log(1+x) = x - .5x^2 + x^2 xR(x) */
/*
b = gx * gx;
y = gx  - 0.5 * b  +  b * R;
*/




/* y = erfc(x) = exp(-x^2) P(x) */
/*y = exp( -gx * gx ) * R;*/

/*y = 2.0 * sqrt(gx) * R;*/
return( y );
}

/* Put here an accurate routine */
/* for the function to be approximated. */
#if 0
static int fflg = 0;
static double ff = 0.0;
#endif

double func(x)
double x;
{
double y;
/*double xx, y, t, u, s, c;*/

#if SPEC1
double exp();
/* exponential, y = exp(x) = 1 + x + .5x^2 + x^2 R(x) */
y = exp(x);
#endif

#if SPEC2
double exp2();
y = exp2(x);
#endif

#if SPEC3
double lgam();
y = lgam(1.0/x);
#endif

#if SPEC4
double gamma();
y = gamma(1.0/x);
#endif

#if SPEC5
/* Bessel, phase */
double xx, t;
double j0(), y0(), floor(), atan();
if( x == 0.0 )
  {
    qx = 0.0;
    qy = 0.0;
    return(0.0);
  }
xx = 1.0/x;
y = j0(xx);
t = y0(xx);
y = atan(t/y);
t = xx - PIO4;
t = t - PI * floor(t/PI + 0.5);
y -= t;
if( y > 0.5*PI )
	y -= PI;
if( y < -0.5*PI )
	y += PI;
#endif

#if SPEC6
/* Bessel, modulus */
double t, u, xx;
double j0(), y0(), sqrt();
xx =1.0/(x);
t = j0(xx);
u = y0(xx);
y = t*t + u*u;
#endif
#if SPEC7
double j0();
y  = j0(x);
#endif
#if SPEC8
/* Bes1, modulus */
double t, u, xx;
double j1(), y1(), sqrt();
xx =1.0/(x*x);
t = j1(xx);
u = y1(xx);
y = sqrt(t*t + u*u);
#endif
#if SPEC9
double j1();
y  = j1(x);
#endif
#if SPEC10
/* bes1, phase */
double xx, t;
double j0(), y0(), floor(), atan();
if( x == 0.0 )
  {
    qx = 0.0;
    qy = 0.0;
    return(0.0);
  }
xx = 1.0/x;
y = j1(xx);
t = y1(xx);
y = atan(t/y);
t = xx - THPIO4;
t = t - PI * floor(t/PI + 0.5);
y -= t;
if( y > 0.5*PI )
	y -= PI;
if( y < -0.5*PI )
	y += PI;
#endif
#if SPEC11 || SPEC12
double y0();
y = y0(x);
#endif
#if SPEC13 || SPEC14
double y1();
y = y1(x);
#endif
#if SPEC15
double erf();
y = erf(x);
#endif
#if SPEC16
double erfc();
y = erfc(1.0/x);
#endif

qx = x;

#if 0
if( fflg == 0 )
	{
	fflg = 1;
	ff = 10.0 * log10(2.0);
	}

if( x == 0.0 )
	{
	y = ff;
	y = 0.0;
	qy = y;
	return y;
	}
#endif


/*
xx = 32.0 * x;
t = 1.0 + exp10( -xx/10.0 );
y = 10.0 * log10(t);
*/

#if 0
/* R = 1 - u^2 P(u^2), u = 1/(pi x^2) */
xx = 1.0/sqrt(PI*x);
fresnl( xx, &s, &c );
/* pi/2 x^2 = pi/2 1/(pi u) */
t = 0.5*PI*xx*xx;
/*y = (0.5-s) * cos(t) - (0.5-c) * sin(t);*/
y = (0.5-c) * cos(t) + (0.5-s) * sin(t);
y = y*PI*xx;
#endif

/*
y = exp( lx( exp(x) ) - 1.0 ) - 1.0;
*/
/*
y = 1.0/sqrt(x);
*/

qy = y;
return( y );
}
