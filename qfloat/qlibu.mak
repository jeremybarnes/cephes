# Unix makefile for libmq.a, qcalc, qccalc

# You MUST EDIT mconf.h to indicate your computer's
# endian-ness and (optionally) its floating point format.

# You MUST COPY either qhead16.h or qhead32.h to qhead.h,
# to match the integer array element size (16 or 32 bits)
# selected below. They are set up for 100-digit precision.
# Edit the 16-bit version to define the lower precision
# option if you want it.

# If you have the readline library (for command line history),
# use the references, below, to USE_READLINE, -lreadline, and -lncurses.

# Uncomment one of the following arithmetic configurations.

# 32-bit words, C language.
# qfltbi requires compiler support of 64-bit long long ints.
# Use the GNU C compiler.
# This configuration has been tested on DEC alpha, SPARC,
# i386 linux, DJGCC MS-DOS, SGI MIPS, m68k
#ARITH = qsqrt.o qflti.o qfltbi.o

# 32-bit integer array elements, i386 assembly language (GNU as)
# Fastest version for IBM PC's.
ARITH = qsqrt.o qflti.o qfltbelf.o
#ARITH = qsqrt.o qflti.o qfltb386.o

# 16-bit integer array elements, C language.
# This should work on most any computer.
#ARITH = qsqrt.o qflti.o qfltb.o

# 16-bit, C, strict rounding rules
# (But not denormal numbers.  Get ieeetest.zoo for that.)
# This is the slowest version, but also the most portable.
# It used to run on a PDP-11.
#ARITH = qsqrta.o qflti.o qflta.o
#DEFS = -DSTICKY=1

# 16-bit integer array, 68020 assembly language
# Fastest version for 68k.
#ARITH = qsqrt.o qflt.o qfltd.o

# 16-bit, 68000 assembly language, OS-9 operating system
#ARITH = qsqrt.o qflt.o qf68k.o

# 16-bit, 68000 assembly language, Motorola Unix operating system
#ARITH = qsqrt.o qflt.o qf68ks.o

# 16-bit, 8086 assembly language
# (Use the supplied Microsoft makefile for this.
# The option comes in both large and small memory models.)
# ARITH = qsqrt.o qflt.o subm.o divn.o mulr.o
# ARITH = qsqrt.o qflt.o subml.o divnl.o mulrl.o

CC= gcc
CFLAGS= -O2 -Wall
INCS= mconf.h qcalc.h qhead.h qtens.h


LIBOBJS= qacosh.o qairy.o qasin.o qasinh.o qatanh.o qatn.o qbeta.o \
qcbrt.o qcgamma.o qcmplx.o qconst.o qcos.o qcosh.o qdawsn.o \
qellie.o qellik.o qellpe.o qellpj.o qellpk.o qerf.o qerfc.o \
qeuclid.o qexp.o qexp10.o qexp2.o qexpn.o qfac.o \
qfresf.o qgamma.o qhy2f1.o qhyp.o qigam.o qigami.o qin.o \
qincb.o qincbi.o qine.o qjn.o qjypn.o qjyqn.o \
qk0.o qkn.o qkne.o qlog.o qlog1.o qlog10.o qndtr.o qndtri.o \
qpow.o qprob.o qpsi.o qrand.o qremain.o qshici.o qsici.o qsimq.o \
qsin.o qsindg.o qsinh.o qspenc.o qstudt.o qtan.o qtanh.o \
qyn.o qzetac.o qfloor.o mtherr.o $(ARITH)


all: libmq.a qcalc qccalc qmtst qparanoi

check: libmq.a qmtst qparanoi
	qparanoi > temp.tmp
	-diff temp.tmp qparanoi.exp
	cd cplus; make check
	qmtst > temp.tmp
	-diff temp.tmp qmtst.exp


libmq.a: $(LIBOBJS) $(INCS)
	rm -f libmq.a
	ar -rv libmq.a $(LIBOBJS)
	-ranlib libmq.a


