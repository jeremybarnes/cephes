/*							qhyp.c		*/

/*							qchyp1f1.c	*/

/*	confluent hypergeometric function
 *
 *                          1           2
 *                       a x    a(a+1) x
 *   F ( a,b;x )  =  1 + ---- + --------- + ...
 *  1 1                  b 1!   b(b+1) 2!
 *
 *
 * Series summation terminates at 70 bits accuracy.
 *
 */

#include "qhead.h"
#include "mconf.h"

extern QELT qone[];
extern qcmplx qcone;
#ifndef ANSIPROT
int qcneg(), qcsin(), qcmul(), qcmov(), qcdiv(), qclog(), qcsub(), qcadd();
int qcexp(), initqgam();
#endif

static qcmplx an, bn, a0, sum, n, t;

int qchyp1f1( a, b, x, y )
qcmplx *a, *b, *x, *y;
{
QELT ae, se;

qmov( qone, &qcone.r[0] );
qclear( &qcone.i[0] );
qcmov( a, &an );		/*an = a;*/
qcmov( b, &bn );		/*bn = b;*/
qcmov( &qcone, &a0 );	/*a0 = 1.0;*/
qcmov( &qcone, &sum );	/*sum = 1.0;*/
qcmov( &qcone, &n );	/*n = 1.0;*/

do
	{
	if( an.r[1] == 0 && an.i[1] == 0 )
		goto done;
	if( bn.r[1] == 0 && bn.i[1] == 0 )
		{
		qinfin(sum.r);
		qinfin(sum.i);
		goto done;
		}
/*
	if( (a0 > 1.0e34) || (n > 130) )
		goto asymf;
*/
	qcmul( &bn, &n, &t );
	qcdiv( &t, &a0, &a0 );
	qcmul( &an, x, &t );
	qcmul( &t, &a0, &a0 );	/*a0 *= (an * x) / (bn * n);*/
	qcadd( &sum, &a0, &sum );	/*sum += a0;*/
	qcadd( &an, &qcone, &an );	/*an += 1.0;*/
	qcadd( &bn, &qcone, &bn );	/*bn += 1.0;*/
	qcadd( &n, &qcone, &n );	/*n += 1.0;*/
	ae = a0.r[1];
	if (a0.i[1] > ae)
	  ae = a0.i[1];
	se = sum.r[1];
	if (sum.i[1] > se)
	  se = sum.i[1];
	}
while( ((int) ae - (int) se) > -70 );

/*printf("1F1( %.2E %.2E %.5E ) = %.3E %.6E\n", a, b, x,  n, sum);*/
done:
qcmov( &sum, y );
return(0);
}
