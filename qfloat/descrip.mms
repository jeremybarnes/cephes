CFLAGS=		/DEBUG/NOLIST
hfiles=		qhead.h-

ofiles=		qcalc.obj-
		qtst1.obj-
		qparanoi.obj-
		cmplx.obj-
		const.obj-
		drand.obj-
		exp.obj-
		floor.obj-
		gamma.obj-
		igam.obj-
		igami.obj-
		incbet.obj-
		incbi.obj-
		log.obj-
		mtherr.obj-
		ndtri.obj-
		polevl.obj-
		pow.obj-
		powi.obj-
		qacosh.obj-
		qairy.obj-
		qasin.obj-
		qasinh.obj-
		qatanh.obj-
		qatn.obj-
		qbeta.obj-
		qcbrt.obj-
		qcgamma.obj-
		qcmplx.obj-
		qconst.obj-
		qcos.obj-
		qcosh.obj-
		qdawsn.obj-
		qellie.obj-
		qellik.obj-
		qellpe.obj-
		qellpj.obj-
		qellpk.obj-
		qerf.obj-
		qerfc.obj-
		qeuclid.obj-
		qexp.obj-
		qexp10.obj-
		qexp2.obj-
		qexpn.obj-
		qfac.obj-
		qfloor.obj-
		qflt.obj-
		qflta.obj-
		qfltb.obj-
		qflti.obj-
		qfresf.obj-
		qgamma.obj-
		qhy2f1.obj-
		qhyp.obj-
		qi1.obj-
		qigam.obj-
		qigami.obj-
		qin.obj-
		qincb.obj-
		qincbi.obj-
		qincg.obj-
		qincgs.obj-
		qine.obj-
		qjn.obj-
		qjypn.obj-
		qjyqn.obj-
		qk0.obj-
		qkn.obj-
		qkne.obj-
		qlog.obj-
		qlog1.obj-
		qlog10.obj-
		qmtst.obj-
		qndtr.obj-
		qndtri.obj-
		qpow.obj-
		qprob.obj-
		qpsi.obj-
		qremain.obj-
		qshici.obj-
		qsici.obj-
		qsimq.obj-
		qsin.obj-
		qsindg.obj-
		qsinh.obj-
		qspenc.obj-
		qsqrt.obj-
		qsqrta.obj-
		qstudt.obj-
		qtan.obj-
		qtanh.obj-
		qyn.obj-
		qzetac.obj-
		sin.obj-
		sqrt.obj-

qmtst.exe : $(ofiles)
	LINK  qmtst/option
qcalc.exe : $(ofiles)
	LINK  qcalc/option
qparanoi.exe : $(ofiles)
	LINK  qparanoi/option
qtst1.exe : $(ofiles)
	LINK  qtst1/option
qcalc.obj : qcalc.c,$(HFILES)
    CC $(CFLAGS) qcalc
qtst1.obj : qtst1.c,$(HFILES)
    CC $(CFLAGS) qtst1
qparanoi.obj : qparanoi.c,$(HFILES)
    CC $(CFLAGS) qparanoi
cmplx.obj : cmplx.c,$(HFILES)
    CC $(CFLAGS) cmplx
const.obj : const.c,$(HFILES)
    CC $(CFLAGS) const
drand.obj : drand.c,$(HFILES)
    CC $(CFLAGS) drand
exp.obj : exp.c,$(HFILES)
    CC $(CFLAGS) exp
floor.obj : floor.c,$(HFILES)
    CC $(CFLAGS) floor
gamma.obj : gamma.c,$(HFILES)
    CC $(CFLAGS) gamma
igam.obj : igam.c,$(HFILES)
    CC $(CFLAGS) igam
igami.obj : igami.c,$(HFILES)
    CC $(CFLAGS) igami
incbet.obj : incbet.c,$(HFILES)
    CC $(CFLAGS) incbet
incbi.obj : incbi.c,$(HFILES)
    CC $(CFLAGS) incbi
log.obj : log.c,$(HFILES)
    CC $(CFLAGS) log
