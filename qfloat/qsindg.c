/*	qsindg.c
 * sin, cos, tan in degrees
 */

#include "qhead.h"

#if WORDSIZE == 32
/* 1.745329251994329576923690768488612713442871888541725456E-2 */
static QELT pi180[NQ] = {
0,EXPONE-6,0,0x8efa3512,0x94e9c8ae,0x0ec5f66e,0x9485c4d9,0x00b7aef5,
0x01b5e6b8,0xe502a9b4,0xc94c8512,0xb6f61167,0x81911487,0x10c50c97};
#else
static short pi180[NQ] = {
0000000,EXPONE-6,0000000,0107372,0032422,0112351,
0144256,0007305,0173156,0112205,0142331,0000270};
#endif

int qtan();

int qsindg( x, y )
QELT x[], y[];
{
QELT w[NQ];

qmul( x, pi180, w );
qsin( w, y );
return 0;
}


int qcosdg( x, y )
QELT x[], y[];
{
QELT w[NQ];

qmul( x, pi180, w );
qcos( w, y );
return 0;
}


int qtandg( x, y )
QELT x[], y[];
{
QELT w[NQ];

qmul( x, pi180, w );
qtan( w, y );
return 0;
}

