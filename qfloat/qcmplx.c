/*				qcmplx.c
 * Q type complex number arithmetic
 *
 * The syntax of arguments in
 *
 * cfunc( a, b, c )
 *
 * is
 * c = b + a
 * c = b - a
 * c = b * a
 * c = b / a.
 */

#include "qhead.h"
#include "mconf.h"

#ifndef ANSIC
#define ANSIC 0
#endif

#ifdef ANSIC
extern double sqrt (double);
extern double fabs (double);
extern double floor (double);
#else
double sqrt(), fabs(), floor();
#endif

extern double MAXNUM;

qcmplx qczero = {
{0,},
{0,},
};
extern qcmplx qczero;
#if WORDSIZE == 32
qcmplx qcone = {
{0,EXPONE,0,0x80000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,},
{0,0x0000,0,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,},
};
#else
qcmplx qcone = {
{0,EXPONE,0,0x8000,0},
{0,},
};
#endif
extern qcmplx qcone;

extern QELT qpi[], qone[];
#ifdef ANSIPROT
void csqrt(cmplx *, cmplx *);
#else
int qatn2(), qcosh(), qsinh(), mtherr();
int qcsqrt(), csqrt();
#endif

/*	c = b + a	*/

int qcadd( a, b, c )
register qcmplx *a, *b;
qcmplx *c;
{

qadd( &a->r[0], &b->r[0], &c->r[0] );
qadd( &a->i[0], &b->i[0], &c->i[0] );
return 0;
}


/*	c = b - a	*/

int qcsub( a, b, c )
register qcmplx *a, *b;
qcmplx *c;
{

qsub( &a->r[0], &b->r[0], &c->r[0] );
qsub( &a->i[0], &b->i[0], &c->i[0] );
return 0;
}

/*	c = b * a
 */
int qcmul( aa, bb, cc )
qcmplx *aa, *bb, *cc;
{
qcmplx a, b;
QELT y[NQ], t[NQ], p[NQ];
long e1, e2, e3, ear, eai, ebr, ebi;

qcmov(aa, &a);
qcmov(bb, &b);
if( (eai = a.i[1]) == 0 )
	{/* pure real */
	qmul( a.r, b.r, cc->r );
	qmul( a.r, b.i, cc->i );
	return 0;
	}
if( (ebi = b.i[1]) == 0 )
	{/* pure real */
	qmul( b.r, a.r, cc->r );
	qmul( b.r, a.i, cc->i );
	return 0;
	}
if( (ear = a.r[1]) == 0 )
	{/* pure imaginary */
	qmul( a.i, b.r, y );
	qmul( a.i, b.i, t );
	goto imret;
	}
if( (ebr = b.r[1]) == 0 )
	{/* pure imaginary */
	qmul( b.i, a.r, y );
	qmul( b.i, a.i, t );
imret:
	if( t[1] != 0 )
		qneg( t );
	qmov( t, cc->r );
	qmov( y, cc->i );
	return 0;
	}
/* Overflow proofing: extract all the exponents
 * and operate with values near 1.
 */
a.r[1] = EXPONE;
a.i[1] = EXPONE;
b.r[1] = EXPONE;
b.i[1] = EXPONE;
qmul( b.r, a.r, y );
qmul( b.i, a.i, t );
e2 = ebr + ear;
e3 = ebi + eai;
e1 = e2 - e3;
/* Equalize exponents in preparation for subtract. */
if( e1 >= 0 )
	{
	if( (long) t[1] <= e1 )
		goto mreal;
	t[1] -= e1;
	}
else
	{
	e2 = e3;
	e1 = -e1;
	if( (long) y[1] > e1 )
		y[1] -= e1;
	else
		qclear(y);
	}
qsub( t, y, y );
mreal:
if( y[1] != 0 )
	{
	e2 = (e2 - (long)EXPONE) + ((long )y[1] - (long)EXPONE);
	if( e2 > (long)MAXEXP )
		{
		mtherr( "qcmul", OVERFLOW );
		qinfin( y );
		}
	else
		{
		if( e2 <= 0 )
			qclear(y);
		else
			y[1] = e2;
		}
	}
qmul( b.r, a.i, t );
qmul( b.i, a.r, p );
e2 = ebr + eai;
e3 = ebi + ear;
e1 = e2 - e3;
if( e1 >= 0 )
	{
	if( (long) p[1] <= e1 )
		goto mimag;
	p[1] -= e1;
	}
else
	{
	e2 = e3;
	e1 = -e1;
	if( (long) t[1] > e1 )
		t[1] -= e1;
	else
		qclear(t);
	}
qadd( p, t, p );
mimag:
if( p[1] != 0 )
	{
	e2 = (e2 - (long)EXPONE) + ((long )p[1] - (long)EXPONE);
	if( e2 > (long)MAXEXP )
		{
		mtherr( "qcmul", OVERFLOW );
		qinfin( p );
		}
	else
		{
		if( e2 <= 0 )
			qclear(p);
		else
			p[1] = e2;
		}
	}
a.r[1] = ear;
a.i[1] = eai;
b.r[1] = ebr;
b.i[1] = ebi;
qmov( y, cc->r );
qmov( p, cc->i );
return 0;
}

