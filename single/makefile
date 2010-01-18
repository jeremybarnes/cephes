# make file for single precision library
CC= gcc
#CC = /a/gnu/linux/gcc/xgcc -B/a/gnu/linux/gcc/ -I/a/gnu/linux/gcc/include
CFLAGS= -O2 -Wall -fno-builtin
INCS= mconf.h
OBJS= acoshf.o airyf.o asinf.o asinhf.o atanf.o \
atanhf.o bdtrf.o betaf.o cbrtf.o chbevlf.o chdtrf.o \
clogf.o cmplxf.o constf.o coshf.o dawsnf.o ellief.o \
ellikf.o ellpef.o ellpkf.o ellpjf.o expf.o exp2f.o \
exp10f.o expnf.o expx2f.o facf.o fdtrf.o floorf.o fresnlf.o \
gammaf.o gdtrf.o hypergf.o hyp2f1f.o igamf.o igamif.o \
incbetf.o incbif.o i0f.o i1f.o ivf.o j0f.o j1f.o \
jnf.o jvf.o k0f.o k1f.o knf.o logf.o log2f.o \
log10f.o nbdtrf.o ndtrf.o ndtrif.o pdtrf.o polynf.o \
powif.o powf.o psif.o rgammaf.o shichif.o sicif.o \
sindgf.o sinf.o sinhf.o spencef.o sqrtf.o stdtrf.o \
struvef.o tandgf.o tanf.o tanhf.o ynf.o zetaf.o \
zetacf.o polevlf.o setprec.o mtherr.o

all: libmf.a mtstf

mtstf: mtstf.o drand.o libmf.a
	$(CC) -o mtstf mtstf.o drand.o libmf.a

mtstf.o: mtstf.c
	$(CC) $(CFLAGS) -O0 -g -c mtstf.c

drand.o: drand.c
	$(CC) -O -g -c drand.c

libmf.a: $(OBJS) $(INCS)
	ar rv libmf.a $(OBJS)
	ranlib libmf.a

# IBM PC:
#setprec.o: setprec.387
#	$(AS) -o setprec.o setprec.387

# ELF versions for linux (no underscores)
#setprec.o: setprelf.387
#	$(AS) -o setprec.o setprelf.387

clean:
	rm -f *.o
	rm -f libmf.a
	rm -f mtstf
