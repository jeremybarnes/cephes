# Unix make file for long double precision function library.
# Be sure to check the need to pad long double constants (XPD),
# and the type of computer, in mconf.h.
# 
# Use $(OBJS) in ar command for libml.a if possible; else *.o
CC= gcc
CFLAGS= -O2 -g -Wall

OBJS= acoshl.o asinhl.o asinl.o atanhl.o atanl.o bdtrl.o btdtrl.o cbrtl.o \
chdtrl.o coshl.o ellpel.o ellpkl.o elliel.o ellikl.o ellpjl.o \
exp10l.o exp2l.o expl.o fdtrl.o gammal.o gdtrl.o igamil.o igaml.o \
incbetl.o incbil.o isnanl.o j0l.o j1l.o jnl.o ldrand.o log10l.o \
log2l.o logl.o nbdtrl.o ndtril.o ndtrl.o pdtrl.o powl.o powil.o \
sinhl.o sinl.o sqrtl.o stdtrl.o tanhl.o tanl.o unityl.o ynl.o \
floorl.o polevll.o unityl.o expm1l.o mtherr.o setprec.o

all: mtstl fltestl testvect lcalc libml.a

mtstl: libml.a mtstl.o $(OBJS)
	$(CC) $(CFLAGS) -o mtstl mtstl.o libml.a
#	coff2exe mtstl

libml.a: $(OBJS) mconf.h
	ar -rv libml.a $(OBJS)
	ranlib libml.a

testvect: testvect.o libml.a
	$(CC) $(CFLAGS) -o testvect testvect.o libml.a

testvect.o: testvect.c
	$(CC) -g -c -o testvect.o testvect.c

lcalc: libml.a lcalc.o ieee.o econst.o $(OBJS)
	$(CC) $(CFLAGS) -o lcalc lcalc.o ieee.o econst.o libml.a $(LIBS)

lcalc.o: lcalc.c lcalc.h ehead.h

lparanoi: libml.a lparanoi.o setprec.o ieee.o econst.o $(OBJS)
	$(CC) $(CFLAGS) -o lparanoi lparanoi.o setprec.o ieee.o econst.o libml.a
 $(LIBS)

ieee.o: ieee.c ehead.h
econst.o: econst.c ehead.h

# Use assembly language sqrt and floor if available.
# sqrtl.c does not give strictly rounded results.
# The floor.c routines are slow.
#floorl.o: floorl.387
#	as -o floorl.o floorl.387
#sqrtl.o: sqrtl.387
#	as -o sqrtl.o sqrtl.387
#setprec.o: setprec.387
#	as -o setprec.o setprec.387

# ELF versions for linux
floorl.o: floorlelf.387
	as -o floorl.o floorlelf.387
sqrtl.o: sqrtlelf.387
	as -o sqrtl.o sqrtlelf.387
setprec.o: setprelf.387
	as -o setprec.o setprelf.387

# 68K routines
# For Sun 3
#floorl.o: floorl.sun
#	as -o floorl.o floorl.sun
#sqrtl.o: sqrtl.sun
#	as -o sqrtl.o sqrtl.sun
#setprec.o: setprec.688
#	as -o setprec.o setprec.688
# For Motorola 68k sysv
# Thanks to Philippe De Muyter <phdm@info.ucl.ac.be>
#floorl.o: floorl.mot
#	as -o floorl.o floorl.mot
#setprec.o: setprec.mot
#	as -o setprec.o setprec.mot
#sqrtl.o: sqrtl.mot
#	as -o sqrtl.o sqrtl.mot

fltestl: fltestl.c libml.a
	$(CC) $(CFLAGS) -o fltestl fltestl.c libml.a

clean:
	rm -f *.o
	rm -f mtstl
	rm -f lcalc
	rm -f fltestl
	rm -f testvect
	rm -f libml.a

# Run test programs
check: mtstl fltestl testvect libml.a
	-mtstl
	-fltestl
	-testvect