qcalc: qcalc.o $(INCS) incbet.o incbi.o gamma.o igami.o igam.o \
log.o exp.o sin.o pow.o powi.o polevl.o ndtri.o sqrt.o floor.o \
const.o libmq.a
	$(CC) $(CFLAGS) -o qcalc qcalc.o incbet.o incbi.o ndtri.o \
gamma.o igami.o igam.o log.o exp.o sin.o pow.o powi.o polevl.o sqrt.o \
floor.o const.o \
libmq.a -lreadline -lncurses -lm
#libmq.a -lm

qcalc.o: qcalc.c
	$(CC) $(CFLAGS) -DUSE_READLINE=1 -c qcalc.c
#	$(CC) $(CFLAGS) -c qcalc.c

mtherr.o: mtherr.c $(INCS)

floor.o: floor.c $(INCS)

const.o: const.c

incbet.o: incbet.c

incbi.o: incbi.c

gamma.o: gamma.c

igami.o: igami.c

igam.o: igam.c

exp.o: exp.c

sin.o: sin.c

pow.o: pow.c

powi.o: powi.c

sqrt.o: sqrt.c



qccalc: qccalc.o cmplx.o sqrt.o floor.o const.o $(INCS) libmq.a
	$(CC) $(CFLAGS) -o qccalc qccalc.o cmplx.o sqrt.o floor.o const.o \
libmq.a -lreadline -lncurses -lm
#libmq.a -lm

qccalc.o: qccalc.c
	$(CC) $(CFLAGS) -DUSE_READLINE=1 -c qccalc.c
#	$(CC) $(CFLAGS) -c qccalc.c

cmplx.o: cmplx.c $(INCS)



qmtst: qmtst.o ndtri.o sqrt.o log.o polevl.o const.o \
drand.o floor.o $(INCS) libmq.a
	$(CC) $(CFLAGS) -o qmtst qmtst.o ndtri.o sqrt.o \
log.o polevl.o const.o floor.o drand.o libmq.a -lm

ndtri.o: ndtri.c $(INCS)

polevl.o: polevl.c $(INCS)

drand.o: drand.c $(INCS)

log.o: log.c



# Test the version of arithmetic that is correctly rounded.
# qflta.c works only with 16-bit wordsize.
#qparanoi: qparanoi.o qflt.o qflta.o qsqrta.o libmq.a
#	$(CC) $(CFLAGS) -o qparanoi qparanoi.o qflt.o qflta.o \
#qsqrta.o libmq.a -lm

# This will test the arithmetic that is actually in the library.
qparanoi: qparanoi.o libmq.a $(INCS)
	$(CC) -o qparanoi qparanoi.o libmq.a -lm

qflt.o: qflt.c $(INCS)
	$(CC) $(CFLAGS) -DSTICKY=1 -c qflt.c

qflta.o: qflta.c $(INCS)
	$(CC) $(CFLAGS) -c qflta.c

qflti.o: qflti.c $(INCS)
	$(CC) $(CFLAGS) $(DEFS) -c qflti.c

qparanoi.o: qparanoi.c $(INCS)
	$(CC) -O -Wno-implicit -c qparanoi.c
#	$(CC) -O -c qparanoi.c

qsqrta.o: qsqrta.c $(INCS)


# i386, coff version (DJGPP)
qfltb386.o: qfltbi.386
	as -o qfltb386.o qfltbi.386

# i386, ELF version (linux)
qfltbelf.o: qfltbelf.386
	as -o qfltbelf.o qfltbelf.386

qfltbi.o: qfltbi.c
	$(CC) $(CFLAGS) -c qfltbi.c

qf68k.o: qf68k.a
	as -o qf68k.o qf68k.a

clean:
	rm -f *.o
	rm -f libmq.a
	rm -f qcalc
	rm -f qccalc
	rm -f qparanoi
	rm -f qmtst
