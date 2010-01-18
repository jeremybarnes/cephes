/* remes.h */
/* Definition file for Remes program */
#define P 24 /* max total degree of polynomials, + 2 */
#define N 20 /* number of items to tabulate for display */

extern double PI; /* 3.14159... */

extern int IPS[];
extern double AA[];
extern double BB[];
extern double param[];
extern double xx[];
extern double mm[];
extern double yy[];
extern double step[];
extern double qx;
extern double qy;
extern double qyaprx;
extern double dev;
extern double apstrt;
extern double apwidt;
extern double apend;
extern double xm;
extern double xn;
extern double ym;
extern double yn;
extern double delta;
extern double eclose;
extern double farther;
extern double spread;
extern int esign;
extern int n;
extern int d;
extern int nd1;
extern int neq;
extern int relerr;
extern int search;
extern int iter;
extern int askitr;
extern char funnam[];
extern char znam[];
void remesa(), remese(), remesp();
void remess(), remesw(), stpini(), pconfig();
int simq();

/* flag bits for type of approximation: */
extern int config;
/* R(z^2) */
#define PXSQ 1
/* z R() */
#define XPX 2
/* Pade form with denominator Q(z) - zP(z) */
#define PADE 4
/* Cody & Waite form x + x^2 R(z)
 * note, if degree of denominator = 0 then set ZER bit also */
#define CW 8
/* squeeze toward low end of approximation interval */
#define SQL 16
/* search for zeros, even if no denominator */
#define ZER 32
/* z^2 R() */
#define X2PX 64
/* squeeze toward high end */
#define SQH 128
/* special modifications done by special() (see remesf.c) */
#define SPECIAL 256
/* truncate coefficients after each solution */
#define TRUNC 512
/* R(z^3) */
#define PXCU 1024