/*						cmplx.c	*/


/*	c = b / a */

int qcdiv( a, b, c )
register qcmplx *a, *b;
qcmplx *c;
{
QELT y[NQ], p[NQ], t[NQ], s[NQ];
long e1, e2, e3, en, ed, ear, eai, ebr, ebi;

qmov( &a->r[0], s );
qmov( &a->i[0], t );
if( (eai = t[1]) == 0 )
	{ /* pure real */
	if( s[1] == 0 )
		goto overf; /* divide by zero */
	qdiv( s, &b->r[0], &c->r[0] );
	qdiv( s, &b->i[0], &c->i[0] );
	return 0;
	}
if( (ear = s[1]) == 0 )
	{ /* pure imaginary */
	qmov( &b->i[0], s );
	qdiv( t, &b->r[0], &c->i[0] );
	qdiv( t, s, &c->r[0] );
	qneg( &c->i[0] );
	return 0;
	}
/* Anti-overflow technique.
 * Extract all the exponents and operate with numbers near 1.
 */
a->r[1] = EXPONE;
a->i[1] = EXPONE;
if( (ebr = b->r[1]) != 0 )
	b->r[1] = EXPONE;
if( (ebi = b->i[1]) != 0 )
	b->i[1] = EXPONE;

 /* y = a->r * a->r  +  a->i * a->i, with exponent ed */
qmul( &a->r[0], &a->r[0], y );
qmul( &a->i[0], &a->i[0], t );
e2 = ear + ear;
e3 = eai + eai;
e1 = e2 - e3;
if( e1 >= 0 )
	{
	ed = e2;
	if( (long) t[1] <= e1 )
		goto mdenom;
	t[1] -= e1;
	}
else
	{
	ed = e3;
	e1 = -e1;
	if( (long) y[1] > e1 )
		y[1] -= e1;
	else
		qclear(y);
	}
qadd( t, y, y );
mdenom:
/* p = (b->r * a->r  +  b->i * a->i)/y */
qmul( &b->r[0], &a->r[0], p );
qmul( &b->i[0], &a->i[0], t );
e2 = ebr + ear;
e3 = ebi + eai;
e1 = e2 - e3;
if( e1 >= 0 )
	{
	en = e2;
	if( (long) t[1] <= e1 )
		goto mreal;
	t[1] -= e1;
	}
else
	{
	en = e3;
	e1 = -e1;
	if( (long) p[1] > e1 )
		p[1] -= e1;
	else
		qclear(p);
	}
qadd( t, p, p );
mreal:
if( p[1] != 0 )
	{
	qdiv( y, p, p );
	e1 = en - ed + (long )p[1];
	if( e1 > (long)MAXEXP )
		{
		mtherr( "qcdiv", OVERFLOW );
		qinfin( p );
		}
	else
		{
		if( e1 <= 0 )
			qclear(p);
		else
			p[1] = e1;
		}
	}
/* c->i = (b->i * a->r  -  b->r * a->i)/y */	
qmul( &b->i[0], &a->r[0], s );
qmul( &b->r[0], &a->i[0], t );
e2 = ebi + ear;
e3 = ebr + eai;
e1 = e2 - e3;
if( e1 >= 0 )
	{
	en = e2;
	if( (long )t[1] <= e1 )
		goto mimag;
	t[1] -= e1;
	}
else
	{
	en = e3;
	e1 = -e1;
	if( (long) s[1] > e1 )
		s[1] -= e1;
	else
		qclear(s);
	}
qsub( t, s, s );
mimag:
if( s[1] != 0 )
	{
	qdiv( y, s, s );
	e1 = en - ed + s[1];
	if( e1 > (long)MAXEXP )
		{
		mtherr( "qcdiv", OVERFLOW );
		qinfin( s );
		}
	else
		{
		if( e1 <= 0 )
			qclear(s);
		else
			s[1] = e1;
		}
	}
/* restore input exponents */
a->r[1] = ear;
a->i[1] = eai;
b->r[1] = ebr;
b->i[1] = ebi;
qmov( s, &c->i[0] );
qmov( p, &c->r[0] );
return 0;

overf:
mtherr( "qcdiv", OVERFLOW );
qinfin( &c->r[0] );
qclear( &c->i[0] );
return 0;
}

