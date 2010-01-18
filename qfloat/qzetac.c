/*							zetac.c
 *
 *	Riemann zeta function
 *
 *
 *
 * SYNOPSIS:
 *
 * int qzetac( x, y );
 * QELT *x, *y;
 *
 * qzetac( x, y );
 *
 *
 *
 * DESCRIPTION:
 *
 *
 *
 *                inf.
 *                 -    -x
 *   zetac(x)  =   >   k   ,   x > 1,
 *                 -
 *                k=2
 *
 * is related to the Riemann zeta function by
 *
 *	Riemann zeta(x) = zetac(x) + 1.
 *
 * Extension of the function definition for x < 1 is implemented.
 *
 *
 * ACCURACY:
 *
 * Series summation terminates at NBITS/2.
 *
 */

/*
 * By AMS55 #23.2.9,
 *
 * zeta(s) =
 *
 *                          inf.
 *  n            1-s          -
 *  -   -s      n            | |  x - [x]
 *  >  k    +  -----  -  s   |    ------- dx
 *  -           s-1        | |       s+1
 * k=1                      -       x
 *                           n
 *
 * for x = Re(s) > 0 and n > 0.  The term with the integral is
 * expanded by the Euler-Maclaurin summation formula into
 *
 *
 *           inf.  B   s(s+1)...(1+j-1)
 *    1       -     2j
 * - ---  +   >    --------------------
 *     s      -               s+j
 *   2n      j=1       (2j)! n
 *
 *  where the B2j are Bernoulli numbers.
 *
 *
 * References:
 *
 * Jahnke, E., and F. Emde, Tables of Functions, 
 * pp 269-274; Dover, 1945 (Note: one of the zeta function
 * expansion coefficients given by them is incorrect.)
 *
 * Froberg, C-E, Introduction to Numerical Analysis, 2nd ed.,
 * pp 231-233; Addison-Wesley, 1969.
 *
 */

/*							zetac	*/

/* Cephes Math Library Release 1.1:  March, 1985
 * Copyright 1985 by Stephen L. Moshier */

#include <stdio.h>
#include "qhead.h"

/* Expansion coefficients
 * for Euler-Maclaurin summation formula
 * (2k)! / B2k
 * where B2k are Bernoulli numbers
 */

#define NBERCOF 30

#define NBER 56
static char *bern_str[2*NBER] = {
 "1","6",  /* B_2 */
 "-1","30",  /* B_4 */
 "1","42",
 "-1","30",
 "5","66",  /* B_10 */
 "-691","2730",
 "7","6",
 "-3617","510",
 "43867","798",
 "-174611","330",  /* B_20 */
 "854513","138",
 "-236364091","2730",
 "8553103","6",
 "-23749461029","870",
 "8615841276005","14322", /* B_30 */
 "-7709321041217","510",
 "2577687858367","6",
 "-26315271553053477373","1919190",
 "2929993913841559","6",
 "-261082718496449122051","13530",  /* B_40 */
 "1520097643918070802691","1806",
 "-27833269579301024235023","690",
 "596451111593912163277961","282",
 "-5609403368997817686249127547", "46410",
 "495057205241079648212477525","66",  /* B_50 */
 "-801165718135489957347924991853","1590",
 "29149963634884862421418123812691","798",
 "-2479392929313226753685415739663229","870",
 "84483613348880041862046775994036021","354",
 "-1215233140483755572040304994079820246041491","56786730",  /* B_60 */
 "12300585434086858541953039857403386151","6",
 "-106783830147866529886385444979142647942017","510",
 "1472600022126335654051619428551932342241899101","64722",
 "-78773130858718728141909149208474606244347001","30",
 "1505381347333367003803076567377857208511438160235","4686",  /* B_70 */
 "-5827954961669944110438277244641067365282488301844260429","140100870",
 "34152417289221168014330073731472635186688307783087","6",
 "-24655088825935372707687196040585199904365267828865801","30",
 "414846365575400828295179035549542073492199375372400483487","3318",
"-4603784299479457646935574969019046849794257872751288919656867","230010", /* B_80 */
"1677014149185145836823154509786269900207736027570253414881613","498",
"-2024576195935290360231131160111731009989917391198090877281083932477",  "3404310",
"660714619417678653573847847426261496277830686653388931761996983","6",
"-1311426488674017507995511424019311843345750275572028644296919890574047",  "61410",
"1179057279021082799884123351249215083775254949669647116231545215727922535",  "272118", /* B_90 */
"-1295585948207537527989427828538576749659341483719435143023316326829946247", "1410",
"1220813806579744469607301679413201203958508415202696621436215105284649447",  "6",
"-211600449597266513097597728109824233673043954389060234150638733420050668349987259", "4501770",
"67908260672905495624051117546403605607342195728504487509073961249992947058239",  "6",
"-94598037819122125295227433069493721872702841533066936133385696204311395415197247711", "33330",  /* B_100 */
"3204019410860907078243020782116241775491817197152717450679002501086861530836678158791", "4326",
"-319533631363830011287103352796174274671189606078272738327103470162849568365549721224053",  "1590",
"36373903172617414408151820151593427169231298640581690038930816378281879873386202346572901",  "642",
"-3469342247847828789552088659323852541399766785760491146870005891371501266319724897592306597338057",  "209191710",
 "7645992940484742892248134246724347500528752413412307906683593870759797606269585779977930217515",  "1518",  /* B_110 */
 };

