//							qfloat.h
//
//      C++ class for q-type floating point arithmetic.
//
// SYNOPSIS:
//
// #include "qfloat.h"
//
// qfloat a, b, c;
// a = b * c - 1.5;
//
// DESCRIPTION:
//
// Provides C++ arithmetic operators, functions, and type conversions
// for Q type floating point arithmetic.  Programs must be linked
// with the library libmq.a of supporting Q type routines.
//
// Using the GNU GCC compiler, the -O3 switch enables inlining which
// generates fairly efficient code for the various function calls.
// This class reportedly works with Microsoft and Borland C++ also.
//
// On a Sun sparcstation, references to long double type should use
// the e113 functions; IBM PC uses e64.  On machines that have no
// extended precision long double, just use etoq, qtoe. 
//
// Cephes Math Library Release 2.4:  April, 1996
// Copyright 1996 by Stephen L. Moshier

#ifndef __QFLOAT_H
#define __QFLOAT_H

// Be sure the declaration below matches QELT and NQ from qhead.h.
// #include "qhead.h"

struct qfloatstruct
  {
//    QELT ar[NQ];
    unsigned int ar[14];
  };

extern "C"
{
  void qclear (qfloatstruct *);
  void e24toq (const float *, qfloatstruct *);
  void etoq (const double *, qfloatstruct *);
  void e64toq (const long double *, qfloatstruct *);
  void e113toq (const long double *, qfloatstruct *);
  void ltoq (long int *, qfloatstruct *);
  void asctoq (const char *, qfloatstruct *);
}

class qfloat
{
  public:
  struct qfloatstruct a;
// Constructor.
  qfloat () {};
//  qfloat () {qclear(&a);};
// Conversions from base classes to qfloat.
  qfloat (float x) {e24toq (&x, &a);}
  qfloat (double x) {etoq (&x, &a);}
//  qfloat (long double x) {etoq (&x, &a);}
  qfloat (long double x) {e64toq (&x, &a);}
//  qfloat (long double x) {e113toq (&x, &a);}
  qfloat (long int x) {ltoq (&x, &a);}
  qfloat (int x) {long lx = x; ltoq (&lx, &a);}
// For conversion and assignment, e.g.,  x = "1.234e4321"
  qfloat (char *x) {asctoq (x, &a);}
// Type conversion to a base class is not possible in C++,
// so you have to use these functions instead, e.g. int i = qtoi(q);
  friend int qtoi (qfloat);
  friend long qtol (qfloat);
  friend float qtof (qfloat);
  friend double qtod (qfloat);
  friend long double qtold (qfloat);
};

extern "C"
{
  void qadd (const qfloat &, const qfloat &, qfloat &);
  void qsub (const qfloat &, const qfloat &, qfloat &);
  void qmul (const qfloat &, const qfloat &, qfloat &);
  void qdiv (const qfloat &, const qfloat &, qfloat &);
  int qcmp (const qfloat &, const qfloat &);
  void qtoasc (const qfloat *, char *, const int);
  void qtoe24 (const qfloat *, float *);
  void qtoe (const qfloat *, double *);
  void qtoe64 (const qfloat *, long double *);
  void qtoe113 (const qfloat *, long double *);
  void qifrac (const qfloat *, long *, qfloat *);
  void qabs (qfloat *);
  void qneg (qfloat *);
  void qsqrt (const qfloat *, qfloat *);
  int mtherr (char *, int);
  void qfloor (const qfloat *, qfloat *);
  void qpow (const qfloat *, const qfloat *, qfloat *);
  void qexp (const qfloat *, qfloat *);
  void qlog (const qfloat *, qfloat *);
  void qsin (const qfloat *, qfloat *);
  void qcos (const qfloat *, qfloat *);
  void qremain (const qfloat *, const qfloat *, qfloat *);
}


// Conversion to base class.

inline int qtoi (qfloat x)
{
  qfloat y;
  long l;
  qifrac(&x, &l, &y);
  return (int) l;
}

inline long qtol (qfloat x) {qfloat y; long l; qifrac(&x, &l, &y); return l;}

