/*							qairy.c
 *
 *	Airy functions
 *
 *
 *
 * SYNOPSIS:
 *
 * int qairy( x, ai, aip, bi, bip );
 * QELT *x, *ai, *aip, *bi, *bip;
 *
 * qairy( x, ai, aip, bi, bip );
 *
 *
 *
 * DESCRIPTION:
 *
 * Solution of the differential equation
 *
 *	y"(x) = xy.
 *
 * The function returns the two independent solutions Ai, Bi
 * and their first derivatives Ai'(x), Bi'(x).
 *
 * Evaluation is by power series summation for small x,
 * by asymptotic expansion for large x.
 *
 *
 * ACCURACY:
 *
 * The asymptotic expansion is truncated at less than full working precision.
 *
 */

/*
Cephes Math Library Release 2.1:  January, 1989
Copyright 1984, 1987, 1989 by Stephen L. Moshier
*/

/* Airy functions */

#include "qhead.h"

/* Flag to determine which functions to compute.
 * aiconf = 0, compute all four functions.
 * aiconf = 1, do Ai, Bi only.
 * aiconf = -1, do Ai', Bi' only.
 */
int aiconf = 0;
extern int aiconf;

/* Gamma(1/3)
 * = 2.6789385347077476336556929409746776441286893779573011009504283275904176E0
 *
 * c2 = 1/(Gamma(1/3) * 3**(1/3)) =
 *  2.5881940379280679840518356018920396347909113835493458221000181385610277E-1
 */
#if WORDSIZE == 32
static QELT qc2[NQ] = {
0,EXPONE-2,0,0x8483fa15,0xb87c545d,0x74df94a0,0x43b3dc3c,0xd463e126,
0xba75161f,0x4d851875,0x3816a8c2,0xc1c78a7d,0x3ce1e0ba,0xff140000,};
#else
/* WORDSIZE is 16 */
#if NQ == 24
static short qc2[NQ] = {
0x0000,EXPONE-2,0x0000,0x8483,0xfa15,0xb87c,0x545d,0x74df,
0x94a0,0x43b3,0xdc3c,0xd463,0xe126,0xba75,0x161f,0x4d85,
0x1875,0x3816,0xa8c2,0xc1c7,0x8a7d,0x3ce1,0xe0ba,0xff14,};
#else
static short qc2[NQ] = {
0x0000,EXPONE-2,0x0000,0x8483,0xfa15,0xb87c,0x545d,0x74df,
0x94a0,0x43b3,0xdc3c,0xd464};
#endif
#endif
/* Gamma(2/3)
 * = 1.3541179394264004169452880281545137855193272660567936983940224679637830E0
 *
 * c1 = 1/(Gamma(2/3) * 3**(2/3)) =
 *  3.5502805388781723926006318600418317639797917419917724058332651030081004E-1
 */
#if WORDSIZE == 32
static QELT qc1[NQ] = {
0,EXPONE-2,0,0xb5c63cb1,0x38adc2f5,0x2daf7609,0xcc9edb52,0xf56200d6,
0x4df3d6c8,0x42466728,0x441708ae,0x08f972aa,0xf93e7687,0xa6880000};
#else
#if NQ == 24
static short qc1[NQ] = {
0x0000,EXPONE-2,0x0000,0xb5c6,0x3cb1,0x38ad,0xc2f5,0x2daf,
0x7609,0xcc9e,0xdb52,0xf562,0x00d6,0x4df3,0xd6c8,0x4246,
0x6728,0x4417,0x08ae,0x08f9,0x72aa,0xf93e,0x7687,0xa688,};
#else
static short qc1[NQ] = {
0x0000,EXPONE-2,0x0000,0xb5c6,0x3cb1,0x38ad,0xc2f5,0x2daf,
0x7609,0xcc9e,0xdb52,0xf562};
#endif
#endif
extern QELT qone[], qtwo[], qpi[];

static QELT qthree[NQ]; /* 1/3 */
static QELT qsqrt3[NQ];
static QELT q216[NQ];
static QELT qk[NQ];
static QELT qnum[NQ];
static QELT qden[NQ];
static QELT aif[NQ];
static QELT aig[NQ];
static QELT zeta[NQ];
static QELT aisin[NQ];
static QELT aicos[NQ];
static QELT z[NQ];
static QELT t[NQ];
static QELT uf[NQ];
static QELT ug[NQ];
static QELT iniflg = 0;
/*
extern QELT zeta[], aisin[], aicos[];
*/
#ifdef ANSIPROT
int qainfg(void), qanfgp(void);
#else
int qainfg(), qanfgp();
int qgamma(), qcbrt();
#endif