/*	b = a	*/

int qcmov( a, b )
register qcmplx *a, *b;
{

qmov( &a->r[0], &b->r[0] );
qmov( &a->i[0], &b->i[0] );
return 0;
}

int qcneg( a )
register qcmplx *a;
{

if( a->r[1] != 0 ) /* don't produce minus 0 */
	a->r[0] = ~a->r[0];
if( a->i[1] != 0 )
	a->i[0] = ~a->i[0];
return 0;
}


/* Absolute value of complex a, returns real y
 */
int qcabs( a, y )
register qcmplx *a;
QELT y[];
{
QELT b[NQ], d[NQ];
long ea, eb;

ea = (unsigned int )a->r[1];
eb = (unsigned int )a->i[1];
if( ((ea - eb) > NBITS) || (eb == 0) )
	{
	qmov( &a->r[0], y );
	y[0] = 0;
	return 0;
	}
if( ((eb - ea) > NBITS) || (ea == 0) )
	{
	qmov( &a->i[0], y );
	y[0] = 0;
	return 0;
	}
/* Rescale to make geometric mean of re and im near to 1 */
ea -= EXPONE;
eb -= EXPONE;
ea = (ea + eb)/2;
qmov( &a->r[0], b );
b[1] = b[1] - ea;
qmul( b, b, b );
qmov( &a->i[0], d );
d[1] = d[1] - ea;
qmul( d, d, d );
/* sqrt( re**2 + im**2 ) */
qadd( b, d, d );
qsqrt( d, y );
/* restore scale factor */
y[1] = y[1] + ea; 
return 0;
}

/* complex exponential */

int qcexp( a, c )
register qcmplx *a, *c;
{
QELT r[NQ], t[NQ], u[NQ];

if( a->r[1] != 0 )
	qexp( &a->r[0], r );
else
	qmov( qone, r );
if( a->i[1] != 0 )
	{
	qcos( &a->i[0], t );
	qsin( &a->i[0], u );
	}
else
	{
	qmov( qone, t );
	qclear( u );
	}
qmul( r, t, &c->r[0] );
qmul( r, u, &c->i[0] );
return 0;
}


/* complex logarithm */

int qclog( a, c )
qcmplx *a, *c;
{
QELT x[NQ], y[NQ];

qcabs( a, y );
qlog( y, x );
#if ANSIC
qatn2( &a->i[0], &a->r[0], y );
#else
qatn2( &a->r[0], &a->i[0], y );
if( qcmp(y, qpi) > 0 )
	{
	qsub( qpi, y, y );
	qsub( qpi, y, y );
	}
#endif
qmov( x, &c->r[0] );
qmov( y, &c->i[0] );
return 0;
}

int qcsin( a, c )
qcmplx *a, *c;
{
QELT e[NQ], ch[NQ], sh[NQ];

qexp( &a->i[0], e );
qdiv( e, qone, ch );
qsub( ch, e, sh );
if( sh[1] > 0 )
	sh[1] -= 1;
qadd( ch, e, ch );
if( ch[1] > 0 )
	ch[1] -= 1;

qsin( &a->r[0], e );
qmul( e, ch, ch );

qcos( &a->r[0], e );
qmul( e, sh, &c->i[0] );
qmov( ch, &c->r[0] );
return 0;
}


