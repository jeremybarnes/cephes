# MSDOS Microsoft make file for qccalc.exe
CFLAGS=/c

qccalc.obj: qccalc.c qhead.h qcalc.h mconf.h
	cl $(CFLAGS) qccalc.c

qcmplx.obj: qcmplx.c qhead.h
	cl $(CFLAGS) qcmplx.c

qcgamma.obj: qcgamma.c qhead.h
	cl $(CFLAGS) qcgamma.c

qccalc.exe: qccalc.obj qlib100.lib qcmplx.obj qcgamma.obj
	link /NOE /MAP /STACK:8192 qccalc qcmplx qcgamma,,,qlib100 \lib\fti;
