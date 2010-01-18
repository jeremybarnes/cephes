/* fltest.c
 * Test program for floor(), frexp(), ldexp()
 */

/*
Cephes Math Library Release 2.1:  December, 1988
Copyright 1984, 1987, 1988 by Stephen L. Moshier (moshier@world.std.com)
*/



/*#include "mconf.h"*/
#define MACHEPL  5.42101086242752217003726400434970855712890625E-20L
#define N 16300

void flierr();
int printf();
void exit();

int
main()
{
long double x, y, y0, z, f, x00, y00;
int i, j, e, e0;
int errfr, errld, errfl, underexp, err, errth, e00;
long double frexpl(), ldexpl(), floorl();


/*
if( 1 )
	goto flrtst;
*/

printf( "Testing frexpl() and ldexpl().\n" );
errth = 0.0L;
errfr = 0;
errld = 0;
underexp = 0;
f = 1.0L;
x00 = 2.0L;
y00 = 0.5L;
e00 = 2;

for( j=0; j<20; j++ )
{
if( j == 10 )
	{
	f = 1.0L;
	x00 = 2.0L;
	e00 = 1;
/* Find 2**(2**14) / 2 */
	for( i=0; i<13; i++ )
		{
		x00 *= x00;
		e00 += e00;
		}
	y00 = x00/2.0L;
	x00 = x00 * y00;
	e00 += e00;
	y00 = 0.5L;
	}
x = x00 * f;
y0 = y00 * f;
e0 = e00;

#if 1
/* If ldexp, frexp support denormal numbers, this should work.  */
for( i=0; i<16448; i++ )
#else
for( i=0; i<16383; i++ )
#endif
	{
	x /= 2.0L;
	e0 -= 1;
	if( x == 0.0L )
		{
		if( f == 1.0L )
			underexp = e0;
		y0 = 0.0L;
		e0 = 0;
		}
	y = frexpl( x, &e );
	if( (e0 < -16383) && (e != e0) )
		{
		if( e == (e0 - 1) )
			{
			e += 1;
			y /= 2.0L;
			}
		if( e == (e0 + 1) )
			{
			e -= 1;
			y *= 2.0L;
			}
		}
	err = y - y0;
	if( y0 != 0.0L )
		err /= y0;
	if( err < 0.0L )
		err = -err;
	if( e0 > -1023 )
		errth = 0.0L;
	else
		{/* Denormal numbers may have rounding errors */
		if( e0 == -16383 )
			{
			errth = 2.0L * MACHEPL;
			}
		else
			{
			errth *= 2.0L;
			}
		}

	if( (x != 0.0L) && ((err > errth) || (e != e0)) )
		{
		printf( "Test %d: ", j+1 );
		printf( " frexpl( %.20Le) =?= %.20Le * 2**%d;", x, y, e );
		printf( " should be %.20Le * 2**%d\n", y0, e0 );
		errfr += 1;
		}
	y = ldexpl( x, 1-e0 );
	err = y - 1.0L;
	if( err < 0.0L )
		err = -err;
	if( (err > errth) && ((x == 0.0L) && (y != 0.0L)) )
		{
		printf( "Test %d: ", j+1 );
		printf( "ldexpl( %.15Le, %d ) =?= %.15Le;", x, 1-e0, y );
		if( x != 0.0L )
			printf( " should be %.15Le\n", f );
		else
			printf( " should be %.15Le\n", 0.0L );
		errld += 1;
		}
	if( x == 0.0L )
		{
		break;
		}
	}
f = f * 1.08005973889L;
}

if( (errld == 0) && (errfr == 0) )
	{
	printf( "No errors found.\n" );
	}

/*flrtst:*/

printf( "Testing floorl().\n" );
errfl = 0;

f = 1.0L/MACHEPL;
x00 = 1.0L;
for( j=0; j<57; j++ )
{
x = x00 - 1.0L;
for( i=0; i<128; i++ )
	{
	y = floorl(x);
	if( y != x )
		{
		flierr( x, y, j );
		errfl += 1;
		}
/* Warning! the if() statement is compiler dependent,
 * since x-0.49 may be held in extra precision accumulator
 * so would never compare equal to x!  The subroutine call
 * y = floor() forces z to be stored as a double and reloaded
 * for the if() statement.
 */
	z = x - 0.49L;
	y = floorl(z);
	if( z == x )
		break;
	if( y != (x - 1.0L) )
		{
		flierr( z, y, j );
		errfl += 1;
		}

	z = x + 0.49L;
	y = floorl(z);
	if( z != x )
		{
		if( y != x )
			{
			flierr( z, y, j );
			errfl += 1;
			}
		}
	x = -x;
	y = floorl(x);
	if( z != x )
		{
		if( y != x )
			{
			flierr( x, y, j );
			errfl += 1;
			}
		}
	z = x + 0.49L;
	y = floorl(z);
	if( z != x )
		{
		if( y != x )
			{
			flierr( z, y, j );
			errfl += 1;
			}
		}
	z = x - 0.49L;
	y = floorl(z);
	if( z != x )
		{
		if( y != (x - 1.0L) )
			{
			flierr( z, y, j );
			errfl += 1;
			}
		}
	x = -x;
	x += 1.0L;
	}
x00 = x00 + x00;
}
y = floorl(0.0L);
if( y != 0.0L )
	{
	flierr( 0.0L, y, 57 );
	errfl += 1;
	}
y = floorl(-0.0L);
if( y != 0.0L )
	{
	flierr( -0.0L, y, 58 );
	errfl += 1;
	}
y = floorl(-1.0L);
if( y != -1.0L )
	{
	flierr( -1.0L, y, 59 );
	errfl += 1;
	}
y = floorl(-0.1L);
if( y != -1.0l )
	{
	flierr( -0.1L, y, 60 );
	errfl += 1;
	}

if( errfl == 0 )
	printf( "No errors found in floorl().\n" );
exit(0);
return 0;
}

void flierr( x, y, k )
long double x, y;
int k;
{
printf( "Test %d: ", k+1 );
printf( "floorl(%.15Le) =?= %.15Le\n", x, y );
}
