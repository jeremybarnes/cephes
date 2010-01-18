/* remesg.c */
/* Global symbol declarations for Remes program */

#include "remes.h"

int IPS[P] = {0,};         /* simq() work vector */
double AA[P*P] = {0.0,};   /* coefficient matrix */
double BB[P] = {0.0,};     /* right hand side vector */
double param[P] = {0.0,};  /* solution vector */
double xx[P] = {0.0,};     /* points in approximation interval */
double mm[P] = {0.0};      /* points of maximum deviation */
double yy[P] = {0.0};      /* the error maxima */
double step[P] = {0.0};    /* step sizes for searching */
double qx = 0.0;           /* function argument */
double qy = 0.0;           /* accurate function value */
double qyaprx = 0.0;       /* approximate function value */
double dev = 0.0;          /* deviation at solution points */
double apstrt = 0.0;       /* start of approximation interval */
double apwidt = 0.0;       /* width of interval */
double apend = 0.0;        /* end of interval */
double xm = 0.0;           /* variables for search of maximum */
double xn = 0.0;
double ym = 0.0;
double yn = 0.0;
double delta = 0.0;
double eclose = 0.0;
double farther = 0.0;
double spread = 0.0;      /* error spread from last iteration */
int esign = 0;            /* sign of error returned by geterr */
int n = 0;                /* degree of numerator polynomial */
int d = 0;                /* degree of denominator polynomial */
int nd1 = 0;              /* n + d + 1 */
int neq = 0;              /* number of equations to solve */
int relerr = 0;           /* relative error criterion flag */
int search = 0;           /* flag for automatic search */
int iter = 0;             /* search-solve iteration number */
int askitr = 0;           /* iteration on which to stop */
