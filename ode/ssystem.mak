# Microsoft C MSDOS make file for ssystem.exe
CFLAGS=/c
LFLAGS=
INCS= int.h
OBJS= ssystem.obj adams.obj rungek.obj

ssystem.obj: ssystem.c $(INCS)
	cl $(CFLAGS) ssystem.c

adams.obj: adams3.c $(INCS)
	cl $(CFLAGS) /Foadams.obj adams3.c

rungek.obj: rungek.c $(INCS)
	cl $(CFLAGS) rungek.c

ssystem.exe: $(OBJS) 
	link $(LFLAGS) $(OBJS);
