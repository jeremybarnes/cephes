# Microsoft Visual C makefile for extended precision functions
# libmq.lib, qcalc.exe, qccalc.exe, qmtst.exe, qparanoi.exe
# Run from command line in MSDOS window:
#   \Program Files\Microsoft Visual Studio\VC98\BIN\vcvars32.bat
# Then run
#   nmake -f msvc.mak
# This will generate the library libmq.lib and several .exe programs.

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
ARITH = qsqrt.obj qflti.obj qfltbi.obj

# 32-bit integer array elements, i386 assembly language (GNU as)
# Fastest version for IBM PC's.
#ARITH = qsqrt.obj qflti.obj qfltbelf.obj
#ARITH = qsqrt.obj qflti.obj qfltb386.obj

# 16-bit integer array elements, C language.
# This should work on most any computer.
#ARITH = qsqrt.obj qflti.obj qfltb.obj

# 16-bit, C, strict rounding rules
# (But not denormal numbers.  Get ieeetest.zoo for that.)
# This is the slowest version, but also the most portable.
# It used to run on a PDP-11.
#ARITH = qsqrta.obj qflti.obj qflta.obj
#DEFS = -DSTICKY=1

# 16-bit integer array, 68020 assembly language
# Fastest version for 68k.
#ARITH = qsqrt.obj qflt.obj qfltd.obj

# 16-bit, 68000 assembly language, OS-9 operating system
#ARITH = qsqrt.obj qflt.obj qf68k.obj

# 16-bit, 68000 assembly language, Motorola Unix operating system
#ARITH = qsqrt.obj qflt.obj qf68ks.obj

# 16-bit, 8086 assembly language
# (Use the supplied Microsoft makefile for this.
# The option comes in both large and small memory models.)
# ARITH = qsqrt.obj qflt.obj subm.obj divn.obj mulr.obj
# ARITH = qsqrt.obj qflt.obj subml.obj divnl.obj mulrl.obj

CC= cl
CFLAGS= /c
INCS= mconf.h qcalc.h qhead.h qtens.h


LIBOBJS= qacosh.obj qairy.obj qasin.obj qasinh.obj qatanh.obj qatn.obj qbeta.obj \
qcgamma.obj qcerf.obj qcgamma.obj qcmplx.obj qchyp1f1.obj qcpolylog.obj \
qcbrt.obj qconst.obj qcos.obj qcosh.obj qdawsn.obj qei.obj \
qellie.obj qellik.obj qellpe.obj qellpj.obj qellpk.obj qerf.obj qerfc.obj \
qeuclid.obj qexp.obj qexp10.obj qexp2.obj qexpm1.obj qexpn.obj qfac.obj \
qfresf.obj qgamma.obj qhy2f1.obj qhyp.obj qigam.obj qigami.obj qin.obj \
qincb.obj qincbi.obj qine.obj qjn.obj qjypn.obj qjyqn.obj qpolylog.obj \
qk0.obj qkn.obj qkne.obj qlog.obj qlog1.obj qlog10.obj qndtr.obj qndtri.obj \
qpow.obj qprob.obj qpsi.obj qrand.obj qremain.obj qshici.obj qsici.obj qsimq.obj \
qsin.obj qsindg.obj qsinh.obj qspenc.obj qstudt.obj qtan.obj qtanh.obj \
qremquo.obj qplanck.obj \
qyn.obj qzetac.obj qfloor.obj mtherr.obj $(ARITH)


all: libmq.lib qcalc.exe qccalc.exe qmtst.exe qparanoi.exe

check: libmq.lib qmtst qparanoi
	qparanoi > temp.tmp
	-diff temp.tmp qparanoi.exp
	cd cplus; make check
	qmtst > temp.tmp
	-diff temp.tmp qmtst.exp


libmq.lib: $(LIBOBJS) $(INCS)
	rm -f libmq.lib
	lib /OUT:libmq.lib $(LIBOBJS)
#	ar -rv libmq.lib $(LIBOBJS)
#	-ranlib libmq.lib