mtherr.obj : mtherr.c,$(HFILES)
    CC $(CFLAGS) mtherr
polevl.obj : polevl.c,$(HFILES)
    CC $(CFLAGS) polevl
pow.obj : pow.c,$(HFILES)
    CC $(CFLAGS) pow
powi.obj : powi.c,$(HFILES)
    CC $(CFLAGS) powi
qacosh.obj : qacosh.c,$(HFILES)
    CC $(CFLAGS) qacosh
qairy.obj : qairy.c,$(HFILES)
    CC $(CFLAGS) qairy
qasin.obj : qasin.c,$(HFILES)
    CC $(CFLAGS) qasin
qasinh.obj : qasinh.c,$(HFILES)
    CC $(CFLAGS) qasinh
qatanh.obj : qatanh.c,$(HFILES)
    CC $(CFLAGS) qatanh
qatn.obj : qatn.c,$(HFILES)
    CC $(CFLAGS) qatn
qbeta.obj : qbeta.c,$(HFILES)
    CC $(CFLAGS) qbeta
qcbrt.obj : qcbrt.c,$(HFILES)
    CC $(CFLAGS) qcbrt
qcgamma.obj : qcgamma.c,$(HFILES)
    CC $(CFLAGS) qcgamma
qcmplx.obj : qcmplx.c,$(HFILES)
    CC $(CFLAGS) qcmplx
qconst.obj : qconst.c,$(HFILES)
    CC $(CFLAGS) qconst
qcos.obj : qcos.c,$(HFILES)
    CC $(CFLAGS) qcos
qcosh.obj : qcosh.c,$(HFILES)
    CC $(CFLAGS) qcosh
qdawsn.obj : qdawsn.c,$(HFILES)
    CC $(CFLAGS) qdawsn
qellie.obj : qellie.c,$(HFILES)
    CC $(CFLAGS) qellie
qellik.obj : qellik.c,$(HFILES)
    CC $(CFLAGS) qellik
qellpe.obj : qellpe.c,$(HFILES)
    CC $(CFLAGS) qellpe
qellpj.obj : qellpj.c,$(HFILES)
    CC $(CFLAGS) qellpj
qellpk.obj : qellpk.c,$(HFILES)
    CC $(CFLAGS) qellpk
qerf.obj : qerf.c,$(HFILES)
    CC $(CFLAGS) qerf
qerfc.obj : qerfc.c,$(HFILES)
    CC $(CFLAGS) qerfc
qeuclid.obj : qeuclid.c,$(HFILES)
    CC $(CFLAGS) qeuclid
qexp.obj : qexp.c,$(HFILES)
    CC $(CFLAGS) qexp
qexp10.obj : qexp10.c,$(HFILES)
    CC $(CFLAGS) qexp10
qexp2.obj : qexp2.c,$(HFILES)
    CC $(CFLAGS) qexp2
qexpn.obj : qexpn.c,$(HFILES)
    CC $(CFLAGS) qexpn
qfac.obj : qfac.c,$(HFILES)
    CC $(CFLAGS) qfac
qfloor.obj : qfloor.c,$(HFILES)
    CC $(CFLAGS) qfloor
qflt.obj : qflt.c,$(HFILES)
    CC $(CFLAGS) qflt
qflta.obj : qflta.c,$(HFILES)
    CC $(CFLAGS) /DEFINE=STICKY=1 qflta
qfltb.obj : qfltb.c,$(HFILES)
    CC $(CFLAGS) qfltb
qflti.obj : qflti.c,$(HFILES)
    CC $(CFLAGS) /DEFINE=STICKY=1 qflti
qfresf.obj : qfresf.c,$(HFILES)
    CC $(CFLAGS) qfresf
qgamma.obj : qgamma.c,$(HFILES)
    CC $(CFLAGS) qgamma
qhy2f1.obj : qhy2f1.c,$(HFILES)
    CC $(CFLAGS) qhy2f1
qhyp.obj : qhyp.c,$(HFILES)
    CC $(CFLAGS) qhyp
