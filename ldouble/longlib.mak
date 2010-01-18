# MSDOS Microsoft C makefile for Cephes library
# Be sure to check the size of a pointer, LARGEMEM, in lcalc.h;
# the size of long double, XPD, in mconf.h; and the type of
# computer in mconf.h.
# "fti.lib" refers to a double precision function library.
CFLAGS=/c
CC=cl
# For large memory model:
#CFLAGS=/c /AL
# Note, fabsl is an intrinsic function in Microsoft.
floorl.obj: floorl.c mconf.h
	$(CC) $(CFLAGS) floorl.c

isnanl.obj: isnanl.c mconf.h
	$(CC) $(CFLAGS) isnanl.c

polevll.obj: polevll.c mconf.h
	$(CC) $(CFLAGS) polevll.c

acoshl.obj: acoshl.c mconf.h
	$(CC) $(CFLAGS) acoshl.c

asinhl.obj: asinhl.c mconf.h
	$(CC) $(CFLAGS) asinhl.c

asinl.obj: asinl.c  mconf.h
	$(CC) $(CFLAGS) asinl.c

atanhl.obj: atanhl.c mconf.h
	$(CC) $(CFLAGS) atanhl.c

atanl.obj: atanl.c mconf.h
	$(CC) $(CFLAGS) atanl.c

bdtrl.obj: bdtrl.c mconf.h
	$(CC) $(CFLAGS) bdtrl.c

btdtrl.obj: btdtrl.c mconf.h
	$(CC) $(CFLAGS) btdtrl.c

cbrtl.obj: cbrtl.c mconf.h
	$(CC) $(CFLAGS) cbrtl.c

chdtrl.obj: chdtrl.c mconf.h
	$(CC) $(CFLAGS) chdtrl.c

coshl.obj: coshl.c mconf.h
	$(CC) $(CFLAGS) coshl.c

ellpel.obj: ellpel.c mconf.h
	$(CC) $(CFLAGS) ellpel.c

ellpkl.obj: ellpkl.c mconf.h
	$(CC) $(CFLAGS) ellpkl.c

elliel.obj: elliel.c mconf.h
	$(CC) $(CFLAGS) elliel.c

ellikl.obj: ellikl.c mconf.h
	$(CC) $(CFLAGS) ellikl.c

ellpjl.obj: ellpjl.c mconf.h
	$(CC) $(CFLAGS) ellpjl.c

exp10l.obj: exp10l.c mconf.h
	$(CC) $(CFLAGS) exp10l.c

exp2l.obj: exp2l.c mconf.h
	$(CC) $(CFLAGS) exp2l.c

expl.obj: expl.c mconf.h
	$(CC) $(CFLAGS) expl.c

fdtrl.obj: fdtrl.c mconf.h
	$(CC) $(CFLAGS) fdtrl.c

gammal.obj: gammal.c mconf.h
	$(CC) $(CFLAGS) gammal.c

gdtrl.obj: gdtrl.c mconf.h
	$(CC) $(CFLAGS) gdtrl.c

igamil.obj: igamil.c mconf.h
	$(CC) $(CFLAGS) igamil.c

igaml.obj: igaml.c mconf.h
	$(CC) $(CFLAGS) igaml.c

incbetl.obj: incbetl.c mconf.h
	$(CC) $(CFLAGS) incbetl.c

incbil.obj: incbil.c mconf.h
	$(CC) $(CFLAGS) incbil.c

j0l.obj: j0l.c mconf.h
	$(CC) $(CFLAGS) j0l.c

j1l.obj: j1l.c mconf.h
	$(CC) $(CFLAGS) j1l.c

jnl.obj: jnl.c mconf.h
	$(CC) $(CFLAGS) jnl.c

ldrand.obj: ldrand.c mconf.h
	$(CC) $(CFLAGS) ldrand.c

#fabsl.obj: fabsl.c mconf.h
#	$(CC) $(CFLAGS) fabsl.c

logl.obj: logl.c mconf.h
	$(CC) $(CFLAGS) logl.c

log2l.obj: log2l.c mconf.h
	$(CC) $(CFLAGS) log2l.c

log10l.obj: log10l.c mconf.h
	$(CC) $(CFLAGS) log10l.c

nbdtrl.obj: nbdtrl.c mconf.h
	$(CC) $(CFLAGS) nbdtrl.c

ndtril.obj: ndtril.c mconf.h
	$(CC) $(CFLAGS) ndtril.c

ndtrl.obj: ndtrl.c mconf.h
	$(CC) $(CFLAGS) ndtrl.c

expx2l.obj: expx2l.c mconf.h
	$(CC) $(CFLAGS) expx2l.c

mtherr.obj: mtherr.c mconf.h
	$(CC) $(CFLAGS) mtherr.c

pdtrl.obj: pdtrl.c mconf.h
	$(CC) $(CFLAGS) pdtrl.c

powl.obj: powl.c mconf.h
	$(CC) $(CFLAGS) powl.c

powil.obj: powil.c mconf.h
	$(CC) $(CFLAGS) powil.c

sinhl.obj: sinhl.c mconf.h
	$(CC) $(CFLAGS) sinhl.c

sinl.obj: sinl.c mconf.h
	$(CC) $(CFLAGS) sinl.c

sqrtl.obj: sqrtl.c mconf.h
	$(CC) $(CFLAGS) sqrtl.c

stdtrl.obj: stdtrl.c mconf.h
	$(CC) $(CFLAGS) stdtrl.c

tanl.obj: tanl.c mconf.h
	$(CC) $(CFLAGS) tanl.c

tanhl.obj: tanhl.c mconf.h
	$(CC) $(CFLAGS) tanhl.c

ynl.obj: ynl.c mconf.h
	$(CC) $(CFLAGS) ynl.c

unityl.obj: unityl.c mconf.h
	$(CC) $(CFLAGS) unityl.c

long.lib: acoshl.obj asinl.obj asinhl.obj atanl.obj atanhl.obj \
bdtrl.obj btdtrl.obj cbrtl.obj chdtrl.obj coshl.obj ellpel.obj \
ellpkl.obj expl.obj \
exp10l.obj exp2l.obj fdtrl.obj gammal.obj gdtrl.obj igamil.obj \
igaml.obj incbetl.obj incbil.obj j0l.obj j1l.obj jnl.obj ldrand.obj \
logl.obj log2l.obj log10l.obj mtherr.obj nbdtrl.obj ndtril.obj ndtrl.obj \
pdtrl.obj powl.obj powil.obj sinl.obj sinhl.obj sqrtl.obj stdtrl.obj \
tanl.obj tanhl.obj unityl.obj ynl.obj polevll.obj floorl.obj \
isnanl.obj mconf.h
	lib @longlib.rsp

mtstl.obj: mtstl.c mconf.h
	cl /c mtstl.c

mtstl.exe: mtstl.obj long.lib
	link /noe mtstl,,,long.lib \lib\fti.lib;

lcalc.obj: lcalc.c
	cl /c /DLARGEMEM=0 lcalc.c

ieee.obj: ieee.c
	cl /c ieee.c

econst.obj: econst.c
	cl /c econst.c

lcalc.exe: lcalc.obj ieee.obj econst.obj long.lib
	link lcalc ieee econst,,,long.lib \lib\fti.lib;

testvect.obj: testvect.c
	cl /c testvect.c

testvect.exe: testvect.obj long.lib
	link testvect,,,long.lib;

