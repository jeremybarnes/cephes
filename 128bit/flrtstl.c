long double floorl(), ldexpl(), frexpl();

main()
{
long double x, f, y, last, z, z0;
int i, k, e, e0, errs;

errs = 0;
f = 0.1L;
x = f;
last = x;
z0 = frexpl( x, &e0 );
printf( "frexpl(%.2e) = %.5e, %d\n", (double)x, (double)z0, e0 );
k = 0;
for( i=0; i<1029; i++ )
	{
	y = ldexpl( f, k );
	if( y != x )
		{
		printf( "ldexpl(%.1e, %d) = %.5e, s.b. %.5e\n",
			(double)f, k, (double)y, (double)x );
		++errs;
		}
	z = frexpl( y, &e );
	if( (e != k+e0) || (z != z0)  )
		{
		printf( "frexpl(%.1e) = %.5e, %d; s.b. %.5e, %d\n",
			(double)y, (double)z, e, (double)z0, k+e0 );
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

f = 0.1L;
x = f;
last = x;
k = 0;
for( i=0; i<1074; i++ )
	{
	y = ldexpl( f, k );
	if( y != x )
		{
		printf( "ldexpl(%.1e, %d) = %.5e, s.b. %.5e\n",
			(double)f, k, (double)y, (double)x );
		++errs;
		}
	z = frexpl( y, &e );
	if( (e > -1023)
	 && ((e != k+e0) || (z != z0))  )
		{
		printf( "frexpl(%.1e) = %.5e, %d; s.b. %.5e, %d\n",
			(double)y, (double)z, e, (double)z0, k+e0 );
		++errs;
		}
	x *= 0.5L;
	if( x == last )
		break;
	last = x;
	k -= 1;
	}
printf( "i = %d\n", k );
prnum( "last y =", &y );

printf( "%d errors\n", errs );
}


prnum(str, x)
char *str;
unsigned short *x;
{
int i;

printf( "%s ", str );
printf( "%.5e\n", (double)(*(long double *)x) );
for( i=0; i<4; i++ )
	printf( "%04x ", *x++ );
}