qcalc.exe: qcalc.obj $(INCS) incbet.obj incbi.obj gamma.obj igami.obj igam.obj \
log.obj exp.obj sin.obj pow.obj powi.obj polevl.obj ndtri.obj sqrt.obj floor.obj \
const.obj libmq.lib
	link qcalc.obj incbet.obj incbi.obj ndtri.obj \
gamma.obj igami.obj igam.obj log.obj exp.obj sin.obj pow.obj powi.obj polevl.obj sqrt.obj \
floor.obj const.obj \
libmq.lib
#libmq.lib -lreadline -lncurses -lm

qcalc.obj: qcalc.c
	$(CC) $(CFLAGS) -c qcalc.c
#	$(CC) $(CFLAGS) -DUSE_READLINE=1 -c qcalc.c

mtherr.obj: mtherr.c $(INCS)

floor.obj: floor.c $(INCS)

const.obj: const.c

incbet.obj: incbet.c

incbi.obj: incbi.c

gamma.obj: gamma.c

igami.obj: igami.c

igam.obj: igam.c

exp.obj: exp.c

sin.obj: sin.c

pow.obj: pow.c

powi.obj: powi.c

sqrt.obj: sqrt.c



qccalc.exe: qccalc.obj cmplx.obj sqrt.obj floor.obj const.obj $(INCS) libmq.lib
	link qccalc.obj cmplx.obj sqrt.obj floor.obj const.obj \
libmq.lib
#libmq.lib -lreadline -lncurses -lm

qccalc.obj: qccalc.c
	$(CC) $(CFLAGS) -c qccalc.c
#	$(CC) $(CFLAGS) -DUSE_READLINE=1 -c qccalc.c


cmplx.obj: cmplx.c $(INCS)



qmtst.exe: qmtst.obj ndtri.obj sqrt.obj log.obj polevl.obj const.obj \
drand.obj floor.obj $(INCS) libmq.lib
	link qmtst.obj ndtri.obj sqrt.obj \
log.obj polevl.obj const.obj floor.obj drand.obj libmq.lib

ndtri.obj: ndtri.c $(INCS)

polevl.obj: polevl.c $(INCS)

drand.obj: drand.c $(INCS)

log.obj: log.c



# Test the version of arithmetic that is correctly rounded.
# qflta.c works only with 16-bit wordsize.
#qparanoi: qparanoi.obj qflt.obj qflta.obj qsqrta.obj libmq.lib
#	link qparanoi.obj qflt.obj qflta.obj \
#qsqrta.obj libmq.lib -lm

# This will test the arithmetic that is actually in the library.
qparanoi.exe: qparanoi.obj libmq.lib $(INCS)
	link qparanoi.obj libmq.lib

qflt.obj: qflt.c $(INCS)
	$(CC) $(CFLAGS) -DSTICKY=1 -c qflt.c

qflta.obj: qflta.c $(INCS)
	$(CC) $(CFLAGS) -c qflta.c

qflti.obj: qflti.c $(INCS)
	$(CC) $(CFLAGS) /Ox $(DEFS) -c qflti.c

qparanoi.obj: qparanoi.c $(INCS)
	$(CC) -O -c qparanoi.c
#	$(CC) -O -Wno-implicit -c qparanoi.c

qsqrta.obj: qsqrta.c $(INCS)


# i386, coff version (DJGPP)
qfltb386.obj: qfltbi.386
	as -o qfltb386.obj qfltbi.386

# i386, ELF version (linux)
qfltbelf.obj: qfltbelf.386
	as -o qfltbelf.obj qfltbelf.386

qfltbi.obj: qfltbi.c
	$(CC) $(CFLAGS) /Ox -c qfltbi.c

qf68k.obj: qf68k.a
	as -o qf68k.obj qf68k.a

clean:
	rm -f *.obj
	rm -f libmq.lib
	rm -f qcalc
	rm -f qccalc
	rm -f qparanoi
	rm -f qmtst
