/* Test program for eigens.c
 */

#define N 11
#define DEBUG 0
static double A[N*N], A0[N*N], X[N*N], D[N*N];
static double RR[N*N];
static double B[N], Y[N];
static int IPS[N];
double maxoffd(), sqrt();

main()
{
long ntrials;
int i, j, k;
double x, e, merr;

merr = 0.0;
ntrials = 0;

loop:

/* Fill A[] with random numbers */
k = N*(N+1)/2;
for( i=0; i<k; i++ )
	{
	drand( &x );
	x -= 1.5;
	x = x + x;
	A[i] = x;
	}
/* Unpack A into a symmetric matrix A0 */
tritosquare( N, A, A0 );

eigens( A, RR, B, N );
mtransp( N, RR, RR );

#if DEBUG
/* Print the computed eigenvalues */
for( i=0; i<N; i++ )
	printf( "%6.2f", B[i] );
/* Print the reduced triangular matrix */
printf( "\n" );
k = 0;
for( i=0; i<N; i++ )
	{
	for( j=0; j<=i; j++ )
		{
		printf( "%6.2f", A[k] );
		k += 1;
		}
	printf( "\n" );
	}

/* Print the original symmetric matrix */
printf( "\n" );
for( i=0; i<N; i++ )
	{
	for( j=0; j<N; j++ )
		printf( "%6.2f", A0[N*i+j] );
	printf( "\n" );
	}

/* Print the matrix of eigenvectors */
printf( "\n" );
for( i=0; i<N; i++ )
	{
	for( j=0; j<N; j++ )
		printf( "%6.2f", RR[N*i+j] );
	printf( "\n" );
	}

#endif

/* Multiply the original matrix by the eigenvectors */
mmmpy( N, N, A0, RR, D );
/* Divide each result vector by the corresponding eigenvalue. */
k = 0;
for( i=0; i<N; i++ )
	{
	x = B[i];
	for( j=0; j<N; j++ )
		{
		D[k] /= x;
		k += 1;
		}
	}

#if DEBUG
printf( "\n" );
for( i=0; i<N; i++ )
	{
	for( j=0; j<N; j++ )
		printf( "%6.2f", D[N*i+j] );
	printf( "\n" );
	}
#endif

/* Compare the result with the computed eigenvector */
mtransp( N, D, D );
e = 0.0;
for( i=0; i<N*N; i++ )
	{
	x = D[i] - RR[i];
	if( x < 0 )
		x = -x;
	if( x > e )
		e = x;
	}
if( e > merr )
	merr = e;
ntrials += 1;
printf( "%4ld %.3e\n", ntrials, merr );
goto loop;
}
