#include <stdio.h>
#include "qfloat.h"
qfloat w, x, y, z;
char str[80];

int
main()
{
int i;

/*
 asctoq( "1.4", &x );
 asctoq( "1.5", &y );
*/
  x = "1.4";
  y = "1.5";

 z = x + y;
 qtoasc( &z, str, 10 );
 printf("1.4 + 1.5 = %s\n", str);

/* i = x + y; */
/*
I = z;
 i = *(int *) &I;
 printf("int value = %d\n", i);
*/

 z = x - y;
 qtoasc( &z, str, 10 );
 printf("1.4 - 1.5 = %s\n", str);


 w = -z;
 qtoasc( &z, str, 10 );
 printf("-(%s) = ", str);
 qtoasc( &w, str, 10 );
 printf("%s\n", str);

 w = fabs (z);
 qtoasc( &z, str, 10 );
 printf("fabs(%s) = ", str);
 qtoasc( &w, str, 10 );
 printf("%s\n", str);

 z = x * y;
 qtoasc( &z, str, 10 );
 printf("1.4 * 1.5 = %s\n", str);

 z = x / y;
 qtoasc( &z, str, 10 );
 printf("1.4 / 1.5 = %s\n", str);

 z = x;
 z += y;
 qtoasc( &z, str, 10 );
 printf("1.4 += 1.5 = %s\n", str);

 z = y;
 z -= x;
 qtoasc( &z, str, 10 );
 printf("1.5 -= 1.4 = %s\n", str);

 y = x;
 z = x + y;
 qtoasc( &z, str, 10 );
 printf("1.4 + 1.4 = %s\n", str);

 z = 1.9;
 x = z;
 qtoasc( &z, str, 10 );
 printf("double 1.9 = %s\n", str);

 if( x == z )
   printf("1.9 == 1.9\n");
 else
   printf("ERROR: 1.9 != 1.9\n");

 qtoasc(&y, str, 10);
 printf("%s ", str);
 if( y != z )
   printf("!= ");
 else
   printf("== ");
 qtoasc(&z, str, 10);
 printf("%s\n", str);

 qtoasc(&y, str, 10);
 printf("%s ", str);
 if( y < z )
   printf("< ");
 else
   printf("not < ");
 qtoasc(&z, str, 10);
 printf("%s\n", str);

 qtoasc(&z, str, 10);
 printf("%s ", str);
 if( z > y )
   printf("> ");
 else
   printf("not > ");
 qtoasc(&y, str, 10);
 printf("%s\n", str);

 z = "1.0e-4000";
 qtoasc(&z, str, 10);
 printf("\"1.0E-4000\" = %s\n", str);
 
 i = sizeof(qfloat);
 printf("sizeof qfloat = %d\n", i);
}

int mtherr (char *prog, int code)
{
printf("%s code %d\n", prog, code);
return 0;
}


