CFLAGS=		/DEBUG/NOLIST
hfiles=		qcalc.h-
		mconf.h-
		ehead.h-

ofiles=		ieetst.obj-
		elog.obj-
		econst.obj-
		drand.obj-
		eexp.obj-
		etanh.obj-
		etodec.obj-
		epow.obj-
		ieee.obj-
		mtherr.obj-

all : ieetst.exe
	
ieetst.exe : $(ofiles)
	LINK  ieetst/option
ieetst.obj : ieetst.c,$(HFILES)
    CC $(CFLAGS) ieetst.c
econst.obj : econst.c,$(HFILES)
    CC $(CFLAGS) econst.c
elog.obj : elog.c,$(HFILES)
    CC $(CFLAGS) elog.c
drand.obj : drand.c,$(HFILES)
    CC $(CFLAGS) drand.c
eexp.obj : eexp.c,$(HFILES)
    CC $(CFLAGS) eexp.c
etanh.obj : etanh.c,$(HFILES)
    CC $(CFLAGS) etanh
etodec.obj : etodec.c,$(HFILES)
    CC $(CFLAGS) etodec
epow.obj : epow.c,$(HFILES)
    CC $(CFLAGS) epow.c
ieee.obj : ieee.c,$(HFILES)
    CC $(CFLAGS) ieee.c
mtherr.obj : mtherr.c,$(HFILES)
    CC $(CFLAGS) mtherr.c

