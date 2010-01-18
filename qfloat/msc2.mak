# Microsoft nmake makefile for qlib.
# Thanks to K. B. Williams (Kbwms@aol.com).
#
# This is for large memory model, Microsoft C.
# For NQ = 24, you MUST COPY
#   the file qhead16.h to qhead.h.
#   the file qheadl24.asm to qhead.asm.
# For NQ = 12, you MUST COPY
#   the file qhead12.h to qhead.h.
#   the file qheadl12.asm to qhead.asm.
# You MUST EDIT mconf.h to indicate BIGENDIAN is 0.
# You MUST EDIT qcalc.h to indicate LARGEMEM  is 1.
# For old linkers, remove the `;' from the link commands.

CC	= cl -AL
CFLAGS	= -nologo -W2 -c
INCS	= qhead16.h
ASM	= masm

#inference rules
.c.obj:
	$(CC) $(CFLAGS) $*.c

.asm.obj:
	$(ASM) $*.asm;		# for new MASM v. 6.10
#	$(ASM) $*.asm/r;	# for older versions

all: qlibl.lib qcalc.exe qparanoi.exe qccalc.exe qmtst.exe time-it.exe

qlibl.lib: mtherr.obj qacosh.obj qairy.obj qasin.obj qasinh.obj qatanh.obj\
	   qatn.obj qbeta.obj qcbrt.obj qconst.obj\
	   qcmplx.obj qcgamma.obj qcerf.obj qcpolylo.obj qchyp1f1.obj\
	   qcos.obj qcosh.obj qdawsn.obj qellie.obj qellik.obj qellpe.obj\
	   qellpj.obj qellpk.obj qerf.obj qerfc.obj qeuclid.obj qexp.obj\
	   qexp10.obj qexp2.obj qexpn.obj qfac.obj qfresf.obj qgamma.obj\
	   qhy2f1.obj qhyp.obj qigam.obj qigami.obj qin.obj\
	   qincb.obj qincbi.obj qine.obj qjn.obj qpolylog.obj\
	   qjypn.obj qjyqn.obj qk0.obj qkn.obj qkne.obj qlog.obj qlog1.obj\
	   qlog10.obj qndtr.obj qndtri.obj qpow.obj qprob.obj qpsi.obj\
	   qremain.obj qshici.obj qsici.obj qsimq.obj qsin.obj qsindg.obj\
	   qsinh.obj qspenc.obj qsqrt.obj qstudt.obj qtan.obj qtanh.obj qyn.obj\
	   qzetac.obj qfloor.obj qflti.obj subml.obj divnl.obj mulrl.obj
	del qlibl.lib
	lib /nologo @qlibasml.rsp