int qccos( a, c )
qcmplx *a, *c;
{
QELT e[NQ], ch[NQ], sh[NQ];

qexp( &a->i[0], e );
qdiv( e, qone, ch );
qsub( ch, e, sh );
if( sh[1] > 0 )
	sh[1] -= 1;
qadd( ch, e, ch );
if( ch[1] > 0 )
	ch[1] -= 1;

qsin( &a->r[0], e );
qmul( e, sh, sh );
qneg( sh );

qcos( &a->r[0], e );
qmul( e, ch, &c->r[0] );
qmov( sh, &c->i[0] );
return 0;
}

int qcasin( a, w )
qcmplx *a, *w;
{
qcmplx ca, ct, zz, z2;


qmov( &a->r[0], &ca.r[0] );
qmov( &a->i[0], &ca.i[0] );

qmov( &ca.i[0], &ct.r[0] );	/* ct.r = -ca.i,    iz */
qneg( &ct.r[0] );
qmov( &ca.r[0], &ct.i[0] );	/* ct.i = ca.r */
qcmul( &ca, &ca, &zz );	/* sqrt( 1 - z*z) */
qsub( &zz.r[0], qone, &zz.r[0] );	/* zz.r = 1 - zz.r */
qneg( &zz.i[0] );		/* zz.i = -zz.i */
qcsqrt( &zz, &z2 );

qcadd( &z2, &ct, &zz );
qclog( &zz, &zz );
qmov( &zz.i[0], &w->r[0] );	/* w->r = zz.i	 mult by 1/i = -i */
qmov( &zz.r[0], &w->i[0] );	/* w->i = -zz.r */
qneg( &w->i[0] );
return 0;
}


int qcsqrt( z, w )
qcmplx *z, *w;
{
qcmplx q, s, y;
double dr, dt, dx, dy;
long ea, eb, ec;
union
  {
    unsigned short s[4];
    double d;
  } dz;

qcmov( z, &y );
ea = (unsigned int )y.r[1];
eb = (unsigned int )y.i[1];
if( (ea == 0) && (eb == 0) )
	{ /* sqrt(0) */
	qclear( w->r );
	qclear( w->i );
	return 0;
	}
/* Rescale to make max(re, im) near to 1 */
if( eb > ea )
	ec = eb;
else
	ec = ea;
ec -= EXPONE;
ec &= ~1;	/* make scale factor an even power of 2 */
/*
ec = ec/2;
ec = 2*ec;
*/
if( ea > ec )
	y.r[1] -= ec;
else
	qclear( &y.r[0] );
if( eb > ec )
	y.i[1] -= ec;
else
	qclear( &y.i[0] );
qtoe( &y.r[0], dz.s );
dx = dz.d;
qtoe( &y.i[0], dz.s );
dy = dz.d;
/* csqrt( &dz, &dz ); */
  dr = sqrt(dx*dx + dy*dy);
  if( dx > 0 )
    {
      dt = sqrt( 0.5 * (dr + dx) );
      dr = fabs( 0.5 * dy / dt );
    }
  else
    {
      dr = sqrt( 0.5 * (dr - dx) );
      dt = fabs( 0.5 * dy / dr );
    }
  if (dy < 0)
    dr = -dr;

dz.d = dt;
etoq( dz.s, &q.r[0] );
dz.d = dr;
etoq( dz.s, &q.i[0] );
/* Fix signs.  */
q.r[0] = 0;
q.i[0] = z->i[0];
/* Newton iteration */
qcdiv( &q, &y, &s );
qcadd( &q, &s, &q );
if( q.r[1] > 0 )
	q.r[1] -= 1;
if( q.i[1] > 0 )
	q.i[1] -= 1;

qcdiv( &q, &y, &s );
qcadd( &q, &s, &q );
if( q.r[1] > 0 )
	q.r[1] -= 1;
if( q.i[1] > 0 )
	q.i[1] -= 1;

#if NQ > 12
qcdiv( &q, &y, &s );
qcadd( &q, &s, &q );
if( q.r[1] > 0 )
	q.r[1] -= 1;
if( q.i[1] > 0 )
	q.i[1] -= 1;
#endif
/* restore half the scale */
ec >>= 1;
if( q.r[1] != 0 )
	q.r[1] += ec;
if( q.i[1] != 0 )
	q.i[1] += ec;
qcmov( &q, w );
return 0;
}



