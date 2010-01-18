CC= cc
CFLAGS= -O2 -Wall
OBJS= qccalc.o
INCS= mconf.h qcalc.h qhead.h

qccalc: qccalc.o $(INCS) libmq.a
	$(CC) $(OBJS) -o qccalc libmq.a -lmd