mtherr.obj:	mtherr.c    mconf.h    protos.h   qprotos.h
qacosh.obj:	qacosh.c    qhead.h   qprotos.h
qairy.obj:	qairy.c     qhead.h   qprotos.h
qasin.obj:	qasin.c     qhead.h   qprotos.h
qasinh.obj:	qasinh.c    qhead.h   qprotos.h
qatanh.obj:	qatanh.c    qhead.h   qprotos.h
qatn.obj:	qatn.c      qhead.h    mconf.h   qprotos.h
qbeta.obj:	qbeta.c     qhead.h   qprotos.h
qcbrt.obj:	qcbrt.c     qhead.h   qprotos.h
qcgamma.obj:	qcgamma.c   qhead.h   qprotos.h
qcerf.obj:	qcerf.c     qhead.h   qprotos.h
qcpolylo.obj:	qcpolylo.c  qhead.h   qprotos.h
qchyp.obj:	qchyp1f1.c  qhead.h   qprotos.h
qcmplx.obj:	qcmplx.c    qhead.h    mconf.h    protos.h   qprotos.h
qconst.obj:	qconst.c    qhead.h   qprotos.h
qcos.obj:	qcos.c      qhead.h   qprotos.h
qcosh.obj:	qcosh.c     qhead.h   qprotos.h
qdawsn.obj:	qdawsn.c    qhead.h   qprotos.h
qellie.obj:	qellie.c    qhead.h   qprotos.h
qellik.obj:	qellik.c    qhead.h   qprotos.h
qellpe.obj:	qellpe.c    qhead.h   qprotos.h
qellpj.obj:	qellpj.c    qhead.h   qprotos.h
qellpk.obj:	qellpk.c    qhead.h   qprotos.h
qerf.obj:	qerf.c      qhead.h   qprotos.h
qerfc.obj:	qerfc.c     qhead.h   qprotos.h
qeuclid.obj:	qeuclid.c   qhead.h   qprotos.h
qexp.obj:	qexp.c      qhead.h   qprotos.h
qexp10.obj:	qexp10.c    qhead.h   qprotos.h
qexp2.obj:	qexp2.c     qhead.h   qprotos.h
qexpn.obj:	qexpn.c     qhead.h   qprotos.h
qfac.obj:	qfac.c      qhead.h   qprotos.h
qfloor.obj:	qfloor.c    qhead.h   qprotos.h
qflt.obj:	qflt.c      mconf.h    qhead.h    qtens.h    protos.h qprotos.h
qflta.obj:	qflta.c     qhead.h   qprotos.h
qflti.obj:	qflti.c     mconf.h    qhead.h    qtens.h    protos.h qprotos.h
qfresf.obj:	qfresf.c    qhead.h   qprotos.h
qgamma.obj:	qgamma.c    qhead.h   qprotos.h
qhy2f1.obj:	qhy2f1.c    mconf.h    qhead.h    protos.h   qprotos.h
qhyp.obj:	qhyp.c      qhead.h   qprotos.h
qigam.obj:	qigam.c     qhead.h   qprotos.h
qigami.obj:	qigami.c    qhead.h   qprotos.h
qin.obj:	qin.c       qhead.h   qprotos.h
qincb.obj:	qincb.c     qhead.h   qprotos.h
qincbi.obj:	qincbi.c    mconf.h    qhead.h    protos.h   qprotos.h
qine.obj:	qine.c      qhead.h   qprotos.h
qjn.obj:	qjn.c       mconf.h    qhead.h    protos.h   qprotos.h
qjypn.obj:	qjypn.c     qhead.h   qprotos.h
qjyqn.obj:	qjyqn.c     qhead.h   qprotos.h
qk0.obj:	qk0.c       qhead.h   qprotos.h
qkn.obj:	qkn.c       mconf.h    qhead.h    protos.h   qprotos.h
qkne.obj:	qkne.c      mconf.h    qhead.h    protos.h   qprotos.h
qlog.obj:	qlog.c      qhead.h   qprotos.h
qlog1.obj:	qlog1.c     qhead.h   qprotos.h
qlog10.obj:	qlog10.c    qhead.h   qprotos.h
qndtr.obj:	qndtr.c     qhead.h   qprotos.h
qndtri.obj:	qndtri.c    qhead.h    mconf.h    protos.h   qprotos.h
qpolylog.obj:	qpolylog.c  qhead.h   qprotos.h
qpow.obj:	qpow.c      qhead.h    mconf.h    protos.h   qprotos.h
qprob.obj:	qprob.c     qhead.h   qprotos.h
qpsi.obj:	qpsi.c      qhead.h   qprotos.h
qremain.obj:	qremain.c   qhead.h   qprotos.h
qshici.obj:	qshici.c    qhead.h   qprotos.h
qsici.obj:	qsici.c     qhead.h   qprotos.h
qsimq.obj:	qsimq.c     qhead.h   qprotos.h
qsin.obj:	qsin.c      qhead.h   qprotos.h
qsindg.obj:	qsindg.c    qhead.h   qprotos.h
qsinh.obj:	qsinh.c     qhead.h   qprotos.h
qspenc.obj:	qspenc.c    qhead.h   qprotos.h
qsqrta.obj:	qsqrta.c    qhead.h    mconf.h    protos.h   qprotos.h
qstudt.obj:	qstudt.c    mconf.h    qhead.h    protos.h   qprotos.h
qtan.obj:	qtan.c      qhead.h   qprotos.h
qtanh.obj:	qtanh.c     qhead.h   qprotos.h
qyn.obj:	qyn.c       qhead.h   qprotos.h
qzetac.obj:	qzetac.c    qhead.h   qprotos.h