int qairy( x, ai, aip, bi, bip )
QELT x[], ai[], aip[], bi[], bip[];
{
QELT temp[NQ];
int i, k;
QELT maxf;
long ll;
union
  {
    unsigned short s[4];
    double d;
  } dx;

ll = 216;
ltoq( &ll, q216);
qmov( x, t );		/* t = abs(x);		*/
t[0] = 0;
qclear( aisin );
qclear( aicos );
qclear( zeta );
if( iniflg == 0 )
	{
	iniflg = 1;
	qmov( qtwo, qthree );
	qadd( qone, qthree, qthree );
	qdiv( qthree, qone, qthree );	/* qthree = 1/3		*/
	qsqrt( qthree, qsqrt3 );	/* qsqrt3 = sqrt(1/3) */
	}

if( t[1] < 3 )
	{
	qgamma( qthree, uf ); /* gamma(1/3) */
	qdiv( uf, qone, bip );
	qcbrt( qthree, z );   /* cbrt(1/3) */
	qmul( bip, z, aip );
	qneg( aip );
	qthree[1] += 1;
	qgamma( qthree, ug ); /* gamma(2/3) */
	qdiv( ug, z, bi );
	qmul( z, bi, ai );
	return(0);
	}

qmul( t, t, z );	/* z = t * t * t		*/
qmul( z, t, z );


qtoe( x, dx.s );

if( dx.d > 20.0 )
	goto asymp;
if( dx.d < -20.0 )
	goto aineg;

/* f, g */
t[0] = x[0];	/* put back the sign */
z[0] = x[0];		/* restore sign of z = x * x * x;*/

if( aiconf < 0 )
	goto doaip;

qmov( qone, aif );	/*f = 1.0;*/
qmov( t, aig );		/* g = x;*/
qmov( qone, uf );	/* uf = 1.0;*/
qmov( t, ug );		/* ug = x;*/
qmov( qone, qk );	/* k = 1.0;*/
while( ((int) aif[1] - (int) uf[1]) < NBITS )
	{
	qmul( uf, z, uf );	/* uf *= z;*/
	qadd( qone, qk, qk );	/* k += 1.0;*/
	qdiv( qk, uf, uf );	/* uf /=k;*/
	qmul( ug, z, ug );	/* ug *= z;*/
	qadd( qone, qk, qk );	/* k += 1.0;*/
	qdiv( qk, ug, ug );	/* ug /=k;*/
	qdiv( qk, uf, uf );	/* uf /=k;*/
	qadd( aif, uf, aif );	/* f += uf;*/
	qadd( qone, qk, qk );	/* k += 1.0;*/
	qdiv( qk, ug, ug );	/* ug /=k;*/
	qadd( aig, ug, aig );	/* g += ug;*/
	}
qmul( qc1, aif, uf );		/* uf = c1 * f;*/
qmul( qc2, aig, ug );		/* ug = c2 * g;*/
qsub( ug, uf, ai );		/* *ai = uf - ug;*/
qadd( ug, uf, bi );		/* *bi = sqrt3 * (uf + ug);*/
qdiv( qsqrt3, bi, bi );
if( aiconf > 0 )
	goto aidone;

/* the deriviative of ai */
doaip:

qmov( qone, qk );		/* k = 4.0;*/
qk[1] += 2;
qmul( t, t, uf );		/* uf = x * x/2.0;*/
uf[1] -= 1;
qmul( qthree, z, ug );		/* ug = z/3.0;*/
qmov( uf, aif );		/* f = uf;*/
qadd( qone, ug, aig );		/* g = 1.0 + ug;*/
qmul( qthree, uf, uf );		/* uf /= 3.0;*/

while( ((int) aig[1] - (int) ug[1]) < NBITS )
	{
	qmul( z, uf, uf );	/* uf *= z;*/
	qdiv( qk, ug, ug );	/* ug /=k;*/
	qadd( qone, qk, qk );	/* k += 1.0;*/
	qmul( z, ug, ug );	/* ug *= z;*/
	qdiv( qk, uf, uf );	/* uf /=k;*/
	qadd( uf, aif, aif );	/* f += uf;*/
	qadd( qone, qk, qk );	/* k += 1.0;*/
	qdiv( qk, ug, ug );	/* ug /=k;*/
	qdiv( qk, uf, uf );	/* uf /=k;*/
	qadd( ug, aig, aig );	/* g += ug;*/
	qadd( qone, qk, qk );	/* k += 1.0;*/
	}

qmul( qc1, aif, uf );		/* uf = c1 * f;*/
qmul( qc2, aig, ug );		/* ug = c2 * g;*/
qsub( ug, uf, aip );		/* *aip = uf - ug;*/
qadd( uf, ug, bip );		/* *bip = sqrt3 * (uf + ug);*/
qdiv( qsqrt3, bip, bip );
goto aidone;

/* negative x */

aineg:


t[0] = 0;
z[0] = 0;
qsqrt( z, zeta );
zeta[1] += 1;		/* zeta = 2.0 * sqrt(z) / 3.0	*/
qmul( qthree, zeta, zeta );

if( aiconf < 0 )
	goto ainegp;

qainfg();
qadd( qone, uf, uf );

qmov( qpi, temp ); /* zeta + pi/4 */
temp[1] -= 2;
qadd( zeta, temp, temp );
qsin( temp, aisin );
qcos( temp, aicos );

qmul( uf, aisin, temp );
qmul( ug, aicos, ai );
qsub( ai, temp, ai );

qsqrt( t, temp );
qmul( qpi, temp, temp );
qsqrt( temp, temp );
qdiv( temp, ai, ai );

qmul( ug, aisin, qk );
qmul( uf, aicos, bi );
qadd( bi, qk, bi );
qdiv( temp, bi, bi );

if( aiconf > 0 )
	goto aidone;


ainegp:

qanfgp();
qadd( qone, uf, uf );

qmov( qpi, temp ); /* zeta + pi/4 */
temp[1] -= 2;
qadd( zeta, temp, temp );
qsin( temp, aisin );
qcos( temp, aicos );

qmul( uf, aicos, temp );
qmul( ug, aisin, aip );
qadd( aip, temp, aip );

qsqrt( t, temp );
qdiv( qpi, temp, temp );
qsqrt( temp, temp );
temp[0] = -1;
qmul( temp, aip, aip );

qmul( ug, aicos, qk );
qmul( uf, aisin, bip );
qsub( qk, bip, bip );
temp[0] = 0;
qmul( temp, bip, bip );
goto aidone;



/* large positive x */
asymp:

qsqrt( z, zeta );
zeta[1] += 1;		/* zeta = 2.0 * sqrt(z) / 3.0	*/
qmul( qthree, zeta, zeta );

if( aiconf < 0 )
	goto adoaip;

maxf = MAXEXP;
k = 1;
qmov( qone, qk );
qmov( qone, qden );
qmov( qone, uf );
qmov( qone, ug );

do
	{
	i = 2*k + 1;
	qmov( qk, temp );
	temp[1] += 1;
	qadd( qone, temp, temp );
	qmov( temp, qnum );

	while( i < (6*k-1) )
		{
		i += 2;
		qadd( qtwo, temp, temp );
		qmuli( temp, qnum, qnum );
		}
	qmuli( q216, qden, qden );
	qmuli( qk, qden, qden );
	qmul( zeta, qden, qden );
	qdiv( qden, qnum, temp );
	if( k & 1 )
		qsub( temp, uf, uf );
	else
		qadd( temp, uf, uf );
	qadd( temp, ug, ug );	/* for Bi */
	k += 1;
	qadd( qone, qk, qk );
	if( temp[1] < maxf )
		maxf = temp[1];
	if( temp[1] > maxf )
		break;
/*
	if( k > 40 )
		break;
*/
	}
while( ((int) qone[1] - (int) temp[1]) < NBITS/2 );

qsqrt( t, temp );
qmul( qpi, temp, temp );
qsqrt( temp, temp );

qexp( zeta, qk );
qmul( qk, temp, ai );
qdiv( ai, uf, ai );
ai[1] -= 1;

qdiv( temp, qk, bi );
qmul( ug, bi, bi );

if( aiconf > 0 )
	goto aidone;




adoaip:

maxf = MAXEXP;
k = 1;
qmov( qone, qk );
qmov( qone, qden );
qmov( qone, uf );
qmov( qone, ug );

do
	{
	i = 2*k + 1;
	qmov( qk, temp );
	temp[1] += 1;
	qadd( qone, temp, temp );
	qmov( temp, qnum );

	while( i < (6*k-3) )
		{
		i += 2;
		qadd( qtwo, temp, temp );
		qmuli( temp, qnum, qnum );
		}
/*
	ck * -(6k+1)/(6k-1)
*/
	qadd( qtwo, temp, temp );	/* *(6k+1) */
	qadd( qtwo, temp, temp );
	qmuli( temp, qnum, qnum );
	if( qnum[0] != 0 )
		qnum[0] = 0;
	else
		qnum[0] = -1;
	qmuli( q216, qden, qden );
	qmuli( qk, qden, qden );
	qmul( zeta, qden, qden );
	qdiv( qden, qnum, temp );
	if( k & 1 )
		qsub( temp, uf, uf );
	else
		qadd( temp, uf, uf );
	qadd( temp, ug, ug );	/* for Bi' */
	if( temp[1] < maxf )
		maxf = temp[1];
	if( temp[1] > maxf )
		break;
	k += 1;
	qadd( qone, qk, qk );
/*
	if( k > 40 )
		break;
*/
	}
while( ((int) qone[1] - (int) temp[1]) < NBITS/2 );

qsqrt( t, temp );
qdiv( qpi, temp, temp );
qsqrt( temp, temp );
qexp( zeta, qk );
qdiv( qk, temp, aip );
qmul( aip, uf, aip );
aip[1] -= 1;
if( aip[0] != 0 )
	aip[0] = 0;
else
	aip[0] = -1;
qmul( qk, temp, bip );
qmul( bip, ug, bip );

aidone:

return(0);
}


