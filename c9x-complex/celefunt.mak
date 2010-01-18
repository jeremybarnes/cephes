
# Makefile for W. J. Cody's tests of elementary complex functions
# transliterated from Fortran to C language.
# Fixes comprised proper naming of functions under test (e.g. FSIN for SIN)
# and revision of the test for `-FSQRT' in tcsqrt.c.

CC = gcc
#CC=/b/gnu/linux/gcc/xgcc -B/b/gnu/linux/gcc/ -I/b/gnu/linux/gcc/include

CFLAGS = -g -O1 -Wall -I.

INCS = celefunt.h complex.h stubs.o

# libmc.a is the complex function library under test.
# stubs.o is compiled with the machine parameter settings in mconf.h
#LIBS = stubs.o libmc.a -lm
# These are intended to get the cephes libraries
#LIBS = libmc.a ../libmd.a ../libmf.a
LIBS = libmc.a -lmd -lmf

all: tcabs tcexp tclog tcpow tcsin tcsqrt \
tcabsf tcexpf tclogf tcpowf tcsinf tcsqrtf

check: all
	-tcabs > tcabs.log
	-tcexp > tcexp.log
	-tclog > tclog.log
	-tcpow > tcpow.log
	-tcsin > tcsin.log
	-tcsqrt > tcsqrt.log
	-tcabsf > tcabsf.log
	-tcexpf > tcexpf.log
	-tclogf > tclogf.log
	-tcpowf > tcpowf.log
	-tcsinf > tcsinf.log
	-tcsqrtf > tcsqrtf.log

tcsqrt: tcsqrt.o $(INCS) libmc.a
	$(CC) -o tcsqrt tcsqrt.o $(LIBS)

tcsin: tcsin.o $(INCS) libmc.a
	$(CC) -o tcsin tcsin.o $(LIBS)

tcpow: tcpow.o $(INCS) libmc.a
	$(CC) -o tcpow tcpow.o $(LIBS)

tclog: tclog.o $(INCS) libmc.a
	$(CC) -o tclog tclog.o $(LIBS)

tcexp: tcexp.o $(INCS) libmc.a
	$(CC) -o tcexp tcexp.o $(LIBS)

tcabs: tcabs.o $(INCS) libmc.a
	$(CC) -o tcabs tcabs.o $(LIBS)

tcsqrtf: tcsqrtf.o $(INCS) libmc.a
	$(CC) -o tcsqrtf tcsqrtf.o $(LIBS)

tcsinf: tcsinf.o $(INCS) libmc.a
	$(CC) -o tcsinf tcsinf.o $(LIBS)

tcpowf: tcpowf.o $(INCS) libmc.a
	$(CC) -o tcpowf tcpowf.o $(LIBS)

tclogf: tclogf.o $(INCS) libmc.a
	$(CC) -o tclogf tclogf.o $(LIBS)

tcexpf: tcexpf.o $(INCS) libmc.a
	$(CC) -o tcexpf tcexpf.o $(LIBS)

tcabsf: tcabsf.o $(INCS) libmc.a
	$(CC) -o tcabsf tcabsf.o $(LIBS)

stubs.o: stubs.c
	$(CC) $(CFLAGS) -c stubs.c

tcabs.o: tcabs.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTDOUBLE -o tcabs.o -c tcabs.c

tcexp.o: tcexp.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTDOUBLE -o tcexp.o -c tcexp.c

tclog.o: tclog.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTDOUBLE -o tclog.o -c tclog.c

tcpow.o: tcpow.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTDOUBLE -o tcpow.o -c tcpow.c

tcsin.o: tcsin.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTDOUBLE -o tcsin.o -c tcsin.c

tcsqrt.o: tcsqrt.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTDOUBLE -o tcsqrt.o -c tcsqrt.c

tcabsf.o: tcabs.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTSINGLE -o tcabsf.o -c tcabs.c

tcexpf.o: tcexp.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTSINGLE -o tcexpf.o -c tcexp.c

tclogf.o: tclog.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTSINGLE -o tclogf.o -c tclog.c

tcpowf.o: tcpow.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTSINGLE -o tcpowf.o -c tcpow.c

tcsinf.o: tcsin.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTSINGLE -o tcsinf.o -c tcsin.c

tcsqrtf.o: tcsqrt.c tablat.c $(INCS)
	$(CC) $(CFLAGS) -DTSINGLE -o tcsqrtf.o -c tcsqrt.c


clean:
	rm -f tcabs
	rm -f tcexp
	rm -f tclog
	rm -f tcpow
	rm -f tcsin
	rm -f tcsqrt
	rm -f tcabsf
	rm -f tcexpf
	rm -f tclogf
	rm -f tcpowf
	rm -f tcsinf
	rm -f tcsqrtf
	rm -f tc*.o
	rm -f tc*.exe
	rm -f tc*.log
