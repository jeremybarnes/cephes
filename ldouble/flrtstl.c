long double floorl(), ldexpl(), frexpl();

#define N 16382
void prnum();
int printf();
void exit();

void main()
{
long double x, f, y, last, z, z0, y1;
int i, k, e, e0, errs;

errs = 0;
f = 0.1L;
x = f;
last = x;
z0 = frexpl( x, &e0 );
printf( "frexpl(%.2Le) = %.5Le, %d\n", x, z0, e0 );
k = 0;
for( i=0; i<N+5; i++ )
	{
	y = ldexpl( f, k );
	if( y != x )
		{
		printf( "ldexpl(%.1Le, %d) = %.5Le, s.b. %.5Le\n",
			f, k, y, x );
		++errs;
		}
	z = frexpl( y, &e );
	if( (e != k+e0) || (z != z0)  )
		{
		printf( "frexpl(%.1Le) = %.5Le, %d; s.b. %.5Le, %d\n",
			y, z, e, z0, k+e0 );
		++errs;
		}
	x += x;
	if( x == last )
		break;
	last = x;
	k += 1;
	}
printf( "i = %d\n", k );
prnum( "last y =", &y );
printf("\n");

f = 0.1L;
x = f;
last = x;
k = 0;
for( i=0; i<N+64; i++ )
	{
	y = ldexpl( f, k );
	if( y != x )
		{
		printf( "ldexpl(%.1Le, %d) = %.5Le, s.b. %.5Le\n",
			f, k, y, x );
		++errs;
		}
	z = frexpl( y, &e );
	if(
#if 1
	   (e > -N+1) &&
#endif
	   ((e != k+e0) || (z != z0))  )
		{
		printf( "frexpl(%.1Le) = %.5Le, %d; s.b. %.5Le, %d\n",
			y, z, e, z0, k+e0 );
		++errs;
		}
	y1 = ldexpl( z, e );
	if( y1 != y )
		{
		printf( "ldexpl(%.1Le, %d) = %.5Le, s.b. %.5Le\n",
			z, e, y1, y );
		++errs;
		}

	x *= 0.5L;
	if( x == 0.0L )
	  break;
	if( x == last )
		break;
	last = x;
	k -= 1;
	}
printf( "i = %d\n", k );
prnum( "last y =", &y );

printf( "\n%d errors\n", errs );
exit(0);
}


void prnum(str, x)
char *str;
unsigned short *x;
{
int i;

printf( "%s ", str );
printf( "%.5Le = ", *(long double *)x );
for( i=0; i<5; i++ )
	printf( "%04x ", *x++ );
}
