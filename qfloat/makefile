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
ARITH = qsqrt.o qflti.o qfltbi.o

# 32-bit integer array elements, i386 assembly language (GNU as)
# Fastest version for IBM PC's.
#ARITH = qsqrt.o qflti.o qfltbelf.o
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
CFLAGS= -g -O2 -Wall -fno-builtin
INCS= mconf.h qcalc.h qhead.h qtens.h


LIBOBJS= qacosh.o qairy.o qasin.o qasinh.o qatanh.o qatn.o qbeta.o \
qcbrt.o qcgamma.o qcmplx.o qchyp1f1.o qcerf.o qconst.o qcos.o qcosh.o \
qcpolylog.o qdawsn.o qei.o qellie.o qellik.o qellpe.o qellpj.o qellpk.o \
qerf.o qerfc.o qeuclid.o qexp.o qexp10.o qexp2.o qexpm1.o qexpn.o qfac.o \
qfresf.o qgamma.o qhy2f1.o qhyp.o qigam.o qigami.o qin.o \
qincb.o qincbi.o qine.o qjn.o qjypn.o qjyqn.o \
qk0.o qkn.o qkne.o qlog.o qlog1.o qlog10.o qndtr.o qndtri.o qplanck.o \
qpolylog.o qpow.o qprob.o qpsi.o qrand.o qremain.o qremquo.o qshici.o qsici.o \
qsimq.o qsin.o qsindg.o qsinh.o qspenc.o qstudt.o qtan.o qtanh.o \
qyn.o qzetac.o qfloor.o mtherr.o $(ARITH)


all: libmq.a qcalc qccalc qmtst qparanoi qcwhitebx

check: libmq.a qmtst qparanoi
	-qparanoi > temp.tmp
	-diff temp.tmp qparanoi.exp
	qmtst > temp.tmp
	-diff temp.tmp qmtst.exp
# Test the C+ class
#	make -f cplus.mak

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
#libmq.a -lreadline -lm
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
#libmq.a -lreadline -lm
#libmq.a -lm

qccalc.o: qccalc.c
	$(CC) $(CFLAGS) -DUSE_READLINE=1 -c qccalc.c
#	$(CC) $(CFLAGS) -c qccalc.c

cmplx.o: cmplx.c $(INCS)



qmtst: qmtst.o ndtri.o sqrt.o log.o polevl.o const.o \
drand.o floor.o $(INCS) libmq.a
	$(CC) $(CFLAGS) -o qmtst qmtst.o ndtri.o sqrt.o \
log.o polevl.o const.o floor.o drand.o libmq.a -lm

qmtst.o: qmtst.c
ndtri.o: ndtri.c $(INCS)

polevl.o: polevl.c $(INCS)

drand.o: drand.c $(INCS)

log.o: log.c

qacosh.o: qacosh.c
qairy.o: qairy.c
qasin.o: qasin.c
qasinh.o: qasinh.c
qatanh.o: qatanh.c
qatn.o: qatn.c
qbeta.o: qbeta.c
qcbrt.o: qcbrt.c
qcgamma.o: qcgamma.c
qcmplx.o: qcmplx.c
qconst.o: qconst.c
qcos.o: qcos.c
qcosh.o: qcosh.c
qdawsn.o: qdawsn.c
qellie.o: qellie.c
qellik.o: qellik.c
qellpe.o: qellpe.c
qellpj.o: qellpj.c
qellpk.o: qellpk.c
qerf.o: qerf.c
qerfc.o: qerfc.c
qeuclid.o: qeuclid.c
qexp.o: qexp.c
qexp10.o: qexp10.c
qexp2.o: qexp2.c
qexpn.o: qexpn.c
qfac.o: qfac.c
qfresf.o: qfresf.c
qgamma.o: qgamma.c
qhy2f1.o: qhy2f1.c
qhyp.o: qhyp.c
qigam.o: qigam.c
qigami.o:qigami.c
qin.o: qin.c
qincb.o: qincb.c
qincbi.o:qincbi.c
qine.o: qine.c
qjn.o: qjn.c
qjypn.o: qjypn.c
qjyqn.o: qjyqn.c
qk0.o: qk0.c
qkn.o: qkn.c
qkne.o: qkne.c
qlog.o: qlog.c
qlog1.o: qlog1.c
qlog10.o: qlog10.c
qndtr.o: qndtr.c
qndtri.o: qndtri.c
qpow.o: qpow.c
qprob.o: qprob.c
qpsi.o: qpsi.c
qrand.o: qrand.c
qremain.o: qremain.c
qshici.o: qshici.c
qsici.o: qsici.c
qsimq.o: qsimq.c
qsin.o: qsin.c
qsindg.o: qsindg.c
qsinh.o: qsinh.c
qspenc.o: qspenc.c
qstudt.o: qstudt.c
qtan.o: qtan.c
qtanh.o: qtanh.c
qyn.o: qyn.c
qzetac.o: qzetac.c
qfloor.o: qfloor.c
mtherr.o: mtherr.c
qsqrt.o: qsqrt.c


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
	$(CC) -O -Wno-implicit -fno-builtin -c qparanoi.c
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

# White box test vectors
qcwhitebx: qcwhitebx.c libmq.a
	$(CC) $(CFLAGS) -o qcwhitebx qcwhitebx.c libmq.a -lm

clean:
	rm -f *.o
	rm -f libmq.a
	rm -f qcalc
	rm -f qccalc
	rm -f qparanoi
	rm -f qmtst