subml.obj:	subml.asm	qheadl.asm
divnl.obj:	divnl.asm	qheadl.asm
mulrl.obj:	mulrl.asm	qheadl.asm

#support for optional extra functions
EXTRAS	= gamma.obj igami.obj ndtri.obj incbi.obj const.obj polevl.obj\
	  igam.obj incbet.obj
LEXTRA	= gamma igami ndtri incbi const polevl igam incbet

gamma.obj:	gamma.c	    mconf.h	protos.h
igam.obj:	igam.c	    mconf.h	protos.h
igami.obj:	igami.c	    mconf.h	protos.h
ndtri.obj:	ndtri.c	    mconf.h	protos.h
incbet.obj:	incbet.c    mconf.h	protos.h
incbi.obj:	incbi.c	    mconf.h	protos.h
polevl.obj:	polevl.c
qparanoi.obj:	qparanoi.c  qhead.h   qprotos.h

# See the unix makefile for MOREFUNS=1 dependents.
# At least some of them will fit, if you want them.
qcalc.obj: qcalc.c qcalc.h qhead.h qprotos.h
	$(CC) $(CFLAGS) -DMOREFUNS=1  qcalc.c

qcalc.exe: qcalc.obj $(EXTRAS) qlibl.lib
	link /nologo /STACK:32000 qcalc $(EXTRAS:.obj=),,,qlibl;

# Test the version of arithmetic that is correctly rounded.
# It is much slower than the others, so it would not ordinarily
# be used.  Note that it does not support denormal numbers.
# Get the package ieeetest.zoo for that.

qsqrta.obj:	qsqrta.c  qhead.h  mconf.h  protos.h   qprotos.h

qflt.obj:	qflt.c mconf.h qhead.h qtens.h protos.h   qprotos.h
	$(CC) $(CFLAGS) /DSTICKY=1 qflt.c

qflta.obj:	qflta.c	qhead.h   qprotos.h

qparanoi.exe: qparanoi.obj qflt.obj qflta.obj qsqrta.obj qlibl.lib
#	link /nologo /NOE qparanoi qflt qflta qsqrta,,,qlibl;
	link /nologo /NOE qparanoi,,,qlibl;

# This will test the arithmetic that is actually in the library.
#qparanoi.exe: qparanoi.obj qlibl.lib
#	link /nologo qparanoi,,,qlibl;

qccalc.obj:	qccalc.c    qhead.h	qcalc.h   qprotos.h
const.obj:	const.c	    mconf.h	protos.h
cmplx.obj:	cmplx.c	    mconf.h	protos.h
qmtst.obj:	qmtst.c	    qhead.h	mconf.h	protos.h   qprotos.h
drand.obj:	drand.c	    mconf.h	protos.h

qccalc.exe: qccalc.obj cmplx.obj const.obj qlibl.lib
	link /nologo /STACK:32000 qccalc cmplx const,,,qlibl;

qmtst.obj:	qmtst.c  qhead.h  mconf.h  protos.h   qprotos.h
	$(CC) $(CFLAGS) /DNTRIALS=100 qmtst.c

qmtst.exe: qmtst.obj ndtri.obj const.obj drand.obj\
		polevl.obj qlibl.lib
	link /nologo /STACK:32000 qmtst ndtri drand polevl\
		const,,,qlibl;

time-it.obj: time-it.c

time-it.exe: time-it.obj
	link /nologo time-it;
	del time-it.obj
