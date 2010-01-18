# This is for small memory model, Microsoft C.
# For NQ = 24, you MUST COPY
#   the file qhead16.h to qhead.h.
#   the file qhead24.asm to qhead.asm.
# For NQ = 12, you MUST COPY
#   the file qhead12.h to qhead.h.
#   the file qhead12.asm to qhead.asm.
# You MUST EDIT mconf.h to indicate BIGENDIAN is 0.
# Remove the `;' from the link commands for old linkers.

CC=cl
CFLAGS=/c
INCS=qhead16.h
ASM=masm

#inference rules
.c.obj:
	$(CC) $(CFLAGS) $*.c

.asm.obj:
	$(ASM) $*.asm/r;


qacosh.obj: qacosh.c

qairy.obj: qairy.c

qasin.obj: qasin.c

qasinh.obj: qasinh.c

qatanh.obj: qatanh.c

qatn.obj: qatn.c

qbeta.obj: qbeta.c

qcbrt.obj: qcbrt.c

qcgamma.obj: qcgamma.c

qcmplx.obj: qcmplx.c

qconst.obj: qconst.c

qcos.obj: qcos.c

qcosh.obj: qcosh.c

qdawsn.obj: qdawsn.c

qellie.obj: qellie.c

qellik.obj: qellik.c

qellpe.obj: qellpe.c

qellpj.obj: qellpj.c

qellpk.obj: qellpk.c

qerf.obj: qerf.c

qerfc.obj: qerfc.c

qeuclid.obj: qeuclid.c

qexp.obj: qexp.c

qexp10.obj: qexp10.c

qexp2.obj: qexp2.c

qexpn.obj: qexpn.c

qfac.obj: qfac.c

qfresf.obj: qfresf.c

qgamma.obj: qgamma.c

qhy2f1.obj: qhy2f1.c

qhyp.obj: qhyp.c

qigam.obj: qigam.c

qigami.obj: qigami.c

qin.obj: qin.c

qincb.obj: qincb.c

qincbi.obj: qincbi.c

qine.obj: qine.c

qjn.obj: qjn.c

qjypn.obj: qjypn.c

qjyqn.obj: qjyqn.c

qk0.obj: qk0.c

qkn.obj: qkn.c

qkne.obj: qkne.c

qlog.obj: qlog.c

qlog1.obj: qlog1.c

qlog10.obj: qlog10.c

qndtr.obj: qndtr.c

qndtri.obj: qndtri.c

qpow.obj: qpow.c

qprob.obj: qprob.c

qpsi.obj: qpsi.c

qremain.obj: qremain.c

qshici.obj: qshici.c

qsici.obj: qsici.c

qsimq.obj: qsimq.c

qsin.obj: qsin.c

qsindg.obj: qsindg.c

qsinh.obj: qsinh.c

qspenc.obj: qspenc.c

qsqrt.obj: qsqrt.c

qstudt.obj: qstudt.c

qtan.obj: qtan.c

qtanh.obj: qtanh.c

qyn.obj: qyn.c

qzetac.obj: qzetac.c

qfloor.obj: qfloor.c

qflti.obj: qflti.c

subm.obj: subm.asm qhead.asm

divn.obj: divn.asm qhead.asm

mulr.obj: mulr.asm qhead.asm

qlib.lib: qacosh.obj qairy.obj qasin.obj qasinh.obj qatanh.obj qatn.obj \
qbeta.obj qcbrt.obj qcgamma.obj qcmplx.obj qconst.obj qcos.obj \
qcosh.obj qdawsn.obj qellie.obj qellik.obj qellpe.obj \
qellpj.obj qellpk.obj qerf.obj qerfc.obj qeuclid.obj qexp.obj \
qexp10.obj qexp2.obj qexpn.obj qfac.obj qfresf.obj qgamma.obj \
qhy2f1.obj qhyp.obj qigam.obj qigami.obj qin.obj \
qincb.obj qincbi.obj qine.obj qjn.obj \
qjypn.obj qjyqn.obj qk0.obj qkn.obj qkne.obj qlog.obj qlog1.obj \
qlog10.obj qndtr.obj qndtri.obj qpow.obj qprob.obj qpsi.obj \
qremain.obj qshici.obj qsici.obj qsimq.obj qsin.obj qsindg.obj \
qsinh.obj qspenc.obj qsqrt.obj qstudt.obj qtan.obj qtanh.obj qyn.obj \
qzetac.obj qfloor.obj qflti.obj subm.obj divn.obj mulr.obj $(INCS)
	del qlib.lib
	lib @qlibasm.rsp


igam.obj: igam.c $(INCS)

incbet.obj: incbet.c $(INCS)

igami.obj: igami.c $(INCS)

incbi.obj: incbi.c $(INCS)

gamma.obj: gamma.c $(INCS)

ndtri.obj: ndtri.c $(INCS)

cmplx.obj: cmplx.c $(INCS)

drand.obj: drand.c $(INCS)

polevl.obj: polevl.c $(INCS)

mtherr.obj: mtherr.c $(INCS)

const.obj: const.c $(INCS)


misc.lib: igam.obj incbet.obj igami.obj incbi.obj gamma.obj ndtri.obj \
cmplx.obj drand.obj polevl.obj mtherr.obj const.obj
	del misc.lib
	lib @misclib.rsp



qparanoi.obj: qparanoi.c $(INCS)

# Test the version of arithmetic that is correctly rounded.
# It is much slower than the others, so it would not ordinarily
# be used.  Note that it does not support denormal numbers.
# Get the package ieeetest.zoo for that.
qsqrta.obj: qsqrta.c $(INCS)

qflt.obj: qflt.c $(INCS)
	$(CC) $(CFLAGS) /DSTICKY=1 qflt.c

qflta.obj: qflta.c $(INCS)

qparanoi.exe: qparanoi.obj qflt.obj qflta.obj qsqrta.obj mtherr.obj qlib.lib misc.lib
	link /NOE qparanoi qflt qflta qsqrta,,,qlib misc;


# This will test the arithmetic that is actually in the library.
#qparanoi.exe: qparanoi.obj mtherr.obj qlib.lib misc.lib
#	link qparanoi,,,qlib misc;



qccalc.obj: qccalc.c $(INCS)
	$(CC) $(CFLAGS) /DMOREFUNS=0 /DLARGEMEM=0 qccalc.c

qccalc.exe: qccalc.obj qlib.lib misc.lib $(INCS)
	link /NOE qccalc,,,qlib misc;




qmtst.obj: qmtst.c $(INCS)
	$(CC) $(CFLAGS) /DNTRIALS=100 qmtst.c

qmtst.exe: qmtst.obj qlib.lib misc.lib $(INCS)
	link qmtst,,,qlib misc;




qcalc.obj: qcalc.c $(INCS)
	$(CC) $(CFLAGS) /DMOREFUNS=0 /DLARGEMEM=0 qcalc.c

qcalc.exe: qcalc.obj qlib.lib misc.lib $(INCS)
	link /NOE qcalc,,,qlib misc;



time-it.obj: time-it.c

time-it.exe: time-it.obj
	link time-it;
	time-it qmtst