/* Auxiliary functions */

extern QELT qnine[];
static QELT ai[NQ];
static QELT aip[NQ];
static QELT bi[NQ];
static QELT bip[NQ];
static QELT tt[NQ];

int qainfg()
{
QELT temp[NQ];
QELT maxf;
int k, i;
long ll;

maxf = MAXEXP;
k = 1;
qmov( qone, qk );
qmov( qone, qden );
/*qmov( qone, uf );*/
qclear( uf );
qclear( ug );
ll = 216;
ltoq( &ll, q216);

do
	{
	i = 2*k + 1;
	qmov( qk, temp );
	temp[1] += 1;
	qadd( qone, temp, temp );
	qmov( temp, qnum );

	while( i < (6*k-1) )
		{
		i += 2;
		qadd( qtwo, temp, temp );
		qmuli( temp, qnum, qnum );
		}
	qmuli( q216, qden, qden );
	qmuli( qk, qden, qden );
	qmul( zeta, qden, qden );
	qdiv( qden, qnum, temp );
	if( k & 1 )
		{
		if( k & 2 )
			qsub( temp, ug, ug );
		else
			qadd( temp, ug, ug );
		}
	else
		{
		if( k & 2 )
			qsub( temp, uf, uf );
		else
			qadd( temp, uf, uf );
		}
	if( temp[1] < maxf )
		maxf = temp[1];
	if( temp[1] > maxf )
		break;
	qadd( qone, qk, qk );
	k += 1;
/*
	if( k > 70 )
		break;
*/
	}
while( ((int) qone[1] - (int) temp[1]) < NBITS/2 );
return 0;
}



