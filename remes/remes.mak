# MSDOS make file for remes.exe
CFLAGS=/c /Od

remes.obj: remes.c remes.h
	cl $(CFLAGS) remes.c

remesa.obj: remesa.c remes.h
	cl $(CFLAGS) remesa.c

remese.obj: remese.c remes.h
	cl $(CFLAGS) remese.c

remesf.obj: remesf.c remes.h
	cl $(CFLAGS) /Od remesf.c

remesg.obj: remesg.c remes.h
	cl $(CFLAGS) remesg.c

remesp.obj: remesp.c remes.h
	cl $(CFLAGS) remesp.c

remess.obj: remess.c remes.h
	cl $(CFLAGS) remess.c

remesw.obj: remesw.c remes.h
	cl $(CFLAGS) remesw.c

simq.obj: simq.c
	cl $(CFLAGS) simq.c

remes.exe: remes.obj remesa.obj remese.obj remesf.obj \
remesg.obj remesp.obj remess.obj remesw.obj simq.obj
  link remes remesa remese remesf remesg remesp remess remesw simq;