int qcacos( z, w )
qcmplx *z, *w;
{
qcmplx t;
QELT p[NQ];

qcasin( z, &t );
qmov( qpi, p );
p[1] -= 1;
qsub( &t.r[0], p, &w->r[0] );
qmov( &t.i[0], &w->i[0] );
qneg( &w->i[0] );
return 0;
}




int qctan( z, w )
qcmplx *z, *w;
{
QELT d[NQ], zr[NQ], zi[NQ], t[NQ];

/* d = cos( 2.0 * z->r ) + cosh( 2.0 * z->i ) */
qmov( z->r, zr );
if (zr[1] != 0)
  zr[1] += 1;
qcos( zr, d );
qmov( z->i, zi );
if (zi[1] != 0)
  zi[1] += 1;
qcosh( zi, t );
qadd( t, d, d );

/* w->r = sin( 2.0 * z->r ) / d; */
qsin( zr, t );
qdiv( d, t, w->r );
/* w->i = sinh( 2.0 * z->i ) / d; */
qsinh( zi, t );
qdiv( d, t, w->i );
return 0;
}

int qccot( z, w )
qcmplx *z, *w;
{
QELT d[NQ], zr[NQ], zi[NQ], t[NQ];


/* d = cosh(2.0 * z->i) - cos(2.0 * z->r) */
qmov( &z->i[0], zi );
if (zi[1] != 0)
  zi[1] += 1;
qcosh( zi, d );
qmov( &z->r[0], zr );
if (zr[1] != 0)
  zr[1] += 1;
qcos( zr, t );
qsub( t, d, d );

/* w->r = sin( 2.0 * z->r ) / d */
qsin( zr, t );
qdiv( d, t, &w->r[0] );
/* w->i = -sinh( 2.0 * z->i ) / d */
qsinh( zi, t );
qdiv( d, t, &w->i[0] );
qneg( &w->i[0] );
return 0;
}

/* subtract nearest integer multiple of pi */
extern QELT qhalf[];

int qredpi( x, y )
QELT x[], y[];
{
QELT t[NQ];
long i;
union
  {
    unsigned short s[4];
    double d;
  } di;

qdiv( qpi, x, t );	/* t = x/PI */
t[0] = 0;
qadd( qhalf, t, t );	/* t += 0.5 */

/*qifrac( t, &i, s );*/	/* i = t */
qtoe( t, di.s );
i = di.d;
if( i != 0 )
	{
	ltoq( &i, t );		/* t = i */
	qmul( qpi, t, t );
	t[0] = x[0];
	qsub( t, x, y );
	}
else
	qmov( x, y );
return 0;
}

static QELT a[NQ] = {0};
static QELT t[NQ] = {0};
static QELT x[NQ] = {0};
static QELT x2[NQ] = {0};
static QELT y[NQ] = {0};
static QELT y2[NQ] = {0};

int qcatan( z, w )
qcmplx *z, *w;
{

qmov( z->r, x );	/* x = z->r */
qmov( z->i, y );	/* y = z->i */


if( x[1] == 0 ) /* pure imaginary */
	{
	qclear( x2 );
	qclear( w->r );
	if( qcmp(qone, y) == 0 )
		{
		qinfin( w->i );
		qneg( w->i );
		goto overf;
		}
	qneg(y);
	if( qcmp(qone, y) == 0 )
		{
		qinfin( w->i );
overf:
		mtherr( "qcatan", SING );
		return 0;
		}
	qneg(y);
	goto imag;
	}
qmul( x, x, x2 );	/* x2 = x * x */
/* a = 1.0 - x2 - (y * y) */
qmul( y, y, y2 );
qsub( y2, qone, a );
qsub( x2, a, a );

/* t = atan2( a, 2.0 * x )/2.0 */
qmov( x, y2 );
if (y2[1] != 0)
  y2[1] += 1;
#if ANSIC
qatn2( y2, a, t );
#else
qatn2( a, y2, t );
#endif
if( t[1] > 0 )
	t[1] -= 1;
qredpi( t, w->r );

imag:
if( y[1] == 0 )
	{
	qclear( w->i );
	return 0;
	}
qsub( qone, y, t );	/* t = y - 1.0 */

/* a = x2 + (t * t) */
qmul( t, t, a );
qadd( x2, a, a );

qadd( qone, y, t );	/* t = y + 1.0 */
/* a = (x2 + (t * t))/a */
qmul( t, t, y2 );
qadd( x2, y2, y2 );
qdiv( a, y2, a );

/* w->i = log(a)/4.0 */
qlog(a, t);
if( t[1] > 1 )
	t[1] -= 2;
qmov( t, w->i );
return 0;
}


