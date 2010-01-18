# Makefile for 128-bit long double precision calculator
# and test programs.
CC= gcc
CFLAGS = -msoft-quad-float -fno-builtin -g -O2 -Wall
#CFLAGS = -mhard-quad-float -fno-builtin -g -O2 -Wall
INCS= mconf.h

OBJS= cbrtll.o expll.o expm1ll.o logll.o tanhll.o powll.o \
powill.o sqrtll.o sinhll.o asinhll.o coshll.o acoshll.o \
sinll.o tanll.o atanll.o asinll.o atanhll.o log10ll.o exp10ll.o \
log2ll.o exp2ll.o log1pll.o j0ll.o j1ll.o jnll.o lgammall.o ndtrll.o \
expx2ll.o floorll.o polevll.o constll.o mtherr.o
IEEOBJS= ieee.o econst.o

all: lcalc mtstl lparanoi monotll libml.a

lcalc: lcalc.o $(OBJS) $(IEEOBJS) $(INCS)
	$(CC) -v -o lcalc lcalc.o $(OBJS) $(IEEOBJS)

mtstl: mtstl.o drand.o $(OBJS) $(INCS)
	$(CC) -o mtstl mtstl.o drand.o $(OBJS)

lparanoi: lparanoi.o $(OBJS) $(IEEOBJS) $(INCS)
	$(CC) -o lparanoi lparanoi.o $(OBJS) $(IEEOBJS)

monotll: monotll.o $(OBJS)
	$(CC) -o monotll monotll.o $(OBJS)

libml.a: $(OBJS) $(INCS)
	ar rv libml.a $(OBJS)
	ranlib libml.a

sqrtll.o: sqrtll.spa
	as -o sqrtll.o sqrtll.spa

clean:
	rm -f *.o
	rm -f libml.a
	rm -f lparanoi
	rm -f mtstl
	rm -f lcalc

