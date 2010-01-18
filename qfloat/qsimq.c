/*							qsimq.c
 *
 *	Solution of simultaneous linear equations AX = B
 *	by Gaussian elimination with partial pivoting
 *
 *
 *
 * SYNOPSIS:
 *
 * double A[n*n], B[n], X[n];
 * int n, flag;
 * int IPS[];
 * int simq();
 *
 * ercode = simq( A, B, X, n, flag, IPS );
 *
 *
 *
 * DESCRIPTION:
 *
 * B, X, IPS are vectors of length n.
 * A is an n x n matrix (i.e., a vector of length n*n),
 * stored row-wise: that is, A(i,j) = A[ij],
 * where ij = i*n + j, which is the transpose of the normal
 * column-wise storage.
 *
 * The contents of matrix A are destroyed.
 *
 * Set flag=0 to solve.
 * Set flag=-1 to do a new back substitution for different B vector
 * using the same A matrix previously reduced when flag=0.
 *
 * The routine returns nonzero on error; messages are printed.
 *
 *
 * ACCURACY:
 *
 * Depends on the conditioning (range of eigenvalues) of matrix A.
 *
 *
 * REFERENCE:
 *
 * Computer Solution of Linear Algebraic Systems,
 * by George E. Forsythe and Cleve B. Moler; Prentice-Hall, 1967.
 *
 */

/*							simq	2 */
#include <stdio.h>
#include "qhead.h"
static QELT em[NQ];
static QELT q[NQ];
static QELT rownrm[NQ];
static QELT big[NQ];
static QELT size[NQ];
static QELT pivot[NQ];
static QELT sum[NQ];
extern QELT qone[];

int simq( A, B, X, n, flag, IPS )
QELT A[], B[], X[];
int n, flag;
int IPS[];
{
int i, j, ij, ip, ipj, ipk, ipn;
int idxpiv, iback;
int k, kp, kp1, kpk, kpn;
int nip, nkp, nm1;
QELT *ptr;

nm1 = n-1;
if( flag < 0 )
	goto solve;

/*	Initialize IPS and X	*/

ij=0;
for( i=0; i<n; i++ )
	{
	IPS[i] = i;
	qclear( rownrm );
	for( j=0; j<n; j++ )
		{
		qmov( &A[NQ*ij], q );	/* q = abs( A[ij] ) */
		q[0] = 0;
		if( qcmp( rownrm, q ) < 0 )	/* rownrm < q */
			qmov( q, rownrm );	/* rownrm = q */
		ij += 1;
		}
	if( rownrm[1] < 2  )
		{
		printf("SIMQ ROWNRM=0");
		return(1);
		}
	qdiv( rownrm, qone, &X[NQ*i] );	/* X[i] = 1.0/rownrm */
	}

/*							simq	3 */
/*	Gaussian elimination with partial pivoting 	*/

for( k=0; k<nm1; k++ )
	{
	qclear( big );
	idxpiv = 0;
	for( i=k; i<n; i++ )
		{
		ip = IPS[i];
		ipk = n*ip + k;
			/* size = abs( A[ipk] ) * X[ip] */
		qmov( &A[NQ*ipk], size );
		size[0] = 0;
		qmul( size, &X[NQ*ip], size );
		if( qcmp( size, big ) > 0 )	/* size > big  */
			{
			qmov( size, big );	/* big = size */
			idxpiv = i;
			}
		}

	if( big[1] < 2 )
		{
		printf( "SIMQ BIG=0" );
		return(2);
		}
	if( idxpiv != k )
		{
		j = IPS[k];
		IPS[k] = IPS[idxpiv];
		IPS[idxpiv] = j;
		}
	kp = IPS[k];
	kpk = n*kp + k;
	qmov( &A[NQ*kpk], pivot );	/* pivot = A[kpk] */
	kp1 = k+1;
	for( i=kp1; i<n; i++ )
		{
		ip = IPS[i];
		ipk = n*ip + k;
		ptr = &A[NQ*ipk];
		qdiv( pivot, ptr, em );	/* em = -A[ipk]/pivot */
		qmov( em, ptr );	/* A[ipk] = -em */
		qneg( em );		
		nip = n*ip;
		nkp = n*kp;
		for( j=kp1; j<n; j++ )
			{
			ipj = nip + j;
				/* A[ipj] = A[ipj] + em * A[nkp + j] */
			ptr = &A[NQ*ipj];
			qmul( &A[NQ*(nkp+j)], em, q );
			qadd( ptr, q, ptr );
			}
		}
	}
kpn = n * IPS[n-1] + n - 1;	/* last element of IPS[n] th row */
if( A[NQ*kpn + 1] < 2 )
	{
	printf( "SIMQ A[kpn]=0");
	return(3);
	}

/*							simq 4 */
/*	back substitution	*/

solve:
ip = IPS[0];
qmov( &B[NQ*ip], &X[0] );	/* X[0] = B[ip] */
for( i=1; i<n; i++ )
	{
	ip = IPS[i];
	ipj = n * ip;
	qclear( sum );
	for( j=0; j<i; j++ )
		{
		qmul( &A[NQ*ipj], &X[NQ*j], q ); /* sum += A[ipj] * X[j] */
		qadd( sum, q, sum );
		++ipj;
		}
	qsub( sum, &B[NQ*ip], &X[NQ*i] ); /* X[i] = B[ip] - sum */
	}

ipn = n * IPS[n-1] + n - 1;
ptr = &X[NQ*(n-1)];
qdiv( &A[NQ*ipn], ptr, ptr );	/* X[n-1] = X[n-1]/A[ipn] */

for( iback=1; iback<n; iback++ )
	{
/* i goes (n-1),...,1	*/
	i = nm1 - iback;
	ip = IPS[i];
	nip = n*ip;
	qclear( sum );
	for( j=i+1; j<n; j++ )
		{
			 /* sum += A[nip+j] * X[j] */
		qmul( &X[NQ*j], &A[NQ*(nip+j)], q );
		qadd( sum, q, sum );
		}
	ptr = &X[NQ*i];
	qsub( sum, ptr, q );	/* X[i] = (X[i] - sum)/A[nip+i] */
	qdiv( &A[NQ*(nip+i)], q, ptr );
	}
return(0);
}