int qanfgp()
{
QELT temp[NQ];
QELT maxf;
int k, i;
long ll;

maxf = MAXEXP;
k = 1;
qmov( qone, qk );
qmov( qone, qden );
/*qmov( qone, uf );*/
qclear( uf );
qclear( ug );
ll = 216;
ltoq( &ll, q216);

do
	{
	i = 2*k + 1;
	qmov( qk, temp );
	temp[1] += 1;
	qadd( qone, temp, temp );
	qmov( temp, qnum );

	while( i < (6*k-3) )
		{
		i += 2;
		qadd( qtwo, temp, temp );
		qmuli( temp, qnum, qnum );
		}
/*
	ck * -(6k+1)/(6k-1)
*/
	qadd( qtwo, temp, temp );	/* *(6k+1) */
	qadd( qtwo, temp, temp );
	qmuli( temp, qnum, qnum );
	if( qnum[0] != 0 )
		qnum[0] = 0;
	else
		qnum[0] = -1;
	qmuli( q216, qden, qden );
	qmuli( qk, qden, qden );
	qmul( zeta, qden, qden );
	qdiv( qden, qnum, temp );
	if( k & 1 )
		{
		if( k & 2 )
			qsub( temp, ug, ug );
		else
			qadd( temp, ug, ug );
		}
	else
		{
		if( k & 2 )
			qsub( temp, uf, uf );
		else
			qadd( temp, uf, uf );
		}
	if( temp[1] < maxf )
		maxf = temp[1];
	if( temp[1] > maxf )
		break;
	k += 1;
	qadd( qone, qk, qk );
/*
	if( k > 70 )
		break;
*/
	}
while( ((int) qone[1] - (int) temp[1]) < NBITS/2 );

return 0;
}