inline float qtof (qfloat x) {float f; qtoe24(&x, &f); return f;}

inline double qtod (qfloat x) {double d; qtoe(&x, &d); return d;}

//inline long double qtold (qfloat x) {long double d; qtoe(&x, &d); return d;}
inline long double qtold (qfloat x) {long double d; qtoe64(&x, &d); return d;}
//inline long double qtold (qfloat x) {long double d; qtoe113(&x, &d); return d;}


// Arithmetic operators.

inline void operator += (qfloat & x, const qfloat & y)
{
  qadd (x, y, x);
}

inline qfloat operator + (const qfloat & x, const qfloat & y)
{
  qfloat z;
  qadd (x, y, z);
  return z;
}

inline qfloat operator - (const qfloat & x, const qfloat & y)
{
  qfloat z;
  qsub (y, x, z);
  return z;
}

// Unary negation.
inline qfloat operator - (const qfloat & x)
{
  qfloat z;
  z = x;
  qneg (&z);
  return z;
}

inline void operator -= (qfloat & x, const qfloat & y)
{
  qsub (y, x, x);
}

inline qfloat operator * (const qfloat & x, const qfloat & y)
{
  qfloat z;
  qmul (x, y, z);
  return z;
}

inline void operator *= (qfloat & x, const qfloat & y)
{
  qmul (y, x, x);
}

inline qfloat operator / (const qfloat & x, const qfloat & y)
{
  qfloat z;
  qdiv (y, x, z);
  return z;
}

inline void operator /= (qfloat & x, const qfloat & y)
{
  qdiv (y, x, x);
}


// Comparisons.

inline int operator == (const qfloat & x, const qfloat & y)
{
  return (qcmp (x, y) == 0);
}

inline int operator != (const qfloat & x, const qfloat & y)
{
  return (qcmp (x, y) != 0);
}

inline int operator < (const qfloat & x, const qfloat & y)
{
  return (qcmp (x, y) == -1);
}

inline int operator > (const qfloat & x, const qfloat & y)
{
  return (qcmp (x, y) == 1);
}

inline int operator >= (const qfloat & x, const qfloat & y)
{
  return (qcmp (x, y) >= 0);
}

inline int operator <= (const qfloat & x, const qfloat & y)
{
  return (qcmp (x, y) <= 0);
}


// Overloaded functions.
// Define other function calls analogously.

inline qfloat sqrt (const qfloat & x)
{
  qfloat y;
  qsqrt (&x, &y);
  return y;
}

inline qfloat floor (const qfloat &x)
{
  qfloat y;
  qfloor (&x, &y);
  return y;
}

// Exact remainder of x/y.
inline qfloat fmod (const qfloat & x, const qfloat & y)
{
  qfloat z;
  qremain (&y, &x, &z);
  return z;
}

inline qfloat fabs (const qfloat & x)
{
  qfloat y;
  y = x;
  qabs (&y);
  return y;
}

// inline qfloat abs (const qfloat & x)
// {
//   qfloat y;
//   y = x;
//   qabs (&y);
//   return y;
// }

inline qfloat pow (const qfloat & x, const qfloat & y)
{
  qfloat z;
  qpow (&x, &y, &z);
  return z;
}

inline qfloat exp (const qfloat & x)
{
  qfloat y;
  qexp (&x, &y);
  return y;
}

inline qfloat log (const qfloat & x)
{
  qfloat y;
  qlog (&x, &y);
  return y;
}

inline qfloat sin (const qfloat & x)
{
  qfloat y;
  qsin (&x, &y);
  return y;
}

inline qfloat cos (const qfloat & x)
{
  qfloat y;
  qcos (&x, &y);
  return y;
}

// Print decimal value of X with N digits.  Precede value with string s1,
// follow with string s2.
#include <stdio.h>
inline void qprint ( const int n, const char *s1, const qfloat &x,
		     const char *s2)
{
  char str[30];
  qtoasc (&x, str, n);
  printf ("%s %s %s", s1, str, s2);
}

#endif /* __QFLOAT_H */
