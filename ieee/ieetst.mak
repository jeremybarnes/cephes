# Microsoft make file for ieetst.
# Define IBMPC in mconf.h.
#
CFLAGS=/c
INCS= mconf.h ehead.h
CC = cl
LINK = link

ieee.obj: ieee.c $(INCS)
	$(CC) $(CFLAGS) ieee.c

elog.obj: elog.c $(INCS)
	$(CC) $(CFLAGS) elog.c

eexp.obj: eexp.c $(INCS)
	$(CC) $(CFLAGS) eexp.c

etanh.obj: etanh.c $(INCS)
	$(CC) $(CFLAGS) etanh.c

epow.obj: epow.c $(INCS)
	$(CC) $(CFLAGS) epow.c

mtherr.obj: mtherr.c $(INCS)
	$(CC) $(CFLAGS) mtherr.c

etodec.obj: etodec.c $(INCS)
	$(CC) $(CFLAGS) etodec.c

ieetst.obj: ieetst.c $(INCS) qcalc.h
	$(CC) $(CFLAGS) ieetst.c

drand.obj: drand.c $(INCS)
	$(CC) $(CFLAGS) drand.c

econst.obj: econst.c $(INCS)
	$(CC) $(CFLAGS) econst.c

ieetst.exe: ieetst.obj ieee.obj econst.obj elog.obj eexp.obj \
 etanh.obj epow.obj drand.obj mtherr.obj
 $(LINK) ieetst ieee econst elog eexp etanh epow drand mtherr;