/* Functions for polynomial approximation */

/* ai * sin(zeta) + bi * cos(zeta) */
int qainf(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];
union
  {
    unsigned short s[4];
    double d;
  } dx;

aiconf = 1;
if( qx[1] < 3 )
	{
	qmov( qone, qy );
	goto fdone;
	}

qtoe( qx, dx.s );
if( dx.d < 0.020539595906443729 )
	{
	qdiv( qx, qone, zeta );
	qainfg();
	qmov( uf, qy );
	goto fdone;
	}

/*
t = 4.0 * x * x / 9.0;
t = 1.0/cbrt(t);
airy( t, &ai, &aip, &bi, &bip );
z = exp( 1.0/x );
t = sqrt(t);
t = sqrt(PI*t);
y = bi * t / z;
*/

qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
tt[0] = -1;
qairy( tt, ai, aip, bi, bip );
tt[0] = 0;

qsqrt( tt, tt );
qmul( tt, qpi, tt );
qsqrt( tt, tt );

qmul( tt, ai, ai );
qmul( tt, bi, bi );

if( (aisin[1] == 0) || (aicos[1] == 0) )
	{
	qdiv( qx, qone, temp );
	qmov( qpi, tt );
	tt[1] -= 2;
	qadd( temp, tt, tt );
	qsin( tt, aisin );
	qcos( tt, aicos );
	}

qmul( aisin, ai, temp );
qmul( aicos, bi, tt );
qadd( tt, temp, qy );
qsub( qone, qy, qy );

fdone: ;
return 0;
}



/* -ai * cos(zeta) + bi * sin( zeta) */

int qaing(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];
union
  {
    unsigned short s[4];
    double d;
  } dx;

aiconf = 1;
if( qx[1] < 3 )
	{
	qclear(qy);
	goto fdone;
	}

qtoe( qx, dx.s );
if( dx.d <= 0.020539595906443729 )
	{
	qdiv( qx, qone, zeta );
	qainfg();
	qmov( ug, qy );
	goto fdone;
	}

qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
tt[0] = -1;
qairy( tt, ai, aip, bi, bip );
tt[0] = 0;

qsqrt( tt, tt );
qmul( tt, qpi, tt );
qsqrt( tt, tt );

qmul( tt, ai, ai );
qmul( tt, bi, bi );

if( (aisin[1] == 0) || (aicos[1] == 0) )
	{
	qdiv( qx, qone, temp );
	qmov( qpi, tt );
	tt[1] -= 2;
	qadd( temp, tt, tt );
	qsin( tt, aisin );
	qcos( tt, aicos );
	}
qmul( aisin, bi, temp );
qmul( aicos, ai, tt );
qsub( tt, temp, qy );

fdone: ;
return 0;
}



/* -aip * cos(zeta) + bip * sin(zeta) */
int qaipnf(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];
union
  {
    unsigned short s[4];
    double d;
  } dx;

aiconf = -1;
if( qx[1] < 3 )
	{
	qmov( qone, qy );
	goto fdone;
	}

qtoe( qx, dx.s );
if( dx.d <= 0.020539595906443729 )
	{
	qdiv( qx, qone, zeta );
	qanfgp();
	qmov( uf, qy );
	goto fdone;
	}

/*
t = 4.0 * x * x / 9.0;
t = 1.0/cbrt(t);
airy( t, &ai, &aip, &bi, &bip );
z = exp( 1.0/x );
t = sqrt(t);
t = sqrt(PI*t);
y = bi * t / z;
*/

qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
tt[0] = -1;
qairy( tt, ai, aip, bi, bip );
tt[0] = 0;

qsqrt( tt, tt );
qdiv( tt, qpi, tt );
qsqrt( tt, tt );

qmul( tt, aip, aip );
qmul( tt, bip, bip );

if( (aisin[1] == 0) || (aicos[1] == 0) )
	{
	qdiv( qx, qone, temp );
	qmov( qpi, tt );
	tt[1] -= 2;
	qadd( temp, tt, tt );
	qsin( tt, aisin );
	qcos( tt, aicos );
	}