/* Complex hyperbolic sine.  */

int
qcsinh (z, w)
     qcmplx *z, *w;
{
  QELT x[NQ], y[NQ];

  qmov( z->r, x );
  qmov( z->i, y );
  /* sinh (x) * cos (y); */
  qsinh( x, w->r );
  qcos( y, w->i );
  qmul( w->i, w->r, w->r );
  /* cosh (x) * sin (y); */
  qcosh( x, w->i );
  qsin( y, x );
  qmul( x, w->i, w->i );
  return 0;
}



/* Complex inverse hyperbolic sine.  */

int
qcasinh (z, w)
     qcmplx *z, *w;
{
  qcmplx u;

  qclear( u.r );
  qmov( qone, u.i );
  qcmul( z, &u, &u );
  qcasin( &u, w );
  qclear( u.r );
  qmov( qone, u.i );
  qneg( u.i );
  qcmul( &u, w, w );
  return 0;
}


/* Complex hyperbolic cosine.  */

int
qccosh (z, w)
     qcmplx *z, *w;
{
  QELT x[NQ], y[NQ], u[NQ];

  qmov( z->r, x );
  qmov( z->i, y );

  /* cosh (x) * cos (y) */
  qcosh( x, w->r );
  qcos( y, u );
  qmul( u, w->r, w->r);

  /* sinh (x) * sin (y) */
  qsinh( x, w->i );
  qsin( y, u );
  qmul( u, w->i, w->i);
  return 0;
}


/* Complex inverse hyperbolic cosine.  */

int
qcacosh (z, w)
     qcmplx *z, *w;
{
  qcmplx u;

  qcacos( z, w );
  qclear( u.r );
  qmov( qone, u.i );
  qcmul( &u, w, w );
  return 0;
}


/* Complex hyperbolic tangent.  */

int
qctanh (z, w)
     qcmplx *z, *w;
{
  QELT x[NQ], y[NQ], d[NQ];

  qmov (z->r, x);
  if (x[1] != 0)
    x[1] += 1;
  qsinh( x, w->r);
  qmov (z->i, y);
  if (y[1] != 0)
    y[1] += 1;
  qsin( y, w->i);

  /* cosh 2x + cos 2y  */
  qcosh (x, d);
  qcos (y, x);
  qadd (x, d, d);

  qdiv (d, w->r, w->r);
  qdiv (d, w->i, w->i);
  return 0;
}


/* Complex inverse hyperbolic tangent.  */

int
qcatanh (z, w)
     qcmplx *z, *w;
{
  qcmplx u;

  qclear( u.r );
  qmov( qone, u.i );
  qcmul (z, &u, &u);  /* i z */
  qcatan (&u, w);
  qclear( u.r );
  qmov( qone, u.i );
  qneg( u.i );
  qcmul (&u, w, w);  /* -i catan iz */
  return 0;
}


/* z = complex x raised to the complex y power */
int qcpow( x, y, z )
qcmplx *x, *y, *z;
{
qcmplx w;

if( (x->r[1] == 0) && (x->i[1] == 0) )
	{ /* powers of zero */
	if( y->r[1] == 0 )
		{ /* real part of exponent = 0 */
		qmov( qone, &z->r[0] );
		qclear( &z->i[0] );
		if( y->i[1] != 0 ) /* indeterminate angle */
			mtherr( "qcpow", DOMAIN );
		return 0;
		}
	if( y->r[0] != 0 )
		{ /* real part negative -> infinity */
		qinfin( &z->r[0] );
		qclear( &z->i[0] );
		mtherr( "qcpow", DOMAIN );
		return 0;
		}
	qclear( &z->r[0] ); /* 0**(+x) = 0 */
	qclear( &z->i[0] );
	return 0;
	}
qclog( x, &w );
qcmul( &w, y, &w );
qcexp( &w, z );
return 0;
}