qi1.obj : qi1.c,$(HFILES)
    CC $(CFLAGS) qi1
qigam.obj : qigam.c,$(HFILES)
    CC $(CFLAGS) qigam
qigami.obj : qigami.c,$(HFILES)
    CC $(CFLAGS) qigami
qin.obj : qin.c,$(HFILES)
    CC $(CFLAGS) qin
qincb.obj : qincb.c,$(HFILES)
    CC $(CFLAGS) qincb
qincbi.obj : qincbi.c,$(HFILES)
    CC $(CFLAGS) qincbi
qincg.obj : qincg.c,$(HFILES)
    CC $(CFLAGS) qincg
qincgs.obj : qincgs.c,$(HFILES)
    CC $(CFLAGS) qincgs
qine.obj : qine.c,$(HFILES)
    CC $(CFLAGS) qine
qjn.obj : qjn.c,$(HFILES)
    CC $(CFLAGS) qjn
qjypn.obj : qjypn.c,$(HFILES)
    CC $(CFLAGS) qjypn
qjyqn.obj : qjyqn.c,$(HFILES)
    CC $(CFLAGS) qjyqn
qk0.obj : qk0.c,$(HFILES)
    CC $(CFLAGS) qk0
qkn.obj : qkn.c,$(HFILES)
    CC $(CFLAGS) qkn
qkne.obj : qkne.c,$(HFILES)
    CC $(CFLAGS) qkne
qlog.obj : qlog.c,$(HFILES)
    CC $(CFLAGS) qlog
qlog1.obj : qlog1.c,$(HFILES)
    CC $(CFLAGS) qlog1
qlog10.obj : qlog10.c,$(HFILES)
    CC $(CFLAGS) qlog10
qmtst.obj : qmtst.c,$(HFILES)
    CC $(CFLAGS) qmtst
qndtr.obj : qndtr.c,$(HFILES)
    CC $(CFLAGS) qndtr
qndtri.obj : qndtri.c,$(HFILES)
    CC $(CFLAGS) qndtri
qpow.obj : qpow.c,$(HFILES)
    CC $(CFLAGS) qpow
qprob.obj : qprob.c,$(HFILES)
    CC $(CFLAGS) qprob
qpsi.obj : qpsi.c,$(HFILES)
    CC $(CFLAGS) qpsi
qremain.obj : qremain.c,$(HFILES)
    CC $(CFLAGS) qremain
qshici.obj : qshici.c,$(HFILES)
    CC $(CFLAGS) qshici
qsici.obj : qsici.c,$(HFILES)
    CC $(CFLAGS) qsici
qsimq.obj : qsimq.c,$(HFILES)
    CC $(CFLAGS) qsimq
qsin.obj : qsin.c,$(HFILES)
    CC $(CFLAGS) qsin
qsindg.obj : qsindg.c,$(HFILES)
    CC $(CFLAGS) qsindg
qsinh.obj : qsinh.c,$(HFILES)
    CC $(CFLAGS) qsinh
qspenc.obj : qspenc.c,$(HFILES)
    CC $(CFLAGS) qspenc
qsqrt.obj : qsqrt.c,$(HFILES)
    CC $(CFLAGS) qsqrt
qsqrta.obj : qsqrta.c,$(HFILES)
    CC $(CFLAGS) qsqrta
qstudt.obj : qstudt.c,$(HFILES)
    CC $(CFLAGS) qstudt
qtan.obj : qtan.c,$(HFILES)
    CC $(CFLAGS) qtan
qtanh.obj : qtanh.c,$(HFILES)
    CC $(CFLAGS) qtanh
qyn.obj : qyn.c,$(HFILES)
    CC $(CFLAGS) qyn
qzetac.obj : qzetac.c,$(HFILES)
    CC $(CFLAGS) qzetac
sin.obj : sin.c,$(HFILES)
    CC $(CFLAGS) sin
sqrt.obj : sqrt.c,$(HFILES)
    CC $(CFLAGS) sqrt