qmul( aicos, aip, temp );
qmul( aisin, bip, tt );
qsub( temp, tt, qy );
qsub( qone, qy, qy );

fdone: ;
return 0;
}





/* -aip * sin(zeta) - bip * cos(zeta) */
int qaipng(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];
union
  {
    unsigned short s[4];
    double d;
  } dx;

aiconf = -1;
if( qx[1] < 3 )
	{
	qmov( qone, qy );
	goto fdone;
	}

qtoe( qx, dx.s );
if( dx.d <= 0.020539595906443729 )
	{
	qdiv( qx, qone, zeta );
	qanfgp();
	qmov( ug, qy );
	goto fdone;
	}

qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
tt[0] = -1;
qairy( tt, ai, aip, bi, bip );
tt[0] = 0;

qsqrt( tt, tt );
qdiv( tt, qpi, tt );
qsqrt( tt, tt );

qmul( tt, aip, aip );
qmul( tt, bip, bip );

if( (aisin[1] == 0) || (aicos[1] == 0) )
	{
	qdiv( qx, qone, temp );
	qmov( qpi, tt );
	tt[1] -= 2;
	qadd( temp, tt, tt );
	qsin( tt, aisin );
	qcos( tt, aicos );
	}

qmul( aisin, aip, temp );
qmul( aicos, bip, tt );
qadd( temp, tt, qy );
qneg( qy );

fdone: ;
return 0;
}




/*
 *            - x**(1/4)  R(1/zeta)
 * Ai'(x)  =  ----------------------
 *             2 sqrt(pi) exp(zeta)
 */
int qaipp(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];

aiconf = -1;
if( qx[1] < 10 )
	{
	qclear( qy );
	goto fdone;
	}

/* tt = cbrt(9*zeta/4 */
qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
qairy( tt, ai, aip, bi, bip );

qsqrt( tt, tt );
qdiv( tt, qpi, tt );
qsqrt( tt, tt );
qmul( tt, aip, qy );

qdiv( qx, qone, temp );
qexp( temp, temp );
qmul( temp, qy, qy );
qy[1] += 1;
qneg( qy );

fdone: ;
return 0;
}





/*
 *          x**(1/4) exp(zeta)
 * Bi'(x) = ----------------- [ 1 + 1/zeta R(1/zeta) ]
 *              sqrt(pi)
 */
int qbipp(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];

aiconf = -1;
if( qx[1] < 10 )
	{
	qclear( qy );
	goto fdone;
	}

qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
qairy( tt, ai, aip, bi, bip );

qsqrt( tt, tt );
qdiv( tt, qpi, tt );
qsqrt( tt, tt );
qmul( tt, bip, qy );

qdiv( qx, qone, temp );
qexp( temp, temp );
qdiv( temp, qy, qy );
qsub( qone, qy, qy );

fdone: ;
return 0;
}





/*
 *                   R(1/zeta)
 * Ai(x)  =  ----------------------------
 *           2 sqrt(pi sqrt(x)) exp(zeta)
 */
int qaiasym(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];

aiconf = 1;
if( qx[1] < 10 )
	{
	qclear( qy );
	goto fdone;
	}

/* tt = cbrt(9*zeta/4 */
qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
qairy( tt, ai, aip, bi, bip );

qsqrt( tt, tt );
qmul( tt, qpi, tt );
qsqrt( tt, tt );
qmul( tt, ai, qy );

qdiv( qx, qone, temp );
qexp( temp, temp );
qmul( temp, qy, qy );
qy[1] += 1;

fdone: ;
return 0;
}




/*
 *             exp(zeta)
 * Bi(x) = ----------------- [ 1 + 1/zeta R(1/zeta) ]
 *          sqrt(pi sqrt(x))
 */
int qbiasym(qx, qy)
QELT qx[], qy[];
{
QELT temp[NQ];

aiconf = 1;
if( qx[1] < 10 )
	{
	qclear( qy );
	goto fdone;
	}

qmul( qx, qx, tt );
tt[1] += 2;
qdiv( qnine, tt, tt );
qcbrt( tt, tt );

qdiv( tt, qone, tt );
qairy( tt, ai, aip, bi, bip );

qsqrt( tt, tt );
qmul( tt, qpi, tt );
qsqrt( tt, tt );
qmul( tt, bi, qy );

qdiv( qx, qone, temp );
qexp( temp, temp );
qdiv( temp, qy, qy );
qsub( qone, qy, qy );

fdone: ;
return 0;
}