/* Initialize Bernoulli numbers from ASCII strings.  */
static QELT A[NBERCOF][NQ];

static QELT logs[33][NQ];

static int qber_ini = 0;
extern int asctoq(), qfloor(), qinfin();
extern QELT qhalf[], qone[], qtwo[], qpi[];

static int
init_bernum()
{
QELT den[NQ], qi[NQ];
int i, k;

if (qber_ini)
  return 0;

k = 0;
qmov(qtwo,qi);
for( i=0; i<NBERCOF; i++ )
  {
    asctoq( bern_str[k++], A[i] );
    asctoq( bern_str[k++], den );
    qdiv( den, A[i], A[i] );
    qfac(qi,den);
    qdiv( den, A[i], A[i] );
    qadd(qtwo,qi,qi);
  }
qmov(qtwo,qi);
for( i = 0; i < 33; i++)
{
  qlog(qi, logs[i]);
  qadd(qone, qi, qi);
}
qber_ini = 1;
return 0;
}


/*
 * Riemann zeta function, minus one
 */

static QELT notwo = 0;
int qzetac();

/* special entry to start summation at 3 instead of 2 */
int qzeta3( x, y )
QELT x[], y[];
{
notwo = 1;
qzetac( x, y );
return 0;
}


int qzetac( x, y )
QELT x[], y[];
{
int i, ia, conv;
long iw;
QELT min;
static QELT a[NQ], b[NQ], k[NQ], t[NQ];
QELT s[NQ], w[NQ];
double fabs();
/*double sin(), abs(), gamma(), power();*/

if (qber_ini == 0)
  {
    init_bernum();
  }

 if( x[0] != 0 )
   {

#if 0
	if( x < -30.8148 )
		{
		puts("zetac arg out of range" );
		return(0.0);
		}
#endif

	/* s = 1.0 - x; */
	qsub(x, qone, s);
	/*	w = zetac( s ); */
	qzetac(s, w );
	qadd(qone, w, w);

	/* b = sin(0.5*PI*x) * power(2.0*PI, x) * gamma(s) * (1.0 + w) / PI; */
	qmul(qhalf, qpi, t);
	qmul(x, t, t);
	qsin(t, t);
	qmul(t, w, w);

	qmov(qpi, t);
	t[1] += 1;
	qpow(t, x, t);
	qmul(t, w, w);

	qgamma(s,t);
	qmul(t,w,w);

	qdiv(qpi, w, w);

	/* return(b - 1.0); */
	qsub( qone, w, y);
	return 0;
   }

/*
if( x >= 127.0 )
	return(0);

i = iround(x);
w = fabs(x - i);
if( w == 0.0 )
	{
	if( i < 31 )
		return( azetac[i] );
	goto pseres;
	}
*/

/*							zetac	*/

/* Euler-Maclaurin summation formula */
/* Accuracy table
 *	iw	bits
 *	10	 75
 *	15	116
 *	17	129
 *	19	139
 *	20	144
 */

iw = 22;		/* w = 32.0 */
ltoq( &iw, w );
qclear( s );		/* s = 0.0  */

if( notwo == 1 )
	ia = 2;
else
	ia = 1;			/* a = 1.0 */


do
	{
	ia += 1;		/* a += 1.0 */
	/* b = power( a, -x ) */
	qmov( &logs[ia-2][0], t );	/* qlog( a, t ) */
	qmul( x, t, t );
	qexp( t, b );
	qdiv( b, qone, b );
	qadd( s, b, s );	/* s += b */
	if( ((int) s[1] - (int) b[1]) > NBITS/2 )
		goto done;
	}
while( ia < iw );	/* while a < w */

qsub( qone, x, t );	/* s += b*w/(x-1.0) */
qdiv( t, w, t );
qmul( t, b, t );
qadd( s, t, s );
qmov( b, t );		/* s -= 0.5 * b */
t[1] -= 1;
qsub( t, s, s );
qmov( qone, a );	/* a = 1.0 */
qclear( k );		/* k = 0.0 */
min = MAXEXP;
conv = 0;
for( i=0; i<30; i++ )
	{
	qadd( x, k, t );	/* a *= x + k */
	qmul( a, t, a );
	qdiv( w, b, b );	/* b /= w */
	qmul( &A[i][0], b, t );	/* t = a*b*A[i] */
	qmul( a, t, t );
	qadd( s, t, s );	/* s = s + t */
	if( t[1] >= min )
		break;
	min = t[1];
	conv = (int) s[1] - (int) t[1];
	if( conv > 144 )
		goto done;
	qadd( qone, k, k );	/* k += 1.0 */
	qadd( x, k, t );	/* a *= x + k */
	qmul( a, t, a );
	qdiv( w, b, b );	/* b /= w */
	qadd( qone, k, k );	/* k += 1.0 */
	}
printf( "zetac %d bits, %d terms\n", conv, i );
done:
qmov( s, y );


/* Basic sum of inverse powers */
/*
pseres:

s = 0.0;
a = 1.0;
do
	{
	a += 2.0;
	b = power( a, -x );
	s += b;
	}
while( b/s > 1.0e-17 );

b = power( 2.0, -x );
s = (s + b)/(1.0-b);
return(s);
*/
return 0;
}
